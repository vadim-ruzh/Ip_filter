#pragma once
#include <ostream>
#include <array>
#include <sstream>
#include <boost/range/adaptors.hpp>
#include <fstream>
#include <regex>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string.hpp>


class IIpAddress
{
public:
	virtual ~IIpAddress() = default;
	virtual bool IsLoopback() const = 0;
	virtual bool IsBroadcast() const = 0;
};

template <typename OctetType, std::size_t OctetNumber, char Delimiter>
class BasicAddress
{
public:
	virtual ~BasicAddress() = default;

	OctetType operator[](const std::size_t& NumberOfOctet) const
	{
		return mAddress[NumberOfOctet];
	}

	friend std::istream& operator>>(std::istream& is, BasicAddress& address)
	{
		std::string buffer;
		is >> buffer;
		address.SetAddress(buffer);

		return is;
	}
	friend std::ostream& operator<<(std::ostream& os,const  BasicAddress& address)
	{
		os << address.GetAddress();
		return os;
	}

	friend bool operator==(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress == a2.mAddress;
	}
	friend bool operator!=(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress != a2.mAddress;
	}
	friend bool operator<(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress < a2.mAddress;
	}
	friend bool operator>(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress > a2.mAddress;
	}
	friend bool operator<=(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress <= a2.mAddress;
	}
	friend bool operator>=(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress >= a2.mAddress;
	}
protected:
	virtual std::string GetAddress() const = 0 ;
	virtual void SetAddress(const std::string& source) = 0;
	char mDelimiter{Delimiter};
	std::array<OctetType, OctetNumber> mAddress{};
};

class IpV4Address final : public BasicAddress<uint8_t, 4, '.'>, public IIpAddress
{
public:
	IpV4Address(const std::string& str);

	bool IsLoopback() const override;
	bool IsBroadcast() const override;
private:
	void SetAddress(const std::string& source) override;
	std::string GetAddress() const override ;
};

class FileReader
{
public:
	FileReader(const std::string& pathToFile);
	FileReader() = delete;
	~FileReader();

	template <typename OutputIterator>
	void SearchByLines(const boost::regex& regExpr, OutputIterator& dest);
private:
	std::fstream mFstrm;
};

template <typename OutputIterator>
void FileReader::SearchByLines(const boost::regex& regExpr, OutputIterator& dest)
{
	std::stringstream fileData;
	fileData << mFstrm.rdbuf();

	std::vector<std::string> lines;
	boost::split(lines, fileData.str(), boost::is_any_of("\n"), boost::token_compress_on);

	for (auto line : lines)
	{
		boost::smatch buffer;
		if (boost::regex_search(line.cbegin(), line.cend(), buffer, regExpr))
		{
			dest = buffer.str();
		}
	}
}
