#pragma once

#include <array>
#include <string>

template <typename Octet, std::size_t OctetNumber, char Delimiter>
class BasicAddress
{
public:
	virtual ~BasicAddress() = default;

	Octet& operator[](std::size_t numberOfOctet) 
	{
		return mAddress[numberOfOctet];
	}
	const Octet& operator[](std::size_t numberOfOctet) const
	{
		return operator[](numberOfOctet);
	}


	friend std::istream& operator>>(std::istream& is, BasicAddress& address)
	{
		std::string buffer;
		is >> buffer;
		address.SetAddress(buffer);

		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, const  BasicAddress& address)
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

	virtual std::string GetAddress() const = 0;
	virtual void SetAddress(const std::string& source) = 0;
protected:
	static const char mDelimiter{ Delimiter };
	std::array<Octet, OctetNumber> mAddress{};
};
