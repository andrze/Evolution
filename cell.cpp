#include "cell.h"
#include <cmath>

Cell::Cell(){}

Cell::Cell(std::vector<Population> populations,
           std::map<Population,double,PopComparator> concentrations,
           double food_concentration,
           std::pair<int, int> position)
{
    this->populations = populations;
    this->concentrations = concentrations;
    this->food_concentration = food_concentration;
    this->position = position;
}

double Cell::population(){
    double pop = 0;
    for(auto&& p: populations){
        pop += concentrations[p];
    }
    return pop;
}

double Cell::enemies(Population pop){
    double en=0;
    for(auto&& p: populations){
        if(p.index != pop.index){
            en += concentrations[p];
        }
    }
    return en;
}

void Cell::fight(){
    std::map<Population, double, PopComparator> killed;
    for(auto&& p1: populations){
        killed[p1] = enemies(p1)*0.1;
    }
    for(auto&& p: populations){
        concentrations[p] -= killed[p];
        if(concentrations[p]<0){
            concentrations[p]=0;
        }
    }
}

void Cell::eat(double consuming_constant){
    double needs = population() * consuming_constant;
    if( needs <= food_concentration ){
        food_concentration -= needs;
        return;
    }
    double shortage = needs - food_concentration;
    double pop = population();
    food_concentration = 0;
    for( auto && p: populations){
        concentrations[p] *= 1-1./pop*(shortage/consuming_constant);
        if(concentrations[p]<0){
            concentrations[p]=0;
        }
    }
}

void Cell::multiply(){
    std::map<Population,double,PopComparator> factors;
    for(auto&& p: populations){
        factors[p] = p.multiply(food_concentration, concentrations[p], enemies(p));
    }
    for(auto&& p: populations){
        concentrations[p] += concentrations[p]*factors[p];
    }
}

double Cell::migrate(Population* pop, Cell* n){
    return pop->migrate(food_concentration,
                        concentrations[*pop],
                        enemies(*pop),
                        n->food_concentration,
                        n->concentrations[*pop],
                        n->enemies(*pop));
}

int cast(double color){
    return std::max(0,std::min(255,static_cast<int>(color)));
}

QColor Cell::getColor(){
    double r=0,g=0,b=0;
    double a=255;
    if(!excluded){
        double pop = population();
        if(pop != 0){
            a=std::min(static_cast<int>(255*pop/5),255);
            for(auto && p: populations){
                r+=p.color.red()*concentrations[p];
                g+=p.color.green()*concentrations[p];
                b+=p.color.blue()*concentrations[p];
            }
            r /= pop;
            g /= pop;
            b /= pop;
        } else {
            r=255;
            g=255;
            b=255;
        }
    }
    return QColor(cast(r),cast(g),cast(b),cast(a));
}
