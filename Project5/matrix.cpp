#include "matrix.h"

template <typename T>
Matrix<T>::Matrix(ssize_t row, ssize_t col, T fill, int dim) : row(row), col(col), dim(dim), row_start(0), col_start(0),
                                                               step_start(0), step_row(1), step_dim(1)
{
    if (row > 0 && col > 0 && dim > 0)
    {
        DBGprint("Matrix(row,col,fill,dim)\n");
        this->data.reset(new T[row * col * dim]);
        for (size_t i = 0; i < row * col * dim; i++)
        {
            this->data[i] = fill;
        }
    }
    else
    {
        if (row <= 0)
        {
            ERROR("Invalid input row number");
        }
        else if (col <= 0)
        {
            ERROR("Invalid input column number");
        }
        else if (dim <= 0)
        {
            ERROR("Invalid input chennel number");
        }
        else
        {
            ERROR("Unknown error");
        }
    }
}

template <typename T>
Matrix<T>::Matrix(ssize_t row, ssize_t col, T *data, int dim) : row(row), col(col), dim(dim), row_start(0), col_start(0),
                                                                step_start(0), step_row(1), step_dim(1)
{
    if (data != NULL && row > 0 && col > 0 && dim > 0)
    {
        DBGprint("Matrix(row,col,data,dim)\n");
        this->data.reset(new T[row * col * dim]);
        for (size_t i = 0; i < row * col * dim; i++)
        {
            this->data[i] = data[i];
        }
    }
    else
    {
        if (data == NULL)
        {
            ERROR("Input data pointer is null");
        }
        else if (row <= 0)
        {
            ERROR("Invalid input row number");
        }
        else if (col <= 0)
        {
            ERROR("Invalid input column number");
        }
        else if (dim <= 0)
        {
            ERROR("Invalid input chennel number");
        }
        else
        {
            ERROR("Unknown error");
        }
    }
}

template <typename T>
void Matrix<T>::print()
{
    T *pdata = this->data.get();
    if (pdata == NULL)
    {
        ERROR("Data pointer of matrix is NULL");
    }
    else
    {
        DBGprint("Matrix.print():\n");
        pdata += step_start;
        for (size_t i = 0; i < row * col * dim; i++)
        {
            std::cout << *pdata << " ";
            if ((i + 1) % (row * col) == 0)
            {
                pdata += step_dim;
                std::cout << "\n"
                          << std::endl;
            }
            else if ((i + 1) % col == 0)
            {
                pdata += step_row;
                std::cout << "\n";
            }
            else
            {
                pdata++;
            }
        }
    }
}

template <typename T>
Matrix<T> Matrix<T>::clone()
{ // can create hard copy of ROI
    T *pdata = this->data.get();
    if (pdata == NULL)
    {
        ERROR("Data pointer of matrix is NULL");
    }
    else
    {
        DBGprint("Matrix.clone()\n");
        T *pdata_new = new T[row * col * dim];
        pdata += step_start;
        for (size_t i = 0; i < row * col * dim; i++)
        {
            pdata_new[i] = *pdata;
            if ((i + 1) % (row * col) == 0)
            {
                pdata += step_dim;
            }
            else if ((i + 1) % col == 0)
            {
                pdata += step_row;
            }
            else
            {
                pdata++;
            }
        }
        Matrix<T> m(row, col, pdata_new, dim);
        delete[] pdata_new;
        return m;
    }
}

template <typename T>
Matrix<T>::~Matrix()
{
    // this->~data.reset();
    DBGprint("~Matrix(), data.use_count() = %ld\n", this->data.use_count());
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &m) : row(m.row), col(m.col), dim(m.dim), row_start(m.row_start), col_start(m.col_start),
                                        step_start(m.step_start), step_row(m.step_row), step_dim(m.step_dim), data(m.data)
{
    if (m.data.get() == NULL)
    {
        ERROR("Data pointer of matrix is NULL");
    }
    else
    {
        DBGprint("Matrix soft copy: use_count = %ld\n", this->data.use_count());
    }
}

template <typename T>
template <typename U>
Matrix<T>::Matrix(const Matrix<U> &m) : row(m.getRow()), col(m.getCol()), dim(m.getDim()), row_start(m.getRow_start()), col_start(m.getCol_start()),
                                        step_start(m.getStep_start()), step_row(m.getStep_row()), step_dim(m.getStep_dim())
{
    U *pdata = m.getDataptr();
    if (pdata == NULL)
    {
        ERROR("Data pointer of matrix is NULL");
    }
    else
    {
        this->data.reset(new T[row * col * dim]);
        for (size_t i = 0; i < row * col * dim; i++)
        {
            this->data[i] = (T)pdata[i];
        }
        DBGprint("Matrix hard copy from other data type\n");
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &m)
{ // soft copy assignment
    if (this == &m)
    {
        return *this;
    }
    else
    {
        if (m.data.get() == NULL)
        {
            ERROR("Data pointer of matrix is NULL");
        }
        else
        {
            this->row = m.row;
            this->col = m.col;
            this->dim = m.dim;
            this->row_start = m.row_start;
            this->col_start = m.col_start;
            this->step_start = m.step_start;
            this->step_row = m.step_row;
            this->step_dim = m.step_dim;
            this->data = m.data;
        }
        return *this;
    }
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &m)
{
    T *pdata1 = this->data.get();
    T *pdata2 = m.data.get();
    if (pdata1 == NULL || pdata2 == NULL)
    {
        ERROR("Data pointer of matrix is NULL");
    }
    else
    {
        if (this->row == m.row && this->col == m.col && this->dim == m.dim)
        {
            pdata1 += this->step_start;
            pdata2 += m.step_start;
            for (size_t i = 0; i < row * col * dim; i++)
            {
                if (*pdata1 != *pdata2)
                {
                    DBGprint("Matrix.operator==: matrices' data don't match\n");
                    return false;
                }
                if ((i + 1) % (row * col) == 0)
                {
                    pdata1 += this->step_dim;
                    pdata2 += m.step_dim;
                }
                else if ((i + 1) % col == 0)
                {
                    pdata1 += this->step_row;
                    pdata2 += m.step_row;
                }
                else
                {
                    pdata1++;
                    pdata2++;
                }
            }
            DBGprint("Matrix.operator==: matrices match\n");
            return true;
        }
        else
        {
            DBGprint("Matrix.operator==: matrices' dimensions don't match\n");
            return false;
        }
    }
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &m)
{
    T *pdata1 = this->data.get();
    T *pdata2 = m.data.get();
    if (pdata1 == NULL || pdata2 == NULL)
    {
        ERROR("Data pointer of matrix is NULL");
    }
    else
    {
        if (this->row == m.row && this->col == m.col && this->dim == m.dim)
        {
            pdata1 += this->step_start;
            pdata2 += m.step_start;
            T *pdata_new = new T[row * col * dim];
            for (size_t i = 0; i < row * col * dim; i++)
            {
                pdata_new[i] = *pdata1 + *pdata2;
                if ((i + 1) % (row * col) == 0)
                {
                    pdata1 += this->step_dim;
                    pdata2 += m.step_dim;
                }
                else if ((i + 1) % col == 0)
                {
                    pdata1 += this->step_row;
                    pdata2 += m.step_row;
                }
                else
                {
                    pdata1++;
                    pdata2++;
                }
            }
            Matrix<T> sum(row, col, pdata_new, dim);
            delete[] pdata_new;
            DBGprint("Matrix.operator+()\n");
            return sum;
        }
        else
        {
            ERROR("Matrices' dimensions don't match\n");
        }
    }
}

template <typename T>
template <typename U>
Matrix<T> &Matrix<T>::operator=(const Matrix<U> &m)
{ // hard copy assignment
    return Matrix(&m);
}