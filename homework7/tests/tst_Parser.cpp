#define BOOST_TEST_MODULE Parser

#include "IVisitor.h"
#include "Command.h"
#include "CommandBulk.h"
#include "Parser.h"

#include <boost/test/unit_test.hpp>
#include <sstream>

class TestVisitor : public IVisitor
{
public:
    void visitCommand(Command &cmd) override
    {
        values.push_back(cmd.value());
    }

    void visitCommandBulk(CommandBulk &bulk) override
    {
        values.clear();
        for (auto &cmd : bulk.commands())
            cmd.accept(*this);
    }

    std::list<std::string> values;
};

BOOST_AUTO_TEST_CASE(parseStaticBlocks)
{
    std::istringstream input(R"""(cmd1
cmd2
cmd3
cmd4
cmd5
)""");
    Parser p(input, 3);
    TestVisitor v;

    auto result = p.parse();
    BOOST_TEST(result.get());
    result->accept(v);
    BOOST_TEST(v.values == std::list<std::string>({"cmd1", "cmd2", "cmd3"}));

    result = p.parse();
    BOOST_TEST(result.get());
    result->accept(v);
    BOOST_TEST(v.values == std::list<std::string>({"cmd4", "cmd5"}));

    result = p.parse();
    BOOST_TEST(!result.get());
}

BOOST_AUTO_TEST_CASE(parseDynamicBlocks)
{
    std::istringstream input(R"""(cmd1
cmd2
{
cmd3
cmd4
}
{
cmd5
cmd6
{
cmd7
cmd8
}
cmd9
}
{
cmd10
cmd11
)""");
    Parser p(input, 3);
    TestVisitor v;

    auto result = p.parse();
    BOOST_TEST(result.get());
    result->accept(v);
    BOOST_TEST(v.values == std::list<std::string>({"cmd1", "cmd2"}));

    result = p.parse();
    BOOST_TEST(result.get());
    result->accept(v);
    BOOST_TEST(v.values == std::list<std::string>({"cmd3", "cmd4"}));

    result = p.parse();
    BOOST_TEST(result.get());
    result->accept(v);
    BOOST_TEST(v.values == std::list<std::string>({"cmd5", "cmd6", "cmd7", "cmd8", "cmd9"}));

    result = p.parse();
    BOOST_TEST(!result.get());
}
