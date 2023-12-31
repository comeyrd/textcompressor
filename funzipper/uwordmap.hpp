#ifndef UWORDMAP_HPP
#define UWORDMAP_HPP
#include <string>
#include <unordered_map>
#include "vectoredTxt.hpp"

class uwordmap
{
public:
    void print();
    void serialize(std::ostream &outStream);
    void deserialize(std::istream &inStream);

    uwordmap(vectoredTxt &ft, int ct) : counter(ct)
    {
        std::vector<std::vector<std::string>> rft = ft.getFt();
        std::unordered_map<std::string, int> textMap;
        for (const auto &row : rft)
        {
            for (const std::string &item : row)
            {
                if (!(this->map.find(item) != this->map.end()))
                {
                    this->map[item] = this->counter;
                    this->counter++;
                    if (item.size() > counter)
                    {
                        maxStrSize = item.size();
                    }
                }
            }
        }
    }

    uwordmap(std::istream &inStream)
    {
        counter = 0;
        deserialize(inStream);
    };

    uwordmap()
    {
        map = std::unordered_map<std::string, int>();
    };
    int getCounterSize();
    int at(std::string str)
    {
        return map[str];
    }
    std::unordered_map<int, std::string> flip()
    {
        std::unordered_map<int, std::string> flippedMap;
        for (const auto &entry : map)
        {
            flippedMap[entry.second] = entry.first;
        }
        return flippedMap;
    };
    int getStrSize();

private:
    std::unordered_map<std::string, int> map;
    int counter;
    int maxStrSize = 0;
};
#endif