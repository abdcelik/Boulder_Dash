#include "BD_game.h"

void Game_Initialization(Game *g)
{
	g->state = LEVELUP;
	g->tex = textures;
	g->panel.tex = panelTex;

	g->panel.cropScore.x = g->panel.cropTime.x = g->panel.cropDia.x = g->panel.cropLevel.x = g->panel.cropHealth.x = 0;
	g->panel.cropScore.y = 0,g->panel.cropTime.y = 46,g->panel.cropDia.y = 46*2,g->panel.cropLevel.y = 46*3,g->panel.cropHealth.y = 46*4;
	g->panel.cropScore.w = 242,g->panel.cropTime.w = 152,g->panel.cropDia.w = g->panel.cropLevel.w = g->panel.cropHealth.w = 120;
	g->panel.cropScore.h = g->panel.cropTime.h = g->panel.cropDia.h = g->panel.cropLevel.h = g->panel.cropHealth.h = 46;
	g->panel.posScore.x = 25*WINDOW_WIDTH/40,g->panel.posTime.x = 10*WINDOW_WIDTH/20,g->panel.posDia.x = 8*WINDOW_WIDTH/20,g->panel.posLevel.x = 6*WINDOW_WIDTH/20,g->panel.posHealth.x = 4*WINDOW_WIDTH/20;
	g->panel.posScore.y = g->panel.posTime.y = g->panel.posDia.y = g->panel.posLevel.y = g->panel.posHealth.y = 100*WINDOW_HEIGHT/5065;
	g->panel.posScore.w = 10*WINDOW_WIDTH/53,g->panel.posTime.w = 10*WINDOW_WIDTH/84,g->panel.posDia.w = g->panel.posLevel.w = g->panel.posHealth.w = 100*WINDOW_WIDTH/1065;	
	g->panel.posScore.h = g->panel.posTime.h = g->panel.posDia.h = g->panel.posLevel.h = g->panel.posHealth.h = 10*WINDOW_HEIGHT/165;	
	g->panel.posTextScore.x = 103*WINDOW_WIDTH/160,g->panel.posTextTime.x = 85*WINDOW_WIDTH/160,g->panel.posTextDia.x = 69*WINDOW_WIDTH/160,g->panel.posTextLevel.x = 54*WINDOW_WIDTH/160,g->panel.posTextHealth.x = 37*WINDOW_WIDTH/160;
	g->panel.posTextScore.y = g->panel.posTextTime.y = g->panel.posTextDia.y = g->panel.posTextLevel.y = g->panel.posTextHealth.y = 0;
	g->panel.posTextScore.w = 6*WINDOW_WIDTH/40,g->panel.posTextTime.w = 3*WINDOW_WIDTH/40,g->panel.posTextDia.w = g->panel.posTextLevel.w = g->panel.posTextHealth.w = 2*WINDOW_WIDTH/40;
	g->panel.posTextScore.h = g->panel.posTextTime.h = g->panel.posTextDia.h = g->panel.posTextLevel.h = g->panel.posTextHealth.h = 1000*WINDOW_HEIGHT/11875;
	g->panel.c.r = g->panel.c.g = g->panel.c.b = g->panel.c.a = 255;
	g->panel.cur_level = 0 , g->panel.score = 0;

	g->miner.tex = minerTex;
	SDL_QueryTexture(g->miner.tex,NULL,NULL,&g->miner.textureWidth,&g->miner.textureHeight);
	g->miner.frameWidth = 3 , g->miner.frameHeight = 4;
	g->miner.cropRect.w = g->miner.textureWidth / g->miner.frameWidth;
	g->miner.cropRect.h = g->miner.textureHeight / g->miner.frameHeight;
	g->miner.cropRect.x = g->miner.cropRect.w , g->miner.cropRect.y = 0;
	g->miner.posRect.x = g->miner.posRect.y = 0;
	g->miner.posRect.w = g->miner.posRect.h = CELL_SIZE;
	g->miner.cameraRect.x = g->miner.cameraRect.y = 0;
	g->miner.cameraRect.w = WINDOW_WIDTH;
	g->miner.cameraRect.h = WINDOW_HEIGHT;
	g->miner.keys[0] = SDL_SCANCODE_UP;
	g->miner.keys[1] = SDL_SCANCODE_DOWN;
	g->miner.keys[2] = SDL_SCANCODE_RIGHT;
	g->miner.keys[3] = SDL_SCANCODE_LEFT;
	g->miner.frameCounter = 0;
	g->miner.x_vel = g->miner.y_vel = 0;
	g->miner.collectedDiamond = 0;
	g->miner.movement = false;
	g->miner.health = 10;
	g->miner.state = ACTIVE;
	g->miner.elapsedTime = g->miner.prevTime = g->miner.curTime = 0;
}

void Get_Event(Game *g)
{
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				g->state = FINISH;
				break;
			case SDL_KEYDOWN:
			{
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_ESCAPE:
						g->state = FINISH;
						break;
					case SDL_SCANCODE_P:
						if(g->state == CONTINUE)
						{
							g->state = PAUSE;
							Mix_PauseMusic();
						}
						else
						{
							g->state = CONTINUE;
							Mix_ResumeMusic();
						}
						break;
					case SDL_SCANCODE_R:
						g->state = LEVELAGAIN;
						break;
					case SDL_SCANCODE_M:
						if(Mix_PausedMusic())
							Mix_ResumeMusic();
						else
							Mix_PauseMusic();
						break;
					case SDL_SCANCODE_F1:
						g->state = CHEAT;
						break;
					default:
						break;
				}
			}
		}
	}
}

void Update_Miner(Game *g)
{
	Objects temp;
	const Uint8 *keyState;
	int i,j,x,y;

	g->miner.movement = true;
	g->miner.frameCounter++;

	keyState = SDL_GetKeyboardState(NULL);

	if(keyState[g->miner.keys[0]])
	{
		g->miner.x_vel = 0 , g->miner.y_vel = -CELL_SIZE;
		g->miner.cropRect.y = g->miner.cropRect.h * 3;
	}
	else if(keyState[g->miner.keys[1]])
	{
		g->miner.x_vel = 0 , g->miner.y_vel = CELL_SIZE;
		g->miner.cropRect.y = 0;
	}
	else if(keyState[g->miner.keys[2]])
	{
		g->miner.x_vel = CELL_SIZE , g->miner.y_vel = 0;
		g->miner.cropRect.y = g->miner.cropRect.h * 2;
	}
	else if(keyState[g->miner.keys[3]])
	{
		g->miner.x_vel = -CELL_SIZE , g->miner.y_vel = 0;
		g->miner.cropRect.y = g->miner.cropRect.h;
	}
	else
	{
		g->miner.x_vel = 0 , g->miner.y_vel = 0;
		g->miner.movement = false;
	}

	i = g->miner.posRect.x/CELL_SIZE;
	x = g->miner.x_vel/CELL_SIZE;
	j = g->miner.posRect.y/CELL_SIZE;
	y = g->miner.y_vel/CELL_SIZE;

	if(g->miner.movement && g->miner.frameCounter >= FPS/MINER_SPEED)
	{
		g->miner.frameCounter = 0;
		g->miner.cropRect.x += g->miner.cropRect.w;
		if(g->miner.cropRect.x >= g->miner.textureWidth)
			g->miner.cropRect.x = 0;

		switch(g->level->map[j+y][i+x].id)
		{
			case EARTH:
				temp = g->level->map[j][i];
				g->level->map[j][i] = g->level->map[j+y][i+x];
				g->level->map[j+y][i+x] = temp;

				g->level->map[j][i].id = EMPTY;
				Set_Object(g,&g->level->map[j][i],g->level->map[j][i].id,g->level->id);
				g->level->map[j][i].posRect.x -= g->miner.x_vel , g->level->map[j][i].posRect.y -= g->miner.y_vel;

				g->level->map[j+y][i+x].posRect.x += g->miner.x_vel , g->level->map[j+y][i+x].posRect.y += g->miner.y_vel;
				g->miner.posRect.x += g->miner.x_vel , g->miner.posRect.y += g->miner.y_vel;
				Mix_PlayChannel(-1,moveEarthEffect,0);
				break;
			case EMPTY:
				temp = g->level->map[j][i];
				g->level->map[j][i] = g->level->map[j+y][i+x];
				g->level->map[j+y][i+x] = temp;

				g->level->map[j][i].posRect.x -= g->miner.x_vel , g->level->map[j][i].posRect.y -= g->miner.y_vel;
				g->level->map[j+y][i+x].posRect.x += g->miner.x_vel , g->level->map[j+y][i+x].posRect.y += g->miner.y_vel;
				g->miner.posRect.x += g->miner.x_vel , g->miner.posRect.y += g->miner.y_vel;
				Mix_PlayChannel(-1,moveSpaceEffect,0);
				break;
			case ROCK:
				if(g->miner.x_vel > 0 && g->level->map[j+y][i+x+1].id == EMPTY)
				{
					temp = g->level->map[j+y][i+x+1];
					g->level->map[j+y][i+x+1] = g->level->map[j+y][i+x];
					g->level->map[j+y][i+x] = temp;

					g->level->map[j+y][i+x+1].posRect.x += CELL_SIZE;
					g->level->map[j+y][i+x].posRect.x -= CELL_SIZE;
				}
				else if(g->miner.x_vel < 0 && g->level->map[j+y][i+x-1].id == EMPTY)
				{
					temp = g->level->map[j+y][i+x-1];
					g->level->map[j+y][i+x-1] = g->level->map[j+y][i+x];
					g->level->map[j+y][i+x] = temp;

					g->level->map[j+y][i+x-1].posRect.x -= CELL_SIZE;
					g->level->map[j+y][i+x].posRect.x += CELL_SIZE;
				}
				break;
			case DIAMOND:
				temp = g->level->map[j][i];
				g->level->map[j][i] = g->level->map[j+y][i+x];
				g->level->map[j+y][i+x] = temp;

				g->level->map[j][i].id = EMPTY;
				Set_Object(g,&g->level->map[j][i],g->level->map[j][i].id,g->level->id);
				g->level->map[j][i].posRect.x -= g->miner.x_vel , g->level->map[j][i].posRect.y -= g->miner.y_vel;

				g->level->map[j+y][i+x].posRect.x += g->miner.x_vel , g->level->map[j+y][i+x].posRect.y += g->miner.y_vel;
				g->miner.posRect.x += g->miner.x_vel , g->miner.posRect.y += g->miner.y_vel;

				g->panel.score += 20;
				g->miner.collectedDiamond++;
				Mix_PlayChannel(-1,collectDiamondEffect,0);
				break;
			case DOOR:
				if(g->miner.collectedDiamond >= g->level->totalDiamond)
				{
					g->miner.posRect.x += g->miner.x_vel;
					g->miner.posRect.y += g->miner.y_vel;
					g->state = LEVELUP;
					Mix_PlayChannel(-1,levelUpEffect,0);
					Level_Final(g);
				}
				break;
			default:
				break;
		}
	}
	else if(g->miner.frameCounter >= FPS/MINER_SPEED)
	{
		g->miner.frameCounter = 0;
		g->miner.cropRect.x = g->miner.cropRect.w;
		g->miner.cropRect.y = 0;
	}

	g->miner.cameraRect.x = g->miner.posRect.x - WINDOW_WIDTH/2;
	g->miner.cameraRect.y = g->miner.posRect.y - WINDOW_HEIGHT/2;

	if(g->miner.cameraRect.x < 0)
		g->miner.cameraRect.x = 0;
	if(g->miner.cameraRect.y < 0)
		g->miner.cameraRect.y = 0;
	if(g->miner.cameraRect.x + g->miner.cameraRect.w >= g->level->column * CELL_SIZE)
		g->miner.cameraRect.x = g->level->column * CELL_SIZE - WINDOW_WIDTH;
	if(g->miner.cameraRect.y + g->miner.cameraRect.h >= g->level->row * CELL_SIZE)
		g->miner.cameraRect.y = g->level->row * CELL_SIZE - WINDOW_HEIGHT;
}

void Update_Objects(Game *g)
{
	int i,j;
	bool alive = true;

	for(i=0; i < g->level->row ; ++i)
		for(j=0; j< g->level->column ; ++j)
			g->level->map[i][j].update = true;

	for(i=0 ; i < g->level->row ; ++i)
	{
		for(j=0; j < g->level->column ; ++j)
		{
			if(g->level->map[i][j].update == true)
				switch(g->level->map[i][j].id)
				{
					case EMPTY:
						Update_Empty(g,i,j);
						break;
					case EARTH:
						Update_Earth(g,i,j);
						break;
					case ROCK:
						alive = Update_Rock(g,i,j);
						break;
					case DIAMOND:
						Update_Diamond(g,i,j);
						break;
					case WATER:
						Update_Water(g,i,j);
						break;
					case DOOR:
						Update_Door(g,i,j);
						break;
					case SPIDER:
						alive = Update_Spider(g,i,j);
						break;
					case MONSTER:
						alive = Update_Monster(g,i,j);
						break;
					default:
						break;
				}
			if(alive == false)
			{
				g->state = LEVELAGAIN;
				return;
			}
		}
	}
}

void Swap_Objects(Objects *obj1, Objects *obj2)
{
	Objects temp;
	SDL_Rect position;

	position = obj1->posRect;
	obj1->posRect = obj2->posRect;
	obj2->posRect = position;

	temp= *obj1;
	*obj1 = *obj2;
	*obj2 = temp;
}

void Update_Texture(Game *g, int row, int column)
{
	g->level->map[row][column].cropRect.y += g->level->map[row][column].cropRect.h;
	if(g->level->map[row][column].cropRect.y >= (g->level->id-1)/2 * 64 + g->level->map[row][column].cropRect.h * g->level->map[row][column].frameHeight)
		g->level->map[row][column].cropRect.y = (g->level->id-1)/2 * 64;
}

void Update_Empty(Game *g, int row, int column)
{
	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;
	if(g->level->map[row][column].frameCounter >= (FPS/OBJ_UPDATE_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;
		Update_Texture(g,row,column);
	}
}

void Update_Earth(Game *g, int row, int column)
{
	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;
	if(g->level->map[row][column].frameCounter >= (FPS/OBJ_UPDATE_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;
		Update_Texture(g,row,column);
	}
}

bool Update_Rock(Game *g, int row, int column)
{
	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;

	if(g->level->map[row][column].frameCounter >= FPS/ROCK_FALL_SPEED)
	{
		if(g->level->map[row][column].state == ROLL)
		{
			if(g->level->map[row+1][column].id != EMPTY)
				Mix_PlayChannel(-1,rockEffect,0);
			switch(g->level->map[row+1][column].id)
			{
				case MINER:
					return false;
				case SPIDER:
					g->level->map[row+1][column].id = EMPTY;
					Set_Object(g,&g->level->map[row+1][column],g->level->map[row+1][column].id,g->level->id);
					Turn_To_Diamond(g,row+1,column,8);
					break;
				case MONSTER:
					g->level->map[row+1][column].id = EMPTY;
					Set_Object(g,&g->level->map[row+1][column],g->level->map[row+1][column].id,g->level->id);
					Turn_To_Diamond(g,row+1,column,12);
					break;
				default:
					break;
			}
		}

		if(g->level->map[row+1][column].id == EMPTY)
			g->level->map[row][column].state = ROLL;
		else
			g->level->map[row][column].state = ACTIVE;
	}

	if(g->level->map[row][column].frameCounter % (FPS/OBJ_UPDATE_SPEED) == 0)
		Update_Texture(g,row,column);

	if(g->level->map[row][column].frameCounter >= (FPS/ROCK_FALL_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;

		if(g->level->map[row+1][column].id == EMPTY)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row+1][column]);
		else if(g->level->map[row+1][column].id == ROCK && g->level->map[row][column-1].id == EMPTY && g->level->map[row+1][column-1].id == EMPTY)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row][column-1]);
		else if(g->level->map[row+1][column].id == ROCK && g->level->map[row][column+1].id == EMPTY && g->level->map[row+1][column+1].id == EMPTY)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row][column+1]);
	}
	return true;
}

void Update_Diamond(Game *g, int row, int column)
{
	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;

	if(g->level->map[row][column].frameCounter % (FPS/OBJ_UPDATE_SPEED) == 0)
		Update_Texture(g,row,column);

	if(g->level->map[row][column].frameCounter >= (FPS/DIA_FALL_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;

		if(g->level->map[row+1][column].id == EMPTY)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row+1][column]);
		else if(g->level->map[row+1][column].id == DIAMOND && g->level->map[row][column-1].id == EMPTY && g->level->map[row+1][column-1].id == EMPTY)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row][column-1]);
		else if(g->level->map[row+1][column].id == DIAMOND && g->level->map[row][column+1].id == EMPTY && g->level->map[row+1][column+1].id == EMPTY)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row][column+1]);
	}
}

bool Update_Spider(Game *g, int row, int column)
{
	int dir[4][2]={{0,1},{1,0},{0,-1},{-1,0}};
	int i,j;

	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;

	if(g->level->map[row+1][column].id == MINER || g->level->map[row-1][column].id == MINER ||
		g->level->map[row][column+1].id == MINER || g->level->map[row][column-1].id == MINER)
		return false;

	if(g->level->map[row][column].frameCounter % (FPS/OBJ_UPDATE_SPEED) == 0)
		Update_Texture(g,row,column);

	if(g->level->map[row][column].frameCounter >= (FPS/SPIDER_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;
		if(g->level->map[row+1][column].id != EMPTY &&
		   g->level->map[row-1][column].id != EMPTY &&
		   g->level->map[row][column+1].id != EMPTY &&
		   g->level->map[row][column-1].id != EMPTY)
			g->level->map[row][column].x_vel = g->level->map[row][column].y_vel = 0;
		else
		{
			g->level->map[row][column].x_vel *= -1;
			g->level->map[row][column].y_vel *= -1;

			for(i=0;i<4;i++)
				if(dir[i][0] == g->level->map[row][column].y_vel && dir[i][1] == g->level->map[row][column].x_vel)
					break;
			for(j=1;j<=4;j++)
			{
				i++;
				if(i > 3) i=0;
				if(g->level->map[row + dir[i][0]][column + dir[i][1]].id == ROCK && g->level->map[row + dir[i][0]][column + dir[i][1]].state == ROLL)
				{
					g->level->map[row][column].x_vel = g->level->map[row][column].y_vel = 0;
					break;
				}
				else if(g->level->map[row + dir[i][0]][column + dir[i][1]].id == EMPTY)
				{
					g->level->map[row][column].x_vel = dir[i][1];
					g->level->map[row][column].y_vel = dir[i][0];
					break;
				}
			}
		}

		Swap_Objects(&g->level->map[row][column],&g->level->map[row + g->level->map[row][column].y_vel][column + g->level->map[row][column].x_vel]);
	}
	return true;
}

bool Update_Monster(Game *g, int row, int column)
{
	int m_x,m_y,x=0,y=0;
	int count1 = 0,count2 = 0;

	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;

	if(g->level->map[row+1][column].id == MINER || g->level->map[row-1][column].id == MINER ||
		g->level->map[row][column+1].id == MINER || g->level->map[row][column-1].id == MINER)
		return false;

	if(g->level->map[row][column].frameCounter % (FPS/OBJ_UPDATE_SPEED) == 0)
		Update_Texture(g,row,column);

	if(g->level->map[row][column].frameCounter >= (FPS/MONSTER_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;
		m_x = g->miner.posRect.x / CELL_SIZE;
		m_y = g->miner.posRect.y / CELL_SIZE;

		if(m_x - column > 0 && m_y - row > 0)
			x = y = 1;
		else if(m_x - column > 0 && m_y - row < 0)
			x = 1 , y = -1;
		else if(m_x - column < 0 && m_y - row > 0)
			x = -1 , y = 1;
		else if(m_x - column < 0 && m_y - row < 0)
			x = y = -1;
		else if(m_x - column == 0 && m_y - row > 0)
			x = 0 , y = 1;
		else if(m_x - column == 0 && m_y - row < 0)
			x = 0 , y = -1;
		else if(m_x - column > 0 && m_y - row == 0)
			x = 1 , y = 0;
		else if(m_x - column < 0 && m_y - row == 0)
			x = -1 , y = 0;

		count1 = Monster_Path(g,row,column+x,x,y);
		count2 = Monster_Path(g,row+y,column,x,y);

		if(count1 == 0 && count2 == 0)
			x = y = 0;
		else if(count1 >= count2)
			y = 0;
		else if(count1 < count2)
			x = 0;

		if(x || y)
			Swap_Objects(&g->level->map[row][column],&g->level->map[row + y][column + x]);
	}
	return true;
}

int Monster_Path(Game *g, int row, int column, int x, int y)
{
	int way1=0,way2=0;
	if (g->level->map[row][column].id != EMPTY)  
		return 0;
	else
	{
		if(y != 0)
			way1 = Monster_Path(g,row+y,column,x,y);
		if(x != 0)
			way2 = Monster_Path(g,row,column+x,x,y);
		if(way1 > way2)
			return(way1+1);
		else
			return(way2+1);
	}
}

void Update_Water(Game *g, int row, int column)
{
	int arr[WATER_MAX_SIZE][2];
	int i,j,r,size=1;

	for(i=0 ; i < WATER_MAX_SIZE ; i++)
		for(j=0 ; j < 2 ; j++)
			arr[i][j] = -1;

	g->level->map[row][column].discharge -= g->miner.curTime - g->miner.prevTime;

	Find_EmptyCell(g,row,column,arr,size);

	if(size < WATER_MAX_SIZE && (int)g->level->map[row][column].discharge <= 0)
	{
		g->level->map[row][column].discharge = g->level->water_discharge;

		for(i=0 ; i < WATER_MAX_SIZE ; i++)
			if(arr[i][0] == -1)
				break;
		if(i != 0)
		{
			r = rand()%i;
			g->level->map[arr[r][0]][arr[r][1]].id = WATER;
			Set_Object(g,&g->level->map[arr[r][0]][arr[r][1]],WATER,g->level->id);
		}
		else
			Turn_Water_Into_Diamond(g,row,column);
	}
}

void Find_EmptyCell(Game *g, int row, int column,int arr[][2], int size)
{
	int i,j;

	g->level->map[row][column].update = false;
	g->level->map[row][column].frameCounter++;

	if(g->level->map[row][column].frameCounter >= (FPS/OBJ_UPDATE_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;
		Update_Texture(g,row,column);
	}

	if(g->level->map[row-1][column].id == WATER && g->level->map[row-1][column].update == true)
		Find_EmptyCell(g,row-1,column,arr,++size);
	else if(g->level->map[row-1][column].id != WATER && size < WATER_MAX_SIZE && (g->level->map[row-1][column].id == EMPTY || g->level->map[row-1][column].id == EARTH))
	{
		for(i=0 ; i < WATER_MAX_SIZE ; i++)
			if(arr[i][0] == -1)
				break;
		for(j=0 ; j <= i ; j++)
			if(arr[j][0] == row-1 && arr[j][1] == column)
			{
				i = -1;
				break;
			}
		if(i != -1)
			arr[i][0] = row-1 , arr[i][1] = column;
	}

	if(g->level->map[row][column-1].id == WATER && g->level->map[row][column-1].update == true)
		Find_EmptyCell(g,row,column-1,arr,++size);
	else if(g->level->map[row][column-1].id != WATER && size < WATER_MAX_SIZE && (g->level->map[row][column-1].id == EMPTY || g->level->map[row][column-1].id == EARTH))
	{
		for(i=0 ; i < WATER_MAX_SIZE ; i++)
			if(arr[i][0] == -1)
				break;
		for(j=0 ; j <= i ; j++)
			if(arr[j][0] == row && arr[j][1] == column-1)
			{
				i = -1;
				break;
			}
		if(i != -1)
			arr[i][0] = row , arr[i][1] = column-1;
	}

	if(g->level->map[row][column+1].id == WATER && g->level->map[row][column+1].update == true)
		Find_EmptyCell(g,row,column+1,arr,++size);
	else if(g->level->map[row][column+1].id != WATER && size < WATER_MAX_SIZE && (g->level->map[row][column+1].id == EMPTY || g->level->map[row][column+1].id == EARTH))
	{
		for(i=0 ; i < WATER_MAX_SIZE ; i++)
			if(arr[i][0] == -1)
				break;
		for(j=0 ; j <= i ; j++)
			if(arr[j][0] == row && arr[j][1] == column+1)
			{
				i = -1;
				break;
			}
		if(i != -1)
			arr[i][0] = row , arr[i][1] = column+1;
	}

	if(g->level->map[row+1][column].id == WATER && g->level->map[row+1][column].update == true)
		Find_EmptyCell(g,row+1,column,arr,++size);
	else if(g->level->map[row+1][column].id != WATER && size < WATER_MAX_SIZE && (g->level->map[row+1][column].id == EMPTY || g->level->map[row+1][column].id == EARTH))
	{
		for(i=0 ; i < WATER_MAX_SIZE ; i++)
			if(arr[i][0] == -1)
				break;
		for(j=0 ; j <= i ; j++)
			if(arr[j][0] == row+1 && arr[j][1] == column)
			{
				i = -1;
				break;
			}
		if(i != -1)
			arr[i][0] = row+1 , arr[i][1] = column;
	}
}

void Turn_Water_Into_Diamond(Game *g, int row, int column)
{
	g->level->map[row][column].id = DIAMOND;
	Set_Object(g,&g->level->map[row][column],DIAMOND,g->level->id);
	
	if(g->level->map[row-1][column].id == WATER)
		Turn_Water_Into_Diamond(g,row-1,column);
	if(g->level->map[row][column-1].id == WATER)
		Turn_Water_Into_Diamond(g,row,column-1);
	if(g->level->map[row][column+1].id == WATER)
		Turn_Water_Into_Diamond(g,row,column+1);
	if(g->level->map[row+1][column].id == WATER)
		Turn_Water_Into_Diamond(g,row+1,column);
}

void Update_Door(Game *g, int row, int column)
{
	g->level->map[row][column].frameCounter++;

	if(g->miner.collectedDiamond < g->level->totalDiamond)
		Set_Object(g,&g->level->map[row][column],BORDER,g->level->id);
	else if(g->level->map[row][column].state == PASSIVE)
	{
		Set_Object(g,&g->level->map[row][column],DOOR,g->level->id);
		g->level->map[row][column].state = ACTIVE;
	}

	if(g->miner.collectedDiamond >= g->level->totalDiamond && g->level->map[row][column].frameCounter >= (FPS/OBJ_UPDATE_SPEED))
	{
		g->level->map[row][column].frameCounter = 0;
		Update_Texture(g,row,column);
	}
}

void Update_Score_Level(Game *g)
{
	char health_buffer[3],level_buffer[3],dia_buffer[3],time_buffer[4],score_buffer[7];

	sprintf(health_buffer,"%02d",g->miner.health);
	sprintf(level_buffer,"%02d",g->panel.cur_level);
	if(g->level->totalDiamond - g->miner.collectedDiamond < 0)
		sprintf(dia_buffer,"%02d",0);
	else
		sprintf(dia_buffer,"%02d",g->level->totalDiamond - g->miner.collectedDiamond);

	if((int)(g->level->totalTime - g->miner.elapsedTime) < 0)
		sprintf(time_buffer,"%03d",0);
	else
		sprintf(time_buffer,"%03d",(g->level->totalTime - g->miner.elapsedTime)/1000);
	sprintf(score_buffer,"%06d",g->panel.score);

	if(!g->panel.textScore)
	{
		SDL_DestroyTexture(g->panel.textHealth);
		SDL_DestroyTexture(g->panel.textLevel);
		SDL_DestroyTexture(g->panel.textDia);
		SDL_DestroyTexture(g->panel.textTime);
		SDL_DestroyTexture(g->panel.textScore);
	}
	
	SDL_Surface *textHealthSur = TTF_RenderText_Solid(font,health_buffer,g->panel.c);
	g->panel.textHealth = SDL_CreateTextureFromSurface(renderer,textHealthSur);
	SDL_FreeSurface(textHealthSur);

	SDL_Surface *textLevelSur = TTF_RenderText_Solid(font,level_buffer,g->panel.c);
	g->panel.textLevel = SDL_CreateTextureFromSurface(renderer,textLevelSur);
	SDL_FreeSurface(textLevelSur);

	SDL_Surface *textDiaSur = TTF_RenderText_Solid(font,dia_buffer,g->panel.c);
	g->panel.textDia = SDL_CreateTextureFromSurface(renderer,textDiaSur);
	SDL_FreeSurface(textDiaSur);

	SDL_Surface *textTimeSur = TTF_RenderText_Solid(font,time_buffer,g->panel.c);
	g->panel.textTime = SDL_CreateTextureFromSurface(renderer,textTimeSur);
	SDL_FreeSurface(textTimeSur);

	SDL_Surface *textScoreSur = TTF_RenderText_Solid(font,score_buffer,g->panel.c);
	g->panel.textScore = SDL_CreateTextureFromSurface(renderer,textScoreSur);
	SDL_FreeSurface(textScoreSur);
}

void Turn_To_Diamond(Game *g, int row, int column, int dia)
{
	int i,j,count=0;

	for(i = row-1 ; i <= row-1+dia/3; ++i)
		for(j = column-1 ; j <= column+1 ; ++j)
		{
			if((g->level->map[i][j].id == EMPTY || g->level->map[i][j].id == EARTH) && count <= dia)
			{
				g->level->map[i][j].id = DIAMOND;
				Set_Object(g,&g->level->map[i][j],g->level->map[i][j].id,g->level->id);
			}
			count++;
		}
}

void Cheat_Game(Game *g)
{
	char text[64] = {'\0'};
	int i;
	SDL_StartTextInput();

	while(g->state == CHEAT)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					g->state = FINISH;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode)
					{
						case SDL_SCANCODE_F1:
							g->state = CONTINUE;
							break;
						case SDL_SCANCODE_ESCAPE:
							g->state = FINISH;
							break;
						case SDL_SCANCODE_RETURN:
							if(str_cmp(text,"levelup") == 0)
							{
								g->state = LEVELUP;
								Level_Final(g);
							}
							else if(str_cmp(text,"addtime") == 0)
								g->miner.elapsedTime -= 10000;
							
							for(i=0;i<64;i++)
								text[i] = '\0';
							break;
						case SDL_SCANCODE_BACKSPACE:
							for(i=0;i<64;i++)
								if(i != 0 && text[i] == '\0')
									text[i-1] = '\0';
							break;
						default:
							break;
					}
					break;
				case SDL_TEXTINPUT:
					strcat(text, event.text.text);
					break;
			}
		}
		Update_Score_Level(g);
		Do_Render(g);
	}

	SDL_StopTextInput();
	g->miner.curTime = SDL_GetTicks();
}

void Do_Render(Game *g)
{
	int i,j;

	SDL_RenderClear(renderer);

	for(i=0 ; i < g->level->row ; ++i)
	{
		for(j=0 ; j < g->level->column ; ++j)
		{
			g->level->map[i][j].drawingRect.x = g->level->map[i][j].posRect.x - g->miner.cameraRect.x;
			g->level->map[i][j].drawingRect.y = g->level->map[i][j].posRect.y - g->miner.cameraRect.y;
			g->level->map[i][j].drawingRect.w = g->level->map[i][j].posRect.w;
			g->level->map[i][j].drawingRect.h = g->level->map[i][j].posRect.h;
			SDL_RenderCopy(renderer,g->tex,&g->level->map[i][j].cropRect,&g->level->map[i][j].drawingRect);
		}
	}

	if(g->miner.state == ACTIVE)
	{
		g->miner.drawingRect.x = g->miner.posRect.x - g->miner.cameraRect.x;
		g->miner.drawingRect.y = g->miner.posRect.y - g->miner.cameraRect.y;
		g->miner.drawingRect.w = g->miner.posRect.w;
		g->miner.drawingRect.h = g->miner.posRect.h;
		SDL_RenderCopy(renderer,g->miner.tex,&g->miner.cropRect,&g->miner.drawingRect);
	}

	SDL_RenderCopy(renderer,g->panel.tex,&g->panel.cropScore,&g->panel.posScore);
	SDL_RenderCopy(renderer,g->panel.tex,&g->panel.cropTime,&g->panel.posTime);
	SDL_RenderCopy(renderer,g->panel.tex,&g->panel.cropDia,&g->panel.posDia);
	SDL_RenderCopy(renderer,g->panel.tex,&g->panel.cropLevel,&g->panel.posLevel);
	SDL_RenderCopy(renderer,g->panel.tex,&g->panel.cropHealth,&g->panel.posHealth);
	SDL_RenderCopy(renderer,g->panel.textScore,NULL,&g->panel.posTextScore);
	SDL_RenderCopy(renderer,g->panel.textTime,NULL,&g->panel.posTextTime);
	SDL_RenderCopy(renderer,g->panel.textDia,NULL,&g->panel.posTextDia);
	SDL_RenderCopy(renderer,g->panel.textLevel,NULL,&g->panel.posTextLevel);
	SDL_RenderCopy(renderer,g->panel.textHealth,NULL,&g->panel.posTextHealth);
	SDL_RenderPresent(renderer);
}
