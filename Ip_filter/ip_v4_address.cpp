#include "ip_v4_address.h"

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>


namespace ip
{
	namespace v4
	{
		Address::Address(const std::string& str)
        {
            Address::SetAddress(str);
        }

        bool Address::IsLoopback() const
        {
            return mAddress[0] == 127;
        }

        bool Address::IsBroadcast() const
        {
            return mAddress[1] == 255 || mAddress[2] == 255 || mAddress[3] == 255;
        }

        void Address::SetAddress(const std::string& source)
        {
            std::vector<std::string> tokens;
            boost::split(tokens, source, boost::is_any_of(std::string{ mDelimiter }));

            if (tokens.size() != mAddress.size())
            {
                throw std::runtime_error("The count of octets is incorrect");
            }

            try
            {
                auto toUint = [](const std::string& str)
                {
                    return boost::numeric_cast<uint8_t>(stoul(str));
                };

                boost::copy(tokens | boost::adaptors::transformed(toUint), mAddress.begin());
            }
            catch (boost::bad_numeric_cast&)
            {
                throw std::runtime_error("The octet values are incorrect");
            }

        }

        std::string Address::GetAddress() const
        {
            std::stringstream buffer;

            auto addressIter = mAddress.begin();
            for(;addressIter != mAddress.end() - 1; ++addressIter)
            {
                buffer << static_cast<unsigned int>(*addressIter);
                buffer << mDelimiter;
            }
            buffer << static_cast<unsigned int>(*addressIter);

            return buffer.str() ;
        }
	}

}

