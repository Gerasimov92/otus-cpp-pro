#define BOOST_TEST_MODULE SaveToFileVisitor

#include "Command.h"
#include "CommandBulk.h"
#include "SaveToFileVisitor.h"

#include <boost/test/unit_test.hpp>
#include <fstream>
#include <sstream>

BOOST_AUTO_TEST_CASE(saveCommandBulk)
{
    SaveToFileVisitor saveVisitor;
    CommandBulk bulk({Command(1, "cmd1"), Command(2, "cmd2"), Command(3, "cmd3")});
    std::string expectedResult("bulk: cmd1, cmd2, cmd3\n");

    bulk.accept(saveVisitor);

    std::ifstream input("bulk1.log");
    BOOST_TEST(input.good());
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string actualResult = buffer.str();
    BOOST_CHECK_EQUAL(actualResult, expectedResult);

    input.close();
    std::remove("bulk1.log");
}
