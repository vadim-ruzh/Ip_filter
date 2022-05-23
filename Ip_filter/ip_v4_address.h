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
		static const boost::regex regularExpression{ R"((\d+\.\d+\.\d+\.\d+))" };

		class Address :
			public BasicAddress<uint8_t, 4, '.'>,
			public IIpAddress
		{
		public:
			explicit Address(const std::string& str);

			bool IsLoopback() const override;
			bool IsBroadcast() const override;

			void SetAddress(const std::string& source) override;
			std::string GetAddress() const override;
		};

	}
}

