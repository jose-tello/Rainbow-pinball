#pragma once

#include "Module.h"
#include "Globals.h"

#include "ModuleTextures.h"

struct SDL_Texture;


class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();
	

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
	

	char playerPuntuation[10] = "0";
};



