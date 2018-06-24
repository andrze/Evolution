#ifndef POPULATION_H
#define POPULATION_H
#include "genotype.h"
#include <string>
#include <fstream>
#include <array>
#include <QColor>

class Population
{
public:
    Population();
    Population(std::ifstream& stream);

    std::string name;
    size_t index;
    Genotype growth;
    Genotype diffusion;
    QColor color;

    double multiply(double food_conc, double self_conc, double enemy_conc);
    double migrate(double food_conc, double self_conc, double enemy_conc, double food_conc2, double self_conc2, double enemy_conc2);
    void save();
    void mutate(double range, bool absolute=false);
};

std::ostream& operator<<(std::ostream& out, Population pop);

struct PopComparator{
    bool operator()(Population lhs, Population rhs);
};


#endif // POPULATION_H
