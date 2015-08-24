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
#include "../misc/defines.hpp"

float Swampman::orientations[9] = {PI4_3, -PI, -PI4_3, PI2, 0., -PI2, PI4, 0., -PI4 };

Swampman::Swampman(glm::vec2 pos, Game &g): BoxObject(glm::vec2(30.f, 20.f)) , game_(g), up(false), down(false), left(false), right(false), throwing(false), hp_(100), ballammo_(3), throwcd_(0)
{
    setPosition(pos);

    ALLEGRO_PATH *path;

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, RESOURCES_DIR);
    al_set_path_filename(path, "swampman.png");
    sprite_ = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));

    al_destroy_path(path);
}
Swampman::~Swampman()
{
    al_destroy_bitmap(sprite_);
}

void Swampman::update()
{
    updateOrientation();

    if(throwcd_) throwcd_--;

    if(hp_ <= 0)
        game_.stop();

    Tile t = game_.map_.what(position.x, position.y);
     if(t.getBiome() == SWAMP && t.ignited())
     {
        hp_ -= 1;
     }

    if(!((up == down) && (right == left))) //if there is movement
    {
        if(t.getBiome() == GRASS)
        {
            setSpeed(2.);
        }
        else if(t.getBiome() == SWAMP)
        {
            setSpeed(2.4);
        }
        step();
    }
    if(throwing && ballammo_ && !throwcd_)
    {
        ballammo_--;
        throwcd_ = 30;

        decltype(position) ballpos = position;
        ballpos += glm::vec2(-sin(this->orientation), cos(this->orientation)) * 32.f;
        auto ptr = std::make_shared<Ball>(ballpos, game_, this->orientation);
        ptr->setSpeed(2. * 1.5);
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
                           15,
                           10,
                           this->position.x - screen_ul_corner.x,
                           this->position.y - screen_ul_corner.y,
                           orientation + PI,
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
            case ALLEGRO_KEY_LCTRL: throwing = true; break;
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
            case ALLEGRO_KEY_LCTRL: throwing = false; break;
            default: break;
        };

    }
}

void Swampman::drawHUD(int topx, int topy)
{
    //draw health bar rect
    al_draw_rectangle(topx + 20., topy + 20., topx + 80., topy + 30., al_map_rgb(186, 54, 54), 1.);

    al_draw_filled_rectangle(topx + 20. , topy + 20. , topx + 20. + (hp_ / 100. * 60), topy + 30., al_map_rgb(241, 31, 31));

    al_draw_textf(Main::main_font, al_map_rgb(241, 31, 31), topx + 110., topy + 22., 0, "%d",hp_);
}
