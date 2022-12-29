#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <memory>
#include <exception>
#include <iostream>
#include <string>

#define ERROR(Msg)                                                    \
    {                                                                 \
        throw MatrixException(Msg, __FUNCTION__, __FILE__, __LINE__); \
    }

#ifdef DEBUG
#define DBGprint(...) printf(__VA_ARGS__)
#else
#define DBGprint(...) 
#endif

template <typename T>
class Matrix
{
    size_t row;
    size_t col;
    int dim;

    size_t row_start;
    size_t col_start;
    size_t step_start;
    size_t step_row;
    int step_dim;

    std::shared_ptr<T[]> data;

public:
    /* Constructor - created from data pointer */
    Matrix(ssize_t row, ssize_t col, T fill, int dim = 1);
    Matrix(ssize_t row, ssize_t col, T *data, int dim = 1);

    /* Copy constructor (soft copy) */
    Matrix(const Matrix &m);

    /* Constructor - created from matrix with other data type (hard copy) */
    template <typename U>
    Matrix(const Matrix<U> &m);

    // /* Constructor - created from matrix ROI (soft copy) */
    Matrix(const Matrix<T>& matrix, size_t row_start, size_t row_end, size_t col_start, size_t col_end); //default: all channels
    Matrix(const Matrix<T>& matrix, size_t row_start, size_t row_end, size_t col_start, size_t col_end, size_t channel_start, size_t channel_end);

    /* Deconstructor */
    ~Matrix();

    /* Operator overloading */
    Matrix &operator=(const Matrix &m);
    bool operator==(const Matrix &m);
    Matrix operator+(const Matrix &m);
    Matrix operator - (const Matrix & m);
    Matrix operator * (const Matrix & m);
    template <typename U>
    Matrix &operator=(const Matrix<U> &m);
    template<typename U> bool operator==(const Matrix<U> &m);
    template<typename U> Matrix operator+(const Matrix<U> &m);
    template<typename U> Matrix operator-(const Matrix<U> &m);
    template<typename U> Matrix operator*(const Matrix<U> &m);
    template<typename U> Matrix operator+(const U & a);
    template<typename U> Matrix operator-(const U & a);
    template<typename U> Matrix operator*(const U & a);

    /* Friend functions */
    template<typename U> friend Matrix operator + (const U & a, const Matrix & m);
    template<typename U> friend Matrix operator - (const U & a, const Matrix & m);
    template<typename U> friend Matrix operator * (const U & a, const Matrix & m);
    friend std::ostream & operator << (std::ostream & os, const Matrix & m); 
    friend std::ifstream & operator >> (std::ifstream & ifs, const Matrix & m); 
    friend std::ofstream & operator << (std::ofstream & ofs, const Matrix & m); 

    /* hard copy */
    Matrix clone();

    /* print matrix */
    void print();

    /* getters */
    size_t getRow() const { return this->row; }
    size_t getCol() const { return this->col; }
    int getDim() const { return this->dim; }
    size_t getRow_start() const { return this->row_start; }
    size_t getCol_start() const { return this->col_start; }
    size_t getStep_start() const { return this->step_start; }
    size_t getStep_row() const { return this->step_row; }
    int getStep_dim() const { return this->step_dim; }
    T *getDataptr() const { return this->data.get(); }
    long getUse_count() const { return this->data.use_count(); }
};

class MatrixException : public std::exception
{
protected:
    std::string msg;
    std::string func;
    std::string file;
    int line;
    MatrixException() = default;

public:
    explicit MatrixException(const char *m, const char *func, const char *file, int line) : msg(m), func(func), file(file), line(line) {}
    [[nodiscard]] const char *what() const noexcept override
    {
        char buf[1000];
        sprintf(buf, "Matrix Error: %s in %s, file %s, line %d", msg.c_str(), func.c_str(), file.c_str(), line);
        const char *Msg = buf;
        return Msg;
    }
};

#endif
