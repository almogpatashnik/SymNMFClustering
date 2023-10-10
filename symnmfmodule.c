# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "symnmf.h"


typedef double* (*CalcFunction)(double*, int, int);  /*Define a function pointer type*/ 

static PyObject *symnmf_calculator(PyObject* self, PyObject* args) {
    
    /*varibales from python*/
    PyObject *lst_h, *lst_w;
    int N, k;
    PyObject *item;
    
    /*varibales we are going to work with and send it to c*/
    double *h, *w;

    /*varibales for function:*/
    double num;/*will contain the content of the float to double we are converting*/
    int len_h_;/*the length of the list representing h that recived from  python*/
    int len_w_;/*same as above for w*/
    int len_res; /*same as above for the result we get from c*/
    int i; /*the parameter for the loop*/

    /*the value we will return*/
    PyObject* python_float;
    PyObject *ret; /*ret is the result of symnmf in python*/
    double* res;/*the result of symnmf in c*/


    /*parsing the varibales*/
    if (!PyArg_ParseTuple(args, "OOii", &lst_h, &lst_w,&k, &N)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

   /*build the required varibles that are going to be sent for the c implementation of the sym method*/
    len_h_ = PyObject_Length(lst_h);
    len_w_ = PyObject_Length(lst_w);
    if (len_h_ < 0 || len_w_ < 0) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    h = (double *)malloc(len_h_ * sizeof(double));
    if (h == NULL) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    w = (double *)malloc(len_w_ * sizeof(double));
    if (w == NULL) {
        free(h);
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    /*initialize h and w*/
    for (i = 0; i < len_h_; i++) {
        item = PyList_GetItem(lst_h, i);
        num = PyFloat_AsDouble(item);
        if (num == -1.0 && PyErr_Occurred()) {
            free(w);
            free(h);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
            return NULL;
        }
        h[i] = num;
    }  

    for (i = 0; i < len_w_; i++) {
        item = PyList_GetItem(lst_w, i);
        num = PyFloat_AsDouble(item);
        if (num == -1.0 && PyErr_Occurred()) {
            free(w);
            free(h);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
            return NULL;
        }
        w[i] = num;
    }  

    /*we are sending the data into our sym method implementation*/
    res = calc_symnmf(h,w,k,N);

    /*in case that error has occured in matricesmethods.c*/
    if(res == NULL){
        free(w);
        free(h);
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    len_res = N*k;
    ret = PyList_New(len_res); 

    if (ret == NULL)/*in case of error PyList_New returns NULL*/
    {
        free(w);
        free(res);
        /* h is being freed in matricesmethods.c, once the function terminates*/
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }
    
    for (int i = 0; i < len_res; ++i)
    {
        python_float = Py_BuildValue("f", res[i]);
        if (python_float == NULL) {
            free(w);
            free(res);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
            return NULL;
        }
        PyList_SetItem(ret, i, python_float);
    }

    
    free(w);
    free(res);
    
    return ret;
   
}


PyObject* calculate(PyObject* self, PyObject* args, CalcFunction calc_func) {
    
    /*varibales from python*/
    PyObject* lst_;
    int d, N;


    PyObject *item;
    
    /*varibales we are going to work with and send it to c*/
    double *vectors;

    /*varibales for function:*/
    double num;
    int len_lst_;
    int len_res; 
    int i; /*the paramter for the loop*/

    /*the value we will return*/
    PyObject* python_float;
    PyObject *ret;
    double* res;


    /*parsing the varibales*/
    if (!PyArg_ParseTuple(args, "Oii", &lst_, &N,&d)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

   /*build the required varibles that are going to be sent for the c implementation of the sym method*/
    len_lst_ = PyObject_Length(lst_); /* =N*dim */
    if (len_lst_ < 0) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    vectors = (double *)malloc(len_lst_ * sizeof(double));
    if (vectors == NULL) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    /*initialize array*/
    for (i = 0; i < len_lst_; i++) {
        item = PyList_GetItem(lst_, i);
        num = PyFloat_AsDouble(item);
        if (num == -1.0 && PyErr_Occurred()) {
            free(vectors);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
            return NULL;
        }
        vectors[i] = num;
    }  

    /*we are sending the data into our sym method implementation*/
    res = calc_func(vectors, N, d); 

    /*in case that error has occured in matricesmethods.c*/
    if(res == NULL){
        free(vectors);
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }

    len_res = N*N;
    ret = PyList_New(len_res); 

    if (ret == NULL)/*in case of error PyList_New returns NULL*/
    {
        free(vectors);
        free(res);
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }
    
    /*building the result into python*/
    for (int i = 0; i < len_res; ++i)
    {
        python_float = Py_BuildValue("f", res[i]);
        if (python_float == NULL) {
            free(vectors);
            free(res);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
            return NULL;
        }
        PyList_SetItem(ret, i, python_float);
    }

    
    free(vectors);
    free(res);
    
    return ret;
   
}

static PyObject* sym_calculator(PyObject* self, PyObject* args) {
    return calculate(self, args, calc_sym);
}

static PyObject* ddg_calculator(PyObject* self, PyObject* args) {
    return calculate(self, args, calc_ddg);
}

static PyObject* norm_calculator(PyObject* self, PyObject* args) {
    return calculate(self, args, calc_norm);
}


static PyMethodDef Methods[] = {
    {
        "sym", /*name exposed to Python*/ 
        sym_calculator, /* C wrapper function*/
        METH_VARARGS, /* received variable args*/
        "Input: list of vectors , N , d. Output: Calculate the similarity matrix" /*documentation*/
    }, {
        "ddg", /* name exposed to Python*/
        ddg_calculator, /* C wrapper function */
        METH_VARARGS, /*received variable args*/ 
        "Input: list of vectors , N , d. Output: Calculate and output the Diagonal Degree Matrix" 
    }, {
        "norm", /*name exposed to Python*/ 
        norm_calculator, /* C wrapper function*/
        METH_VARARGS, /*received variable args*/
        "Input: list of vectors , N , d. Output: Calculate and output the normalized similarity matrix"
    },{
        "symnmf", /*name exposed to Python*/
        symnmf_calculator, /*C wrapper function*/
        METH_VARARGS, /*received variable args*/
        "Input: initial h , w , k, N. Perform full the symNMF algorithm."
    }, {
        NULL, NULL, 0, NULL
    }
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule", /*name of module exposed to python*/
    "the module calculates several matrices that belongs to symnmf algorithm for given data", /*module documentation*/
    -1,  
    Methods /*adding the methods from above*/
};

PyMODINIT_FUNC PyInit_symnmfmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef); /*creating the module*/
    if (!m) { /*in case that something went worng*/
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred");
        return NULL;
    }
    return m;
}


