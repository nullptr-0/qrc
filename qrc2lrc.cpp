#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <iomanip>
#include <filesystem>

/*!
 *  @brief  Convert microseconds to LRC time
 *  @param  microsecond The microsecond timestamp to be converted
 *  @return The formatted LRC time as a string (mm:ss.ss)
 */
std::string MicrosecondsToLrcTime(long long microsecond)
{
    double seconds = microsecond / 1000.0;
    int min = seconds / 60;
    double sec = seconds - 60 * min;
    std::stringstream timeStrStream;
    timeStrStream << std::setfill('0') << std::setw(2) << min << ":" << std::setw(5) << std::fixed << std::setprecision(2) << sec;
    return timeStrStream.str();
}

/*!
 *  @brief  Parse the QRC file and convert to LRC format
 *  @param  QRCPath The path to the QRC file
 *  @param  error   Reference to an error message if an error occurs
 */
void QrcToLrc(const std::string& qrcPath, const std::string& lrcPath, std::string& error)
{
    std::ifstream qrcFile(qrcPath);
    std::ofstream lrcFile(lrcPath);
    if (!qrcFile.is_open())
    {
        error = "cannot open file " + qrcPath;
        return;
    }

    std::string curLine;
    std::regex reg(R"(^\[(\d+))"); // Match lines starting with '[' followed by digits

    while (getline(qrcFile, curLine))
    {
        std::smatch match;
        if (std::regex_search(curLine, match, reg))
        {
            // Extract microseconds and convert to LRC time
            long long microseconds = std::stoll(match[1].str());
            lrcFile << "[" << MicrosecondsToLrcTime(microseconds) << "]";

            // Print the rest of the line, excluding brackets
            bool print = true;
            for (size_t i = 0; i < curLine.length(); i++)
            {
                if (curLine[i] == '[' || curLine[i] == '(')
                {
                    print = false;
                }
                else if (curLine[i] == ']' || curLine[i] == ')')
                {
                    print = true;
                }
                else if (print)
                {
                    lrcFile << curLine[i];
                }
            }
            lrcFile << "\n";
        }
    }
    lrcFile.close();
    qrcFile.close();
}

void ProcessSingleFile(const std::string& qrcPath)
{
    std::filesystem::path lrcPath = qrcPath;
    lrcPath.replace_extension(".lrc");
    std::string error;
    QrcToLrc(qrcPath, lrcPath.string(), error);
    if (!error.empty())
    {
        std::cerr << "Error: " << error << std::endl;
    }
    return;
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::string qrcPath;
        std::cin >> qrcPath;
        ProcessSingleFile(qrcPath);
    }
    else {
        for (int i = 1; i < argc; i++)
        {
            ProcessSingleFile(argv[i]);
        }
    }
    return 0;
}
