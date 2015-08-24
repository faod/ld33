#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include <memory>
#include <allegro5/allegro.h>
#include <string>

class Menu
{
    public:
        Menu(int width, int height); 
        ~Menu();

        void update();        
        void processInput(ALLEGRO_EVENT ev); 
        void draw();

    private:
        int width_;
        int height_;
        ALLEGRO_BITMAP* bm_;
                
};

#endif
