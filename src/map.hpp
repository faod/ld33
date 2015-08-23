#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <memory>
class Game;

enum BIOME {GRASS,SWAMP, ROCK};

class Tileset
{
    public:
        Tileset(ALLEGRO_COLOR cl);
        Tileset(const char* filename);

        std::shared_ptr<ALLEGRO_BITMAP> operator<<(size_t tile);
    private:
        void loadTiles();

        ALLEGRO_BITMAP* spritesheet_;
        std::vector<ALLEGRO_BITMAP*> tiles_;
};

class Map;
class Tile
{
    public:
        Tile(int x = 0, int y = 0, BIOME b = GRASS);

        bool operator==(const BIOME &b);

        void update(Map &m);
        void ignite(unsigned int time);


        inline int getx() const { return x_; };
        inline int gety() const { return y_; };

        void setVoisins(BIOME v[9]);
        void setVoisins(int id, BIOME b);
        BIOME getBiome() const;
        size_t topleft() const;
        size_t topright() const;
        size_t botleft() const;
        size_t botright() const;

        static std::shared_ptr<ALLEGRO_BITMAP> getFlameFrame(size_t num);
    private:
        static ALLEGRO_BITMAP *flames;
        int x_;
        int y_;
        BIOME biome_;
        BIOME voisins_[9];
        unsigned int ignitetime_;
};

class Map
{
    public:


        Map(int width, int height, float resolution, Game& game);

        void update();
        void draw(int xpos, int ypos, int w, int h);
        void drawTile(const Tile &t, std::shared_ptr<ALLEGRO_BITMAP> mask = nullptr);
        void updatevoisins(int x, int y);
    private:
        void swampgrassgen(float resolution);
        void updatevoisins();
        void rockgen(float resolution);
        void drawswampgrass();
        void drawrock();

        int width_;
        int height_;
        Tileset grass_;
        Tileset swamp_;
        Tileset rock_;
        ALLEGRO_BITMAP *bm_;
        std::vector<std::vector< Tile >> tiles_;
        Game &game_;
};

#endif //MAP_HPP
