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

		class Address : public BasicAddress<uint8_t, 4,'.'>, public IIpAddress
		{
		public:
			template<typename T>
			explicit Address(const T& val)
			{
				SetAddress(val);
			}

			bool IsLoopback() const override;
			bool IsBroadcast() const override;
		protected:
			uint8_t StringToOctet(const boost::string_view& str)  const override ;
			std::string OctetToString(const uint8_t& octet) const override;
		};
	}
}

