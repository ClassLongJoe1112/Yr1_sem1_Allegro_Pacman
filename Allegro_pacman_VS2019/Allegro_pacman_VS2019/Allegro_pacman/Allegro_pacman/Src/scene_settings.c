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
#include "game.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"


// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID settingBGM;
static ALLEGRO_SAMPLE_ID eatBeanSE; //for testing vol
static int gameTitleW;
static int gameTitleH;

static Button btnGoMenu;
static Button btnGloPlusVol;
static Button btnGloMinusVol;
static Button btnGloMute;
static Button btnBgmPlusVol;
static Button btnBgmMinusVol;
static Button btnBgmMute;
static Button btnEfPlusVol;
static Button btnEfMinusVol;
static Button btnEfMute;

static void init() {


	// Create buttons

	btnGoMenu = button_create(SCREEN_W / 2 - 50, SCREEN_H - 120, 100, 100, "Assets/menu.png", "Assets/menu.png");

	btnGloPlusVol = button_create(SCREEN_W / 2 + 100, 70, 100, 100, "Assets/plus_vol.png", "Assets/plus_vol.png");
	btnGloMinusVol = button_create(SCREEN_W / 2 - 50 , 70, 100, 100, "Assets/minus_vol.png", "Assets/minus_vol.png");
	btnGloMute = button_create(SCREEN_W / 2 - 200, 70, 100, 100, "Assets/mute_vol.png", "Assets/mute_vol.png");

	btnBgmPlusVol = button_create(SCREEN_W / 2 + 100, 285, 100, 100, "Assets/plus_vol.png", "Assets/plus_vol.png");
	btnBgmMinusVol = button_create(SCREEN_W / 2 - 50, 285, 100, 100, "Assets/minus_vol.png", "Assets/minus_vol.png");
	btnBgmMute = button_create(SCREEN_W / 2 - 200, 285, 100, 100, "Assets/mute_vol.png", "Assets/mute_vol.png");

	btnEfPlusVol = button_create(SCREEN_W / 2 + 100, 500, 100, 100, "Assets/plus_vol.png", "Assets/plus_vol.png");
	btnEfMinusVol = button_create(SCREEN_W / 2 - 50, 500, 100, 100, "Assets/minus_vol.png", "Assets/minus_vol.png");
	btnEfMute = button_create(SCREEN_W / 2 - 200, 500, 100, 100, "Assets/mute_vol.png", "Assets/mute_vol.png");

	settingBGM = play_bgm(settingBgm, music_volume);
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
		30,
		ALLEGRO_ALIGN_CENTER,
		"GLOBAL VOLUME"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		285-40,
		ALLEGRO_ALIGN_CENTER,
		"MUSIC VOLUME"
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		500-40,
		ALLEGRO_ALIGN_CENTER,
		"EFFECT VOLUME"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H / 2.0 * 3.0 + 20,
		ALLEGRO_ALIGN_CENTER,
		"GAME MAP"
	);

	// TODO: Draw button
	// Uncomment and fill the code below

	drawButton(btnGoMenu);
	drawButton(btnGloPlusVol);
	drawButton(btnGloMinusVol);
	drawButton(btnGloMute);
	drawButton(btnBgmPlusVol);
	drawButton(btnBgmMinusVol);
	drawButton(btnBgmMute);
	drawButton(btnEfPlusVol);
	drawButton(btnEfMinusVol);
	drawButton(btnEfMute);



}
// The only function that is shared across files.


static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {

	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	btnGoMenu.hovered = pnt_in_rect(mouse_x, mouse_y, btnGoMenu.body);

	btnGloPlusVol.hovered = pnt_in_rect(mouse_x, mouse_y, btnGloPlusVol.body);
	btnGloMinusVol.hovered = pnt_in_rect(mouse_x, mouse_y, btnGloMinusVol.body);
	btnGloMute.hovered = pnt_in_rect(mouse_x, mouse_y, btnGloMute.body);

	btnBgmPlusVol.hovered = pnt_in_rect(mouse_x, mouse_y, btnBgmPlusVol.body);
	btnBgmMinusVol.hovered = pnt_in_rect(mouse_x, mouse_y, btnBgmMinusVol.body);
	btnBgmMute.hovered = pnt_in_rect(mouse_x, mouse_y, btnBgmMute.body);

	btnEfPlusVol.hovered = pnt_in_rect(mouse_x, mouse_y, btnEfPlusVol.body);
	btnEfMinusVol.hovered = pnt_in_rect(mouse_x, mouse_y, btnEfMinusVol.body);
	btnEfMute.hovered = pnt_in_rect(mouse_x, mouse_y, btnEfMute.body);
}



//	TODO: When btnSettings clicked, switch to settings scene
//	Uncomment and fill the code below

static void on_mouse_down() {
	if (btnGoMenu.hovered)
		game_change_scene(scene_menu_create());

	if (btnGloMute.hovered) {
		stop_bgm(settingBGM);
		music_volume = 0;
		effect_volume = 0;
		
		game_log("Music Volume: %f", music_volume);
		game_log("Effect Volume: %f", effect_volume);
	}
	if (btnBgmMute.hovered) {
		stop_bgm(settingBGM);
		music_volume = 0;
		
		game_log("Music Volume: %f", music_volume);
	}
	if (btnEfMute.hovered) {
		stop_bgm(settingBGM);
		effect_volume = 0;

		game_log("Effect Volume: %f", effect_volume);
	}
	if (btnGloPlusVol.hovered) {
		if (true)
			//music_volume + 1 <= 1 && effect_volume + 1 <= 1
		{
			stop_bgm(settingBGM);
			
			if(music_volume < 1) music_volume += 0.2;
			if(effect_volume < 1) effect_volume += 0.2;
			
			//menuBGM = play_bgm(themeMusic, music_volume);
			eatBeanSE = play_audio(PACMAN_MOVESOUND, music_volume);
			game_log("Volume: increase");
			game_log("Music Volume: %f", music_volume);
			game_log("Effect Volume: %f", effect_volume);
		}
	}
	if (btnBgmPlusVol.hovered) {
		if (true)
			//music_volume + 1 <= 1 && effect_volume + 1 <= 1
		{
			stop_bgm(settingBGM);

			if(music_volume<1) music_volume += 0.2;
			
			//menuBGM = play_bgm(themeMusic, music_volume);
			eatBeanSE = play_audio(PACMAN_MOVESOUND, music_volume);
			game_log("Volume: increase");
			game_log("Music Volume: %f", music_volume);
		}
	}
	if (btnEfPlusVol.hovered) {
		if (true)
			//music_volume + 1 <= 1 && effect_volume + 1 <= 1
		{
			stop_bgm(settingBGM);
			if (effect_volume < 1) effect_volume += 0.2;

			//menuBGM = play_bgm(themeMusic, music_volume);
			eatBeanSE = play_audio(PACMAN_MOVESOUND, effect_volume);
			game_log("Volume: increase");
			game_log("Effect Volume: %f", effect_volume);
		}
	}
	if (btnGloMinusVol.hovered) {
		if (true)
			//music_volume - 1 >= -1 && effect_volume - 1 >= -1
		{
			stop_bgm(settingBGM);
			if(music_volume > 0.1) music_volume -= 0.2;
			if (effect_volume > 0.1) effect_volume -= 0.2;
			//menuBGM = play_bgm(themeMusic, music_volume);
			eatBeanSE = play_audio(PACMAN_MOVESOUND, music_volume);
			game_log("Music Volume: %f", music_volume);
			game_log("Effect Volume: %f", effect_volume);
		}
	}
	if (btnBgmMinusVol.hovered) {
		if (true)
			//music_volume - 1 >= -1 && effect_volume - 1 >= -1
		{
			stop_bgm(settingBGM);
			if(music_volume>0.1) music_volume -= 0.2;
			//menuBGM = play_bgm(themeMusic, music_volume);
			eatBeanSE = play_audio(PACMAN_MOVESOUND, music_volume);
			game_log("Music Volume: %f", music_volume);
		}
	}
	if (btnEfMinusVol.hovered) {
		if (true)
			//music_volume - 1 >= -1 && effect_volume - 1 >= -1
		{
			stop_bgm(settingBGM);
			if (effect_volume > 0.1) effect_volume -= 0.2;
			//menuBGM = play_bgm(themeMusic, music_volume);
			eatBeanSE = play_audio(PACMAN_MOVESOUND, effect_volume);
			game_log("Effect Volume: %f", effect_volume);
		}
	}
}


static void destroy() {
	stop_bgm(settingBGM);
	al_destroy_bitmap(btnGoMenu.default_img);
	al_destroy_bitmap(btnGoMenu.hovered_img);
	al_destroy_bitmap(btnGloMute.default_img);
	al_destroy_bitmap(btnGloMute.hovered_img);
	al_destroy_bitmap(btnGloPlusVol.default_img);
	al_destroy_bitmap(btnGloPlusVol.hovered_img);
	al_destroy_bitmap(btnGloMinusVol.default_img);
	al_destroy_bitmap(btnGloMinusVol.hovered_img);
	al_destroy_bitmap(btnBgmMute.default_img);
	al_destroy_bitmap(btnBgmMute.hovered_img);
	al_destroy_bitmap(btnBgmPlusVol.default_img);
	al_destroy_bitmap(btnBgmPlusVol.hovered_img);
	al_destroy_bitmap(btnBgmMinusVol.default_img);
	al_destroy_bitmap(btnBgmMinusVol.hovered_img);
	al_destroy_bitmap(btnEfMute.default_img);
	al_destroy_bitmap(btnEfMute.hovered_img);
	al_destroy_bitmap(btnEfPlusVol.default_img);
	al_destroy_bitmap(btnEfPlusVol.hovered_img);
	al_destroy_bitmap(btnEfMinusVol.default_img);
	al_destroy_bitmap(btnEfMinusVol.hovered_img);
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
Scene scene_settings_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Setting";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Setting scene created");
	return scene;
}




