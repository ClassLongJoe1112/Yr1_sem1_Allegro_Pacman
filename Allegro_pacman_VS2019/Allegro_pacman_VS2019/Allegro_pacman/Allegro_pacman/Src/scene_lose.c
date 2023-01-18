// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_win.h"
#include "scene_lose.h"
#include "utility.h"
#include "shared.h"



// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID loseBGM;


static Button btnGoMenu;
static Button btnRetry;

static void init() {

	// Create buttons
	btnGoMenu = button_create(SCREEN_W / 2 - 250, SCREEN_H - 300, 100, 100, "Assets/menu.png", "Assets/menu.png");
	btnRetry = button_create(SCREEN_W / 2 + 150, SCREEN_H - 300, 100, 100, "Assets/retry.png", "Assets/retry.png");
	loseBGM = play_bgm(loseBgm, music_volume);
	
}

static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.3;
	const float offset_w = 50;
	const float offset_h = 20;

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		300,
		ALLEGRO_ALIGN_CENTER,
		"YOU LOSE, TRY AGAIN!!!"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2-200,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"MENU"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2+200,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"RETRY"
	);
	// TODO: Draw button
	// Uncomment and fill the code below

	drawButton(btnGoMenu);
	drawButton(btnRetry);
}
// The only function that is shared across files.


static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {

	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	btnGoMenu.hovered = pnt_in_rect(mouse_x, mouse_y, btnGoMenu.body);
	btnRetry.hovered = pnt_in_rect(mouse_x, mouse_y, btnRetry.body);
	/*
	btnSoundUp.hovered = pnt_in_rect(mouse_x, mouse_y, btnSoundUp.body);
	btnSoundDown.hovered = pnt_in_rect(mouse_x, mouse_y, btnSoundDown.body);
	btnSoundMute.hovered = pnt_in_rect(mouse_x, mouse_y, btnSoundMute.body);
	*/
}



//	TODO: When btnSettings clicked, switch to settings scene
//	Uncomment and fill the code below

static void on_mouse_down() {
	if (btnGoMenu.hovered)
		game_change_scene(scene_menu_create());
	if (btnRetry.hovered)
		game_change_scene(scene_main_create());
}


static void destroy() {
	stop_bgm(loseBGM);
	al_destroy_bitmap(btnGoMenu.default_img);
	al_destroy_bitmap(btnGoMenu.hovered_img);
	al_destroy_bitmap(btnRetry.default_img);
	al_destroy_bitmap(btnRetry.hovered_img);
}

static void on_key_down(int keycode) {

	switch (keycode) {
		default:
			break;
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_lose_create(void){
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Lose";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Lose scene created");
	return scene;
}




