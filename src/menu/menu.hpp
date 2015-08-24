#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include <memory>
#include <allegro5/allegro.h>
#include <string>

class MenuItem
{
    
};

class Menu
{
    public:
        Menu(std::shared_ptr<ALLEGRO_BITMAP> bm, int width, int height, std::string title); 

        void update();        
        void processInput(ALLEGRO_EVENT ev); 
        void draw();

    private:
        int width_;
        int height_;
        std::shared_ptr<ALLEGRO_BITMAP> bm_;
        std::string title_;
                
};

#endif
