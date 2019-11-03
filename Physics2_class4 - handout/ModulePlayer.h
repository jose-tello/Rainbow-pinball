#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "SDL\include\SDL.h"

#define PLAYER_MAX_LIFES 6

class PhysBody;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	void LoseGame();
	void DrawFlipers();

public:
	int lifes;
	uint game_over;

private:
	PhysBody* leftFliper;
	PhysBody* rightFliper;
	PhysBody* leftUpFliper;

	SDL_Rect player_fliper_left;
	SDL_Rect player_fliper_right;

	p2List<PhysBody*> player_flipers;
	p2List<SDL_Rect*> player_flipers_rects;

	PhysBody* ball_kicker;
	PhysBody* ball_kicker_pivot;

	SDL_Texture* sfx_spritesheet = nullptr;
	SDL_Texture* launcher = nullptr;
};