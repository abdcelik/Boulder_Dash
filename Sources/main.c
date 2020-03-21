#include "BD_basis.h"
#include "BD_game.h"
#include "BD_animation.h"

int main()
{
	Game game;
	srand(time(NULL));

	if(SDL_Initialization() == false)
	{
		printf("Error! Initialization\n");
		return false;
	}
	if(SDL_Load_Texture_Audio_TTF() == false)
	{
		printf("Error! Loading Textures\n");
		return false;
	}

	Game_Initialization(&game);

	if(Import_Levels(&game) == false)
	{
		printf("Error! Importing Levels\n");
		return false;
	}
	
	Game_Intro(&game);

	while(game.state != FINISH)
	{
		game.miner.prevTime = game.miner.curTime;
		game.miner.curTime = SDL_GetTicks();
		game.miner.elapsedTime += game.miner.curTime - game.miner.prevTime;

		if((int)(game.level->totalTime - game.miner.elapsedTime) < 0)
			game.state = LEVELAGAIN;

		Get_Event(&game);

		if(game.state == CHEAT)
			Cheat_Game(&game);

		if(game.state == PAUSE)
			game.miner.elapsedTime -= game.miner.curTime - game.miner.prevTime;

		if(game.state == CONTINUE)
		{
			Update_Miner(&game);
			Update_Objects(&game);
			Update_Score_Level(&game);
			Do_Render(&game);
		}

		if(game.state == LEVELUP)
		{
			game.miner.collectedDiamond = 0;
			game.state = CONTINUE;

			if(game.panel.cur_level != 0)
				game.level = game.level->next;
			game.panel.cur_level++;

			if(game.state == CONTINUE && game.level != NULL)
			{
				Set_Textures(&game);
				Set_Music(&game);
				for(int i=0; i < game.level->row; ++i)
					for(int j=0; j < game.level->column; ++j)
						if(game.level->map[i][j].id == MINER)
							game.miner.posRect.x = j * CELL_SIZE , game.miner.posRect.y = i * CELL_SIZE;
				
				Level_Show(&game);
				game.miner.elapsedTime = 0;
				game.miner.curTime = SDL_GetTicks();
			}
			else if(game.level == NULL)
			{
				game.state = FINISH;
				Game_Final(&game,0);
			}
		}

		if(game.state == LEVELAGAIN)
		{
			Level_Again(&game,game.level->id);
			Set_Music(&game);
			for(int i=0; i < game.level->row; ++i)
				for(int j=0; j < game.level->column; ++j)
					if(game.level->map[i][j].id == MINER)
						game.miner.posRect.x = j * CELL_SIZE , game.miner.posRect.y = i * CELL_SIZE;
		}

		if(game.miner.health < 1)
		{
			game.state = FINISH;
			Game_Final(&game,1);
		}
	}

	SDL_DestroyTextures_And_Quit(&game);
	return 0;
}
