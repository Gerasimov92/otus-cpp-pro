#include "print_ip.h"
#include <iostream>
#include <list>
#include <tuple>
#include <vector>

int main()
{
    print_ip(std::cout, char(-1));
    std::cout << std::endl;

    print_ip(std::cout, short(0));
    std::cout << std::endl;

    print_ip(std::cout, int(2130706433));
    std::cout << std::endl;

    print_ip(std::cout, long(8875824491850138409));
    std::cout << std::endl;

    const char *ip = "192.168.1.1";
    print_ip(std::cout, ip);
    std::cout << std::endl;

    print_ip(std::cout, "192.168.1.2");
    std::cout << std::endl;

    print_ip(std::cout, std::string("192.168.1.3"));
    std::cout << std::endl;

    print_ip(std::cout, std::vector{192, 168, 1, 4});
    std::cout << std::endl;

    print_ip(std::cout, std::list{192, 168, 1, 5});
    std::cout << std::endl;

    print_ip(std::cout, std::tuple{192, 168, 1, 6});
    std::cout << std::endl;

    return 0;
}
