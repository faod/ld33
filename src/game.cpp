#include <algorithm>
#include <memory>
#include <cstdlib>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "game.hpp"
#include "objects/swampman.hpp"
#include "objects/villager.hpp"
#include "menu/menu.hpp"


Game::Game(Main& m) : main_(m), map_(atoi(al_get_config_value(main_.config, "", "map_width")),
                                     atoi(al_get_config_value(main_.config, "", "map_height")),
                                     atof(al_get_config_value(main_.config, "", "perlin_resolution")),
                                     *this)
{
    std::shared_ptr<Swampman> ptr = std::make_shared<Swampman, glm::vec2>(glm::vec2(800., 300.), *this);
    addObject(ptr);
    swampman_ = ptr;

    glm::vec2 v;
    std::srand(al_get_time()*1000);
    for (int i=0; i<10; i++) {
        do {
            v = glm::vec2(std::rand() % map_.getWidth() * 32 + 16, std::rand() % map_.getHeight() * 32 + 16);
        } while (map_.what(v.x, v.y) != GRASS);

        addObject(std::make_shared<Villager, glm::vec2&>(v, *this));
    }
    
    menu_   = std::make_shared<Menu>(m.screen_w, m.screen_h, *this);
    playing = false;

    tmp_ = al_create_bitmap(main_.screen_w, main_.screen_h);
}

void Game::update()
{
    if(playing)
    {
        map_.update();

        for(int i = 0; i < objects_.size(); ++i)
            if(objects_[i]) objects_[i]->update();

        auto it = std::remove(objects_.begin(), objects_.end(), nullptr);
        objects_.erase(it, objects_.end());
    }
    else
    {
        menu_->update();
    }
}


void Game::refresh()
{
    if(playing)
    {
        al_set_target_bitmap(tmp_);
        al_clear_to_color(al_map_rgb(55, 55, 55));

        glm::vec2 smpos = swampman_->getPosition();
        map_.draw(static_cast<int>(smpos[0]) - main_.screen_w / 2.,
              static_cast<int>(smpos[1]) - main_.screen_h / 2.,
               main_.screen_w, main_.screen_h);

        for(auto it = objects_.begin(); it != objects_.end(); ++it)
            (*it)->draw(glm::vec2(static_cast<int>(smpos.x) - main_.screen_w / 2.,
                              static_cast<int>(smpos.y) - main_.screen_h / 2.));

        swampman_->drawHUD(main_.screen_w / 4, main_.screen_h / 4);

        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(tmp_, main_.screen_w / 4, main_.screen_h / 4, main_.screen_w / 2, main_.screen_h / 2, 0, 0, main_.screen_w, main_.screen_h, 0);
     }
     else
     {
        menu_->draw();
     }
}

void Game::addObject(std::shared_ptr<Object> obj)
{
    if(std::find(objects_.begin(), objects_.end(), obj) == objects_.end())
        objects_.push_back(obj);
}

void Game::removeObject(std::shared_ptr<Object> obj)
{
    auto it = std::find(objects_.begin(), objects_.end(), obj);
    if(it != objects_.end())
        it->reset();

}

void *Game::startInput(ALLEGRO_THREAD* t, void* arg)
{
    static_cast<Game*>(arg)->input(t);
    return NULL;
}

void Game::input(ALLEGRO_THREAD* t)
{
ALLEGRO_EVENT ev;

    while(al_wait_for_event(main_.inputEQ, &ev), main_.loop)
    {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            main_.loop = false;
            break;
        }
        else if(!swampman_ && playing) continue;
        else if(!playing)
        {
            menu_->processInput(ev);
        }
        else
        {
            swampman_->processInput(ev);
        }
    }

}

void Game::start()
{
    playing = true;
}

void Game::stop()
{
    playing = false;
}
