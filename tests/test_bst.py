import random
import string

import pytest

from fgshun_bst._bst import BinarySearchTree as CBST
from fgshun_bst.bst import BinarySearchTree as PyBST


@pytest.fixture(params=(PyBST, CBST), ids=('Py', 'C'))
def BinarySearchTree(request):
    return request.param


def test_bst_int(BinarySearchTree):
    values = [3, 1, 4, 1, 5, 9]
    py_set = frozenset(values)

    tree = BinarySearchTree()
    for c in values:
        tree.add(c)

    for i in range(10):
        assert (c in tree) is (c in py_set)


def test_bst_str(BinarySearchTree):
    s = 'fgshun'
    py_set = frozenset(s)

    tree = BinarySearchTree()
    for c in s:
        tree.add(c)

    for c in string.ascii_letters:
        assert (c in tree) is (c in py_set)


def test_bst_iter(BinarySearchTree):
    values = [3, 1, 4, 1, 5, 9]

    tree = BinarySearchTree()
    for c in values:
        tree.add(c)

    for a, b in zip(sorted(frozenset(values)), tree):
        assert a == b


def test_bst_more_items(BinarySearchTree):
    tree = BinarySearchTree()
    r = random.Random(20231221)
    for i in (r.randint(0, 10000000) for _ in range(10000)):
        tree.add(i)
    assert 0 not in tree
    assert 7697470 in tree
    assert 2226772 in tree
    assert len(tree) == 9990
