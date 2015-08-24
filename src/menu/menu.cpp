#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>




#include "menu.hpp"
#include "../main.hpp"
#include "../failure.hpp"

Menu::Menu(int width, int height) : width_(width), height_(height)
{
    ALLEGRO_PATH *path;
    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, RESOURCES_DIR);
    al_set_path_filename(path, "home.png");
    bm_ = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    if(!bm_) throw Failure("failed to load menu bitmap");
    al_destroy_path(path);
}

Menu::~Menu()
{
    al_destroy_bitmap(bm_);
}


void Menu::processInput(ALLEGRO_EVENT ev)
{
}

void Menu::draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(bm_, 0, 0, 128, 128, (width_ / 2) - (128 * 2), (height_ / 2) - (128 * 2), 128 * 4, 128 * 4, 0);
}

void Menu::update()
{
}
