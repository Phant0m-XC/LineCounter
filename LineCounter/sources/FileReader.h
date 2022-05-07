#pragma once

#include <filesystem>
#include <fstream>
#include <string>

namespace LineCounter
{
    /**
    * @struct ChunkInfo
    * @brief Contains file chunk data
    */
    struct ChunkInfo
    {
        std::string _fileChunk; ///< a chunk of file
        bool _isLastChank;      ///< is the last chunk from the file
    };

    /**
    * @class FileReader
    * @brief Class is intended for opening, reading files
    */
    class FileReader final
    {
    public:
        explicit FileReader(const std::filesystem::path& filePath);

        FileReader(const FileReader&) = delete;
        FileReader(FileReader&&) = delete;
        FileReader& operator=(const FileReader&) = delete;
        FileReader& operator=(FileReader&&) = delete;

        ~FileReader();

        /**
        * @brief Function is used to getting next chunk info from file
        * @return ChunkInfo
        */
        ChunkInfo GetChunk();

        /**
        * @brief Function is used to getting info about either file is opened or not
        * @return if file is opened returns true otherwise false
        */
        bool isOpen() const;

        /**
        * @brief Function is used to getting info about either file reached EOF or not
        * @return if file reached EOF returns true otherwise false
        */
        bool isEOF() const;

    private:
        std::ifstream _fileStream;   ///< file stream
    };
}
