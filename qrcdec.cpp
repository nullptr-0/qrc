#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include "des.h"
#include "zlib.h"

unsigned char KEY1[] = "!@#)(NHLiuy*$%^&";
unsigned char KEY2[] = "123ZXC!@#)(*$%^&";
unsigned char KEY3[] = "!@#)(*$%^&abcDEF";

int des(unsigned char* buff, unsigned char* key, int len)
{
    BYTE schedule[16][6];
    des_key_setup(key, schedule, DES_ENCRYPT);
    for (int i = 0; i < len; i += 8)
    {
        des_crypt(buff + i, buff + i, schedule);
    }
    return 0;
}

int ddes(unsigned char* buff, unsigned char* key, int len)
{
    BYTE schedule[16][6];
    des_key_setup(key, schedule, DES_DECRYPT);
    for (int i = 0; i < len; i += 8)
    {
        des_crypt(buff + i, buff + i, schedule);
    }
    return 0;
}

// Function to convert a single hex character to its decimal value
unsigned char hexCharToByte(char hex) {
    if ('0' <= hex && hex <= '9') {
        return hex - '0';
    }
    if ('a' <= hex && hex <= 'f') {
        return hex - 'a' + 10;
    }
    if ('A' <= hex && hex <= 'F') {
        return hex - 'A' + 10;
    }
    throw std::invalid_argument("Invalid hex character");
}

// Function to convert a hex string to a byte array
std::vector<unsigned char> fromHex(const std::string& hexStr) {
    if (hexStr.length() % 2 != 0) {
        throw std::invalid_argument("Hex string must have an even length");
    }

    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        unsigned char byte = (hexCharToByte(hexStr[i]) << 4) | hexCharToByte(hexStr[i + 1]);
        bytes.push_back(byte);
    }
    return bytes;
}

void LyricDecode(std::string& content)
{
    auto src = fromHex(content);
    size_t srcLen = src.size();
    ddes(src.data(), KEY1, srcLen);
    des(src.data(), KEY2, srcLen);
    ddes(src.data(), KEY3, srcLen);
    unsigned long dstLen = 1024 * 1024;
    std::vector<char> dst(dstLen);
    int res = uncompress((uint8_t*)dst.data(), &dstLen, src.data(), srcLen);
    if (res == Z_OK) {
        content = dst.data();
    }
    else {
        content = "";
    }
    return;
}

std::string AddPrefixToFilename(const std::string& filepath, const std::string& prefix) {
    std::filesystem::path path(filepath);
    std::string newFilename = prefix + path.filename().string();
    std::filesystem::path newPath = path.parent_path() / newFilename;
    return newPath.string();
}

void ProcessSingleFile(const std::string& srcPath)
{
    std::string content;
    std::fstream input(srcPath, std::ios::in);
    input >> content;
    input.close();
    LyricDecode(content);
    if (!content.empty())
    {
        std::string prefix = "decrypt_";
        std::string dstPath = AddPrefixToFilename(srcPath, prefix);
        std::fstream output(dstPath, std::ios::out);
        output << content;
        output.close();
    }
    else
    {
        std::cerr << "Error processing " << srcPath << "\n";
    }
    return;
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::string srcPath;
        std::cin >> srcPath;
        ProcessSingleFile(srcPath);
    }
    else {
        for (int i = 1; i < argc; i++)
        {
            ProcessSingleFile(argv[i]);
        }
    }

    return 0;
}
