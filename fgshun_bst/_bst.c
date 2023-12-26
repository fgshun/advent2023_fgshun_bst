#define PY_SSIZE_T_CLEAN
#include <Python.h>


typedef struct NODE {
    PyObject *value;
    struct NODE *left;
    struct NODE *right;
} Node;


typedef struct {
    PyObject_HEAD
    Node *root;
} BinarySearchTreeObject;


static PyObject *
bst_new(PyTypeObject *subtype, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {NULL};
    BinarySearchTreeObject *self;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist)) { return NULL; }

    self = PyObject_GC_New(BinarySearchTreeObject, subtype);
    if (!self) { return NULL; }
    self->root = NULL;
    PyObject_GC_Track(self);

    return (PyObject *)self;
}


static int
bst_traverse_inner(Node *node, visitproc visit, void *arg)
{
    if (node == NULL) { return 0; }

    bst_traverse_inner(node->left, visit, arg);
    bst_traverse_inner(node->right, visit, arg);
    Py_VISIT(node->value);
    return 0;
}


static int
bst_traverse(BinarySearchTreeObject *self, visitproc visit, void *arg)
{
    bst_traverse_inner(self->root, visit, arg);
    return 0;
}


static void
bst_clear_inner(Node *node)
{
    if (node == NULL) { return; }

    bst_clear_inner(node->left);
    bst_clear_inner(node->right);
    Py_CLEAR(node->value);
}


static int
bst_clear(BinarySearchTreeObject *self)
{
    bst_clear_inner(self->root);
    return 0;
}


static void
bst_dealloc_inner(Node *node)
{
    if (node == NULL) { return; }

    bst_dealloc_inner(node->left);
    node->left = NULL;
    bst_dealloc_inner(node->right);
    node->right = NULL;
    PyMem_Free(node);
}


static void
bst_dealloc(BinarySearchTreeObject *self)
{
    PyObject_GC_UnTrack(self);
    bst_clear(self);
    bst_dealloc_inner(self->root);
    self->root = NULL;
    PyObject_GC_Del(self);
}


static Py_ssize_t
bst_len_inner(Node *node)
{
    if (node == NULL) {
        return 0;
    }

    return bst_len_inner(node->left) + bst_len_inner(node->right) + 1;
}


static Py_ssize_t
bst_len(BinarySearchTreeObject *self)
{
    if (self->root == NULL) {
        return 0;
    }

    return bst_len_inner(self->root);
}


static int
bst_iter_inner(Node *node, PyObject *temp)
{
    if (node == NULL) { return 0; }

    if (bst_iter_inner(node->left, temp) == -1) {
        return -1;
    }
    if (PyList_Append(temp, node->value) == -1) {
        return -1;
    }
    if (bst_iter_inner(node->right, temp) == -1) {
        return -1;
    }
    return 0;
}


static PyObject *
bst_iter(BinarySearchTreeObject *self)
{
    PyObject *temp;
    if (!(temp = PyList_New(0))) { return NULL; }

    if (bst_iter_inner(self->root, temp) == -1) {
        Py_XDECREF(temp);
        return NULL;
    }

    PyObject *iter = PyObject_GetIter(temp);
    Py_XDECREF(temp);
    return iter;
}


static int
bst_contains(BinarySearchTreeObject *self, PyObject *value)
{
    Node *cur = self->root;
    while (cur) {
        switch (PyObject_RichCompareBool(value, cur->value, Py_LT)) {
        case 1:
            cur = cur->left;
            break;
        case 0:
            switch (PyObject_RichCompareBool(cur->value, value, Py_LT)) {
            case 1:
                cur = cur->right;
                break;
            case 0:
                return 1;
            default:
                return -1;
            }
            break;
        default:
            return -1;
        }
    }
    return 0;
}


static PyObject *
bst_add(BinarySearchTreeObject *self, PyObject *value)
{
    Node *cur = self->root;
    if (cur == NULL) {
        cur = PyMem_Calloc(sizeof(Node), 1);
        if (cur == NULL) { return NULL; }
        Py_INCREF(value);
        cur->value = value;
        self->root = cur;
        Py_RETURN_NONE;
    }

    while (1) {
        switch (PyObject_RichCompareBool(value, cur->value, Py_LT)) {
        case 1:
            /* value < cur->value */
            if (cur->left == NULL) {
                cur->left = PyMem_Calloc(sizeof(Node), 1);
                if (cur->left == NULL) { return NULL; }
                Py_INCREF(value);
                cur->left->value = value;
                Py_RETURN_NONE;
            } else {
                cur = cur->left;
            }
            break;
        case 0:
            switch (PyObject_RichCompareBool(cur->value, value, Py_LT)) {
            case 1:
                /* cur->value < value */
                if (cur->right == NULL) {
                    cur->right = PyMem_Calloc(sizeof(Node), 1);
                    if (cur->right == NULL) { return NULL; }
                    Py_INCREF(value);
                    cur->right->value = value;
                    Py_RETURN_NONE;
                } else {
                    cur = cur->right;
                }
                break;
            case 0:
                /* value == cur->value */
                Py_RETURN_NONE;
                break;
            default:
                return NULL;
            }
            break;
        default:
            return NULL;
        }
    }
}


static PyObject *
simple_class_getitem(PyObject *type, PyObject *item)
{
    Py_INCREF(type);
    return type;
}


static PyMethodDef bst_methods[] = {
    {"add", (PyCFunction)bst_add, METH_O, NULL},
    {"__class_getitem__", simple_class_getitem, METH_O|METH_CLASS, NULL},
    {NULL, NULL, 0, NULL} /* Sentinel */
};


static PyType_Slot bst_slots[] = {
    {Py_tp_methods, bst_methods},
    {Py_tp_new, bst_new},
    {Py_tp_iter, (getiterfunc)bst_iter},
    {Py_tp_dealloc, bst_dealloc},
    {Py_tp_traverse, bst_traverse},
    {Py_tp_clear, bst_clear},
    {Py_sq_contains, bst_contains},
    {Py_sq_length, bst_len},
    {0, 0},
};


static PyType_Spec bst_spec = {
    .name = "fgshun_bst.BinarySearchTree",
    .basicsize = sizeof(BinarySearchTreeObject),
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .slots = bst_slots,
};


static int moddef_exec(PyObject *module) {
    PyObject *bst_type;

    bst_type = PyType_FromSpec(&bst_spec);
    if (!bst_type) { goto error; }
    if (PyModule_AddObject(module, "BinarySearchTree", bst_type)) { goto error; }
    return 0;
error:
    Py_XDECREF(bst_type);
    Py_DECREF(module);
    return -1;
}


static PyModuleDef_Slot moddef_slots[] = {
    {Py_mod_exec, moddef_exec},
    {0, NULL}
};


static struct PyModuleDef moddef_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_bst",
    .m_slots = moddef_slots,
};


PyMODINIT_FUNC PyInit__bst(void) {
    return PyModuleDef_Init(&moddef_module);
}
