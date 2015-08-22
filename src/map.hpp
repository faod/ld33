#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

class Map
{
    public:

        enum BIOME {SWAMP, GRASS};

        Map(int width, int height, float resolution);

        void draw(int xpos, int ypos, int w, int h);
        void toggleTile(int x, int y);
    private:
        int width_;
        int height_;
        ALLEGRO_BITMAP *bm_;
        std::vector<std::vector< BIOME >> tiles_;
};

#endif //MAP_HPP
