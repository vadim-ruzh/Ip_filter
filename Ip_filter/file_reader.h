#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

class FileReader
{
public:
	explicit FileReader(const std::string& pathToFile);
	FileReader() = delete;
	~FileReader();

	template <typename OutputIterator, typename Delimiter>
	void Search(Delimiter delimiter, const boost::regex& regExpr, OutputIterator dest);
private:
	std::fstream mFileStream;
};

template <typename OutputIterator, typename Delimiter>
void FileReader::Search(Delimiter delimiter, const boost::regex& regExpr, OutputIterator dest)
{
	std::stringstream fileData;
	fileData << mFileStream.rdbuf();

	std::vector<std::string> dataParts;
	boost::split(dataParts, fileData.str(), delimiter, boost::token_compress_on);

	for (std::string& part : dataParts)
	{
		boost::smatch buffer;
		if (boost::regex_search(part.cbegin(), part.cend(), buffer, regExpr))
		{
			dest = typename OutputIterator::container_type::value_type{ buffer.str() };
		}
	}
}

