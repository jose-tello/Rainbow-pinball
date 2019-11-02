#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleAudio.h"


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	int lifes;
	uint game_over;
};