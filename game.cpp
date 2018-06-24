#include <vector>
#include "data.h"
#include "game.h"
#include "cell.h"
#include <cstdlib>

Game::Game(){}

Game::Game(Data data)
{
    this->populations = data.populations;
    this->food_diffusion = data.food_diffusion;
    this->consuming_constant = data.consuming_constant;
    this->size = data.field_size;
    std::map<Population,double,PopComparator> popmap;
    for(auto&& p: data.populations){
        popmap[p] = 0.;
    }
    for(int i=0; i<=2*size-2; i++){
        int j0=std::max(0,i+1-size);
        int jmax = 2*size-2;
        if(i < size-1){
            jmax = i+size-1;
        }
        for(int j=j0; j<=jmax; j++){
            Cell newCell(populations, popmap, .15, std::make_pair(i,j));
            cells[{i,j}] = newCell;
        }
    }
    for(auto&& item: cells){
        auto& c = item.second;
        int i=c.position.first, j=c.position.second;
        if( j!=0 && i-j<size-1 ){
            cells[{i,j}].neibourghs.push_back(&(cells[{i,j-1}]));
            cells[{i,j-1}].neibourghs.push_back(&(cells[{i,j}]));
        }
        if( i!=0 && j!=0 ){
            cells[{i,j}].neibourghs.push_back(&(cells[{i-1,j-1}]));
            cells[{i-1,j-1}].neibourghs.push_back(&(cells[{i,j}]));
        }
        if( i!=0 && j-i<size-1 ){
            cells[{i,j}].neibourghs.push_back(&(cells[{i-1,j}]));
            cells[{i-1,j}].neibourghs.push_back(&(cells[{i,j}]));
        }
    }

    auto begin = cells.begin();
    for(int n=0; n<=size*size/2; n++){
        int cell = (cells.size()*std::rand())/RAND_MAX;
        auto it = std::next(begin, cell);
        if(it->second.position == std::make_pair<int,int>(0,0) ||
            it->second.position == std::make_pair<int,int>(2*size-2,size-1) ||
            it->second.position == std::make_pair<int,int>(size-1,2*size-2)){
            n--;
            continue;
        }
        it->second.excluded = true;
        for(auto&& n: it->second.neibourghs){
            for(size_t i=0; i < n->neibourghs.size(); i++){
                if(n->neibourghs[i]==&it->second){
                    n->neibourghs.erase(n->neibourghs.begin()+i);
                }
            }
        }
        it->second.neibourghs.clear();

    }


    if(populations.size()==2) {
        cells[{0,size-1}].concentrations[populations[0]]=1.;
        cells[{2*size-2,size-1}].concentrations[populations[1]]=1.;
    } else {
        cells[{0,0}].concentrations[populations[0]]=0.1;
        cells[{2*size-2,size-1}].concentrations[populations[1]]=0.1;
        cells[{size-1,2*size-2}].concentrations[populations[2]]=0.1;
    }

}

void Game::move(){
    for(auto&& item: cells){
        auto& cell = item.second;
        cell.fight();
        cell.eat(consuming_constant);
        cell.multiply();
        cell.food_concentration += 0.1;
    }
    migrate();
    diffusion();
    turn++;
}

void Game::migrate(){
    std::map<Population,std::map<std::pair<int,int>,double>, PopComparator> moves;
    for(auto && p: populations){
        for( auto && item: cells){
            auto& c = item.second;
            if(c.concentrations[p] <=0.0001){
                continue;
            }
            auto pos = c.position;
            int n_neibourghs = c.neibourghs.size();
            for(auto&& n: c.neibourghs){
                double move=c.migrate(&p,n)*c.concentrations[p]/(2*n_neibourghs);
                moves[p][pos] -= move;
                moves[p][n->position] += move;
            }
        }
    }
    for(auto && p: populations){
        for( auto && item: cells){
            auto& c = item.second;
            auto pos = c.position;
            cells[pos].concentrations[p] += moves[p][pos];
            if(cells[pos].concentrations[p]<0){
                cells[pos].concentrations[p]=0;
            }
        }
    }
    return;
}

void Game::diffusion(){
    std::map<std::pair<int,int>,double > moves;
    for( auto && item: cells){
        auto& c = item.second;
        auto pos = c.position;
        moves[pos] = 0;
        for(auto && n: c.neibourghs){
            moves[pos] += (c.food_concentration-n->food_concentration)*food_diffusion*(1-food_diffusion);
        }
    }
    for( auto && item: cells){
        auto& c = item.second;
        auto pos = c.position;
        c.food_concentration -= moves[pos];
    }
}

std::map< Population, double, PopComparator > Game::get_results(){
    std::map< Population, double, PopComparator > score;
    for(auto && p: populations){
        score[p] = 0;
        for( auto && item: cells){
            score[p] += item.second.concentrations[p];
        }
    }
    return score;
}

std::map<Population, double, PopComparator> Game::play(int n_turns){
    for(int i=0; i<n_turns; i++){
        move();
    }
    return get_results();
}

Population winner(std::map<Population, double, PopComparator> score){
    Population winner=score.begin()->first;
    for(auto it=std::next(score.begin()); it!=score.end(); ++it){
        if(score[winner]<it->second){
            winner = it->first;
        }
    }
    return winner;
}

