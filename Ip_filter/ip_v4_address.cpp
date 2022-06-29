#include "ip_v4_address.h"

#include <sstream>

namespace ip
{
	namespace v4
	{
		bool Address::IsLoopback() const
        {
            return mAddress[0] == 127;
        }

        bool Address::IsBroadcast() const
        {
            return mAddress[1] == 255 || mAddress[2] == 255 || mAddress[3] == 255;
        }

        uint8_t Address::ConvertStringToOctet(const std::string& str) const
        {
            return boost::numeric_cast<uint8_t>(std::stoul(str));
        }

        std::string Address::ConvertOctetToString(const uint8_t& octet) const 
        {
            std::stringstream ssm;

            ssm << static_cast<unsigned int>(octet);

            return ssm.str();
        }
	}

}

