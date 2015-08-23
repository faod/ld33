#pragma once

#ifndef SWAMPMAN_HPP
#define SWAMPMAN_HPP

#include "object.hpp"

class Swampman : public BoxObject
{
    public: 
        Swampman(glm::vec2 position);
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

        unsigned int hp_;

        void updateOrientation();
};

#endif 
