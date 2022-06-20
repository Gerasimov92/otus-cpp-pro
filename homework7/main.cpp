#include "IVisitable.h"
#include "IVisitor.h"
#include "Parser.h"
#include "PrintVisitor.h"
#include "SaveToFileVisitor.h"

#include <iostream>
#include <list>
#include <memory>
#include <string>

int main(int argc, char *argv[])
{
    int maxBulkSize = 1;
    if (argc >= 2)
    {
        maxBulkSize = std::atoi(argv[1]);
        if (maxBulkSize < 1)
            maxBulkSize = 1;
    }

    std::list<std::unique_ptr<IVisitor>> visitors;
    visitors.push_back(std::make_unique<PrintVisitor>(std::cout));
    visitors.push_back(std::make_unique<SaveToFileVisitor>());

    Parser p(std::cin, maxBulkSize);

    while (1)
    {
        auto bulk = p.parse();
        if (!bulk)
            break;

        for (auto &v : visitors)
            bulk->accept(*v);
    }

    return 0;
}
