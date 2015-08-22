
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "map.hpp"
#include "noise/perlin.hpp"

#include <algorithm>

Map::Map(int width, int height, float resolution) : width_(width), height_(height)
{
    bm_ = al_create_bitmap(width_, height_);
    al_set_target_bitmap(bm_);

    const float max = static_cast<float>(std::max(width_, height_));

    for(int i = 0; i < width_; ++i)
    {
        for(int j = 0; j < height_; ++j)
        {
            float vec[2] = {(i + 0.5f) / max * resolution, (j + 0.5f) / max * resolution};
            al_draw_pixel(i, j , al_map_rgb_f(noise2(vec), 0., 0.));
        }
    }
    al_set_target_backbuffer(al_get_current_display());
}

void Map::draw()
{
    al_draw_scaled_bitmap(bm_, 0, 0, width_, height_, 0, 0, width_ * 32, height_ * 32, 0);
}
