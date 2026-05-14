#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "queue.h"

/*
    PyQueue is a real Python object that wraps your C Queue*
*/
typedef struct {
    PyObject_HEAD
    Queue* q;
} PyQueue;


/* -------------------------
   Destructor helpers
--------------------------*/

static void pyobject_destructor(void* ptr)
{
    Py_DECREF((PyObject*)ptr);
}


/* -------------------------
   PyQueue methods
--------------------------*/

static int PyQueue_init(PyQueue* self, PyObject* args, PyObject* kwds)
{
    self->q = queue_init();

    if (!self->q) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate Queue");
        return -1;
    }

    return 0;
}


/*
    put(self, obj)
*/
static PyObject* PyQueue_put(PyQueue* self, PyObject* obj)
{
    if (!self->q) {
        PyErr_SetString(PyExc_RuntimeError, "Queue not initialized");
        return NULL;
    }

    Py_INCREF(obj);

    queue_put(self->q, (void*)obj);

    Py_RETURN_NONE;
}


/*
    get(self)
*/
static PyObject* PyQueue_get(PyQueue* self, PyObject* Py_UNUSED(ignored))
{
    if (!self->q) {
        PyErr_SetString(PyExc_RuntimeError, "Queue not initialized");
        return NULL;
    }

    QueueNode* node = queue_get(self->q);

    if (!node) {
        Py_RETURN_NONE;
    }

    PyObject* obj = (PyObject*)node->data;

    free(node);

    return obj;
}


/*
    length(self)
*/
static PyObject* PyQueue_length(PyQueue* self, PyObject* Py_UNUSED(ignored))
{
    if (!self->q)
        return PyLong_FromLong(0);

    size_t len = queue_length(self->q);

    return PyLong_FromSize_t(len);
}


/*
    empty(self)
*/
static PyObject* PyQueue_empty(PyQueue* self, PyObject* Py_UNUSED(ignored))
{
    if (!self->q)
        Py_RETURN_TRUE;

    if (is_queue_empty(self->q))
        Py_RETURN_TRUE;

    Py_RETURN_FALSE;
}


/* -------------------------
   dealloc
--------------------------*/

static void PyQueue_dealloc(PyQueue* self)
{
    if (self->q) {
        queue_free(self->q, pyobject_destructor);
        self->q = NULL;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}


/* -------------------------
   Methods table
--------------------------*/

static PyMethodDef PyQueue_methods[] = {
    {"put", (PyCFunction)PyQueue_put, METH_O, "Put an item into the queue"},
    {"get", (PyCFunction)PyQueue_get, METH_NOARGS, "Get an item from the queue"},
    {"length", (PyCFunction)PyQueue_length, METH_NOARGS, "Get queue length"},
    {"empty", (PyCFunction)PyQueue_empty, METH_NOARGS, "Check if queue is empty"},
    {NULL, NULL, 0, NULL}
};


/* -------------------------
   Type definition
--------------------------*/

static PyTypeObject PyQueueType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "cqueue.Queue",
    .tp_basicsize = sizeof(PyQueue),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "C Queue wrapper",
    .tp_methods = PyQueue_methods,
    .tp_init = (initproc)PyQueue_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)PyQueue_dealloc,
};


/* -------------------------
   module definition
--------------------------*/

static PyModuleDef cqueuemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "cqueue",
    .m_doc = "Python wrapper for C queue",
    .m_size = -1,
};


/* -------------------------
   module init
--------------------------*/

PyMODINIT_FUNC PyInit_cqueue(void)
{
    PyObject* m;

    if (PyType_Ready(&PyQueueType) < 0)
        return NULL;

    m = PyModule_Create(&cqueuemodule);

    if (!m)
        return NULL;

    Py_INCREF(&PyQueueType);

    if (PyModule_AddObject(m, "Queue", (PyObject*)&PyQueueType) < 0) {
        Py_DECREF(&PyQueueType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
