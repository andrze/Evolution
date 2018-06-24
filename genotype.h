#ifndef GENOTYPE_H
#define GENOTYPE_H
#include <vector>
#include <string>
#include <iostream>

class Genotype
{
public:
    Genotype();
    Genotype( std::vector<std::vector<std::vector<double> > > genes );

    std::vector<std::vector<std::vector<double> > > genes;

    double phenotype( std::vector<double> environment );

    void mutate(double range, bool absolute=false);
};

std::ostream& operator<<(std::ostream& out, Genotype genotype);

#endif // GENOTYPE_H
