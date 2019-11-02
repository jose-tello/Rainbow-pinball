#pragma once

#include "Module.h"
#include "Globals.h"
#include "ModuleTextures.h"

#define MAX_PUNTUATION_LENGHT 10

struct SDL_Texture;


class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();
	
	void SumPuntuation(int points);

private:
	
	void GetPuntuations();
	void BlitPuntuation();
	
	

public:
	int numbers;
	int typography1;
	int stopedTimer;

	bool doubleKO;
	bool timeOver;

	SDL_Texture* graphics = nullptr;
	SDL_Rect puntuationUI;
	SDL_Rect ballsRemaining;
	

	char playerPuntuation[MAX_PUNTUATION_LENGHT] = "0";
};



