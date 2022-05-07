#include "FileReader.h"

using namespace LineCounter;

namespace
{
    constexpr std::size_t CHUNK = 4'096;
}

FileReader::FileReader(const std::filesystem::path& filePath)
{
    _fileStream.open(filePath, std::ios::in);
}

FileReader::~FileReader()
{
    _fileStream.close();
}

ChunkInfo FileReader::GetChunk()
{
    std::string buffer(CHUNK, 0);
    if (_fileStream.is_open())
    {
        _fileStream.read(buffer.data(), CHUNK);
    }
    return { buffer, _fileStream.eof() /*isLastChunk*/ };
}

bool FileReader::isOpen() const
{
    return _fileStream.is_open();
}

bool FileReader::isEOF() const
{
    return _fileStream.eof();
}
