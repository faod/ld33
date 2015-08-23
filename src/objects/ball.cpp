#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>



#include "ball.hpp"
#include "../game.hpp"

Ball::Ball(glm::vec2 position, Game &g, float direction) : BoxObject(glm::vec2(32.f, 32.f)), game_(g), distleft_(360.)
{
    setPosition(position);
    orientation = direction;
    setSpeed(2.);

    sprite_ = al_create_bitmap(32, 32);
    al_set_target_bitmap(sprite_);
    al_clear_to_color(al_map_rgb(0, 150, 150));
    al_set_target_backbuffer(al_get_current_display());
}

Ball::~Ball()
{
    al_destroy_bitmap(sprite_);
}

void Ball::update()
{
    auto ptr = shared_from_this();
    if(distleft_ <= 0.)
    {
        game_.removeObject(ptr);
    }

    step();
    distleft_-= speed;
}

void Ball::draw(glm::vec2 screen_ul_corner)
{
    al_draw_rotated_bitmap(sprite_,
                           16,
                           16,
                           this->position.x - screen_ul_corner.x,
                           this->position.y - screen_ul_corner.y,
                           orientation,
                           0);
    drawHull(screen_ul_corner);
}
