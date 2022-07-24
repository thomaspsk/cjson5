#ifndef RUNNING_MAIN
#include <Python.h>
#endif

#include <iostream>
#include <vector>
#include <stack>
//#include <string>

using namespace std;

enum states {
    _initial,
    _comment,
    _lineComment,
    _blockComment,
    _possibleEndBlockComment,
    _openDict,
    _closeDict,
    _openList,
    _closeList,
//    _openDict,
//    _closeDict,
//    _openList,
//    _closeList,
//    _key,
//    _int,
//    _float,
//    _str,
//    _bool,
//    _whitespace,
//    _comma,
};

bool isAlpha(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

bool isNumeric(char c) {
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

bool isAlphanumeric(char c) {
    if (isAlpha(c) || isNumeric(c))
        return true;
    return false;
}

bool isWhitespace(char c) {
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        return true;
    return false;
}

string parseJSON(char *s) {
    enum states state = _initial;
    string cleaned = "";
    stack<states> stateStack; //  Only states that we can return to should be pushed. ex: dict, list, comments, str
    stateStack.push(state);
    for (unsigned long i = 0; i < strlen(s); ++i) {
        char c = s[i];
        if (state == _initial) {
            if (isWhitespace(c)) {
                continue;
            } else if (c == '/') {
                state = _comment;
                continue;
            } else if (c == '{') {
                state = _openDict;
                cleaned.push_back(c);
                stateStack.push(state);
            } else if (c == '[') {
                state = _openList;
                cleaned.push_back(c);
                stateStack.push(state);
            } else {
                throw "Invalid transition";
            }
        } else if (state == _comment) {
            if (c == '/') {
                state = _lineComment;
                stateStack.push(state);
            } else if (c == '*') {
                state = _blockComment;
                stateStack.push(state);
            } else {
                throw "Invalid transition";
            }
            continue;
        } else if (state == _lineComment) {
            if (c != '\n') {
                continue;
            }
            stateStack.pop();
            state = stateStack.top();
            continue;
        } else if (state == _blockComment) {
            if (c != '*') {
                continue;
            }
            state = _possibleEndBlockComment;
            stateStack.push(state);
        } else if (state == _possibleEndBlockComment) {
            if (c == '/') {
                stateStack.pop();
                stateStack.pop();
                state = stateStack.top();
                continue;
            }
            stateStack.pop();
            state = stateStack.top(); // Back to _blockComment
        } else if (state == _openDict) {
            if (isWhitespace(c)) {
                continue;
            } else if (c == '}') {
                state = _closeDict;
                cleaned.push_back(c);
                stateStack.push(state);
            } else if (c == '/') {
                state = _comment;
                continue;
            }
            // TODO: So much more
        } else if (state == _closeDict) {
            if (isWhitespace(c)) {
                continue;
            }
        } else if (state == _openList) {
            if (isWhitespace(c)) {
                continue;
            } else if (c == ']') {
                state = _closeList;
                cleaned.push_back(c);
                stateStack.push(state);
            } else if (c == '/') {
                state = _comment;
                continue;
            }
            // TODO: So much more
        } else if (state == _closeList) {
            if (isWhitespace(c)) {
                continue;
            }
        }
    }

    return cleaned;
}

#ifndef RUNNING_MAIN
static PyObject *method_cjson5Loads(PyObject *self, PyObject *args) {
    char *s;
    int bytes_copied = 1;

    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "s", &s)) {
        return NULL;
    }

    string cleaned = parseJSON(s);
    const char *c_Cleaned = cleaned.c_str();

    cout << c_Cleaned << endl;

    return Py_BuildValue(c_Cleaned);
}

static PyObject *method_cjson5Dumps(PyObject *self, PyObject *args) {
    return PyUnicode_FromString("potato");
}

static PyMethodDef cjson5Methods[] = {
        {"loads", method_cjson5Loads, METH_VARARGS, "Takes a JSON5 string and converts it into a python dict or list"},
        {"dumps", method_cjson5Dumps, METH_VARARGS, "Takes a JSON serializable python object and converts it into a string"},
        {NULL, NULL, 0, NULL}
};


static struct PyModuleDef fcjson5module = {
        PyModuleDef_HEAD_INIT,
        "cjson5",
        "C++ implementation of a JSON5 serializer/deserializer",
        -1,
        cjson5Methods
};

PyMODINIT_FUNC PyInit_cjson5(void) {
    return PyModule_Create(&fcjson5module);
}
#endif

int main(int argc, char* argv[]) {
    char json[] = "// This is a line comment!\n"
                  "/*\n"
                  "    Block comment!\n"
                  "*/\n"
                  "{\n"
                  "\n"
                  "}";
    string cleaned = parseJSON(json);
    cout << cleaned << endl;

    return 0;
}
