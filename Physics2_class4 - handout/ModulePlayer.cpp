#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	game_over = App->audio->LoadFx("pinball/game_over.wav");
	lifes = 3;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->player->lifes < 1) {
		App->audio->PlayFx(App->player->game_over);
		App->fade->FadeToBlack(2);
		App->player->lifes = 3;
	}


	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate() {

	return UPDATE_CONTINUE;
}





