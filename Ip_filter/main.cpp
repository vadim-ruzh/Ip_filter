#include <iostream>
#include "Ip_filter.h"


    // 222.173.235.246
    // 222.130.177.64
    // 222.82.198.61
    // ...
    // 1.70.44.170
    // 1.29.168.152
    // 1.1.234.8

    // TODO filter by first byte and output
    // ip = filter(1)

    // 1.231.69.33
    // 1.87.203.225
    // 1.70.44.170
    // 1.29.168.152
    // 1.1.234.8

    // TODO filter by first and second bytes and output
    // ip = filter(46, 70)

    // 46.70.225.39
    // 46.70.147.26
    // 46.70.113.73
    // 46.70.29.76

    // TODO filter by any byte and output
    // ip = filter_any(46)

    // 186.204.34.46
    // 186.46.222.194
    // 185.46.87.231
    // 185.46.86.132
    // 185.46.86.131
    // 185.46.86.131
    // 185.46.86.22
    // 185.46.85.204
    // 185.46.85.78
    // 68.46.218.208
    // 46.251.197.23
    // 46.223.254.56
    // 46.223.254.56
    // 46.182.19.219
    // 46.161.63.66
    // 46.161.61.51
    // 46.161.61.51
    // 46.161.60.92
    // 46.161.60.35
    // 46.161.58.202
    // 46.161.56.241
    // 46.161.56.203
    // 46.161.56.174
    // 46.161.56.106
    // 46.161.56.106
    // 46.101.163.119
    // 46.101.127.145
    // 46.70.225.39
    // 46.70.147.26
    // 46.70.113.73
    // 46.70.29.76
    // 46.55.46.98
    // 46.49.43.85
    // 39.46.86.85

int main()
{
    std::vector<ip::v4::Address> ipAddresses;

    //Reading from a file 
    const std::string pathToFile("C:\\ProgramData\\ip_filter.tsv");
    const std::regex regExp(R"((\d+\.\d+\.\d+\.\d+))");
    ip::SearchInFile(pathToFile, "\n", regExp, 
        [&ipAddresses](const std::string& str)
        {
	        ipAddresses.emplace_back(str);
        });

    //Sorting
    std::sort(ipAddresses.begin(), ipAddresses.end(),
        [](auto first, auto second)
        {
            return first.Data() > second.Data();
        }
    );


    //Filtering
    std::vector<ip::v4::Address> filteredIpAdresses;
    std::back_insert_iterator<std::vector<ip::v4::Address>>fi(filteredIpAdresses);
    std::copy_if(ipAddresses.begin(), ipAddresses.end(), fi, 
        [](ip::v4::Address& ad)
        {
            const auto ipData = ad.Data();
            return ipData[0] == 1;
        });


    //Output to console 
	for (auto each : filteredIpAdresses)
	{
		std::cout << each.ToString() << std::endl;
	}
}
