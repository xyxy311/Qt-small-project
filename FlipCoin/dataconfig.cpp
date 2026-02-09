#include "dataconfig.h"
#include <vector>

dataConfig::dataConfig(unsigned char n) {
    data = std::vector<std::vector<unsigned char>>(4, std::vector<unsigned char>(4, 1));
    for (int i = 0; i < 16; i++) {
        this->data[i / 4][i % 4] = 1;
    }
    switch (n) {
    case 1:
        this->data[1][2] = this->data[3][2] = this->data[2][2] = 0;
        this->data[2][1] = this->data[2][3] = this->data[2][2] = 0;
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
    }
}

std::vector<std::vector<unsigned char>> dataConfig::getData() {
    return this->data;
}
