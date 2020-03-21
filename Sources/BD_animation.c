#include "BD_animation.h"
#include "BD_game.h"

void Game_Intro(Game *g)
{
	Objects button,s1cloud,s2cloud,m1cloud,b1cloud,b2cloud;
	Mix_Music *introMusic;
	bool enter = false , howto = false;
	int cloud_size,i=0;
	int buttonarr[3][2] = {
		{1000*WINDOW_WIDTH/2327,1000*WINDOW_HEIGHT/1853},
		{1000*WINDOW_WIDTH/2327-100*WINDOW_WIDTH/1207,1000*WINDOW_HEIGHT/1853+10*WINDOW_HEIGHT/158},
		{1000*WINDOW_WIDTH/2327,1000*WINDOW_HEIGHT/1853+20*WINDOW_HEIGHT/158}
	};

	introMusic = Mix_LoadMUS(MUSIC_FILE_PATH"/Title_Screen.mp3");
	button.cropRect.x = 98 , button.cropRect.y = 64*6;
	button.cropRect.w = button.cropRect.h = 16;
	button.posRect.w = button.posRect.h = WINDOW_WIDTH/32;

	cloud_size = WINDOW_WIDTH / 32;
	s1cloud.cropRect.x = 0 , m1cloud.cropRect.x = 16 , b1cloud.cropRect.x = 48;
	s1cloud.cropRect.y = m1cloud.cropRect.y = b1cloud.cropRect.y = 64*6;
	s1cloud.cropRect.w = 16 , m1cloud.cropRect.w = 32 , b1cloud.cropRect.w = 50;
	s1cloud.cropRect.h = m1cloud.cropRect.h = b1cloud.cropRect.h = 16;
	s1cloud.posRect.x = WINDOW_WIDTH , m1cloud.posRect.x = WINDOW_WIDTH/2 , b1cloud.posRect.x = WINDOW_WIDTH/4;
	s1cloud.posRect.y = 3*WINDOW_HEIGHT/10 , m1cloud.posRect.y = WINDOW_HEIGHT/5 , b1cloud.posRect.y = WINDOW_HEIGHT/25;
	s1cloud.posRect.w = 2*cloud_size , m1cloud.posRect.w = 4 * cloud_size , b1cloud.posRect.w = 8 * cloud_size;
	s1cloud.posRect.h = 5*cloud_size/4 , m1cloud.posRect.h = 2 *cloud_size , b1cloud.posRect.h = 3 * cloud_size;
	s2cloud = s1cloud;
	s2cloud.posRect.x = WINDOW_WIDTH/3;
	s2cloud.posRect.y = WINDOW_HEIGHT/2;
	b2cloud = b1cloud;
	b2cloud.posRect.x = 3*WINDOW_WIDTH/4 , b2cloud.posRect.y = WINDOW_HEIGHT/50;

	Mix_PlayMusic(introMusic,-1);
	Mix_VolumeMusic(16);

	while(g->state != FINISH && enter == false)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				g->state = FINISH;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				g->state = FINISH;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN)
				enter = true;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_UP && i>0 && howto == false)
					i--;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN && i<2 && howto == false)
					i++;
		}

		button.posRect.x = buttonarr[i][0];
		button.posRect.y = buttonarr[i][1];

		if(enter && button.posRect.y == buttonarr[1][1])
		{
			if(howto == false)
				howto = true;
			else
				howto = false;
			enter = false;
		}
		else if(enter && button.posRect.y == buttonarr[0][1] && howto == false)
			enter = true;
		else if(enter && button.posRect.y == buttonarr[2][1] && howto == false)
			g->state = FINISH;

		s1cloud.posRect.x -= 2 , s2cloud.posRect.x -= 2;
		m1cloud.posRect.x -- , b1cloud.posRect.x -- , b2cloud.posRect.x --;

		if(s1cloud.posRect.x + s1cloud.posRect.h < 0)
			s1cloud.posRect.x = WINDOW_WIDTH;
		if(s2cloud.posRect.x + s2cloud.posRect.h < 0)
			s2cloud.posRect.x = WINDOW_WIDTH;
		if(m1cloud.posRect.x + m1cloud.posRect.w < 0)
			m1cloud.posRect.x = WINDOW_WIDTH;
		if(b1cloud.posRect.x + b1cloud.posRect.w < 0)
			b1cloud.posRect.x = WINDOW_WIDTH;
		if(b2cloud.posRect.x + b2cloud.posRect.w < 0)
			b2cloud.posRect.x = WINDOW_WIDTH;

		SDL_RenderClear(renderer);
		if(howto == false)
		{
			SDL_RenderCopy(renderer,introTex,NULL,NULL);
			SDL_RenderCopy(renderer,g->tex,&button.cropRect,&button.posRect);
		}
		else
			SDL_RenderCopy(renderer,howtoTex,NULL,NULL);
		SDL_RenderCopy(renderer,g->tex,&s1cloud.cropRect,&s1cloud.posRect);
		SDL_RenderCopy(renderer,g->tex,&s2cloud.cropRect,&s2cloud.posRect);
		SDL_RenderCopy(renderer,g->tex,&m1cloud.cropRect,&m1cloud.posRect);
		SDL_RenderCopy(renderer,g->tex,&b1cloud.cropRect,&b1cloud.posRect);
		SDL_RenderCopy(renderer,g->tex,&b2cloud.cropRect,&b2cloud.posRect);
		SDL_RenderPresent(renderer);
	}
}

void Game_Final(Game *g, int situation)
{
	SDL_Texture *finalText;
	SDL_Rect textPos;
	bool quit = false;
	char buffer[16];

	Mix_PlayMusic(ending_music,-1);
	Mix_VolumeMusic(MUSIC_VOL);

	if(situation == 0)	sprintf(buffer,"%s","You Win");
	else	sprintf(buffer,"%s","Game Over");

	SDL_Surface *sur = TTF_RenderText_Solid(font,buffer,g->panel.c);
	finalText = SDL_CreateTextureFromSurface(renderer,sur);
	SDL_FreeSurface(sur);

	textPos.x = 16*WINDOW_WIDTH/40 , textPos.y = WINDOW_HEIGHT/20;
	textPos.w = WINDOW_WIDTH/5 , textPos.h = WINDOW_HEIGHT/10;
	g->panel.posScore.x = 65*WINDOW_WIDTH/160 , g->panel.posScore.y = 3*WINDOW_HEIGHT/10;

	while(!quit)
	{
		while(SDL_PollEvent(&event))
			if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN  && (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE || event.key.keysym.scancode == SDL_SCANCODE_RETURN)))
				quit = true;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,finalTex,NULL,NULL);
		SDL_RenderCopy(renderer,finalText,NULL,&textPos);
		SDL_RenderCopy(renderer,g->panel.textScore,NULL,&g->panel.posScore);
		SDL_RenderPresent(renderer);
	}
}

void Level_Final(Game *g)
{
	int add = -4,count = 0;
	Uint32 prev_time = 0,cur_time = 0;

	g->miner.cropRect.x = g->miner.cropRect.w;
	g->miner.cropRect.y = 0;

	Mix_PauseMusic();
	Mix_VolumeChunk(bonusScoreEffect,128);
	Mix_PlayChannel(-1,bonusScoreEffect,0);
	
	cur_time = SDL_GetTicks();
	prev_time = cur_time;

	while(cur_time - prev_time < 5000)
	{
		cur_time = SDL_GetTicks();

		if((g->level->totalTime - g->miner.elapsedTime)/1000 > 0)
		{
			g->panel.score++;
			g->miner.elapsedTime += 1000;
		}
		else
			Mix_VolumeChunk(bonusScoreEffect,0);
		
		if(count == -CELL_SIZE)
			add = 4;
		else if(count == 0)
			add = -4;

		g->miner.posRect.y += add;
		count += add;

		Update_Score_Level(g);
		Do_Render(g);
		SDL_Delay(10);
	}

	Mix_HaltChannel(-1);
}

void Level_Show(Game *g)
{
	int i,j;

	g->miner.cameraRect.x = g->level->column * CELL_SIZE - WINDOW_WIDTH;
	g->miner.cameraRect.y = g->level->row * CELL_SIZE - WINDOW_HEIGHT;

	while(g->miner.cameraRect.x > 0 || g->miner.cameraRect.y > 0)
	{
		SDL_RenderClear(renderer);

		for(i=0;i<g->level->row;++i)
			for(j=0;j<g->level->column;++j)
			{
				g->level->map[i][j].drawingRect.x = g->level->map[i][j].posRect.x - g->miner.cameraRect.x;
				g->level->map[i][j].drawingRect.y = g->level->map[i][j].posRect.y - g->miner.cameraRect.y;
				g->level->map[i][j].drawingRect.w = g->level->map[i][j].posRect.w;
				g->level->map[i][j].drawingRect.h = g->level->map[i][j].posRect.h;
				SDL_RenderCopy(renderer,g->tex,&g->level->map[i][j].cropRect,&g->level->map[i][j].drawingRect);
			}

		if(g->miner.state == ACTIVE)
		{
			g->miner.drawingRect.x = g->miner.posRect.x - g->miner.cameraRect.x;
			g->miner.drawingRect.y = g->miner.posRect.y - g->miner.cameraRect.y;
			g->miner.drawingRect.w = g->miner.posRect.w;
			g->miner.drawingRect.h = g->miner.posRect.h;
			SDL_RenderCopy(renderer,g->miner.tex,&g->miner.cropRect,&g->miner.drawingRect);
		}
		
		SDL_RenderPresent(renderer);

		if(g->miner.cameraRect.y != 0)
			g->miner.cameraRect.y -= 5;
		else
			g->miner.cameraRect.x -= 5;

		SDL_Delay(10);
	}
}
