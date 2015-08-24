#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>




#include "menu.hpp"
#include "../main.hpp"
Menu::Menu(std::shared_ptr<ALLEGRO_BITMAP> bm, int width, int height, std::string title) : bm_(bm), width_(width), height_(height), title_(title)
{

}

void Menu::processInput(ALLEGRO_EVENT ev)
{
}

void Menu::draw()
{
    al_draw_bitmap(bm_.get(), al_get_display_width(al_get_current_display()) / 2 - width_, al_get_display_height(al_get_current_display()) / 2 - height_, 0);
    
    al_draw_text(Main::main_font, al_map_rgb(255, 0, 0), al_get_display_width(al_get_current_display()) / 2, (al_get_display_height(al_get_current_display()) / 2 - height_) + 30, 0, title_.c_str());
}

void Menu::update()
{
}
