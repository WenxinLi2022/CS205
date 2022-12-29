#include "matrix.h"

int main()
{
    try
    {
        Matrix<int> m1(3, 2, 2);
        m1.print();
        int data2[9] = {0, 1, 2, 3, 5, 6, 7, 8, 9};
        Matrix<int> m2(1, 3, data2, 3);
        m2.print();
        Matrix m3 = m2.clone();
        std::cout << "m3.data.use_count() = " << m3.getUse_count() << std::endl;
        m3.print();
        Matrix<int> m4 = m1;
        std::cout << "m4.data.use_count() = " << m4.getUse_count() << std::endl;
        m4.print();
        Matrix<double> m5 = m4;
        std::cout << "m5.data.use_count() = " << m5.getUse_count() << std::endl;
        m5.print();
        Matrix m6 = m1 + m4;
        m6.print();
        std::cout << "m6.data.use_count() = " << m6.getUse_count() << std::endl;
    }
    catch (MatrixException &e)
    {
        std::cout << e.what() << std::endl;
    }
}