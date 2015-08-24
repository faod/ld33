#pragma once

#ifndef SWAMPMAN_HPP
#define SWAMPMAN_HPP

#include "object.hpp"

class Game;

class Swampman : public BoxObject
{
    public: 
        Swampman(glm::vec2 position, Game &g);
        ~Swampman();

        static float orientations[9];

        virtual void update();
        virtual void draw(glm::vec2 screen_ul_corner);
        virtual void step();

        void drawHUD(int width, int height);

        void processInput(ALLEGRO_EVENT ev);

    private:
        ALLEGRO_BITMAP *sprite_;

        Game &game_;

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
