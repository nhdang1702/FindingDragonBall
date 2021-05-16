#ifndef COMMON_FUNC_H_

#define COMMON_FUNC_H_

#include<windows.h>
#include<string>
#include<vector>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//Audio
static Mix_Music* g_music = NULL;
static Mix_Chunk* g_bullet_sound[2];
static Mix_Chunk* g_explosion_sound = NULL;
static Mix_Chunk* g_playerdie_sound = NULL;
static Mix_Chunk* g_coin_sound = NULL;
static Mix_Chunk* g_steps_sound = NULL;

//Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
//Color

const int COLOR_KEY_R = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_B = 255;
const int RENDER_DRAW_COLOR = 0xFF;

//fps
const int FRAME_PER_SECOND = 35;

//Map
#define BLANK_TILE 0
#define TILE_SIZE 64 // dinh nghia kich thuoc 1 o

#define MAX_MAP_X 400 // dinh nghia kich thuoc file map
#define MAX_MAP_Y 10

struct Map
{
	int start_x_;
	int start_y_;

	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_;
};

//Movement
struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
	int atk_;

};

namespace SDLCommonFunc
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	
}

#endif