#include "BD_basis.h"

bool SDL_Initialization()
{
	Uint32 renderer_flag = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		printf("Error! Initializing : %s\n",SDL_GetError());
		return false;
	}

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) != 0)
	{
		printf("Error! Mixer could not initialize : %s\n",Mix_GetError());
		return false;
	}

	if(TTF_Init() != 0)
	{
		printf("Error! TTF Initializing : %s\n",TTF_GetError());
		return false;
	}
	
	win = SDL_CreateWindow("Boulder Dash",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
	if(!win)
	{
		printf("Error! Creating window : %s\n",SDL_GetError());
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(win,-1,renderer_flag);
	if(!renderer)
	{
		printf("Error! Renderering : %s\n",SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	return true;
}

bool SDL_Load_Texture_Audio_TTF()
{
	SDL_Surface *sur = IMG_Load(IMG_FILE_PATH"/Textures.png");
	if(!sur) return false;
	textures = SDL_CreateTextureFromSurface(renderer,sur);
	SDL_FreeSurface(sur);
	if(!textures) return false;

	SDL_Surface *introSur = IMG_Load(IMG_FILE_PATH"/intro.png");
	if(!introSur) return false;
	introTex = SDL_CreateTextureFromSurface(renderer,introSur);
	SDL_FreeSurface(introSur);
	if(!introTex) return false;

	SDL_Surface *howtoSur = IMG_Load(IMG_FILE_PATH"/howto.png");
	if(!howtoSur) return false;
	howtoTex = SDL_CreateTextureFromSurface(renderer,howtoSur);
	SDL_FreeSurface(howtoSur);
	if(!howtoTex) return false;

	SDL_Surface *finalSur = IMG_Load(IMG_FILE_PATH"/final.png");
	if(!finalSur) return false;
	finalTex = SDL_CreateTextureFromSurface(renderer,finalSur);
	SDL_FreeSurface(finalSur);
	if(!finalTex) return false;

	SDL_Surface *minerSur = IMG_Load(IMG_FILE_PATH"/miner.png");
	if(!minerSur) return false;
	minerTex = SDL_CreateTextureFromSurface(renderer,minerSur);
	SDL_FreeSurface(minerSur);
	if(!minerTex) return false;

	SDL_Surface *panelSur = IMG_Load(IMG_FILE_PATH"/Panel.png");
	if(!panelSur) return false;
	panelTex = SDL_CreateTextureFromSurface(renderer,panelSur);
	SDL_FreeSurface(panelSur);
	if(!panelTex) return false;

	font = TTF_OpenFont(TTF_FILE_PATH"/game.ttf",20);
	if(!font) return false;

	boulder_world_music = Mix_LoadMUS(MUSIC_FILE_PATH"/1-Boulder_World.mp3");
	if(!boulder_world_music) return false;

	ice_world_music = Mix_LoadMUS(MUSIC_FILE_PATH"/2-Ice_World.mp3");
	if(!ice_world_music) return false;

	ocean_world_music = Mix_LoadMUS(MUSIC_FILE_PATH"/3-Ocean_World.mp3");
	if(!ocean_world_music) return false;

	relic_world_music = Mix_LoadMUS(MUSIC_FILE_PATH"/4-Relic_World.mp3");
	if(!relic_world_music) return false;

	sand_world_music = Mix_LoadMUS(MUSIC_FILE_PATH"/5-Sand_World.mp3");
	if(!sand_world_music) return false;

	volcanic_world_music = Mix_LoadMUS(MUSIC_FILE_PATH"/6-Volcanic_World.mp3");
	if(!volcanic_world_music) return false;

	ending_music = Mix_LoadMUS(MUSIC_FILE_PATH"/7-Ending.mp3");
	if(!volcanic_world_music) return false;

	collectDiamondEffect = Mix_LoadWAV(MUSIC_FILE_PATH"/Diamond.wav");
	if(!collectDiamondEffect) return false;

	moveEarthEffect = Mix_LoadWAV(MUSIC_FILE_PATH"/MoveEarth.wav");
	if(!moveEarthEffect) return false;

	moveSpaceEffect = Mix_LoadWAV(MUSIC_FILE_PATH"/MoveSpace.wav");
	if(!moveSpaceEffect) return false;

	rockEffect = Mix_LoadWAV(MUSIC_FILE_PATH"/Rock.wav");
	if(!rockEffect) return false;

	bonusScoreEffect = Mix_LoadWAV(MUSIC_FILE_PATH"/BonusPoints.wav");
	if(!bonusScoreEffect) return false;

	levelUpEffect = Mix_LoadWAV(MUSIC_FILE_PATH"/LevelUp.wav");
	if(!levelUpEffect) return false;

	return true;
}

void Set_Textures(Game *g)
{
	for(int i=0 ; i < g->level->row ; i++)
		for(int j=0 ; j < g->level->column ; j++)
			Set_Object(g,&g->level->map[i][j],g->level->map[i][j].id,g->level->id);
}

void Set_Object(Game *g, Objects *cur_obj, Object name, int level)
{
	cur_obj->frameCounter = 0;
	cur_obj->discharge = g->level->water_discharge;
	cur_obj->movement = false;
	cur_obj->x_vel = cur_obj->y_vel = 0;

	cur_obj->cropRect.w = cur_obj->cropRect.h = 16;
	cur_obj->cropRect.y = (level-1)/2 * 64;
	cur_obj->frameWidth = 1;
	cur_obj->frameHeight = 4;

	switch(name)
	{
		case MINER:
		case EMPTY:
			cur_obj->cropRect.x = 16*2;
			if((level-1)/2 == 0 || (level-1)/2 == 1)
				cur_obj->frameHeight = 1;
			break;
		case EARTH:
			cur_obj->cropRect.x = 16*1;
			if((level-1)/2 == 0 || (level-1)/2 == 1 || (level-1)/2 == 3)
				cur_obj->frameHeight = 1;
			break;
		case BORDER:
			cur_obj->cropRect.x = 16*0;
			cur_obj->frameHeight = 1;
			break;
		case ROCK:
			cur_obj->cropRect.x = 16*3;
			if((level-1)/2 == 4)
				cur_obj->frameHeight = 1;
			break;
		case DIAMOND:
			cur_obj->cropRect.x = 16*4;
			break;
		case SPIDER:
			cur_obj->cropRect.x = 16*10;
			break;
		case MONSTER:
			cur_obj->cropRect.x = 16*9;
			break;					
		case WATER:
			cur_obj->cropRect.x = 16*7;
			break;
		case DOOR:
			cur_obj->state = PASSIVE;
			cur_obj->cropRect.x = 16*6;
			break;	
	}
}

void Set_Music(Game *g)
{
	if(g->level->id >= 0 && g->level->id <= 2)
		cur_music = boulder_world_music;
	else if(g->level->id >= 3 && g->level->id <= 4)
		cur_music = ice_world_music;
	else if(g->level->id >= 5 && g->level->id <= 6)
		cur_music = ocean_world_music;
	else if(g->level->id >= 7 && g->level->id <= 8)
		cur_music = relic_world_music;
	else if(g->level->id >= 9 && g->level->id <= 10)
		cur_music = sand_world_music;
	else if(g->level->id >= 11 && g->level->id <= 12)
		cur_music = volcanic_world_music;

	Mix_PlayMusic(cur_music,-1);
	Mix_Volume(-1,2*MUSIC_VOL);
	Mix_VolumeMusic(MUSIC_VOL);
}

bool Import_Levels(Game *g)
{
	FILE *fp;
	Level *iter_level;
	Objects *cur_obj;
	int i,j;
	char buffer[255];

	fp = fopen(LEVEL_FILE_PATH"/level.txt","r");
	if(!fp) return false;

	g->level = (Level*)malloc(sizeof(Level));
	g->level->next = NULL;
	iter_level = g->level;

	while(!feof(fp))
	{
		while(str_cmp(buffer,"#level#\n") != 0)
			fgets(buffer,255,fp);
		
		fscanf(fp,"%s%d",buffer,&iter_level->id);
		fscanf(fp,"%s%d",buffer,&iter_level->row);
		fscanf(fp,"%s%d",buffer,&iter_level->column);
		fscanf(fp,"%s%d",buffer,&iter_level->totalDiamond);
		fscanf(fp,"%s%u",buffer,&iter_level->totalTime);
		fscanf(fp,"%s%u",buffer,&iter_level->water_discharge);

		iter_level->map = (Objects**)calloc(iter_level->row,sizeof(Objects*));
		for(i=0 ; i < iter_level->row ; i++)
			iter_level->map[i] = (Objects*)calloc(iter_level->column,sizeof(Objects));

		while(str_cmp(buffer,"#map#\n") != 0)
			fgets(buffer,255,fp);

		for(i=0 ; i < iter_level->row ; i++)
		{
			fgets(buffer,255,fp);
			for(j=0 ; j < iter_level->column ; j++)
			{
				cur_obj = &iter_level->map[i][j];
				cur_obj->id = buffer[j];
				cur_obj->posRect.x = j * CELL_SIZE;
				cur_obj->posRect.y = i * CELL_SIZE;
				cur_obj->posRect.w = cur_obj->posRect.h = CELL_SIZE;
			}
		}

		while(str_cmp(buffer,"#level#\n") != 0 && !feof(fp))
			fgets(buffer,255,fp);

		if(!feof(fp))
		{	
			iter_level->next = (Level*)malloc(sizeof(Level));
			iter_level = iter_level->next;
			iter_level->next = NULL;
		}
	}
	fclose(fp);
	return true;
}

void Level_Again(Game *g, int level_id)
{
	Objects *cur_obj;
	FILE *fp;
	char search[10],buffer[255];
	int i,j;

	g->state = CONTINUE;
	g->miner.health--;
	g->miner.collectedDiamond = 0;
	g->miner.elapsedTime = 0;
	sprintf(search,"ID= %d\n",level_id);

	fp = fopen(LEVEL_FILE_PATH"/level.txt","r");

	while(str_cmp(search,buffer) != 0)
		fgets(buffer,255,fp);
	while(str_cmp(buffer,"#map#\n") != 0)
		fgets(buffer,255,fp);

	for(i=0 ; i < g->level->row ; ++i)
		free(g->level->map[i]);
	free(g->level->map);

	g->level->map = (Objects**)calloc(g->level->row,sizeof(Objects*));
	for(i=0 ; i < g->level->row ; ++i)
		g->level->map[i] = (Objects*)calloc(g->level->column,sizeof(Objects));

	for(i=0 ; i < g->level->row ; ++i)
	{
		fgets(buffer,255,fp);
		for(j=0 ; j < g->level->column ; ++j)
		{
			cur_obj = &g->level->map[i][j];
			cur_obj->id = buffer[j];
			cur_obj->posRect.x = j * CELL_SIZE;
			cur_obj->posRect.y = i * CELL_SIZE;
			cur_obj->posRect.w = cur_obj->posRect.h = CELL_SIZE;
		}
	}
	fclose(fp);

	Set_Textures(g);
}

void Free_Level(Level *cur)
{
	if(cur != NULL)
	{
		if(cur->next != NULL)
			Free_Level(cur->next);

		for(int i=0 ; i< cur->row ; i++)
			free(cur->map[i]);
		free(cur->map);
		free(cur);
	}
}

void SDL_DestroyTextures_And_Quit(Game *g)
{
	SDL_DestroyTexture(textures);
	SDL_DestroyTexture(introTex);
	SDL_DestroyTexture(howtoTex);
	SDL_DestroyTexture(finalTex);
	SDL_DestroyTexture(minerTex);
	SDL_DestroyTexture(panelTex);
	if(!g->panel.textScore)
	{
		SDL_DestroyTexture(g->panel.textHealth);
		SDL_DestroyTexture(g->panel.textLevel);
		SDL_DestroyTexture(g->panel.textDia);
		SDL_DestroyTexture(g->panel.textTime);
		SDL_DestroyTexture(g->panel.textScore);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_FreeChunk(collectDiamondEffect);
	Mix_FreeChunk(moveEarthEffect);
	Mix_FreeChunk(moveSpaceEffect);
	Mix_FreeChunk(rockEffect);
	Mix_FreeChunk(bonusScoreEffect);
	Mix_FreeChunk(levelUpEffect);
	Mix_FreeMusic(boulder_world_music);
	Mix_FreeMusic(ice_world_music);
	Mix_FreeMusic(ocean_world_music);
	Mix_FreeMusic(relic_world_music);
	Mix_FreeMusic(sand_world_music);
	Mix_FreeMusic(volcanic_world_music);
	Mix_FreeMusic(ending_music);
	Mix_Quit();
	SDL_Quit();
	Free_Level(g->level);
}

int str_cmp(char *s1, char *s2)
{
	while(*s1 && *s2 && *s1 == *s2)
		s1++,s2++;
	return *s1 - *s2;
}
