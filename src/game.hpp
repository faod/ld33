/**
	Game - Entry point for your game
**/
#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "main.hpp"


class Game {
public:
    Game(Main& m);
	int flags;

    void update();
    void refresh();

private:
    Main& main_;
};

#endif
