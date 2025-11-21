#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *cvarint_encode(PyObject *self, PyObject *args)
{
    int i = 0;
    unsigned long n, part;
    unsigned char out[10];

    if (!PyArg_ParseTuple(args, "k", &n))
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
    int n = 0;
    Py_buffer varint;

    if (!PyArg_ParseTuple(args, "y*", &varint))
    {
        return NULL;
    };

    for (int i = varint.len - 1; i >= 0; i--)
    {
        n <<= 7;
        n |= ((char *)varint.buf)[i] & 0x7F;
    }

    PyBuffer_Release(&varint);
    return PyLong_FromUnsignedLong(n);
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

// DOCS
// https://docs.python.org/3/extending/extending.html
// https://docs.python.org/3/c-api/arg.html
// https://docs.python.org/3/c-api/stable.html