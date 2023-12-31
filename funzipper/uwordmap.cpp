#include "uwordmap.hpp"
#include <iostream>

int uwordmap::getCounterSize()
{
    if (counter <= 0xFF)
    {
        return 1; // 1 byte for values up to 255
    }
    else if (counter <= 0xFFFF)
    {
        return 2; // 2 bytes for values up to 65535
    }
    else if (counter <= 0xFFFFFF)
    {
        return 3; // 3 bytes for values up to 16777215
    }
    else
    {
        return 4; // 4 bytes for values up to 4294967295
    }
}

int uwordmap::getStrSize()
{
    if (maxStrSize <= 0xFF)
    {
        return 1; // 1 byte for values up to 255
    }
    else if (maxStrSize <= 0xFFFF)
    {
        return 2; // 2 bytes for values up to 65535
    }
    else if (maxStrSize <= 0xFFFFFF)
    {
        return 3; // 3 bytes for values up to 16777215
    }
    else
    {
        return 4; // 4 bytes for values up to 4294967295
    }
}
void uwordmap::print()
{
    std::cout << "counter: " << this->counter << std::endl;
    for (const auto &pair : this->map)
    {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }
}

void uwordmap::serialize(std::ostream &outStream)
{
    int size = getCounterSize();
    outStream.write(reinterpret_cast<const char *>(&size), sizeof(char)); // write the byte num needed to store the int
    outStream.write(reinterpret_cast<const char *>(&counter), size);      // write the byte num needed to store the int
    int strSize = getStrSize();
    outStream.write(reinterpret_cast<const char *>(&strSize), sizeof(char)); // write the byte num needed to store the int
    // Serialize the map size
    std::unordered_map<std::string, int>::size_type mapSize = map.size();
    outStream.write(reinterpret_cast<const char *>(&mapSize), sizeof(std::unordered_map<std::string, int>::size_type));

    // Serialize each key-value pair in the map
    for (const auto &entry : map)
    {
        size_t strLength = entry.first.size();                                               // get the length of the string
        outStream.write(reinterpret_cast<const char *>(&strLength), sizeof(char) * strSize); // write the length of the string
        outStream.write(entry.first.c_str(), strLength);                                     // write the c_string
        outStream.write(reinterpret_cast<const char *>(&entry.second), sizeof(char) * size); // write the fzInt
    }
}

void uwordmap::deserialize(std::istream &inStream)
{
    int size = 0;
    int strSize = 0;
    inStream.read(reinterpret_cast<char *>(&size), sizeof(char));
    inStream.read(reinterpret_cast<char *>(&counter), size);
    inStream.read(reinterpret_cast<char *>(&strSize), sizeof(char));
    // Deserialize the map size
    std::unordered_map<std::string, int>::size_type mapSize;
    inStream.read(reinterpret_cast<char *>(&mapSize), sizeof(std::unordered_map<std::string, int>::size_type));
    // Deserialize each key-value pair in the map
    for (int i = 0; i < mapSize; ++i)
    {
        int strLength = 0;
        inStream.read(reinterpret_cast<char *>(&strLength), sizeof(char) * strSize);
        char strBuffer[strLength];
        inStream.read(strBuffer, strLength);
        std::string str(strBuffer, strLength);
        int fzInt = 0;
        inStream.read(reinterpret_cast<char *>(&fzInt), sizeof(char) * size);

        map[str] = fzInt;
    }
}