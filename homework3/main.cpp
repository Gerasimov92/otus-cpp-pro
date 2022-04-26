#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>

#include "ForwardList.h"
#include "ReserveAllocator.h"

int factorial(int n)
{
    return (n == 0 || n == 1) ? 1 : factorial(n - 1) * n;
}

int main()
{
    std::map<int, int> stdAllocMap;
    std::map<int, int, std::less<int>, ReserveAllocator<std::pair<const int, int>, 10>> reserveAllocMap;

    for (int i = 0; i < 15; i++)
    {
        int value = factorial(i);
        stdAllocMap[i] = value;
        reserveAllocMap[i] = value;
    }

    auto copyOfReserveAllocMap = reserveAllocMap;
    for (int i = 0; i < 10; i++)
    {
        copyOfReserveAllocMap[i] = i * 100;
    }

    std::cout << "Test ReserveAllocator" << std::endl;

    const int w = 12;
    std::cout << std::setw(3) << "Key"
              << std::setw(w) << "Std.alloc"
              << std::setw(w) << "Res.alloc"
              << std::setw(w) << "Cpy of res" << std::endl;

    for (int i = 0; i < 15; i++)
    {
        std::cout << std::setw(3) << i
                  << std::setw(w) << stdAllocMap[i]
                  << std::setw(w) << reserveAllocMap[i]
                  << std::setw(w) << copyOfReserveAllocMap[i]
                  << std::endl;
    }
    std::cout << std::endl;

    ForwardList<int> stdAllocList;
    ForwardList<int, ReserveAllocator<int, 10>> reserveAllocList;

    for (int i = 0; i < 15; i++)
    {
        stdAllocList.push_front(i);
        reserveAllocList.push_front(i);
    }

    auto copyList = reserveAllocList;

    auto print = [](const int &value) { std::cout << std::setw(3) << value; };

    std::cout << "Test ForwardList<int>" << std::endl;
    std::cout << "Std alloc: ";
    std::for_each(stdAllocList.cbegin(), stdAllocList.cend(), print);
    std::cout << std::endl;

    std::cout << "Res alloc: ";
    std::for_each(reserveAllocList.cbegin(), reserveAllocList.cend(), print);
    std::cout << std::endl;

    std::cout << "Cpy alloc: ";
    std::for_each(copyList.cbegin(), copyList.cend(), print);
    std::cout << std::endl;

    return 0;
}
