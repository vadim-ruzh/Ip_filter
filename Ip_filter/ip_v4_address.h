#pragma once

#include "basic_address.h"

#include <boost/range/adaptors.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace ip
{
	class IIpAddress
	{
	public:
		virtual ~IIpAddress() = default;
		virtual bool IsLoopback() const = 0;
		virtual bool IsBroadcast() const = 0;
	};

	namespace v4 
	{
		static const boost::regex ipV4RegExpr{ R"((\d+\.\d+\.\d+\.\d+))" };

		template <typename Ty>
		class AddressImpl : public BasicAddress<AddressImpl<Ty>> , IIpAddress
		{
		public:
			using octet_type = typename address_traits<AddressImpl>::octet_type;

			template<typename T>
			explicit AddressImpl(const T& val)
			{
				this->SetAddress(val);
			}

			static std::string OctetToString(const octet_type& octet)
			{
				std::stringstream ssm;
				ssm << static_cast<unsigned int>(octet);
				return ssm.str();
			}
			static octet_type StringToOctet(const boost::string_view& str)
			{
				try
				{
					return boost::numeric_cast<uint8_t>(std::stoul(str.data()));
				}
				catch (boost::bad_numeric_cast&)
				{
					throw std::runtime_error("The octet value are incorrect");
				}
			}

			bool IsLoopback() const override
			{
				return this->mAddress[0] == 127;
			}
			bool IsBroadcast() const override
			{
				return this->mAddress[1] == 255 || this->mAddress[2] == 255 || this->mAddress[3] == 255;
			}
		};

		template <typename Ty>
		struct address_traits<AddressImpl<Ty>>
		{
			using octet_type = uint8_t;
			static constexpr size_t OctetsCount = 4;
			static constexpr char Delimiter = '.';
		};

		using address = AddressImpl<uint8_t>;
	}
}
 
