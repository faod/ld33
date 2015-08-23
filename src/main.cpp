#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>


#include "main.hpp"
#include "failure.hpp"
#include "game.hpp"

using std::cerr;
using std::endl;

// Initialize Allegro and its addons
Main::Main() {

	if (!al_init()) throw Failure("failed to initialize allegro!");

    //load allegro_config
    ALLEGRO_PATH *path;
    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, RESOURCES_DIR);
    al_set_path_filename(path, "config.cfg");

    int res = al_filename_exists(al_path_cstr(path, '/'));

    if(res)
    {
        //if config file found
        config = al_load_config_file(al_path_cstr(path, '/'));
    }
    else
    {
        //create default config
        config = al_create_config();
        al_set_config_value(config, "", "window_height", "600");
        al_set_config_value(config, "", "window_width", "800");
        al_set_config_value(config, "", "map_width", "50");
        al_set_config_value(config, "", "map_height", "20");
        al_set_config_value(config, "", "perlin_resolution", "150");
        al_save_config_file(al_path_cstr(path, '/'), config);
    }
    al_destroy_path(path);

    //Set screen size
	this->screen_h = atoi(al_get_config_value(config, "", "window_height"));
	this->screen_w = atoi(al_get_config_value(config, "", "window_width"));

	refresh_tick = 30;
	animation_tick = 60;


	al_set_app_name(APP_NAME);
	al_set_org_name(APP_NAME);

	// Creates a modern OpenGL context
	al_set_new_display_flags(ALLEGRO_OPENGL);
	//al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0); // Modern OpenGL (compatibility context)
	//al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 | ALLEGRO_PROGRAMMABLE_PIPELINE); // using Allegro's Shader Manager

	if (display = al_create_display(screen_w, screen_h), !display) throw Failure("failed to create display!");

	if (!al_install_keyboard()) throw Failure("failed to initialize the keyboard!");
	if (!al_install_mouse()) throw Failure("failed to initialize the mouse!");
	//if (!al_install_joystick()) throw Failure("failed to initialize the joystick!");

	//if (!(refreshEQ = al_create_event_queue())) throw Failure("failed to create the frame event queue!");
	if (!(animationEQ = al_create_event_queue())) throw Failure("failed to create the animation event queue!");
	if (!(inputEQ = al_create_event_queue())) throw Failure("failed to create the input event queue!");

	//if (!(refreshTimer = al_create_timer(1.0/refresh_tick))) throw Failure("failed to initialise the frame timer!");
	if (!(animationTimer = al_create_timer(1.0/animation_tick))) throw Failure("failed to initialise the animation timer!");

	if (!al_init_image_addon()) throw Failure("failed to initialise image support!");

	if (!al_install_audio()) throw Failure("failed to initialise audio support!");
	if (!al_init_acodec_addon()) throw Failure("failed to initialise audio codecs support!");

	if (!al_init_primitives_addon()) throw Failure("failed to initialise primitives!");

	al_init_font_addon();
	if (!(console_font = al_create_builtin_font())) throw Failure("failed to initialise console font!");

	// ALLEGRO_EVENT_DISPLAY_*
	al_register_event_source(inputEQ, al_get_display_event_source(display));
	// ALLEGRO_EVENT_JOYSTICK_*
	//al_register_event_source(inputEQ, al_get_joystick_event_source());
	// ALLEGRO_EVENT_KEY_*
	al_register_event_source(inputEQ, al_get_keyboard_event_source());

	// ALLEGRO_EVENT_TIMER frame update
	//al_register_event_source(frameEQ, al_get_timer_event_source(frameTimer));
	// ALLEGRO_EVENT_TIMER animation update
	al_register_event_source(animationEQ, al_get_timer_event_source(animationTimer));


}

// Deinitialize Allegro and its addons
Main::~Main() {
	al_destroy_font(console_font);

	al_shutdown_primitives_addon();
	
	al_uninstall_audio();

	al_shutdown_image_addon();

	al_destroy_timer(animationTimer);
	//al_destroy_timer(frameTimer);

	al_destroy_event_queue(inputEQ);
	al_destroy_event_queue(animationEQ);
	//al_destroy_event_queue(refreshEQ);
	
	//al_uninstall_joystick();
	al_uninstall_mouse();
	al_uninstall_keyboard();
	
	al_destroy_display(display);

    al_destroy_config(config);
}

/** OpenGL 3.3+ modern **/
int main(int argc, char *argv[]) {
	try {
		Main m;
		ALLEGRO_COLOR grey  = al_map_rgb(55, 55, 55);

		ALLEGRO_EVENT ev;

        Game game(m);

        ALLEGRO_THREAD* input = al_create_thread(Game::startInput, &game);
        if(input)
        {
            al_start_thread(input);
        }
        else
        {
            throw Failure("failure to create input thread");
        }
		int step = 0;
		al_start_timer(m.animationTimer);
        m.loop = true;
		while (al_wait_for_event(m.animationEQ, &ev), m.loop) {
			al_clear_to_color(grey);

            game.update();
            game.refresh();

			al_flip_display();

		}
	}
	catch (Failure f) {
		cerr << f.what() << endl;
	}
	return 0;
}
