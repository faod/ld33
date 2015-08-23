
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

ALLEGRO_BITMAP *Tile::flames = NULL;

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

/*******************
 *
 *
 *  _______ ___       _
 *     |     |   |   |
 *     |     |   |   |_
 *     |    _|_  |__ |_
 *
 *
 *
 * **************/



Tile::Tile(int x, int y, BIOME b) : x_(x), y_(y), biome_(b), voisins_{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS}, ignitetime_(0)
{
}

std::shared_ptr<ALLEGRO_BITMAP> Tile::getFlameFrame(size_t num)
{
    if(flames == NULL)
    {
        ALLEGRO_PATH *path;

        path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        al_append_path_component(path, RESOURCES_DIR);
        al_set_path_filename(path, "flames.png");
        flames = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));

        al_destroy_path(path);
    }
    if(num < 3)
    {
        return std::shared_ptr<ALLEGRO_BITMAP>(al_create_sub_bitmap(flames, num * 32, 0, 32, 32), al_destroy_bitmap);
    }
    return NULL;
}

bool Tile::operator==(const BIOME &b)
{
    return b == biome_;
}

void Tile::setVoisins(BIOME v[9])
{
    for(int i = 0; i < 9; ++i) voisins_[i] = v[i];
}

BIOME Tile::getBiome() const
{
    return biome_;
}

size_t Tile::topleft() const
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

size_t Tile::topright() const
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

size_t Tile::botleft() const
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

size_t Tile::botright() const
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

void Tile::update(Map &m)
{
    if(biome_ == SWAMP && ignitetime_)
    {
        ignitetime_--;

        if(ignitetime_ == 0)
        {
            biome_ = GRASS;
            m.updatevoisins(x_, y_);
            m.drawTile(*this, nullptr);
        }
        else
        {
            m.drawTile(*this, getFlameFrame(ignitetime_ % 3));
        }
    }
}

void Tile::ignite(unsigned int time)
{
    if(biome_ == SWAMP)
    {
        ignitetime_ = time;
    }
}

void Tile::setVoisins(int id, BIOME b)
{
    voisins_[id] = b;
}


/***************
 *               __
 * |\_/|   /\   |  |
 * |   |  /__\  |__|
 * |   | /    \ |
 *
 *
 *
 * **************/



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
    rockgen(resolution);
    updatevoisins();
    drawrock();
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
    for(int y = 0; y < tiles_.size(); y++)
    {
        for(int x = 0; x < tiles_[y].size(); x++)
        {
            drawTile(tiles_[y][x], nullptr);
        }
    }
}

void Map::updatevoisins(int x, int y)
{
    BIOME b = tiles_[y][x].getBiome();
    if(x >= 1 && y >= 1)
    {
        tiles_[y - 1][x - 1].setVoisins(8, b);
        if(tiles_[y - 1][x - 1].getBiome() == SWAMP)
            drawTile(tiles_[y - 1][x - 1], nullptr);
    }
    if(y >= 1)
    {
        tiles_[y - 1][x].setVoisins(7, b);
        if(tiles_[y - 1][x].getBiome() == SWAMP)
            drawTile(tiles_[y - 1][x], nullptr);
    }
    if(x < tiles_[y].size() - 1 && y >= 1)
    {
        tiles_[y - 1][x + 1].setVoisins(6, b);
        if(tiles_[y - 1][x + 1].getBiome() == SWAMP)
            drawTile(tiles_[y - 1][x + 1], nullptr);
    }
    if(x >= 1)
    {
        tiles_[y][x - 1].setVoisins(5, b);
        if(tiles_[y][x - 1].getBiome() == SWAMP)
            drawTile(tiles_[y][x - 1], nullptr);
    }
    if(x < tiles_[y].size() - 1)
    {
        tiles_[y][x + 1].setVoisins(3, b);
        if(tiles_[y][x + 1].getBiome() == SWAMP)
            drawTile(tiles_[y][x + 1], nullptr);
    }
    if(x >= 1 && y < tiles_.size() - 1)
    {
        tiles_[y + 1][x - 1].setVoisins(0, b);
        if(tiles_[y + 1][x - 1].getBiome() == SWAMP)
            drawTile(tiles_[y + 1][x - 1], nullptr);
    }
    if(y < tiles_.size() - 1)
    {
        tiles_[y + 1][x].setVoisins(1, b);
        if(tiles_[y + 1][x].getBiome() == SWAMP)
            drawTile(tiles_[y + 1][x], nullptr);
    }
    if(x < tiles_[y].size() - 1 && y < tiles_.size() - 1)
    {
        tiles_[y + 1][x + 1].setVoisins(2, b);
        if(tiles_[y + 1][x + 1].getBiome() == SWAMP)
            drawTile(tiles_[y + 1][x + 1], nullptr);
    }
}

void Map::drawrock()
{
    al_set_target_bitmap(bm_);
    for(int y = 0; y < tiles_.size(); y++)
    {
        for(int x = 0; x < tiles_[y].size(); x++)
        {
            BIOME b = tiles_[y][x].getBiome();

            if(b == ROCK)
            {
                drawTile(tiles_[y][x], nullptr);
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
            }
        }
    }
}

void Map::update()
{
    static bool start = true;
    for(int y = 0; y < tiles_.size(); ++y)
    {
        for(int x = 0; x < tiles_[y].size(); ++x)
        {
            tiles_[y][x].update(*this);
            if(start && x % 2)
            {
                tiles_[y][x].ignite(100);
            }
        }
    }
                start = false;
}

void Map::draw(int xpos, int ypos, int w, int h)
{
    al_draw_bitmap_region(bm_, xpos, ypos, w, h, 0, 0, 0);
}


void Map::drawTile(const Tile &t, std::shared_ptr<ALLEGRO_BITMAP> ptr)
{
    std::shared_ptr<ALLEGRO_BITMAP> bm;
    Tileset* ts = nullptr;

    switch(t.getBiome())
    {
        case GRASS: ts = &grass_; break;
        case SWAMP: ts = &swamp_; break;
        case ROCK:  ts = &rock_;  break;
        default: break;
    };

    al_set_target_bitmap(bm_);

    bm = *ts << t.topleft();
    al_draw_bitmap(bm.get(), t.getx() * 32, t.gety() * 32, 0);
    bm = *ts << t.topright();
    al_draw_bitmap(bm.get(), t.getx() * 32 + 16, t.gety() * 32, 0);
    bm = *ts << t.botleft();
    al_draw_bitmap(bm.get(), t.getx() * 32, t.gety() * 32 + 16, 0);
    bm = *ts << t.botright();
    al_draw_bitmap(bm.get(), t.getx() * 32 + 16, t.gety() * 32 + 16, 0);

    if(ptr != nullptr)
    {
        al_draw_bitmap(ptr.get(), t.getx() * 32, t.gety() * 32, 0);
    }
    al_set_target_backbuffer(al_get_current_display());
}
