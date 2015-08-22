
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "map.hpp"
#include "noise/perlin.hpp"

#include <algorithm>

Map::Map(int width, int height, float resolution) : width_(width), height_(height), tiles_(width_, std::vector<BIOME>(height))
{
    bm_ = al_create_bitmap(width_ * 32 , height_ * 32);
    al_set_target_bitmap(bm_);

    const float max = static_cast<float>(std::max(width_, height_));

    for(int i = 0; i < width_; ++i)
    {
        for(int j = 0; j < height_; ++j)
        {
            float vec[2] = {(i + 0.5f) / max * resolution, (j + 0.5f) / max * resolution};
            float per = noise2(vec);

            if(per < -0.2)
            {
                tiles_[i][j] = SWAMP;
                al_draw_filled_rectangle(i * 32, j * 32, i * 32 + 32, j * 32 + 32, al_map_rgb(0, 255, 0));
            }
            else
            {
                tiles_[i][j] = GRASS;
                al_draw_filled_rectangle(i * 32, j * 32, i * 32 + 32, j * 32 + 32,  al_map_rgb(255, 255, 0));
            }
        }
    }
    al_set_target_backbuffer(al_get_current_display());
}

void Map::draw()
{
    al_draw_bitmap(bm_, 0, 0, 0);
}

void Map::toggleTile(int x, int y)
{
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
    al_set_target_backbuffer(al_get_current_display());
}
