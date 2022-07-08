#include <iostream>
#include <boost/range/algorithm.hpp>
#include <boost/lambda2.hpp>
#include "file_reader.h"
#include "ip_v4_address.h"

using ipCollection = std::vector<ip::v4::address>;
using namespace boost::lambda2;


int main()
{
    const std::string pathToFile{ R"(C:\ProgramData\ip_filter.tsv)" };
    const std::ostream_iterator<ip::v4::address> output(std::cout, "\n");

    ipCollection addresses;
    FileReader(pathToFile).Search(boost::is_any_of("\n"),
										ip::v4::ipV4RegExpr,
											std::back_inserter(addresses));

	boost::range::sort(addresses, std::greater<ip::v4::address>());

    boost::range::copy(addresses | 
						boost::adaptors::filtered(_1[1] == 1),output);
    boost::range::copy(addresses | 
						boost::adaptors::filtered(_1[0] == 46 && _1[1] == 70) , output);
    boost::range::copy(addresses | 
						boost::adaptors::filtered(_1[0] == 46 || _1[1] == 46 || _1[2] == 46 || _1[3] == 46), output);
}