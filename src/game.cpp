
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "game.hpp"

Game::Game(Main& m) : main_(m), map_(atoi(al_get_config_value(main_.config, "", "map_width")),
                                     atoi(al_get_config_value(main_.config, "", "map_height")),
                                     atof(al_get_config_value(main_.config, "", "perlin_resolution")))
{
}

void Game::update()
{

}


void Game::refresh()
{
    static int xpos = 0;
    map_.draw(xpos, 0, main_.screen_w, main_.screen_h);
    xpos++;
}
