from unittest import TestCase


class TestCJSON5(TestCase):

    def test_cjson5_import(self):
        import cjson5
        # print(cjson5.loads('{}'))

    def test_cjson5_loads(self):
        import cjson5
        loaded = cjson5.loads('{}')
        self.assertEqual(dict(), loaded)

    def test_cjson5_loads_with_json5_features_base_dict(self):
        import cjson5
        with open('data/test_cjson5_loads_with_json5_features_base_dict.txt') as f:
            file_contents = f.read()
        loaded = cjson5.loads(file_contents)
        self.assertEqual(dict(), loaded)

    def test_cjson5_loads_with_json5_features_base_list(self):
        import cjson5
        with open('data/test_cjson5_loads_with_json5_features_base_list.txt') as f:
            file_contents = f.read()
        loaded = cjson5.loads(file_contents)
        self.assertEqual(list(), loaded)

    def test_cjson5_dumps(self):
        import cjson5
        # print(cjson5.dumps(dict()))
