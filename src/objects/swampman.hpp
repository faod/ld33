#pragma once

#ifndef SWAMPMAN_HPP
#define SWAMPMAN_HPP

#include "object.hpp"

class Game;

class Character: public BoxObject {
protected:
	Game &game;
	glm::vec2 dimensions;
public:
	Character(glm::vec2 wh, Game &g): BoxObject(wh), game(g), dimensions(wh) {};

	/// A modified step() that does not allow the character to leave the map
	virtual void step();
};

class Swampman : public Character
{
    public: 
        Swampman(glm::vec2 position, Game &g);
        ~Swampman();

        static float orientations[9];

        virtual void update();
        virtual void draw(glm::vec2 screen_ul_corner);

        void drawHUD(int width, int height);

        void processInput(ALLEGRO_EVENT ev);

    private:
        ALLEGRO_BITMAP *sprite_;

        bool up;
        bool down;
        bool left;
        bool right;
        bool throwing;

        unsigned int ballammo_;
        unsigned int throwcd_;
        unsigned int hp_;

        void updateOrientation();
};

#endif 
