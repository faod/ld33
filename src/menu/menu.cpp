#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>




#include "menu.hpp"
#include "../main.hpp"
#include "../failure.hpp"
#include "../game.hpp"

ALLEGRO_AUDIO_STREAM *Menu::str_ = NULL;
ALLEGRO_VOICE        *Menu::voice_ = NULL;


Menu::Menu(int width, int height, Game &g) : width_(width), height_(height), game_(g)
{
    ALLEGRO_PATH *path;
    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, RESOURCES_DIR);
    al_set_path_filename(path, "home.png");
    bm_ = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    if(!bm_) throw Failure("failed to load menu bitmap");
    al_destroy_path(path);

    al_set_target_bitmap(bm_);
    al_draw_text(Main::main_font, al_map_rgb(255, 0, 0), 128 / 2, 128 / 2, ALLEGRO_ALIGN_CENTRE, "PRESS SPACE");
    al_set_target_backbuffer(al_get_current_display());

    if(!str_)
    {
        path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        al_append_path_component(path, RESOURCES_DIR);
        al_set_path_filename(path, "menu.xm");
        str_ = al_load_audio_stream(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), 4, 2048);
        al_set_audio_stream_playmode(str_, ALLEGRO_PLAYMODE_LOOP);
        al_destroy_path(path);
    }

    al_attach_audio_stream_to_voice(str_, Game::voice);

}


Menu::~Menu()
{
    al_destroy_bitmap(bm_);
    al_detach_audio_stream(str_);
}


void Menu::processInput(ALLEGRO_EVENT ev)
{
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
        {
            game_.start();
        }
    }
}

void Menu::draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(bm_, 0, 0, 128, 128, (width_ / 2) - (128 * 2), (height_ / 2) - (128 * 2), 128 * 4, 128 * 4, 0);

}

void Menu::update()
{
}
