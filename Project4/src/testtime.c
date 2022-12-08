#include "cblas.h"
#include "matrix.h"

double testOpenBlas(const Matrix *pmatrix1, const Matrix *pmatrix2);
Matrix *readMatrix(size_t n, int index);
double testTime(size_t n, int type);
float* generateData(size_t n);

double testOpenBlas(const Matrix *pmatrix1, const Matrix *pmatrix2)
{
    if (checkMatrix(pmatrix1) && checkMatrix(pmatrix2) && pmatrix1->col == pmatrix2->row)
    {
        Matrix *p = (Matrix *)calloc(1, sizeof(Matrix));
        p->pData = (float *)calloc(pmatrix1->row * pmatrix2->col, sizeof(float));
        p->row = pmatrix1->row;
        p->col = pmatrix2->col;
        if (p != NULL && p->pData != NULL)
        {
            const enum CBLAS_ORDER Order = CblasRowMajor;
            const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
            const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
            const int M = pmatrix1->row;
            const int N = pmatrix2->col;
            const int K = pmatrix1->col;
            const float alpha = 1;
            const float beta = 0;
            const int lda = K;
            const int ldb = N;
            const int ldc = N;
            double start, end;
            start = omp_get_wtime();
            cblas_sgemm(Order, TransA, TransB, M, N, K, alpha, pmatrix1->pData, lda, pmatrix2->pData, ldb, beta, p->pData, ldc);
            end = omp_get_wtime();
            DBGprintf("Multiplied two matrices. at func \"testOpenBlas\"\n");
            return end - start;
        }
        else
        {
            printf("ERROR: Memory allocation failed! at func \"testOpenBlas\"\n");
            return 0;
        }
    }
    else if (!checkMatrix(pmatrix1) || !checkMatrix(pmatrix2))
    {
        printf("ERROR: Invalid input matrix! at func \"testOpenBlas\"\n");
        return 0;
    }
    else
    {
        printf("ERROR: matrix1.col and matrix.row didn't match! at func \"testOpenBlas\"\n");
        return 0;
    }
}

float* generateData(size_t n){
	float *p = (float *)aligned_alloc(256, n * n * sizeof(float)); 
	if (p == NULL){
		printf("Failed to allocate memory\n");
        return NULL;
	}else{
        float a;
        double start, end;
        start = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 0; i < n*n; i++){
            a = (rand() % 100+1.f)/100;
            p[i] = a;
        }
        end = omp_get_wtime();
        printf("generated Matrix with n = %ld with time = %f s\n", n,end-start);
        return p;
    }
}

double testTime(size_t n, int type)
{
    // clock_t start, end;
    // Matrix *m1 = readMatrix(n, 1);
    // Matrix *m2 = readMatrix(n, 2);
    float* p1 = generateData(n);
    float* p2 = generateData(n);
    Matrix *m1 = createMatrix(n, n, p1);
    Matrix *m2 = createMatrix(n, n, p2);
    Matrix *m3;
    // start = clock();
    double start, end;
    // switch (type)
    // {
    // case 0:
    //     m3 = matmul_plain(m1, m2);
    //     break;
    // case 1:
    //     m3 = matmul_improved_SIMD(m1, m2);
    //     break;
    // case 2:
    //     m3 = matmul_improved_OPENMP(m1, m2);
    //     break;
    //     // case 3:
    //     // m3 = testOpenBlas(m1,m2);
    // }
    start = omp_get_wtime();
    switch (type)
    {
    case 0:
        m3 = matmul_plain(m1, m2);
        break;
    case 1:
        m3 = matmul_improved_SIMD(m1, m2);
        break;
    case 2:
        m3 = matmul_improved_OPENMP(m1, m2);
        break;
        // case 3:
        // m3 = testOpenBlas(m1,m2);
    }
    // end = clock();
    end = omp_get_wtime();
    if (type == 3)
    {
        double time_openblas = testOpenBlas(m1, m2);
        deleteMatrix(&m1);
        deleteMatrix(&m2);
        return time_openblas;
    }
    if (m3 != NULL)
    {
        // printMatrix(m3);
        deleteMatrix(&m1);
        deleteMatrix(&m2);
        deleteMatrix(&m3);
        // return (double) (end - start) / CLOCKS_PER_SEC;
        return end - start;
    }
    else
    {
        printf("m3 is NULL\n");
        deleteMatrix(&m1);
        deleteMatrix(&m2);
        deleteMatrix(&m3);
        return 0.0;
    }
}