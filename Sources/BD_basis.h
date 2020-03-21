#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#ifndef BD_basis
#define BD_basis

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 760
#define FPS 60
#define MINER_SPEED 10
#define ROCK_FALL_SPEED 10
#define DIA_FALL_SPEED 10
#define SPIDER_SPEED 6
#define MONSTER_SPEED 6
#define OBJ_UPDATE_SPEED 12
#define WATER_MAX_SIZE 200
#define CELL_SIZE 40
#define MUSIC_VOL 16

#define IMG_FILE_PATH "Images"
#define MUSIC_FILE_PATH "Musics"
#define TTF_FILE_PATH "TTFs"
#define LEVEL_FILE_PATH "Level"

typedef enum{false,true}bool;
typedef enum{EMPTY=' ',MINER='p',EARTH='.',BORDER='|',ROCK='o',DIAMOND='*',SPIDER='c',MONSTER='e',WATER='x',DOOR='g'}Object;
typedef enum{PASSIVE,ACTIVE,ROLL,PAUSE,CONTINUE,LEVELUP,LEVELAGAIN,FINISH,CHEAT}State;

typedef struct
{
	SDL_Rect posRect,cropRect;
	SDL_Rect cameraRect,drawingRect;
	SDL_Texture *tex;
	SDL_Scancode keys[4];
	int textureWidth,textureHeight;
	int frameWidth,frameHeight;
	float frameCounter;
	int x_vel,y_vel;
	int collectedDiamond;
	bool movement;
	int health;
	Uint32 elapsedTime,prevTime,curTime;
	State state;
}Miner;

typedef struct
{
	Object id;
	SDL_Rect posRect,cropRect;
	SDL_Rect drawingRect;
	int frameWidth,frameHeight;
	int frameCounter;
	int x_vel,y_vel;
	Uint32 discharge;
	bool movement;
	bool update;
	State state;
}Objects;

typedef struct level_node
{
	int id;
	int row,column;
	int totalDiamond;
	Uint32 water_discharge;
	Uint32 totalTime;
	Objects **map;
	struct level_node *next;
}Level;

typedef struct
{
	int cur_level,score;
	SDL_Texture *tex,*textLevel,*textDia,*textTime,*textScore,*textHealth;
	SDL_Rect cropScore,cropTime,cropDia,cropLevel,cropHealth;
	SDL_Rect posScore,posTime,posDia,posLevel,posHealth;
	SDL_Rect posTextScore,posTextTime,posTextDia,posTextLevel,posTextHealth;
	SDL_Color c;
}Panel;

typedef struct
{
	Miner miner;
	Level *level;
	Panel panel;
	State state;
	SDL_Texture *tex;
}Game;

SDL_Window *win;
SDL_Renderer *renderer;
SDL_Event event;
TTF_Font *font;

SDL_Texture *textures;
SDL_Texture *introTex;
SDL_Texture *howtoTex;
SDL_Texture *finalTex;
SDL_Texture *minerTex;
SDL_Texture *panelTex;

Mix_Music *cur_music;
Mix_Music *boulder_world_music;
Mix_Music *ice_world_music;
Mix_Music *ocean_world_music;
Mix_Music *relic_world_music;
Mix_Music *sand_world_music;
Mix_Music *volcanic_world_music;
Mix_Music *ending_music;

Mix_Chunk *collectDiamondEffect;
Mix_Chunk *moveEarthEffect;
Mix_Chunk *moveSpaceEffect;
Mix_Chunk *rockEffect;
Mix_Chunk *bonusScoreEffect;
Mix_Chunk *levelUpEffect;

bool SDL_Initialization();
bool SDL_Load_Texture_Audio_TTF();
void Set_Textures(Game *g);
void Set_Object(Game *g, Objects *cur_obj, Object name, int level);
void Set_Music(Game *g);
bool Import_Levels(Game *g);
void Level_Again(Game *g, int level_id);
void Free_Level(Level *cur);
void SDL_DestroyTextures_And_Quit(Game *g);
int str_cmp(char*,char*);

#endif
