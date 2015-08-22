#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "swampman.hpp"

Swampman::Swampman(glm::vec2 pos)
{
    setPosition(pos);

    sprite_ = al_create_bitmap(32, 32);
    al_set_target_bitmap(sprite_);

    al_draw_filled_rectangle(0, 0, 32, 32, al_map_rgb(255, 0, 0));

    al_set_target_backbuffer(al_get_current_display());

}
Swampman::~Swampman()
{
    al_destroy_bitmap(sprite_);
}

void Swampman::refresh()
{

}

void Swampman::draw()
{
    al_draw_bitmap(sprite_, al_get_display_width(al_get_current_display()) / 2 - 16, 
                            al_get_display_height(al_get_current_display()) / 2 - 16,
                            0);    
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

