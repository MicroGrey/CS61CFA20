#include "numc.h"
#include <structmember.h>

PyTypeObject Matrix61cType;

/* Helper functions for initalization of matrices and vectors */

/*
 * Return a tuple given rows and cols
 */
PyObject *get_shape(int rows, int cols) {
  if (rows == 1 || cols == 1) {
    return PyTuple_Pack(1, PyLong_FromLong(rows * cols));
  } else {
    return PyTuple_Pack(2, PyLong_FromLong(rows), PyLong_FromLong(cols));
  }
}
/*
 * Matrix(rows, cols, low, high). Fill a matrix random double values
 */
int init_rand(PyObject *self, int rows, int cols, unsigned int seed, double low,
              double high) {
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    rand_matrix(new_mat, seed, low, high);
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * Matrix(rows, cols, val). Fill a matrix of dimension rows * cols with val
 */
int init_fill(PyObject *self, int rows, int cols, double val) {
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed)
        return alloc_failed;
    else {
        fill_matrix(new_mat, val);
        ((Matrix61c *)self)->mat = new_mat;
        ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    }
    return 0;
}

/*
 * Matrix(rows, cols, 1d_list). Fill a matrix with dimension rows * cols with 1d_list values
 */
int init_1d(PyObject *self, int rows, int cols, PyObject *lst) {
    if (rows * cols != PyList_Size(lst)) {
        PyErr_SetString(PyExc_ValueError, "Incorrect number of elements in list");
        return -1;
    }
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(new_mat, i, j, PyFloat_AsDouble(PyList_GetItem(lst, count)));
            count++;
        }
    }
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * Matrix(2d_list). Fill a matrix with dimension len(2d_list) * len(2d_list[0])
 */
int init_2d(PyObject *self, PyObject *lst) {
    int rows = PyList_Size(lst);
    if (rows == 0) {
        PyErr_SetString(PyExc_ValueError,
                        "Cannot initialize numc.Matrix with an empty list");
        return -1;
    }
    int cols;
    if (!PyList_Check(PyList_GetItem(lst, 0))) {
        PyErr_SetString(PyExc_ValueError, "List values not valid");
        return -1;
    } else {
        cols = PyList_Size(PyList_GetItem(lst, 0));
    }
    for (int i = 0; i < rows; i++) {
        if (!PyList_Check(PyList_GetItem(lst, i)) ||
                PyList_Size(PyList_GetItem(lst, i)) != cols) {
            PyErr_SetString(PyExc_ValueError, "List values not valid");
            return -1;
        }
    }
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(new_mat, i, j,
                PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(lst, i), j)));
        }
    }
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * This deallocation function is called when reference count is 0
 */
void Matrix61c_dealloc(Matrix61c *self) {
    deallocate_matrix(self->mat);
    Py_TYPE(self)->tp_free(self);
}

/* For immutable types all initializations should take place in tp_new */
PyObject *Matrix61c_new(PyTypeObject *type, PyObject *args,
                        PyObject *kwds) {
    /* size of allocated memory is tp_basicsize + nitems*tp_itemsize*/
    Matrix61c *self = (Matrix61c *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

/*
 * This matrix61c type is mutable, so needs init function. Return 0 on success otherwise -1
 */
int Matrix61c_init(PyObject *self, PyObject *args, PyObject *kwds) {
    /* Generate random matrices */
    if (kwds != NULL) {
        PyObject *rand = PyDict_GetItemString(kwds, "rand");
        if (!rand) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
        if (!PyBool_Check(rand)) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
        if (rand != Py_True) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }

        PyObject *low = PyDict_GetItemString(kwds, "low");
        PyObject *high = PyDict_GetItemString(kwds, "high");
        PyObject *seed = PyDict_GetItemString(kwds, "seed");
        double double_low = 0;
        double double_high = 1;
        unsigned int unsigned_seed = 0;

        if (low) {
            if (PyFloat_Check(low)) {
                double_low = PyFloat_AsDouble(low);
            } else if (PyLong_Check(low)) {
                double_low = PyLong_AsLong(low);
            }
        }

        if (high) {
            if (PyFloat_Check(high)) {
                double_high = PyFloat_AsDouble(high);
            } else if (PyLong_Check(high)) {
                double_high = PyLong_AsLong(high);
            }
        }

        if (double_low >= double_high) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }

        // Set seed if argument exists
        if (seed) {
            if (PyLong_Check(seed)) {
                unsigned_seed = PyLong_AsUnsignedLong(seed);
            }
        }

        PyObject *rows = NULL;
        PyObject *cols = NULL;
        if (PyArg_UnpackTuple(args, "args", 2, 2, &rows, &cols)) {
            if (rows && cols && PyLong_Check(rows) && PyLong_Check(cols)) {
                return init_rand(self, PyLong_AsLong(rows), PyLong_AsLong(cols), unsigned_seed, double_low,
                                 double_high);
            }
        } else {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
    }
    PyObject *arg1 = NULL;
    PyObject *arg2 = NULL;
    PyObject *arg3 = NULL;
    if (PyArg_UnpackTuple(args, "args", 1, 3, &arg1, &arg2, &arg3)) {
        /* arguments are (rows, cols, val) */
        if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) && (PyLong_Check(arg3)
                || PyFloat_Check(arg3))) {
            if (PyLong_Check(arg3)) {
                return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyLong_AsLong(arg3));
            } else
                return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyFloat_AsDouble(arg3));
        } else if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) && PyList_Check(arg3)) {
            /* Matrix(rows, cols, 1D list) */
            return init_1d(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), arg3);
        } else if (arg1 && PyList_Check(arg1) && arg2 == NULL && arg3 == NULL) {
            /* Matrix(rows, cols, 1D list) */
            return init_2d(self, arg1);
        } else if (arg1 && arg2 && PyLong_Check(arg1) && PyLong_Check(arg2) && arg3 == NULL) {
            /* Matrix(rows, cols, 1D list) */
            return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), 0);
        } else {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return -1;
    }
}

/*
 * List of lists representations for matrices
 */
PyObject *Matrix61c_to_list(Matrix61c *self) {
    int rows = self->mat->rows;
    int cols = self->mat->cols;
    PyObject *py_lst = NULL;
    if (self->mat->is_1d) {  // If 1D matrix, print as a single list
        py_lst = PyList_New(rows * cols);
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                PyList_SetItem(py_lst, count, PyFloat_FromDouble(get(self->mat, i, j)));
                count++;
            }
        }
    } else {  // if 2D, print as nested list
        py_lst = PyList_New(rows);
        for (int i = 0; i < rows; i++) {
            PyList_SetItem(py_lst, i, PyList_New(cols));
            PyObject *curr_row = PyList_GetItem(py_lst, i);
            for (int j = 0; j < cols; j++) {
                PyList_SetItem(curr_row, j, PyFloat_FromDouble(get(self->mat, i, j)));
            }
        }
    }
    return py_lst;
}

PyObject *Matrix61c_class_to_list(Matrix61c *self, PyObject *args) {
    PyObject *mat = NULL;
    if (PyArg_UnpackTuple(args, "args", 1, 1, &mat)) {
        if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
            PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
            return NULL;
        }
        Matrix61c* mat61c = (Matrix61c*)mat;
        return Matrix61c_to_list(mat61c);
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }
}

/*
 * Add class methods
 */
PyMethodDef Matrix61c_class_methods[] = {
    {"to_list", (PyCFunction)Matrix61c_class_to_list, METH_VARARGS, "Returns a list representation of numc.Matrix"},
    {NULL, NULL, 0, NULL}
};

/*
 * Matrix61c string representation. For printing purposes.
 */
PyObject *Matrix61c_repr(PyObject *self) {
    PyObject *py_lst = Matrix61c_to_list((Matrix61c *)self);
    return PyObject_Repr(py_lst);
}

/* NUMBER METHODS */

/*
 * Add the second numc.Matrix (Matrix61c) object to the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject *Matrix61c_add(Matrix61c *self, PyObject *args) {
    /* TODO: YOUR CODE HERE */
    PyObject* mat = args;

    if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
        return NULL;
    }
    Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* result = NULL;
    if (allocate_matrix(&result, self->mat->rows, self->mat->cols)){
        return NULL;
    }
    if (add_matrix(result, self->mat, ((Matrix61c *)mat)->mat)){
        return NULL;
    }
    mat61c->mat = result;
    mat61c->shape = self->shape;
    return (PyObject *)mat61c;
}

/*
 * Substract the second numc.Matrix (Matrix61c) object from the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject *Matrix61c_sub(Matrix61c* self, PyObject* args) {
    /* TODO: YOUR CODE HERE */
    PyObject* mat = args;

    if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
        return NULL;
    }
    Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* result = NULL;
    if (allocate_matrix(&result, self->mat->rows, self->mat->cols)){
        return NULL;
    }
    if (sub_matrix(result, self->mat, ((Matrix61c *)mat)->mat)){
        return NULL;
    }
    mat61c->mat = result;
    mat61c->shape = self->shape;
    return (PyObject *)mat61c;
}

/*
 * NOT element-wise multiplication. The first operand is self, and the second operand
 * can be obtained by casting `args`.
 */
PyObject *Matrix61c_multiply(Matrix61c* self, PyObject *args) {
    /* TODO: YOUR CODE HERE */
    PyObject* mat = args;

    if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
        return NULL;
    }
    Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* result = NULL;
    if (allocate_matrix(&result, self->mat->rows, self->mat->cols)){
        return NULL;
    }
    if (mul_matrix(result, self->mat, ((Matrix61c *)mat)->mat)){
        return NULL;
    }
    mat61c->mat = result;
    mat61c->shape = self->shape;
    return (PyObject *)mat61c;
}

/*
 * Negates the given numc.Matrix.
 */
PyObject *Matrix61c_neg(Matrix61c* self) {
    /* TODO: YOUR CODE HERE */
    Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* result = NULL;
    if (allocate_matrix(&result, self->mat->rows, self->mat->cols)){
        return NULL;
    }
    if (neg_matrix(result, self->mat)) {
        return NULL;
    }
    mat61c->mat = result;
    mat61c->shape = self->shape;
    return (PyObject *)mat61c;
}

/*
 * Take the element-wise absolute value of this numc.Matrix.
 */
PyObject *Matrix61c_abs(Matrix61c *self) {
    /* TODO: YOUR CODE HERE */
    Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* result = NULL;
    if (allocate_matrix(&result, self->mat->rows, self->mat->cols)){
        return NULL;
    }
    if (abs_matrix(result, self->mat)) {
        return NULL;
    }
    mat61c->mat = result;
    mat61c->shape = self->shape;
    return (PyObject *)mat61c;
}

/*
 * Raise numc.Matrix (Matrix61c) to the `pow`th power. You can ignore the argument `optional`.
 */
PyObject *Matrix61c_pow(Matrix61c *self, PyObject *pow, PyObject *optional) {
    /* TODO: YOUR CODE HERE */
    Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* result = NULL;
    if (allocate_matrix(&result, self->mat->rows, self->mat->cols)){
        return NULL;
    }
    if (!PyLong_Check(pow)) {
        PyErr_SetString(PyExc_TypeError, "pow must be an integer");
        return NULL;
    }
    int exponent = (int)PyLong_AsLong(pow);
    if (pow_matrix(result, self->mat, exponent)) {
        return NULL;
    }
    mat61c->mat = result;
    mat61c->shape = self->shape;
    return (PyObject *)mat61c;
}

/*
 * Create a PyNumberMethods struct for overloading operators with all the number methods you have
 * define. You might find this link helpful: https://docs.python.org/3.6/c-api/typeobj.html
 */
PyNumberMethods Matrix61c_as_number = {
    /* TODO: YOUR CODE HERE */
    .nb_add = (binaryfunc)Matrix61c_add,
    .nb_subtract = (binaryfunc)Matrix61c_sub,
    .nb_multiply = (binaryfunc)Matrix61c_multiply,
    .nb_negative = (unaryfunc)Matrix61c_neg,
    .nb_absolute = (unaryfunc)Matrix61c_abs,
    .nb_power = (ternaryfunc)Matrix61c_pow,
    .nb_inplace_add = NULL,    // 不实现就地加法 +=
    .nb_inplace_subtract = NULL,
    .nb_inplace_multiply = NULL
};


/* INSTANCE METHODS */

/*
 * Given a numc.Matrix self, parse `args` to (int) row, (int) col, and (double/int) val.
 * Return None in Python (this is different from returning null).
 */
PyObject *Matrix61c_set_value(Matrix61c *self, PyObject* args) {
    /* TODO: YOUR CODE HERE */
    int col, row;
    PyObject* py_val;
    if (!PyArg_ParseTuple(args, "iiO", &row, &col, &py_val)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments. Expected: (int row, int col, float/int value)");
        return NULL;
    }
    if (row < 0 || row >= self->mat->rows || col < 0 || col >= self->mat->cols) {
        PyErr_SetString(PyExc_IndexError, "Row or column index out of bounds");
        return NULL;
    }
    double val;
    if (PyFloat_Check(py_val)) {
        val = PyFloat_AsDouble(py_val);
    } else if (PyLong_Check(py_val)) {
        val = (double)PyLong_AsLong(py_val);
    } else {
        PyErr_SetString(PyExc_TypeError, "Value must be a float or integer");
        return NULL;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }
    set(self->mat, row, col, val);
    Py_INCREF(Py_None);
    return Py_None;

}

/*
 * Given a numc.Matrix `self`, parse `args` to (int) row and (int) col.
 * Return the value at the `row`th row and `col`th column, which is a Python
 * float/int.
 */
PyObject *Matrix61c_get_value(Matrix61c *self, PyObject* args) {
    /* TODO: YOUR CODE HERE */
    int col, row;
    if (!PyArg_ParseTuple(args, "ii", &row, &col)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments. Expected: (int row, int col, float/int value)");
        return NULL;
    }
    if (row < 0 || row >= self->mat->rows || col < 0 || col >= self->mat->cols) {
        PyErr_SetString(PyExc_IndexError, "Row or column index out of bounds");
        return NULL;
    }
    double val = get(self->mat, row, col);
    PyObject* py_float = PyFloat_FromDouble(val);
    return py_float;
}

/*
 * Create an array of PyMethodDef structs to hold the instance methods.
 * Name the python function corresponding to Matrix61c_get_value as "get" and Matrix61c_set_value
 * as "set"
 * You might find this link helpful: https://docs.python.org/3.6/c-api/structures.html
 */
PyMethodDef Matrix61c_methods[] = {
    /* TODO: YOUR CODE HERE */
    {
        "get", 
        (PyCFunction)Matrix61c_get_value, 
        METH_VARARGS, 
        "Get the value at specified (row, col) in the matrix"
    },
    {
        "set", 
        (PyCFunction)Matrix61c_set_value, 
        METH_VARARGS, 
        "Set the value at specified (row, col) in the matrix"
    },
    {NULL, NULL, 0, NULL}
};

/* INDEXING */

/*
 * Given a numc.Matrix `self`, index into it with `key`. Return the indexed result.
 */
PyObject *Matrix61c_subscript(Matrix61c* self, PyObject* key) {
    /* TODO: YOUR CODE HERE */
    // 处理元组索引（如 matrix[i, j] 或 matrix[start:end, step]）
    if (PyTuple_Check(key)) {
        Py_ssize_t size = PyTuple_GET_SIZE(key);
        if (size != 2) {
            PyErr_SetString(PyExc_IndexError, "Matrix subscript must have exactly two dimensions");
            return NULL;
        }

        PyObject* row_key = PyTuple_GetItem(key, 0);
        PyObject* col_key = PyTuple_GetItem(key, 1);

        // 解析行和列索引
        int row_start, row_end, row_step;
        int col_start, col_end, col_step;
        if (parse_slice(row_key, self->mat->rows, &row_start, &row_end, &row_step) != 0 ||
            parse_slice(col_key, self->mat->cols, &col_start, &col_end, &col_step) != 0) {
            return NULL;  // parse_slice 已设置异常
        }

        Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
        matrix* mat = NULL;
        if (allocate_matrix_ref(&mat, self->mat, row_start, col_start, row_end - row_start, col_end - col_start)) {
            return NULL;
        }
        Py_INCREF(self);
        mat61c->mat = mat;
        mat61c->shape = get_shape(mat->rows, mat->cols);
        return (PyObject*)mat61c;
    // 处理单索引（如 matrix[i] 或 matrix[start:end:step]）
    } else {
        int start, end, step;
        if (parse_slice(key, self->mat->rows, &start, &end, &step) != 0) {
            return NULL;
        }

        Matrix61c* mat61c = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
        matrix* mat = NULL;
        if (allocate_matrix_ref(&mat, self->mat, start, 0, end - start, self->mat->cols)) {
            return NULL;
        }
        Py_INCREF(self);
        mat61c->mat = mat;
        mat61c->shape = get_shape(mat->rows, mat->cols);
        return (PyObject*)mat61c;
    }
    
}

// 辅助函数：解析切片或整数索引
int parse_slice(PyObject* key, int dim_size, int* start, int* end, int* step) {
    if (PyIndex_Check(key)) {  // 处理整数索引
        int index = PyLong_AsLong(key);
        if (index < 0) index += dim_size;  // 支持负索引
        if (index < 0 || index >= dim_size) {
            PyErr_SetString(PyExc_IndexError, "Index out of bounds");
            return -1;
        }
        *start = index;
        *end = index + 1;
        *step = 1;
    } else if (PySlice_Check(key)) {  // 处理切片
        Py_ssize_t slice_start, slice_end, slice_step;
        if (PySlice_Unpack(key, &slice_start, &slice_end, &slice_step) < 0) {
            return -1;
        }
        // 处理负步长（需要额外检查）
        if (slice_step != 1) {
            PyErr_SetString(PyExc_NotImplementedError, "Step other than 1 is not supported");
            return -1;
        }
        // 调整索引到有效范围
        *start = clamp_index(slice_start, dim_size);
        *end = clamp_index(slice_end, dim_size);
        *step = slice_step;
    } else {  // 不支持的类型
        PyErr_SetString(PyExc_TypeError, "Indices must be integers or slices");
        return -1;
    }
    return 0;
}

// 辅助函数：将 Python 切片索引转换为有效范围
int clamp_index(Py_ssize_t idx, int dim_size) {
    if (idx < 0) {
        idx += dim_size;
        return (idx < 0) ? 0 : idx;
    } else if (idx > dim_size) {
        return dim_size;
    }
    return idx;
}

/*
 * Given a numc.Matrix `self`, index into it with `key`, and set the indexed result to `v`.
 */
int Matrix61c_set_subscript(Matrix61c* self, PyObject *key, PyObject *v) {
    /* TODO: YOUR CODE HERE */
    // 处理元组索引（如 matrix[i, j] 或 matrix[start:end, step]）
    if (PyTuple_Check(key)) {
        Py_ssize_t size = PyTuple_GET_SIZE(key);
        if (size != 2) {
            PyErr_SetString(PyExc_IndexError, "Matrix subscript must have exactly two dimensions");
            return -1;
        }

        PyObject* row_key = PyTuple_GetItem(key, 0);
        PyObject* col_key = PyTuple_GetItem(key, 1);

        // 解析行和列索引
        int row_start, row_end, row_step;
        int col_start, col_end, col_step;
        if (parse_slice(row_key, self->mat->rows, &row_start, &row_end, &row_step) != 0 ||
            parse_slice(col_key, self->mat->cols, &col_start, &col_end, &col_step) != 0) {
            return -1;  // parse_slice 已设置异常
        }

        int row_size = row_end - row_start;
        int col_size = col_end - col_start;
        if (row_size == 1 && col_size == 1) {  // 单元素索引
            double val;
            if (PyFloat_Check(v)) {
                val = PyFloat_AsDouble(v);
            } else if (PyLong_Check(v)) {
                val = (double)PyLong_AsLong(v);
            } else {
                PyErr_SetString(PyExc_TypeError, "Value must be a float or integer");
                return -1;
            }
            if (row_start < 0 || row_start >= self->mat->rows || col_start < 0 || col_start >= self->mat->cols) {
                PyErr_SetString(PyExc_IndexError, "Index out of bounds");
                return -1;
            }
            set(self->mat, row_start, col_start, val);
            return 0;
        } else {  // 切片索引
            if (!PyList_Check(v)) {
                PyErr_SetString(PyExc_TypeError, "Value must be a list");
                return -1;
            }
            if (row_size * col_size != PyList_Size(v)) {
                PyErr_SetString(PyExc_ValueError, "Incorrect number of elements in list");
                return -1;
            }
            int count = 0;
            for (int i = row_start; i < row_end; i++) {
                for (int j = col_start; j < col_end; j++) {
                    double val;
                    PyObject* py_val = PyList_GetItem(v, count);
                    if (PyFloat_Check(py_val)) {
                        val = PyFloat_AsDouble(py_val);
                    } else if (PyLong_Check(py_val)) {
                        val = (double)PyLong_AsLong(py_val);
                    } else {
                        PyErr_SetString(PyExc_TypeError, "Value must be a float or integer");
                        return -1;
                    }
                    set(self->mat, i, j, val);
                    count++;
                }
            }
            return 0;
        }
    // 处理单索引（如 matrix[i] 或 matrix[start:end:step]）
    } else {
        int row_idx = PyLong_AsLong(key);
        if (row_idx < 0 || row_idx >= self->mat->rows) {
            PyErr_SetString(PyExc_IndexError, "Row index out of range");
            return -1;
        }

        if (self->mat->is_1d == 1) {  // **单列矩阵：允许 `mat[i] = x`**
            double val;
            if (PyFloat_Check(v)) {
                val = PyFloat_AsDouble(v);
            } else if (PyLong_Check(v)) {
                val = (double)PyLong_AsLong(v);
            } else if (PyList_Check(v) && PyList_Size(v) == 1) {
                PyObject* value = PyList_GetItem(v, 0);
                val = PyFloat_AsDouble(value);
            } else {
                PyErr_SetString(PyExc_TypeError, "Value must be a float, integer, or a single-element list");
                return -1;
            }
            set(self->mat, row_idx, 0, val);
            return 0;
        }

        if (!PyList_Check(v)) {
            PyErr_SetString(PyExc_TypeError, "Expected a list for row assignment");
            return -1;
        }
        if (PyList_Size(v) != self->mat->cols) {
            PyErr_SetString(PyExc_ValueError, "Row dimension mismatch");
            return -1;
        }

        for (int j = 0; j < self->mat->cols; j++) {
            PyObject* value = PyList_GetItem(v, j);
            double val = PyFloat_AsDouble(value);
            if (PyErr_Occurred()) return -1;
            set(self->mat, row_idx, j, val);
        }
        return 0;

    }
}

PyMappingMethods Matrix61c_mapping = {
    NULL,
    (binaryfunc) Matrix61c_subscript,
    (objobjargproc) Matrix61c_set_subscript,
};

/* INSTANCE ATTRIBUTES*/
PyMemberDef Matrix61c_members[] = {
    {
        "shape", T_OBJECT_EX, offsetof(Matrix61c, shape), 0,
        "(rows, cols)"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject Matrix61cType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "numc.Matrix",
    .tp_basicsize = sizeof(Matrix61c),
    .tp_dealloc = (destructor)Matrix61c_dealloc,
    .tp_repr = (reprfunc)Matrix61c_repr,
    .tp_as_number = &Matrix61c_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,
    .tp_doc = "numc.Matrix objects",
    .tp_methods = Matrix61c_methods,
    .tp_members = Matrix61c_members,
    .tp_as_mapping = &Matrix61c_mapping,
    .tp_init = (initproc)Matrix61c_init,
    .tp_new = Matrix61c_new
};


struct PyModuleDef numcmodule = {
    PyModuleDef_HEAD_INIT,
    "numc",
    "Numc matrix operations",
    -1,
    Matrix61c_class_methods
};

/* Initialize the numc module */
PyMODINIT_FUNC PyInit_numc(void) {
    PyObject* m;

    if (PyType_Ready(&Matrix61cType) < 0)
        return NULL;

    m = PyModule_Create(&numcmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&Matrix61cType);
    PyModule_AddObject(m, "Matrix", (PyObject *)&Matrix61cType);
    printf("CS61C Fall 2020 Project 4: numc imported!\n");
    fflush(stdout);
    return m;
}