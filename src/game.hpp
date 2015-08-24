/**
	Game - Entry point for your game
**/
#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>

#include "main.hpp"
#include "map.hpp"
#include "menu/menu.hpp"
#include "objects/object.hpp"

class Swampman;

class Game {
public:
    Game(Main& m);

    static void *startInput(ALLEGRO_THREAD* t, void* arg);

    void input(ALLEGRO_THREAD* t);
    void update();
    void refresh();

    void start();
    void stop();

    void removeObject(std::shared_ptr<Object> obj);
    void addObject(std::shared_ptr<Object> obj);

private:
    Main& main_;
    std::vector<std::shared_ptr<Object>> objects_;
    std::shared_ptr<Swampman> swampman_;
    std::shared_ptr<Menu>     menu_;
    Map map_;

    bool playing;
};

#endif
