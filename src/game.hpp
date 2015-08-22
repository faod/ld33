/**
	Game - Entry point for your game
**/
#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "main.hpp"
#include "map.hpp"

class Game {
public:
    Game(Main& m);
	int flags;

    void update();
    void refresh();

private:
    Main& main_;
    Map map_;
};

#endif
