#include "matrix.h"

/* create a matrix: initialize a matrix with given size */
Matrix *createMatrix(ssize_t row, ssize_t col, float *pinput)
{
    if (row > 0 && col > 0 && pinput != NULL)
    {
        Matrix *p = (Matrix *)calloc(1, sizeof(Matrix));
        p->pData = (float *)calloc(row * col, sizeof(float));
        if (p != NULL && p->pData != NULL)
        {
            p->row = row;
            p->col = col;
            memcpy(p->pData, pinput, sizeof(float) * p->row * p->col); // if length(pinput)!=row*col, memcpy may cause data over/undeerflow, but cannot be detected
            DBGprintf("Created matrix with %lu rows and %lu columns. at func \"createMatrix\"\n", p->row, p->col);
            return p;
        }
        else
        {
            printf("ERROR: Memory allocation failed! at func \"createMatrix\"\n");
            return NULL;
        }
    }
    else
    {
        if (pinput == NULL)
        {
            printf("ERROR: Invalid input data pointer! at func \"createMatrix\"\n");
        }
        else
        {
            printf("ERROR: Invalid matrix size! at func \"createMatrix\"\n");
        }

        return NULL;
    }
}

/* check if a matrix is initialized, return 0 if invalid */
inline int checkMatrix(const Matrix *pmatrix)
{
    return (pmatrix != NULL && pmatrix->col > 0 && pmatrix->row > 0 && pmatrix->pData != NULL);
}

/* print the values of a matrix */
int printMatrix(const Matrix *pmatrix)
{
    if (checkMatrix(pmatrix))
    {
        for (int i = 0; i < pmatrix->row * pmatrix->col; i++)
        {
            printf("%f ", pmatrix->pData[i]);
            if ((i + 1) % pmatrix->col == 0)
                printf("\n");
        }
        return 0;
    }
    else
    {
        printf("ERROR: Invalid input matrix! at func \"printMatrix\"\n");
        return -1;
    }
}

/* set value for given entry of a matrix */
int setValue(Matrix *ppmatrix, ssize_t row, ssize_t col, float value)
{
    if (row >= 0 && col >= 0 && ppmatrix != NULL)
    {
        if (checkMatrix(ppmatrix) && row < (ppmatrix)->row && col < (ppmatrix)->col)
        {
            (ppmatrix)->pData[row * (ppmatrix)->col + col] = value;
            DBGprintf("Setted entry (%ld, %ld) = %f. at func \"setValue\"\n", row, col, (ppmatrix)->pData[row * (ppmatrix)->col + col]);
            return 0;
        }
        else if (!checkMatrix(ppmatrix))
        {
            printf("ERROR: Invalid input matrix! at func \"setValue\"\n");
            return -1;
        }
        else
        {
            printf("ERROR: Invalid entry position! at func \"setValue\"\n");
            return -1;
        }
    }
    else if (ppmatrix == NULL)
    {
        printf("ERROR: Pointer of the matrix pointer is NULL! at func \"setValue\"\n");
        return -1;
    }
    else
    {
        printf("ERROR: Invalid entry position! at func \"setValue\"\n");
        return -1;
    }
}

/* delete a matrix */
int deleteMatrix(Matrix **ppmatrix)
{
    if (ppmatrix != NULL)
    {
        if (checkMatrix(*ppmatrix))
        {
            free((*ppmatrix)->pData);
            (*ppmatrix)->pData = NULL;
            free(*ppmatrix);
            *ppmatrix = NULL;
            DBGprintf("Freed matrix. at func \"deleteMatrix\"\n");
            return 0;
        }
        else
        {
            printf("ERROR: Invalid input matrix! at func \"deleteMatrix\"\n");
            return -1;
        }
    }
    else
    {
        printf("ERROR: Pointer of the matrix pointer is NULL! at func \"deleteMatrix\"\n");
        return -1;
    }
}

/* copy a matrix (copy the data from a matrix to another) */
Matrix *copyMatrix(const Matrix *pmatrix)
{
    if (checkMatrix(pmatrix))
    {
        Matrix *p = (Matrix *)calloc(1, sizeof(Matrix));
        p->pData = (float *)calloc(pmatrix->row * pmatrix->col, sizeof(float));
        if (p != NULL && p->pData != NULL)
        {
            p->row = pmatrix->row;
            p->col = pmatrix->col;
            memcpy(p->pData, pmatrix->pData, sizeof(float) * p->row * p->col);
            DBGprintf("Created matrix with %lu rows and %lu columns. at func \"copyMatrix\"\n", p->row, p->col);
            return p;
        }
        else
        {
            printf("ERROR: Memory allocation failed! at func \"copyMatrix\"\n");
            return NULL;
        }
    }
    else
    {
        printf("ERROR: Invalid input matrix! at func \"copyMatrix\"\n");
        return NULL;
    }
}

/* multiply two matrices. */
Matrix *matmul_plain(const Matrix *pmatrix1, const Matrix *pmatrix2)
{
    if (checkMatrix(pmatrix1) && checkMatrix(pmatrix2) && pmatrix1->col == pmatrix2->row)
    {
        Matrix *p = (Matrix *)calloc(1, sizeof(Matrix));
        p->pData = (float *)calloc(pmatrix1->row * pmatrix2->col, sizeof(float));
        if (p != NULL && p->pData != NULL)
        {
            p->row = pmatrix1->row;
            p->col = pmatrix2->col;
            size_t n = pmatrix1->col;
            for (int i = 0; i < p->row; i++)
            {
                for (int j = 0; j < p->col; j++)
                {
                    for (int k = 0; k < n; k++)
                    {
                        p->pData[i * p->col + j] += pmatrix1->pData[i * n + k] * pmatrix2->pData[k * n + j];
                    }
                }
            }
            DBGprintf("Multiplied two matrices. at func \"matmul_plain\"\n");
            return p;
        }
        else
        {
            printf("ERROR: Memory allocation failed! at func \"matmul_plain\"\n");
            return NULL;
        }
    }
    else if (!checkMatrix(pmatrix1) || !checkMatrix(pmatrix2))
    {
        printf("ERROR: Invalid input matrix! at func \"matmul_plain\"\n");
        return NULL;
    }
    else
    {
        printf("ERROR: matrix1.col and matrix.row didn't match! at func \"matmul_plain\"\n");
        return NULL;
    }
}

Matrix *matmul_improved_SIMD(const Matrix *pmatrix1, const Matrix *pmatrix2)
{
    if (checkMatrix(pmatrix1) && checkMatrix(pmatrix2) && pmatrix1->col == pmatrix2->row)
    {
        Matrix *p = (Matrix *)calloc(1, sizeof(Matrix));
        // p->pData = (float *)calloc(pmatrix1->row * pmatrix2->col, sizeof(float));
        p->pData = (float *)aligned_alloc(256, pmatrix1->row * pmatrix2->col * sizeof(float));
        memset(p->pData, 0, pmatrix1->row * pmatrix2->col * sizeof(float));
        if (p != NULL && p->pData != NULL)
        {
            p->row = pmatrix1->row;
            p->col = pmatrix2->col;
            size_t n = pmatrix1->col;
            //#pragma omp for
            for (int i = 0; i < p->row; i++)
            {
                //#pragma omp for
                for (int j = 0; j < p->col / 8; j += 8)
                {
                    __m256 res, row, col;
                    __m256 dif, sum;
                    for (int k = 0; k < n; k++)
                    {
                        __m256 res, row, col;
                        __m256 dif, sum;
                        // p->pData[i * p->col + j] += pmatrix1->pData[i * n + k] * pmatrix2->pData[k * n + j];
                        // row = _mm256_load_ps(pmatrix1->pData+(i * n + k));
                        float r = pmatrix1->pData[i * n + k];
                        row[0] = r;
                        row[1] = r;
                        row[2] = r;
                        row[3] = r;
                        row[4] = r;
                        row[5] = r;
                        row[6] = r;
                        row[7] = r;

                        col = _mm256_loadu_ps(pmatrix2->pData + (k * n + j));
                        // col[0] = pmatrix2->pData[k * n + j];
                        // col[1] = pmatrix2->pData[(k+1) * n + j];
                        // col[2] = pmatrix2->pData[(k+2) * n + j];
                        // col[3] = pmatrix2->pData[(k+3) * n + j];
                        // col[4] = pmatrix2->pData[(k+4) * n + j];
                        // col[5] = pmatrix2->pData[(k+5) * n + j];
                        // col[6] = pmatrix2->pData[(k+6) * n + j];
                        // col[7] = pmatrix2->pData[(k+7) * n + j];
                        res = _mm256_mul_ps(row, col);
                        dif = _mm256_loadu_ps(p->pData + (i * n + j));
                        sum = _mm256_add_ps(dif, res);
                        _mm256_storeu_ps(p->pData + (i * n + j), sum);

                        // printf("i,j,k = %d, %d, %d\n",i,j,k);
                        // printMatrix(p);
                    }
                }
                for (size_t j = p->col - (p->col) % 8; j < p->col; j++)
                {
                    for (size_t k = 0; k < n; k++)
                    {
                        p->pData[i * p->col + j] += pmatrix1->pData[i * n + k] * pmatrix2->pData[k * n + j];
                    }
                }
            }
            DBGprintf("Multiplied two matrices. at func \"matmul_improved\"\n");
            return p;
        }
        else
        {
            printf("ERROR: Memory allocation failed! at func \"matmul_improved\"\n");
            return NULL;
        }
    }
    else if (!checkMatrix(pmatrix1) || !checkMatrix(pmatrix2))
    {
        printf("ERROR: Invalid input matrix! at func \"matmul_improved\"\n");
        return NULL;
    }
    else
    {
        printf("ERROR: matrix1.col and matrix.row didn't match! at func \"matmul_improved\"\n");
        return NULL;
    }
}

Matrix *matmul_improved_OPENMP(const Matrix *pmatrix1, const Matrix *pmatrix2)
{
    if (checkMatrix(pmatrix1) && checkMatrix(pmatrix2) && pmatrix1->col == pmatrix2->row)
    {
        Matrix *p = (Matrix *)calloc(1, sizeof(Matrix));
        // p->pData = (float *)calloc(pmatrix1->row * pmatrix2->col, sizeof(float));
        p->pData = (float *)aligned_alloc(256, pmatrix1->row * pmatrix2->col * sizeof(float));
        memset(p->pData, 0, pmatrix1->row * pmatrix2->col * sizeof(float));
        if (p != NULL && p->pData != NULL)
        {
            p->row = pmatrix1->row;
            p->col = pmatrix2->col;
            size_t n = pmatrix1->col;

#pragma omp parallel
            {
#pragma omp for
                for (size_t i = 0; i < p->row; i++)
                {
#pragma omp parallel for
                    for (size_t j = 0; j < p->col / 8; j += 8)
                    {
                        __m256 res, row, col;
                        __m256 dif, sum;
                        for (size_t k = 0; k < n; k++)
                        {
                            __m256 res, row, col;
                            __m256 dif, sum;
                            float r = pmatrix1->pData[i * n + k];
                            row[0] = r;
                            row[1] = r;
                            row[2] = r;
                            row[3] = r;
                            row[4] = r;
                            row[5] = r;
                            row[6] = r;
                            row[7] = r;

                            col = _mm256_loadu_ps(pmatrix2->pData + (k * n + j));
                            res = _mm256_mul_ps(row, col);
                            dif = _mm256_loadu_ps(p->pData + (i * n + j));
                            sum = _mm256_add_ps(dif, res);
                            _mm256_storeu_ps(p->pData + (i * n + j), sum);
                        }
                    }
#pragma omp parallel for
                    for (size_t j = p->col - (p->col) % 8; j < p->col; j++)
                    {
#pragma omp parallel for
                        for (size_t k = 0; k < n; k++)
                        {
                            p->pData[i * p->col + j] += pmatrix1->pData[i * n + k] * pmatrix2->pData[k * n + j];
                        }
                    }
                }
            }
            DBGprintf("Multiplied two matrices. at func \"matmul_improved\"\n");
            return p;
        }
        else
        {
            printf("ERROR: Memory allocation failed! at func \"matmul_improved\"\n");
            return NULL;
        }
    }
    else if (!checkMatrix(pmatrix1) || !checkMatrix(pmatrix2))
    {
        printf("ERROR: Invalid input matrix! at func \"matmul_improved\"\n");
        return NULL;
    }
    else
    {
        printf("ERROR: matrix1.col and matrix.row didn't match! at func \"matmul_improved\"\n");
        return NULL;
    }
}
