#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <regex>
#include <string>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string.hpp>



class Iip
{
public:
	virtual ~Iip() = default;
	
	virtual bool IsLoopback() = 0;
	/* TODO
	virtual bool IsMulticast() = 0;
	*/
};


class IFormat
{
public:
	virtual ~IFormat() = default;
	virtual std::string ToString() = 0;
	virtual void FromString(const std::string& source) = 0;
};


namespace ip
{
	namespace v4
	{
		class Address final : public IFormat, Iip
		{
		public:
			explicit Address(const std::string& source) noexcept(false);

			void FromString(const std::string& source) noexcept(false) override;
			std::string ToString() override;

			bool IsLoopback() override;

			std::array<uint8_t, 4>& Data();
		private:
			std::string delimiter_{ "." };
			std::array<uint8_t, 4> addr_;
		};
	}

	/* TODO
	namespace v6
	{
		class Address : public IFormat, Iip
		{
		public:
			//bool IsLoopback() override;
			//bool IsMulticast() override;
			std::string ToString() override;
			void FromString(const std::string& source) override;
			std::array<uint16_t, 8> data();
		private:
			std::string delimiter_{ ":" };
			uint16_t ConvertFromHexToDec(const std::string& hexValue);
			std::array<uint16_t, 8> addr_;
		};

	}
	*/

	void SearchInFile(const std::string& path, const std::string& delim, const std::regex& regExpr, std::function<void(std::string)> callback);

	template <typename T>
	void SearchInFile(const std::string& path,const std::string& delim, const std::regex& regExpr, std::back_insert_iterator<T>& dest)
	{
		SearchInFile(path, delim, regExpr, [&dest](std::string str) {dest = str; });
	}
}
