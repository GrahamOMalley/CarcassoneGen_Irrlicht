#ifndef TILE_ENGINE
#define TILE_ENGINE

#define MAP_WIDTH 5
#define MAP_HEIGHT 5

#include "irrlicht.h"
#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>

namespace tile_engine
{
    class tileTexture
    {
        public:

            //static Texture2D TileSetTexture;
            static int tileWidth;// = 32;
            static int tileHeight;// = 32;

            static irr::core::rect<int> getSourceRectangle(irr::video::ITexture* tex, int tileIndex);
    };

    class tileMapCell
    {
        public:
            int tileID;
            int rot;
            std::string data;

            void rotate();
            void setData();

            tileMapCell()
                : tileID(0), rot(0) 
            { this->setData(); }

            tileMapCell(int id)
                : tileID(id), rot(0) 
            { this->setData(); }

            tileMapCell(int id, int r)
                : tileID(id), rot(0) 
            { 
                this->setData(); 
                for (int i = 0; i < r; ++i) 
                    this->rotate();
                
            }
    };


    class tileMap
    {
        public:
            typedef std::array<tileMapCell, MAP_HEIGHT> columns;
            std::array<columns, MAP_WIDTH> rows;
            
            typedef std::list<std::pair<int, int> > tileList;
            tileList filledTiles;
            
            tileMap();
            void generateNextTile();
            tileList getNeighbours(const std::pair<int, int> &p);

            bool tileFits(const tileMapCell &m, const std::pair<int, int> &f);
    };
}
#endif
