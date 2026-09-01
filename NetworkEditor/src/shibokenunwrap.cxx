#ifdef CLAM_NETWORKEDITOR_USE_PYTHON
// Copied from ipyclam/src/shibokenunwrap.cxx
#include "shibokenunwrap.hxx"
#include <shiboken6/basewrapper.h>
#include <iostream>
#include <QObject>

static void * error(const std::string & msg)
{
	std::cerr << msg << std::endl;
	return 0;
}

void * shibokenUnwrap(PyObject * pyobject)
{
	if (not Shiboken::Object::checkType(pyobject))
		return error("Not a shiboken object");

	SbkObject * sbkobject = (SbkObject *) pyobject;

	PyTypeObject * type = Py_TYPE(pyobject);

	void * cppobject = Shiboken::Object::cppPointer(sbkobject, type);
	if (not cppobject)
		return error("Not a QObject");

	return cppobject;
}

PyObject * shibokenWrap(QObject * qobject)
{
	return Shiboken::Object::newObject(
		Shiboken::ObjectType::typeForTypeName("QWidget"), qobject, false, true);
}
#endif
