#ifndef GAME_H
#define GAME_H
#include <vector>
#include "cell.h"
#include "data.h"
#include "population.h"

class Game
{
public:
    Game();
    Game(Data data);

    std::map<std::pair<int,int>, Cell> cells;

    void move();
    std::map<Population, double, PopComparator> get_results();
    std::map<Population, double, PopComparator> play(int n_turns);
    int turn=0;

private:
    std::vector< Population > populations;
    double food_diffusion;
    double consuming_constant;
    int size=4;

    void migrate();
    void diffusion();
};

Population winner(std::map<Population, double, PopComparator>);

#endif // GAME_H
