#define BOOST_TEST_MODULE PrintVisitor

#include "Command.h"
#include "CommandBulk.h"
#include "PrintVisitor.h"

#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(printCommandBulk)
{
    std::ostringstream output;
    PrintVisitor printVisitor(output);
    CommandBulk bulk({Command(1, "cmd1"), Command(2, "cmd2"), Command(3, "cmd3")});
    std::string expectedResult("bulk: cmd1, cmd2, cmd3\n");

    bulk.accept(printVisitor);
    std::string actualResult = output.str();

    BOOST_CHECK_EQUAL(actualResult, expectedResult);
}
