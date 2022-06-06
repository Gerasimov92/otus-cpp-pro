#include <cassert>
#include <iostream>
#include "Matrix.h"

void test()
{
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);

    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);

    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    for (auto c : matrix)
    {
        auto [x, y, v] = c;
        std::cout << x << y << v << std::endl;
    }

    ((matrix[100][100] = 314) = -1) = 217;
    assert(matrix[100][100] == 217);
}

int main()
{
    test();
    std::cout << std::endl;

    Matrix<int, 0> m;

    for (int i = 0; i <= 9; i++)
        m[i][i] = i;

    for (int i = 0; i <= 9; i++)
        m[i][9 - i] = 9 - i;

    for (int r = 1; r <= 8; r++)
    {
        for (int c = 1; c <= 8; c++)
        {
            std::cout << m[r][c];
            if (c != 8)
                std::cout << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Matrix size = " << m.size() << std::endl;
    for (const auto &e : m)
    {
        auto [r, c, v] = e;
        std::cout << "[" << r << ", " << c << "] = " << v << std::endl;
    }

    return 0;
}
