#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <string>

class Game;

class Menu
{
    public:
        Menu(int width, int height, Game& g);
        ~Menu();

        void update();
        void processInput(ALLEGRO_EVENT ev);
        void draw();

    private:
        int width_;
        int height_;
        Game &game_;
        ALLEGRO_BITMAP* bm_;
        static ALLEGRO_AUDIO_STREAM *str_;
        static ALLEGRO_VOICE        *voice_;

};

#endif
