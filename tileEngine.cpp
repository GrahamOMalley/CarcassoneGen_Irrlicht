#include "tileEngine.h"

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

using namespace std;
using namespace tile_engine;

using namespace irr;
using namespace core;
using namespace video;

// Tile
int tileTexture::tileWidth = 80;
int tileTexture::tileHeight = 80;

rect<int> tileTexture::getSourceRectangle(ITexture* tex, int tileIndex)
{
    //0,0, 80, 80 | 80, 0, 160, 80 | 160, 0, 240, 80
    return rect<int>(tileIndex * tileWidth, 0, tileWidth*(tileIndex+1), tileHeight);
}

// TileMapCell
void tileMapCell::rotate()
{
    // rotate the tile clockwise

    // only allowable values 0 1 2 3
    rot++;
    if(rot > 3)
        rot = 0;
    // CCRR -> RCCR
    data = data[3] + data.substr(0, 3);
}

void tileMapCell::setData()
{
    switch (tileID)
    {
        case 0: data = std::string("NNNN"); break;
        case 1: data = std::string("CCCC"); break;
        case 2: data = std::string("CCCG"); break;
        case 3: data = std::string("CCCR"); break;
        case 4: data = std::string("CCGG"); break;
        case 5: data = std::string("CCGG"); break;
        case 6: data = std::string("CCRR"); break;
        case 7: data = std::string("CGCG"); break;
        case 8: data = std::string("GCGC"); break;
        case 9: data = std::string("GCGG"); break;
        case 10: data = std::string("GCRR"); break;
        case 11: data = std::string("GGGG"); break;
        case 12: data = std::string("GGGR"); break;
        case 13: data = std::string("GRGR"); break;
        case 14: data = std::string("RCGR"); break;
        case 15: data = std::string("RCRG"); break;
        case 16: data = std::string("RCRR"); break;
        case 17: data = std::string("RGGR"); break;
        case 18: data = std::string("RGRR"); break;
        case 19: data = std::string("RRRR"); break;
    }
}

// TileMap
tileMap::tileMap()
{
}

tileMap::tileList tileMap::getNeighbours(const std::pair<int, int> &p)
{
    tileMap::tileList l;
    // up
    if((this->rows[p.second-1][p.first].tileID ==0) && (p.second > 0))
        l.push_back(std::make_pair(p.first, p.second - 1));

    // down
    if (p.second < MAP_HEIGHT - 1)
    {
        if (this->rows.at(p.second + 1).at(p.first).tileID == 0)
            l.push_back(std::make_pair(p.first, p.second + 1));
    }

    // left
    if (p.first > 0)
    {
        if (this->rows.at(p.second).at(p.first - 1).tileID == 0)
            l.push_back(std::make_pair(p.first - 1, p.second));
    }

    // right
    if (p.first < MAP_WIDTH - 1)
    {
        if (this->rows.at(p.second).at(p.first + 1).tileID == 0)
            l.push_back(std::make_pair(p.first + 1, p.second));
    }
    return l;
}

// TODO: clean up these ifs, can take up 1/4 of the space :P
bool tileMap::tileFits(const tileMapCell &m, const std::pair<int, int> &f)
{
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    // check up
    // up
    if (f.second > 0)
    {
        // tile above is empty
        if (this->rows[f.second - 1][f.first].tileID == 0)
            up = true;
        else
        {
            //m[left][UP][right][down] == other[left][up][right][DOWN]
            if (m.data[1] == this->rows[f.second - 1][f.first].data[3])
                up = true;
        }
    }
    else up = true;

    // down
    if (f.second < MAP_HEIGHT - 1)
    {
        // tile below is empty
        if (this->rows[f.second + 1][f.first].tileID == 0)
            down = true;
        else
        {
            //m[left][up][right][DOWN] == other[left][UP][right][down]
            if (m.data[3] == this->rows[f.second + 1][f.first].data[1])
                down = true;
        }
    }
    else down = true;

    // left
    if (f.first > 0)
    {
        // tile to the left is empty
        if (this->rows[f.second][f.first - 1].tileID == 0)
            left = true;
        else
        {
            //m[LEFT][up][right][down] == other[left][up][RIGHT][down]
            if (m.data[0] == this->rows[f.second][f.first - 1].data[2])
                left = true;
        }
    }
    else left = true;

    // right
    if (f.first < MAP_WIDTH - 1)
    {
        // tile to the right is empty
        if (this->rows[f.second][f.first + 1].tileID == 0)
            right = true;
        else
        {
            //m[left][up][RIGHT][down] == other[LEFT][up][right][down]
            if (m.data[2] == this->rows[f.second][f.first + 1].data[0])
                right = true;
        }
    }
    else right = true;

    return (up && down && left && right);
}

void tileMap::generateNextTile()
{
    if(filledTiles.size() <= 0)
    {
        // need to initialize, select one tile as seed
        srand((unsigned)time(0)); 

        int tid = rand() % 20;
        int r = rand() % 4;
        int i = rand() % 5;
        int j = rand() % 5;
        this->rows[i][j] = tileMapCell(tid, r);
        filledTiles.push_back(std::make_pair(i,j));
    }
    else if (filledTiles.size() < (MAP_WIDTH*MAP_HEIGHT))
    {
        // pick one at random
        auto it = filledTiles.begin();
        std::advance(it, rand() % filledTiles.size() );

        tileList neighbours = getNeighbours(*it);
        if (neighbours.size() == 0) 
        {
            filledTiles.erase(it);
            this->generateNextTile();
        }
        else
        {
            //pick an eligible tile at random
            //std::pair<int, int> et = neighbours.ElementAt(random.Next(0, neighbours.size()));
            auto nit = neighbours.begin();
            std::advance(nit, rand() % neighbours.size());
            // randomly generate tiles until a match is found
            for (int hardCount = 0; hardCount < 1000; hardCount++)
            {
                // if no match can be found, mark tile as not placeable
                tileMapCell m = tileMapCell(rand() % MAP_WIDTH);
                //for each rotation
                for (int r = 0; r < 4; r++)
                {
                    if (tileFits(m, *nit))
                    {
                        this->rows[(*nit).second][(*nit).first] = m;
                        filledTiles.push_back(*nit);
                        hardCount = 1000;
                        return;
                    }
                    else
                    {
                        m.rotate();
                    }
                }
            }
        }
    }
}
