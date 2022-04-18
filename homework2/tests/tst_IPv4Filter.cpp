#define BOOST_TEST_MODULE IPv4Filter

#include "IPv4Filter.h"

#include <fstream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>

using boost::uuids::detail::md5;

std::string toString(const md5::digest_type &digest)
{
    const auto intDigest = reinterpret_cast<const int*>(&digest);
    std::string result;
    boost::algorithm::hex(intDigest, intDigest + (sizeof(md5::digest_type) / sizeof(int)), std::back_inserter(result));
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return std::tolower(c); });
    return result;
}

BOOST_AUTO_TEST_CASE(filter)
{
    std::ifstream fin("ip_filter.tsv");
    std::ostringstream sout;
    IPv4Filter::filter(fin, sout);

    md5 hash;
    md5::digest_type digest;
    hash.process_bytes(sout.str().c_str(), sout.str().size());
    hash.get_digest(digest);
    auto md5 = toString(digest);

    BOOST_CHECK_EQUAL(md5, "24e7a7b2270daee89c64d3ca5fb3da1a");
}
