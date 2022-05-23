#include <iostream>
#include <boost/range/algorithm.hpp>
#include "Ip_filter.h"

using ipCollection = std::vector<IpV4Address>;


int main()
{
    ipCollection addresses;
	std::back_insert_iterator<ipCollection> backInsertAddresses(addresses);

    const boost::regex regExpr(R"((\d+\.\d+\.\d+\.\d+))");
    const std::string pathToFile("C:\\ProgramData\\ip_filter.tsv");

    FileReader fileReader(pathToFile);
    fileReader.SearchByLines(regExpr, backInsertAddresses);

    boost::range::sort(addresses, [](const auto& a1, const auto& a2) {return a1 > a2; });

    const std::ostream_iterator<IpV4Address> output(std::cout, "\n");


	//список адресов первый байт которых равен 1
    const auto filter = [](const IpV4Address& ip) {return ip[0] == 1; };
    boost::range::copy(addresses | boost::adaptors::filtered(filter),output);
    //список адресов первый байт которых равен 46, а второй 70
    const auto filter2 = [](const IpV4Address& ip) {return ip[0] == 46 && ip[1] == 70; };
    boost::range::copy(addresses | boost::adaptors::filtered(filter2), output);
    //список адресов любой байт которых равен 46
    const auto filter3 =[](const IpV4Address& ip){return ip[0] == 46 || ip[1] == 46 || ip[2] == 46 || ip[3] == 46;};
    boost::range::copy(addresses | boost::adaptors::filtered(filter3), output);

}