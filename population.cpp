#include "population.h"
#include <sstream>
#include <iostream>

Population::Population()
{
}

Population::Population(std::ifstream& stream){
    std::string line;
    getline(stream,name);
    std::vector<std::vector<std::vector<double> > > diffusion, growth;
    for(int i=0; i<3; i++){             //Growth genes
        std::vector<std::vector<double> > arr;
        for(int j=0;j<3;j++){
            getline(stream, line);
            std::istringstream sstream(line);
            std::vector<double> row;
            for(int k=0;k<3;k++){
                double arg;
                sstream >> arg;
                row.push_back(arg);
            }
            arr.push_back(row);
            if(i==2){
                break;
            }
        }
        growth.push_back(arr);
    }
    this->growth = growth;

    for(int i=0; i<3; i++){             //Migration genes
        std::vector<std::vector<double> > arr;
        for(int j=0;j<3;j++){
            getline(stream, line);
            std::istringstream sstream(line);
            std::vector<double> row;
            int max_k=3;
            if(i==0){
                max_k = 6;
            }
            for(int k=0;k<max_k;k++){
                double arg;
                sstream >> arg;
                row.push_back(arg);
            }
            arr.push_back(row);
            if(i==2){
                break;
            }
        }
        diffusion.push_back(arr);
    }
    this->diffusion = diffusion;

    getline(stream, line);
    std::istringstream sstream(line);
    int r,g,b;
    sstream >> r >> g >> b;
    this->color = QColor(r,g,b);
};

double Population::multiply(double food_conc, double self_conc, double enemy_conc){
    std::vector<double> env = {food_conc,self_conc,enemy_conc};
    if(self_conc==0){
        return 0;
    }
    double gr = growth.phenotype(env);
    return gr*gr;
}

double Population::migrate(double food_conc, double self_conc, double enemy_conc, double food_conc2,double self_conc2, double enemy_conc2){
    if(self_conc==0){
        return 0;
    }
    std::vector<double> env = {food_conc,self_conc,enemy_conc,food_conc2,self_conc2,enemy_conc2};
    double diff = diffusion.phenotype(env);
    return diff*diff;
}

void Population::save(){
    std::ofstream stream;
    stream.open(name+".dat");
    stream<<*this;
}

void Population::mutate(double range, bool absolute){
    growth.mutate(range, absolute);
    diffusion.mutate(range, absolute);
}

std::ostream& operator<<(std::ostream& out, Population pop){
    out<<pop.name<<std::endl;
    out<<pop.growth<<pop.diffusion;
    QColor c=pop.color;
    out<<c.red()<<' '<<c.green()<<' '<<c.blue()<<std::endl;
    return out;
}

bool PopComparator::operator()(Population lhs, Population rhs){
    return lhs.index<rhs.index;
}
