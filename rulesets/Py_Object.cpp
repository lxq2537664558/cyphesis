#include <stdio.h>
#include <unistd.h>

#include <Python.h>

#include "Python_API.h"

/*
 * Beginning of Object methods section.
 */

/*
 * Object methods structure.
 */

PyMethodDef Object_methods[] = {
	/* {"demo",        (PyCFunction)Xxo_demo,  1}, */
	{NULL,          NULL}           /* sentinel */
};

/*
 * Beginning of Object standard methods section.
 */

static void Object_dealloc(AtlasObject *self)
{
    if (self->m_obj != NULL) {
        delete self->m_obj;
    }
    Py_XDECREF(self->Object_attr);
    PyMem_DEL(self);
}

static PyObject * Object_getattr(AtlasObject *self, char *name)
{
    return Py_FindMethod(Object_methods, (PyObject *)self, name);
}

static int Object_setattr( AtlasObject *self, char *name, PyObject *v)
{
    return 0;
}

PyTypeObject Object_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/*ob_size*/
	"Object",			/*tp_name*/
	sizeof(AtlasObject),		/*tp_basicsize*/
	0,				/*tp_itemsize*/
	/* methods */
	(destructor)Object_dealloc,	/*tp_dealloc*/
	0,				/*tp_print*/
	(getattrfunc)Object_getattr,	/*tp_getattr*/
	(setattrfunc)Object_setattr,	/*tp_setattr*/
	0,				/*tp_compare*/
	0,				/*tp_repr*/
	0,				/*tp_as_number*/
	0,				/*tp_as_sequence*/
	0,				/*tp_as_mapping*/
	0,				/*tp_hash*/
};

/*
 * Beginning of Object creation functions section.
 */

AtlasObject * newAtlasObject(PyObject *arg)
{
	AtlasObject * self;
	self = PyObject_NEW(AtlasObject, &Object_Type);
	if (self == NULL) {
		return NULL;
	}
	self->Object_attr = NULL;
	return self;
}

/*
 * Utility functions to munge between Object related types and python types
 */

PyObject * MapType_asPyObject(Object::MapType & map)
{
    PyObject * args_pydict = PyDict_New();
    Object::MapType::iterator I;
    AtlasObject * item;
    for(I=map.begin();I!=map.end();I++) {
        const string & key = I->first;
        item = newAtlasObject(NULL);
        if (item == NULL) {
            PyErr_SetString(PyExc_TypeError,"error creating map");
            return NULL;
        }
        item->m_obj = new Object(I->second);
        PyDict_SetItemString(args_pydict,(char *)key.c_str(),(PyObject *)item);
    }
    return(args_pydict);
}

PyObject * ListType_asPyObject(Object::ListType & list)
{
    PyObject * args_pylist = PyList_New(list.size());
    Object::ListType::iterator I;
    int j=0;
    AtlasObject * item;
    for(I=list.begin();I!=list.end();I++,j++) {
        item = newAtlasObject(NULL);
        if (item == NULL) {
            PyErr_SetString(PyExc_TypeError,"error creating list");
            return NULL;
        }
        item->m_obj = new Object(*I);
        PyList_SetItem(args_pylist, j, (PyObject *)item);
    }
    return(args_pylist);
}

PyObject * Object_asPyObject(Object & obj)
{
    PyObject * ret = NULL;
    switch (obj.GetType()) {
        case Object::TYPE_INT:
            ret = PyLong_FromLong(obj.AsInt());
            break;
        case Object::TYPE_FLOAT:
            ret = PyFloat_FromDouble(obj.AsFloat());
            break;
        case Object::TYPE_STRING:
            ret = PyString_FromString(obj.AsString().c_str());
            break;
        case Object::TYPE_MAP:
            ret = MapType_asPyObject(obj.AsMap());
            break;
        case Object::TYPE_LIST:
            ret = ListType_asPyObject(obj.AsList());
            break;
        default:
            break;
    }
    return(ret);
}

Object::ListType PyListObject_asListType(PyObject * list)
{
    Object::ListType argslist;
    AtlasObject * item;
    for(int i = 0; i < PyList_Size(list); i++) {
        item = (AtlasObject *)PyList_GetItem(list, i);
        if ((PyTypeObject*)PyObject_Type((PyObject *)item) != &Object_Type) {
            PyErr_SetString(PyExc_TypeError,"list contains non Atlas Object");
            return Object::ListType();
        }
        argslist.push_back(*(item->m_obj));
    }
    return(argslist);
}

Object::MapType PyDictObject_asMapType(PyObject * dict)
{
    Object::MapType argsmap;
    AtlasObject * item;
    PyObject * list = PyDict_Keys(dict);
    PyObject * key;
    for(int i = 0; i < PyDict_Size(list); i++) {
        key = PyList_GetItem(list, i);
        item = (AtlasObject *)PyDict_GetItem(dict, key);
        if ((PyTypeObject*)PyObject_Type((PyObject *)item) != &Object_Type) {
            PyErr_SetString(PyExc_TypeError,"dict contains non Atlas Object");
            return Object::MapType();
        }
        argsmap[PyString_AsString(key)] = *(item->m_obj);
    }
    return(argsmap);
}

Object PyObject_asObject(PyObject * o)
{
    if (PyInt_Check(o)) {
        return(Object((int)PyLong_AsLong(o)));
    }
    if (PyFloat_Check(o)) {
        return(Object(PyFloat_AsDouble(o)));
    }
    if (PyString_Check(o)) {
        return(Object(PyString_AsString(o)));
    }
    if (PyList_Check(o)) {
        return(Object(PyListObject_asListType(o)));
    }
    if (PyDict_Check(o)) {
        return(Object(PyDictObject_asMapType(o)));
    }
    if ((PyTypeObject*)PyObject_Type((PyObject *)o) == &Object_Type) {
        AtlasObject * obj = (AtlasObject *)o;
        return *(obj->m_obj);
    }
    return Object();
}
