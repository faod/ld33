#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "swampman.hpp"

#define PI 3.141592653589793
#define PI2 1.5707963267948966
#define PI4 0.7853981633974483
#define PI4_3 2.356194490192345


float Swampman::orientations[9] = {PI4_3, -PI, -PI4_3, PI2, 0., -PI2, PI4, 0., -PI4 };

Swampman::Swampman(glm::vec2 pos, Game &g): BoxObject(glm::vec2(32.f, 32.f)) , game_(g), up(false), down(false), left(false), right(false), hp_(50)
{
    setPosition(pos);

    sprite_ = al_create_bitmap(32, 32);
    al_set_target_bitmap(sprite_);

    al_draw_filled_rectangle(0, 0, 32, 32, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(14, 26, 18 , 30 , al_map_rgb(0  , 255, 0));
    al_set_target_backbuffer(al_get_current_display());

}
Swampman::~Swampman()
{
    al_destroy_bitmap(sprite_);
}

void Swampman::update()
{
    updateOrientation();

    if(!((up == down) && (right == left))) //if there is movement
    {
        setSpeed(5.);
        step();
    }
}

void Swampman::updateOrientation()
{
    if(up && !down && !right && left)
        orientation = orientations[0];
    if(up && (left == right))
        orientation = orientations[1];
    if(up && !down && right && !left)
        orientation = orientations[2];
    if(left && (up == down))
        orientation = orientations[3];
    if(right && (up == down))
        orientation = orientations[5];
    if(left && down && !up && !right)
        orientation = orientations[6];
    if(down && (left == right))
        orientation = orientations[7];
    if(right && !left && down && !up)
        orientation = orientations[8];
}

void Swampman::draw(glm::vec2 screen_ul_corner)
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


void Swampman::processInput(ALLEGRO_EVENT ev)
{
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:    up    = true; break;
            case ALLEGRO_KEY_DOWN:  down  = true; break;
            case ALLEGRO_KEY_LEFT:  left  = true; break;
            case ALLEGRO_KEY_RIGHT: right = true; break;
            default: break;
        };
    }
    if(ev.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:    up    = false; break;
            case ALLEGRO_KEY_DOWN:  down  = false; break;
            case ALLEGRO_KEY_LEFT:  left  = false; break;
            case ALLEGRO_KEY_RIGHT: right = false; break;
            default: break;
        };

    }
}

void Swampman::drawHUD(int width, int height)
{
    //draw health bar rect
    al_draw_rectangle(width / 2. - 40, height - 20., width / 2. + 40, height - 10., al_map_rgb(186, 54, 54), 1.);

    al_draw_filled_rectangle(width / 2. - 40, height - 20., (width / 2. - 40.) + (hp_ / 100. * 80), height - 10., al_map_rgb(241, 31, 31));
}
