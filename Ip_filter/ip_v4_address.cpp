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

        uint8_t Address::StringToOctet(const boost::string_view& str) const
        {
            try
            {
                return boost::numeric_cast<uint8_t>(std::stoul(str.to_string()));
            }
            catch (boost::bad_numeric_cast&)
            {
                throw std::runtime_error("The octet value are incorrect");
            }
        }

        std::string Address::OctetToString(const uint8_t& octet) const 
        {
            std::stringstream ssm;

            ssm << static_cast<unsigned int>(octet);

            return ssm.str();
        }
	}

}

