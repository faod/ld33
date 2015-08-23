#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include <memory>
#include <string>

#include "swampman.hpp"
#include "ball.hpp"
#include "../game.hpp"

#define PI 3.141592653589793
#define PI2 1.5707963267948966
#define PI4 0.7853981633974483
#define PI4_3 2.356194490192345


float Swampman::orientations[9] = {PI4_3, -PI, -PI4_3, PI2, 0., -PI2, PI4, 0., -PI4 };

Swampman::Swampman(glm::vec2 pos, Game &g): BoxObject(glm::vec2(32.f, 32.f)) , game_(g), up(false), down(false), left(false), right(false), throwing(false), hp_(100), ballammo_(3), throwcd_(0)
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

    if(throwcd_) throwcd_--;

    if(!((up == down) && (right == left))) //if there is movement
    {
        setSpeed(5.);
        step();
    }
    if(throwing && ballammo_ && !throwcd_)
    {
        ballammo_--;
        throwcd_ = 30;

        decltype(position) ballpos = position;
        ballpos += glm::vec2(-sin(this->orientation), cos(this->orientation)) * 32.f;
        auto ptr = std::make_shared<Ball>(ballpos, game_, this->orientation);
        ptr->setSpeed(5. * 1.5);
        game_.addObject(ptr);
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
            case ALLEGRO_KEY_UP:    up       = true; break;
            case ALLEGRO_KEY_DOWN:  down     = true; break;
            case ALLEGRO_KEY_LEFT:  left     = true; break;
            case ALLEGRO_KEY_RIGHT: right    = true; break;
            case ALLEGRO_KEY_RCTRL: throwing = true; break;
            default: break;
        };
    }
    if(ev.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:    up       = false; break;
            case ALLEGRO_KEY_DOWN:  down     = false; break;
            case ALLEGRO_KEY_LEFT:  left     = false; break;
            case ALLEGRO_KEY_RIGHT: right    = false; break;
            case ALLEGRO_KEY_RCTRL: throwing = false; break;
            default: break;
        };

    }
}

void Swampman::drawHUD(int width, int height)
{
    //draw health bar rect
    al_draw_rectangle(20., 20., 100., 30., al_map_rgb(186, 54, 54), 1.);

    al_draw_filled_rectangle(20. , 20. , 20. + (hp_ / 100. * 80), 30., al_map_rgb(241, 31, 31));

    al_draw_textf(Main::console_font, al_map_rgb(241, 31, 31), 110., 22., 0, "%d",hp_);
}
