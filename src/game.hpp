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
#include "objects/object.hpp"

class Game {
public:
    Game(Main& m);
	int flags;

    void update();
    void refresh();

    void addObject(std::shared_ptr<Object> obj);
private:
    Main& main_;
    Map map_;
    std::vector<std::shared_ptr<Object>> objects_;
};

#endif
