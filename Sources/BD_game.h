#include "BD_basis.h"
#include "BD_animation.h"

#ifndef BD_game
#define BD_game

void Game_Initialization(Game *g);
void Get_Event(Game *g);
void Update_Miner(Game *g);
void Update_Objects(Game *g);
void Swap_Objects(Objects *obj1, Objects *obj2);
void Update_Texture(Game *g, int row, int column);
void Update_Empty(Game *g, int row, int column);
void Update_Earth(Game *g, int row, int column);
bool Update_Rock(Game *g, int row, int column);
void Update_Diamond(Game *g, int row, int column);
bool Update_Spider(Game *g, int row, int column);
bool Update_Monster(Game *g, int row, int column);
int Monster_Path(Game *g, int row, int column, int x, int y);
void Update_Water(Game *g, int row, int column);
void Find_EmptyCell(Game *g, int row, int column,int arr[][2], int size);
void Turn_Water_Into_Diamond(Game *g, int row, int column);
void Update_Door(Game *g, int row, int column);
void Update_Score_Level(Game *g);
void Turn_To_Diamond(Game *g, int row, int column, int dia);
void Cheat_Game(Game *g);
void Do_Render(Game *g);

#endif
