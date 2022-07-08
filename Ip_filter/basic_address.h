#pragma once

#include <array>
#include <vector>
#include <string>
#include <cstddef>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/concept_check.hpp>
#include "my_type_traits.h"
#include <boost/operators.hpp>


static constexpr unsigned BitsInByte = 8;

template <typename Implementation>
struct address_traits;

template <typename Implementation>
class BasicAddress : boost::totally_ordered<BasicAddress<Implementation>>
{
public:
	using Octet = typename address_traits<Implementation>::octet_type;
	static constexpr size_t OctetsCount= address_traits<Implementation>::OctetsCount;
	static constexpr char Delimiter = address_traits<Implementation>::Delimiter;

	virtual ~BasicAddress() = default;

	class OctetProxy
	{
	public:
		OctetProxy() = delete;
		OctetProxy(Octet& data) : mData(data) {}

		OctetProxy(const OctetProxy& other) : mData(other.mData){}
		OctetProxy& operator=(const OctetProxy& other)
		{
			if(other != this)
			{
				mData = other.mData;
			}

			return *this;
		}

		template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
		OctetProxy& operator=(T val)
		{
			mData = boost::numeric_cast<Octet>(val);
			return *this;
		}
		OctetProxy& operator=(boost::string_view str)
		{
			mData = Implementation::StringToOctet(str);
			return *this;
		}

		operator Octet()
		{
			return mData;
		}
	private:
		Octet& mData;
	};

	OctetProxy operator[](std::size_t position) && = delete;
	OctetProxy operator[](std::size_t position) &
	{
		return OctetProxy(mAddress[position]);
	}
	Octet operator[](std::size_t position) const &
	{
		return mAddress[position];
	}

	friend std::istream& operator>>(std::istream& istrm, BasicAddress& address)
	{
		std::string buffer;
		istrm >> buffer;
		address.SetAddress(buffer);

		return istrm;
	}
	friend std::ostream& operator<<(std::ostream& ostrm, const  BasicAddress& address)
	{
		ostrm << address.GetAddress();
		return ostrm;
	}

	friend bool operator==(const BasicAddress& lhs, const BasicAddress& rhs) noexcept
	{
		return lhs.mAddress == rhs.mAddress;
	}
	friend bool operator<(const BasicAddress& lhs, const BasicAddress& rhs) noexcept
	{
		return lhs.mAddress < rhs.mAddress;
	}

	void SetAddress(boost::string_view source) noexcept(false)
	{
		static const std::string delim{ Delimiter };
		std::vector<std::string> tokens;

		boost::split(tokens, source, boost::is_any_of(delim));

		tokens.size() == mAddress.size()
			? boost::transform(tokens, mAddress.begin(), Implementation::StringToOctet)
			: throw std::runtime_error("The count of octets is incorrect");
	}

	template<typename T,typename = std::enable_if_t<is_container_v<T>>>
	void SetAddress(const T& source) noexcept(false)
	{
		auto octetValidation = [](const auto& value)
		{
			return boost::numeric_cast<Octet>(value);
		};

		source.size() == mAddress.size()
			? boost::transform(source, mAddress.begin(), octetValidation)
			: throw std::runtime_error("The count of octets is incorrect");
	}

	template<typename T,typename = std::enable_if_t<std::is_integral_v<T>>>
	void SetAddress(T source) noexcept
	{
		static constexpr size_t shift = sizeof(Octet) * BitsInByte;
		constexpr T bitmask = ~Octet(0);

		for (Octet& octet : boost::adaptors::reverse(mAddress))
		{
			octet = static_cast<Octet>(source & bitmask);
			source >>= shift;
		}
	}

	std::string GetAddress() const noexcept
	{
		static const std::string delim{ Delimiter };

		return boost::algorithm::join(
			mAddress | boost::adaptors::transformed(Implementation::OctetToString), delim);
	}
protected:
	std::array<Octet, OctetsCount> mAddress{};
};

	