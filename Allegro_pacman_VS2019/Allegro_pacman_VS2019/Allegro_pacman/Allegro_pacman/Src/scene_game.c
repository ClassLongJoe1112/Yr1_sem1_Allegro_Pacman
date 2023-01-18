#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_win.h"
#include "scene_lose.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"


// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;

bool game_over = false;
bool won = false;

/* Internal variables*/
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;

bool debug_mode = false;
bool cheat_mode = false;

//static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_BITMAP* pmanLittleIcon = NULL;
static ALLEGRO_BITMAP* pmanImage = NULL;
static ALLEGRO_BITMAP* clydeImage = NULL;
static ALLEGRO_BITMAP* pinkyImage = NULL;
static ALLEGRO_BITMAP* blinkyImage = NULL;
static ALLEGRO_BITMAP* inkyImage = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static ALLEGRO_SAMPLE_ID gameBGM;



/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void render_after_die_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	game_over = false;
	won = false;
	game_main_Score = 0;
	const char* map1 = "Assets/map_nthu.txt";
	const char* map2 = "Assets/map_joe.txt";
	const char* map3 = "Assets/map_test.txt";

	// create map
	basic_map = create_map(map3); //之後再改成自己嘅地圖
	stop_bgm(gameBGM);
	gameBGM = play_bgm(gameBgm, music_volume);

	/*
	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	*/
	pmanLittleIcon = load_bitmap("Assets/small_pacman.png");
	pmanImage= load_bitmap("Assets/pacman_image.png");
	clydeImage = load_bitmap("Assets/clyde_image.png");
	inkyImage = load_bitmap("Assets/inky_image.png");
	pinkyImage = load_bitmap("Assets/pinky_image.png");
	blinkyImage = load_bitmap("Assets/blinky_image.png");

	// [TODO]
	// Create map from .txt file and design your own map!!
	// basic_map = create_map("Assets/map_nthu.txt"); //之後再改成自己嘅地圖
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacman\n");
	}
	
	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	
	ghosts = (Ghost*)malloc(sizeof(Ghost) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}

static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman,'.');
		game_main_Score += 10;
		basic_map->beansCount--;
		//game_log("you ate a bean\n");
		break;
	case 'P':
		pacman_eatItem(pman, 'P');
		game_main_Score += 50;
		basic_map->beansCount--;
		break;
	default:
		break;
	}
	
	// [HACKATHON 1-4]
	// erase the item you eat from map
	// be careful no erasing the wall block.
	basic_map->map[Grid_y][Grid_x]=' ';
}
static void status_update(void) {

	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		const RecArea pmanRec = getDrawArea(pman->objData, GAME_TICK_CD);
		const RecArea ghostRec = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		
		if(!cheat_mode && RecAreaOverlap(pmanRec, ghostRec))
		{
			game_log("collide with ghost\n");

			pman->n_life--;
			if (pman->n_life) { //仲有命先原位
				//pacman番原位
				pman->objData.Coord.x = 24;
				pman->objData.Coord.y = 24;
				pman->objData.preMove = NONE;
				pman->objData.nextTryMove = NONE;
				pman->objData.facing = generateRandomNumber(2, 3);
				//四隻ghost
				for (int i = 0;i < GHOST_NUM;i++) {
					ghosts[i]->objData.Coord.x = 1 + 11 * i;
					ghosts[i]->objData.Coord.y = 9;
				}
			}
			
			al_stop_timer(game_tick_timer);
			render_after_die_screen();
			al_resume_timer(game_tick_timer);

			if (pman->n_life == 0) {
				pacman_die();
				game_over = true;
				al_rest(0.1);
				break;
			}
		}
		if (basic_map->beansCount == 0) {
			won = true;
			break;
		}
	}
}

static void update(void) {
	
	if (won){
		game_log("You Won\n");
		al_rest(1.0);
		game_change_scene(scene_win_create());
		return;
	}
	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		
		al_start_timer(pman->death_anim_counter);

		if(al_get_timer_count(pman->death_anim_counter)==120) //64tick係1秒 128tick係2秒 192tick係3秒
			game_change_scene(scene_lose_create());
		
		return;
	}

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {
	
	
	al_clear_to_color(al_map_rgb(0, 0, 0));

	/*
	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	*/
	const float pacman_scale = 0.05;
	const float ghost_scale = 0.1;

	al_draw_scaled_bitmap(
		pmanImage,
		0, 0,
		al_get_bitmap_width(pmanImage), al_get_bitmap_height(pmanImage),
		25, 690,
		al_get_bitmap_width(pmanImage)*pacman_scale, al_get_bitmap_height(pmanImage)*pacman_scale,
		0
	);

	al_draw_scaled_bitmap(
		clydeImage,
		0, 0,
		al_get_bitmap_width(clydeImage), al_get_bitmap_height(clydeImage),
		700, 680,
		al_get_bitmap_width(clydeImage) * ghost_scale, al_get_bitmap_height(clydeImage) * ghost_scale,
		0
	);

	al_draw_scaled_bitmap(
		blinkyImage,
		0, 0,
		al_get_bitmap_width(blinkyImage), al_get_bitmap_height(blinkyImage),
		175, 680,
		al_get_bitmap_width(blinkyImage) * ghost_scale, al_get_bitmap_height(blinkyImage) * ghost_scale,
		0
	);

	al_draw_scaled_bitmap(
		pinkyImage,
		0, 0,
		al_get_bitmap_width(pinkyImage), al_get_bitmap_height(pinkyImage),
		600, 680,
		al_get_bitmap_width(pinkyImage) * ghost_scale, al_get_bitmap_height(pinkyImage) * ghost_scale,
		0
	);
	for (int i = 0 ; i < pman->n_life ; i++) {
		al_draw_scaled_bitmap(
			pmanLittleIcon,
			0, 0,
			al_get_bitmap_width(pmanLittleIcon), al_get_bitmap_height(pmanLittleIcon),
			120 + 25*i,
			17,
			al_get_bitmap_width(pmanLittleIcon) * ghost_scale, al_get_bitmap_height(pmanLittleIcon) * ghost_scale,
			0
		);
	}
	al_draw_text(
		menuFont,
		al_map_rgb(250, 255, 255),
		70,
		16,
		ALLEGRO_ALIGN_CENTER,
		"life #:"
	);

	al_draw_scaled_bitmap(
		inkyImage,
		0, 0,
		al_get_bitmap_width(inkyImage), al_get_bitmap_height(inkyImage),
		500, 680,
		al_get_bitmap_width(inkyImage) * ghost_scale * 3.75, al_get_bitmap_height(inkyImage) * ghost_scale * 3.75,
		0
	);
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	//originally:
	/*
		al_draw_text(...);
	*/
	char ScoreValue[20];

	sprintf_s(ScoreValue, 19,"score: %d", game_main_Score);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 80,
		ALLEGRO_ALIGN_CENTER,
		ScoreValue
	);
	al_draw_text(
		menuFont,
		al_map_rgb(250, 253, 15),
		SCREEN_W / 2,
		SCREEN_H - 60,
		ALLEGRO_ALIGN_CENTER,
		"________"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(250, 253, 15),
		SCREEN_W / 2,
		SCREEN_H - 120,
		ALLEGRO_ALIGN_CENTER,
		"________"
	);

	draw_map(basic_map);

	pacman_draw(pman);

	if (game_over) //唔影響draw death anim
		return;
	
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	pacman_destory(pman);
	for (int i = 0;i < GHOST_NUM;i++) {
		ghost_destory(ghosts[i]);
	}
	al_destroy_bitmap(pmanLittleIcon);
	al_destroy_bitmap(pmanImage);
	al_destroy_bitmap(clydeImage);
	al_destroy_bitmap(pinkyImage);
	al_destroy_bitmap(blinkyImage);
	al_destroy_bitmap(inkyImage);

	stop_bgm(gameBGM);
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_P:
			al_stop_timer(game_tick_timer);
			break;
		case ALLEGRO_KEY_L:
			al_resume_timer(game_tick_timer);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
		
	default:
		break;
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);

	//是但比個facing先，否則ready嗰兩秒會冇image，ghost同樣
	pman->objData.facing = generateRandomNumber(2,3); 
	pacman_draw(pman);
	
	for (int i = 0; i < GHOST_NUM; i++) {
		ghosts[i]->objData.facing = generateRandomNumber(2,3);
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}

static void render_after_die_screen(void) {

	
	char LifeRemaining[20];

	sprintf_s(LifeRemaining, 19, "%d lives remaining", pman->n_life);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		LifeRemaining
	);

	al_flip_display();
	al_rest(2.0);
}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}