#ifndef CELL_H
#define CELL_H
#include <vector>
#include <QPainter>
#include "population.h"

class Cell
{
public:
    Cell();
    Cell(std::vector<Population> populations,
         std::map<Population,double,PopComparator> concentrations,
         double food_concentration,
         std::pair<int,int> position);

    std::vector<Population> populations;
    std::vector< Cell* > neibourghs;
    std::map<Population,double,PopComparator> concentrations;
    double food_concentration;
    std::pair<int,int> position;
    int size=30;
    bool excluded=false;

    double population();
    double enemies(Population pop);
    void fight();
    void eat(double consuming_constant);
    void multiply();
    double migrate(Population* pop, Cell *neibourgh);
    QColor getColor();
private:
};


#endif // CELL_H
