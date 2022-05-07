#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <numeric>

#include "FileReader.h"
#include "TaskManager.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Using of application:" << std::endl;
        std::cout << argv[0] << " [DIRECTORY_PATH]" << std::endl;
        std::cout << "DIRECTORY_PATH - a path to directory which contains text files." << std::endl;
        return EXIT_SUCCESS;
    }

    const std::filesystem::path directoryPath(argv[1]);
    std::vector<std::future<std::size_t>> results;

    const auto counter = [](LineCounter::ChunkInfo chunkInfo) -> std::size_t
    {
        std::stringstream sstream;
        sstream << chunkInfo._fileChunk;
        auto count = std::count(std::istreambuf_iterator<char>(sstream), std::istreambuf_iterator<char>(), '\n');

        // process last non-empty line
        if (chunkInfo._isLastChank)
        {
            try
            {
                const auto pos = chunkInfo._fileChunk.find_last_of('\n');
                if ((pos == std::string::npos && chunkInfo._fileChunk.at(0) != '\0') // buffer was filled by zero
                    || ((pos != chunkInfo._fileChunk.size() - 1) && chunkInfo._fileChunk.at(pos + 1) != '\0'))
                {
                    ++count;
                }
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Exception was occurred. File: " << __FILE__ << " Line: " << __LINE__
                    << " Reason: " << ex.what();
            }
        }
        return count;
    };

    const auto totalCores = std::thread::hardware_concurrency();
    LineCounter::TaskManager taskManager(totalCores > 1 ? totalCores - 1 : totalCores);

    try
    {
        if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath))
        {
            std::filesystem::directory_iterator end;
            for (std::filesystem::directory_iterator iter(directoryPath); iter != end; ++iter)
            {
                const auto& filePath = iter->path();
                if (std::filesystem::is_regular_file(filePath))
                {
                    LineCounter::FileReader reader(filePath);
                    while (reader.isOpen() && !reader.isEOF())
                    {
                        auto chunkOfFile = reader.GetChunk();
                        LineCounter::TTask task(std::bind(counter, std::move(chunkOfFile)));
                        auto futureResult = task.get_future();
                        taskManager.AddTask(std::move(task));
                        results.emplace_back(std::move(futureResult));
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception was occurred. File: " << __FILE__ << " Line: " << __LINE__
            << " Reason: " << ex.what();
    }

    taskManager.Finish();
    taskManager.WaitAll();

    auto totalCount = std::accumulate(results.begin(), results.end(), static_cast<std::size_t>(0), [](auto&& result, auto& value)
        {
            return result + value.get();
        });

    std::cout << totalCount << std::endl;

    return EXIT_SUCCESS;
}
