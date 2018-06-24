#ifndef DATA_H
#define DATA_H
#include <vector>
#include <fstream>
#include "population.h"

class Data
{
public:
    Data();
    Data(std::vector<Population> populations);
    Data(std::ifstream& stream);

    std::vector<Population> populations;
    int field_size=4;
    int num_turns=100;
    double consuming_constant=0.1;
    double food_diffusion=0.1;
};

#endif // DATA_H
