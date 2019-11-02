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
	void UpdatePuntuation();

private:
	
	void BlitPuntuation();
	void BlitLives();
	
	

public:
	int numbers;
	int typography1;

	SDL_Texture* graphics = nullptr;

	SDL_Rect puntuationUI;
	SDL_Rect ballsRemaining;
	

	char playerPuntuation[MAX_PUNTUATION_LENGHT] = "0";
	char previousPuntuation[MAX_PUNTUATION_LENGHT] = "0";
	char maxPuntuation[MAX_PUNTUATION_LENGHT] = "0";
};



