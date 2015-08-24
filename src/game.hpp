/**
	Game - Entry point for your game
**/
#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>

#include "main.hpp"
#include "objects/object.hpp"
#include "map.hpp"
#include "menu/menu.hpp"

class Swampman;

class Game {
public:
    Main& main_;
    std::vector<std::shared_ptr<Object>> objects_;
    std::shared_ptr<Swampman> swampman_;
    std::shared_ptr<Menu>     menu_;
    Map map_;
    ALLEGRO_BITMAP *tmp_;


    bool playing;

    Game(Main& m);

    static void *startInput(ALLEGRO_THREAD* t, void* arg);

    static ALLEGRO_VOICE *voice;
    static ALLEGRO_AUDIO_STREAM *str;

    void input(ALLEGRO_THREAD* t);
    void update();
    void refresh();

    void start();
    void stop();

    void removeObject(std::shared_ptr<Object> obj);
    void addObject(std::shared_ptr<Object> obj);
};

#endif
