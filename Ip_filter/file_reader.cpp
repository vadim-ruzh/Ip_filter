#include "file_reader.h"

FileReader::FileReader(const std::string& pathToFile):
	mFileStream(pathToFile)
{
    if (!mFileStream.is_open())
    {
        throw std::runtime_error("Failed to open file");
    }
}

FileReader::~FileReader()
{
    mFileStream.close();
}
