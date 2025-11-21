#define PY_SSIZE_T_CLEAN
#include <Python.h>

// https://docs.python.org/3/extending/extending.html

static PyObject *cvarint_encode(PyObject *self, PyObject *args)
{
    int i = 0;
    int n, part;
    unsigned char out[10];

    if (!PyArg_ParseTuple(args, "b", &n))
    {
        return NULL;
    };

    while (n > 0)
    {
        part = n & 0x7F;
        n >>= 7;
        if (n > 0)
        {
            part |= 0x80;
        }
        out[i++] = part;
    }

    return PyBytes_FromStringAndSize((char *)out, i);
}

static PyObject *cvarint_decode(PyObject *self, PyObject *args)
{
    return NULL;
}

static PyMethodDef CVarintMethods[] = {
    {"encode", cvarint_encode, METH_VARARGS, "Encode an integer as varint."},
    {"decode", cvarint_decode, METH_VARARGS,
     "Decode varint bytes to an integer."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef cvarintmodule = {
    PyModuleDef_HEAD_INIT, "cvarint",
    "A C implementation of protobuf varint encoding", -1, CVarintMethods};

PyMODINIT_FUNC PyInit_cvarint(void) { return PyModule_Create(&cvarintmodule); }
