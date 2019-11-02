#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModuleTextures.h"

class PhysBody;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	void LoseGame();

public:
	int lifes;
	uint game_over;

private:
	PhysBody* leftBumper;
	PhysBody* rightBumper;
	PhysBody* leftUpBumper;

	SDL_Rect player_bumper_left;
	SDL_Rect player_bumper_right;

	p2List<PhysBody*> player_bumpers;
	p2List<SDL_Rect*> player_bumpers_list;

	SDL_Texture* sfx_spritesheet = nullptr;
};