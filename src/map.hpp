#pragma once
#ifndef MAP_HPP
#define MAP_HPP


class Map
{
    public:
        Map(int width, int height, float resolution);

        void draw();
    private:
        int width_;
        int height_;
        ALLEGRO_BITMAP *bm_;

};

#endif //MAP_HPP
