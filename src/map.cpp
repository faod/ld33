
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "map.hpp"
#include "noise/perlin.hpp"
#include "game.hpp"


Tileset::Tileset(ALLEGRO_COLOR cl)
{
    spritesheet_ = al_create_bitmap(64, 64);
    al_set_target_bitmap(spritesheet_);
    al_clear_to_color(cl);
    al_set_target_backbuffer(al_get_current_display());
}
Tileset::Tileset(const char* filename)
{
    ALLEGRO_PATH *path;

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, RESOURCES_DIR);
    al_set_path_filename(path, filename);
    spritesheet_ = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));

    al_destroy_path(path);
}

std::shared_ptr<ALLEGRO_BITMAP> Tileset::operator<<(size_t tile)
{
    ALLEGRO_BITMAP *bm = al_create_sub_bitmap(spritesheet_, (tile % 4) * 16, (tile / 4) * 16, 16, 16);
    return std::shared_ptr<ALLEGRO_BITMAP>(bm, al_destroy_bitmap);
}

Tile::Tile(int x, int y, BIOME b) : x_(x), y_(y), biome_(b), voisins_{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS}
{
}

bool Tile::operator==(const BIOME &b)
{
    return b == biome_;
}

void Tile::setVoisins(BIOME v[9])
{
    for(int i = 0; i < 9; ++i) voisins_[i] = v[i];
}

BIOME Tile::getBiome()
{
    return biome_;
}

size_t Tile::topleft()
{
    if(voisins_[0] != biome_ && voisins_[1] != biome_ && voisins_[3] != biome_)
        return 2;
    if(voisins_[0] == biome_ && voisins_[1] == biome_ && voisins_[3] == biome_)
        return 8;
    if(voisins_[1] == biome_ && voisins_[3] == biome_)
        return 0;
    if(voisins_[1] != biome_ && voisins_[3] != biome_)
        return 2;

    if(voisins_[1] == biome_)
        return 10;

    return 11;
}

size_t Tile::topright()
{
    if(voisins_[1] != biome_ && voisins_[2] != biome_ && voisins_[5] != biome_)
        return 3;
    if(voisins_[1] == biome_ && voisins_[2] == biome_ && voisins_[5] == biome_)
        return 9;
    if(voisins_[1] == biome_ && voisins_[5] == biome_)
        return 1;
    if(voisins_[1] != biome_ && voisins_[5] != biome_)
        return 3;

    if(voisins_[1] == biome_)
    return 15;

    return 11;


}

size_t Tile::botleft()
{
    if(voisins_[3] != biome_ && voisins_[6] != biome_ && voisins_[7] != biome_)
        return 6;
    if(voisins_[3] == biome_ && voisins_[6] == biome_ && voisins_[7] == biome_)
        return 12;
    if(voisins_[3] == biome_ && voisins_[7] == biome_)
        return 4;
    if(voisins_[3] != biome_ && voisins_[7] != biome_)
        return 6;

    if(voisins_[3] == biome_)
    return 14;

    return 10;

}

size_t Tile::botright()
{

    if(voisins_[5] != biome_ && voisins_[8] != biome_ && voisins_[7] != biome_)
        return 7;
    if(voisins_[5] == biome_ && voisins_[8] == biome_ && voisins_[7] == biome_)
        return 13;
    if(voisins_[5] == biome_ && voisins_[7] == biome_)
        return 5;
    if(voisins_[5] != biome_ && voisins_[7] != biome_)
        return 7;

    if(voisins_[5] == biome_)
    return 14;

    return 15;
}

Map::Map(int width, int height, float resolution, Game& game) : width_(width),
                                                                height_(height),
                                                                grass_(al_map_rgb(106, 190, 48)),
                                                                swamp_("swamp.png"),
                                                                rock_("rocks.png"),
                                                                tiles_(height_, std::vector<Tile>(width_)),
                                                                game_(game)
{
    bm_ = al_create_bitmap(width_ * 32 , height_ * 32);
    al_set_target_bitmap(bm_);

    swampgrassgen(resolution);
    updatevoisins();
    drawswampgrass();

    reseed(al_get_time() * 1000);
    //rockgen(resolution);
    al_set_target_backbuffer(al_get_current_display());
}

void Map::updatevoisins()
{
    for(int y = 0; y < tiles_.size(); y++)
    {
        for(int x = 0; x < tiles_[y].size(); x++)
        {
            BIOME voisins[9];
            voisins[0] = (y >= 1 && x >= 1) ? tiles_[y - 1][x - 1].getBiome() : tiles_[y][x].getBiome();
            voisins[1] = (y >= 1)           ? tiles_[y - 1][x].getBiome() : tiles_[y][x].getBiome();
            voisins[2] = (y >= 1 && x < tiles_[y - 1].size() - 1) ? tiles_[y - 1][x + 1].getBiome() : tiles_[y][x].getBiome();

            voisins[3] = (x >= 1) ? tiles_[y][x - 1].getBiome() : tiles_[y][x].getBiome();
            voisins[4] =  tiles_[y][x].getBiome();
            voisins[5] = (x < tiles_[y].size() - 1) ? tiles_[y][x + 1].getBiome() : tiles_[y][x].getBiome();

            voisins[6] = (y < tiles_.size() - 1 && x >= 1) ? tiles_[y + 1][x - 1].getBiome() : tiles_[y][x].getBiome();
            voisins[7] = (y < tiles_.size() - 1) ? tiles_[y + 1][x].getBiome() : tiles_[y][x].getBiome();
            voisins[8] = (y < tiles_.size() - 1 && x < tiles_[y + 1].size() - 1) ? tiles_[y + 1][x + 1].getBiome() : tiles_[y][x].getBiome();

            tiles_[y][x].setVoisins(voisins);
        }
    }
}

void Map::drawswampgrass()
{
    al_set_target_bitmap(bm_);
    for(int y = 0; y < tiles_.size(); y++)
    {
        for(int x = 0; x < tiles_[y].size(); x++)
        {
           BIOME b = tiles_[y][x].getBiome();
           if(b == SWAMP)
           {
                std::shared_ptr<ALLEGRO_BITMAP> bm = swamp_ << tiles_[y][x].topleft();
                al_draw_bitmap(bm.get(), x * 32, y * 32, 0);
                bm = swamp_ << tiles_[y][x].topright();
                al_draw_bitmap(bm.get(), x * 32 + 16, y * 32, 0);
                bm = swamp_ << tiles_[y][x].botleft();
                al_draw_bitmap(bm.get(), x * 32, y * 32 + 16, 0);
                bm = swamp_ << tiles_[y][x].botright();
                al_draw_bitmap(bm.get(), x * 32 + 16, y * 32 + 16, 0);
           }
           else if(b == GRASS)
           {
                std::shared_ptr<ALLEGRO_BITMAP> bm = grass_ << tiles_[y][x].topleft();
                al_draw_bitmap(bm.get(), x * 32, y * 32, 0);
                bm = grass_ << tiles_[y][x].topright();
                al_draw_bitmap(bm.get(), x * 32 + 16, y * 32, 0);
                bm = grass_ << tiles_[y][x].botleft();
                al_draw_bitmap(bm.get(), x * 32, y * 32 + 16, 0);
                bm = grass_ << tiles_[y][x].botright();
                al_draw_bitmap(bm.get(), x * 32 + 16, y * 32 + 16, 0);
           }
        }
    }
    al_set_target_backbuffer(al_get_current_display());
}

void Map::swampgrassgen(float resolution)
{
    const float max = static_cast<float>(std::max(width_, height_));

    for(int y = 0; y < height_; ++y)
    {
        for(int x = 0; x < width_; ++x)
        {
            float vec[2] = {(x + 0.5f) / max * resolution, (y + 0.5f) / max * resolution};
            float per = noise2(vec);

            if(per < -0.2)
            {
                tiles_[y][x] = Tile(x, y, SWAMP);
            }
            else
            {
                tiles_[y][x] = Tile(x, y , GRASS);
            }
        }
    }

}

void Map::rockgen(float resolution)
{
    const float max = static_cast<float>(std::max(width_, height_));

    for(int y = 0; y < height_; ++y)
    {
        for(int x = 0; x < width_; ++x)
        {
            float vec[2] = {(x + 0.5f) / max * resolution, (y + 0.5f) / max * resolution};
            float per = noise2(vec);

            if(per < -0.4)
            {
                tiles_[y][x] = Tile(x, y , ROCK);
                al_draw_filled_rectangle(x * 32, y * 32, x * 32 + 32, y * 32 + 32, al_map_rgb(222, 184, 135));
            }
        }
    }
}


void Map::draw(int xpos, int ypos, int w, int h)
{
    al_draw_bitmap_region(bm_, xpos, ypos, w, h, 0, 0, 0);
}

void Map::toggleTile(int x, int y)
{
 /*   if(tiles_[x][y] == ROCK)
        return;

    al_set_target_bitmap(bm_);

    ALLEGRO_COLOR cl;
    if( tiles_[x][y] == SWAMP)
    {
        tiles_[x][y] = GRASS;
        cl = al_map_rgb(255, 255, 0);
    }
    else
    {
        tiles_[x][y] = SWAMP;
        cl = al_map_rgb(0, 255, 0);
    }
    al_draw_filled_rectangle(x * 32, y * 32, x * 32 + 32, y * 32 + 32, cl);
    al_set_target_backbuffer(al_get_current_display());*/
}
