#include "genotype.h"
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <ctime>

Genotype::Genotype()
{
}

Genotype::Genotype( std::vector<std::vector<std::vector<double> > > genes ){
    this->genes = genes;
}

double Genotype::phenotype( std::vector<double> environment ){
    double val=0;
    std::vector<double> vals;
    for(size_t i=0; i<genes.size(); i++){
        for(size_t j=0; j<genes[i].size(); j++){
            val=0;
            for(size_t k=0; k<genes[i][j].size(); k++){
                val+=genes[i][j][k]*environment[k];
            }
            if(i==genes.size()-1){
                return std::tanh(val);
            }
            vals.push_back(std::tanh(val));
        }
        environment = vals;
        vals.clear();
    }
    return 0;
}

void Genotype::mutate(double range, bool absolute){
    std::srand(std::time(0));
    for(auto&& g_arr: genes){
        for(auto&& g_row: g_arr){
            for(auto&& gene: g_row){
                double factor = range*2*(static_cast<double>(std::rand())/RAND_MAX-0.5);
                if(absolute){
                    gene += factor;
                } else {
                    gene += factor*gene;
                }
            }
        }
    }
}

std::ostream& operator<<(std::ostream& out, Genotype genotype){
    for(auto&& g_arr: genotype.genes){
        for(auto&& g_row: g_arr){
            for(auto&& g: g_row){
                out<<g<<' ';
            }
            out<<std::endl;
        }
    }
    return out;
}
