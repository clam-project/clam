#include "sipunwrap.hxx"
#include <boost/python.hpp>
#include <iostream>

static PyObject * incref(PyObject * object)
{
	Py_INCREF(object);
	return object;
}

void * sipUnwrap(PyObject *obj_ptr)
{
	PyObject *sip_module = PyImport_ImportModule("PyQt6.sip");
	if (!sip_module) return nullptr;

	PyObject *result = PyObject_CallMethod(sip_module, "unwrapinstance", "O", obj_ptr);
	Py_DECREF(sip_module);
	if (!result) { PyErr_Clear(); return nullptr; }

	void *ptr = PyLong_AsVoidPtr(result);
	Py_DECREF(result);
	return ptr;
}


PyObject * sipWrap(QObject * object)
{
	if (!object) return incref(Py_None);

	PyObject *core = PyImport_ImportModule("PyQt6.QtCore");
	if (!core) return incref(Py_None);

	PyObject *qobject_type = PyObject_GetAttrString(core, "QObject");
	Py_DECREF(core);
	if (!qobject_type) return incref(Py_None);

	PyObject *sip_module = PyImport_ImportModule("PyQt6.sip");
	if (!sip_module) { Py_DECREF(qobject_type); return incref(Py_None); }

	long addr = reinterpret_cast<long>(object);
	PyObject *sip_obj = PyObject_CallMethod(sip_module, "wrapinstance", "lO", addr, qobject_type);
	Py_DECREF(sip_module);
	Py_DECREF(qobject_type);
	if (!sip_obj) return incref(Py_None);
	return sip_obj;
}
