#define PY_SSIZE_T_CLEAN
#include <Python.h>


static int bst_exec(PyObject *module) {
    if (PyModule_AddIntConstant(module, "one", 1)) { goto error; }
    return 0;
error:
    Py_DECREF(module);
    return -1;
}


static PyModuleDef_Slot bst_slots[] = {
    {Py_mod_exec, bst_exec},
    {0, NULL}
};


static struct PyModuleDef bst_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_bst",
    .m_slots = bst_slots,
};


PyMODINIT_FUNC PyInit__bst(void) {
    return PyModuleDef_Init(&bst_module);
}
