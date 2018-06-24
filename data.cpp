#include "data.h"

Data::Data()
{

}

Data::Data(std::vector<Population> populations){
    this->populations = populations;
}

Data::Data(std::ifstream& stream){
    populations.clear();
    for(int i=0; i<1; i++){
        populations.push_back(Population(stream));
        populations[i].index=i;
    }
}
