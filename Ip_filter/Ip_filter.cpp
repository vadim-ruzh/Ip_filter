#include "Ip_filter.h""
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>

IpV4Address::IpV4Address(const std::string& str)
{
	IpV4Address::SetAddress(str);
}

bool IpV4Address::IsLoopback() const
{
    return mAddress[0] == 127;
}

bool IpV4Address::IsBroadcast() const
{
    return mAddress[1] == 255 || mAddress[2] == 255 || mAddress[3] == 255;
}

void IpV4Address::SetAddress(const std::string& source)
{
    std::vector<std::string> tokens;
    boost::split(tokens, source, boost::is_any_of(std::string{ mDelimiter }));

    if (tokens.size() != mAddress.size())
    {
        throw std::runtime_error("The count of octets is incorrect");
    }

    try
    {
        auto StrToUint = [](const std::string& str)
        {
	        return boost::numeric_cast<uint8_t>(stoul(str));
        };

        boost::copy(tokens | boost::adaptors::transformed(StrToUint),mAddress.begin());
    }
    catch (boost::bad_numeric_cast& )
    {
        throw std::runtime_error("The octet values are incorrect");
    }

}

std::string IpV4Address::GetAddress() const
{
	std::stringstream buffer;
    for (const uint8_t& a : mAddress)
    {
        buffer << static_cast<unsigned int>(a);
        buffer << mDelimiter;
    }

    std::string result = buffer.str();
    result.pop_back();
    return result;
}

FileReader::FileReader(const std::string& pathToFile) : mFstrm(pathToFile)
{
    if(!mFstrm.is_open())
    {
        throw std::runtime_error("Failed to open file");
    }
}

FileReader::~FileReader()
{
    mFstrm.close();
}

