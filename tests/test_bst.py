import string

from fgshun_bst.bst import BinarySearchTree


def test_bst_int():
    values = [3, 1, 4, 1, 5, 9]
    py_set = frozenset(values)

    tree = BinarySearchTree()
    for c in values:
        tree.add(c)

    for i in range(10):
        assert (c in tree) is (c in py_set)


def test_bst_str():
    s = 'fgshun'
    py_set = frozenset(s)

    tree = BinarySearchTree()
    for c in s:
        tree.add(c)

    for c in string.ascii_letters:
        assert (c in tree) is (c in py_set)
