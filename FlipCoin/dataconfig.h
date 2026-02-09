#ifndef DATACONFIG_H
#define DATACONFIG_H
#include <vector>

class dataConfig
{
public:
    dataConfig(unsigned char n);
    std::vector<std::vector<unsigned char>> getData();
private:
    std::vector<std::vector<unsigned char>> data;
};

#endif // DATACONFIG_H
