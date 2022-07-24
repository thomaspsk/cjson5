from setuptools import setup, Extension
import json


def main():
    setup(name="cjson5",
          version="0.1.0",
          description="C++ implementation of a JSON5 serializer/deserializer",
          author="Thomas Kearvell",
          author_email="tom.kearvell@gmail.com",
          ext_modules=[Extension("cjson5", ["src/cjson5.cpp"])])


if __name__ == "__main__":
    main()
