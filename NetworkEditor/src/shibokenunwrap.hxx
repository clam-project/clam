// Copied from ipyclam/src/shibokenunwrap.hxx
#ifndef shibokenpunwrap_hxx
#define shibokenpunwrap_hxx

#ifdef CLAM_NETWORKEDITOR_USE_PYTHON
#include <Python.h>

class QObject;
void * shibokenUnwrap(PyObject *obj_ptr);
PyObject * shibokenWrap(QObject * object);
#endif

#endif//shibokenpunwrap_hxx
