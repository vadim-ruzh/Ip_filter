#include "Ip_filter.h"
#include <fstream>
#include <sstream>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string.hpp>


namespace detail
{
    template<typename IpIter>
    std::string GlueIpToString(IpIter begin,IpIter end,const std::string& delim)
    {
        std::stringstream result;
        for (; begin != end - 1; ++begin)
        {
            result << static_cast<unsigned int>(*begin);
            result << delim.c_str();
        }
        result << static_cast<unsigned int>(*begin);

        return result.str();
    }
}

ip::v4::Address::Address(const std::string& source) noexcept(false) : addr_{}
{
    FromString(source);
}

void ip::v4::Address::FromString(const std::string& source)
{
    std::vector<std::string> tokens;
    boost::split(tokens, source, boost::is_any_of(delimiter_));

    if (tokens.size() != addr_.size())
    {
        throw std::runtime_error("The count of octets is incorrect");
    }

    try
    {
        std::transform(tokens.begin(), tokens.end(), addr_.begin(),
            [](const std::string& str)
            {
                return boost::numeric_cast<uint8_t>(stoul(str));
            });
    }
    catch (boost::bad_numeric_cast& e)
    {
        throw std::runtime_error("The octet values are incorrect");
    }
}

std::string ip::v4::Address::ToString()
{
    return detail::GlueIpToString(addr_.begin(), addr_.end(), delimiter_);
}

bool ip::v4::Address::IsLoopback()
{
    return addr_[0] == 127;
}

std::array<uint8_t, 4>& ip::v4::Address::Data() 
{
    return addr_;
}




/*

ip::File_Reader::File_Reader(const std::string& pathToFile) : m_fs(pathToFile){}

ip::File_Reader::~File_Reader()
{
	m_fs.close();
}


void ip::File_Reader::Search(const std::string& delim, const std::regex& regExpr, std::function<void(std::string)> callback) const
{
    std::stringstream fileData;
    fileData << m_fs.rdbuf();

    std::vector<std::string> lines;
    boost::split(lines, fileData.str(), boost::is_any_of(delim), boost::token_compress_on);

    for (auto line : lines)
    {
        std::smatch buffer;
        if (std::regex_search(line.cbegin(), line.cend(), buffer, regExpr))
        {
            callback(buffer.str());
        }
    }
}

*/

void ip::SearchInFile(const std::string& path, const std::string& delim, const std::regex& regExpr,std::function<void(std::string)> callback)
{
    std::ifstream m_fs(path);

    std::stringstream fileData;
    fileData << m_fs.rdbuf();

    std::vector<std::string> lines;
    boost::split(lines, fileData.str(), boost::is_any_of(delim));

    for (auto line : lines)
    {
        std::smatch buffer;
        if (std::regex_search(line.cbegin(), line.cend(), buffer, regExpr))
        {
            callback(buffer.str());
        }
    }
}

