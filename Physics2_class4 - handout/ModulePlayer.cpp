#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "ModuleUI.h"
#include "ModuleRender.h"
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

	sfx_spritesheet = App->textures->Load("pinball/sfx_spritesheet.png");

	game_over = App->audio->LoadFx("pinball/game_over.wav");
	lifes = 3;

	int lpoints[14] = {
	22, 0,
	97, 42,
	97, 53,
	85, 58,
	6, 27,
	0, 12,
	5, 1
	};

	int rpoints[14] = {
	75, 0,
	0, 42,
	0, 53,
	12, 58,
	91, 27,
	97, 12,
	92, 1
	};


	leftBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f - 85, 824, -10, 10, lpoints, 14, -0.30f, -0.02f);
	player_bumpers.add(leftBumper);
	player_bumper_left.x = 120; player_bumper_left.y = 61; player_bumper_left.w = 98; player_bumper_left.h = 59;
	player_bumpers_list.add(&player_bumper_left);


	rightBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f + 120, 824, -90, 10, rpoints, 14, 0.02f, 0.40f);
	player_bumpers.add(rightBumper);
	player_bumper_right.x = 120; player_bumper_right.y = 0; player_bumper_right.w = 98; player_bumper_right.h = 59;
	player_bumpers_list.add(&player_bumper_right);


	leftUpBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f - 220, 345, -10, 10, lpoints, 14, -0.30f, 0.15f);
	player_bumpers.add(leftUpBumper);
	player_bumper_left.x = 120; player_bumper_left.y = 61; player_bumper_left.w = 98; player_bumper_left.h = 59;
	player_bumpers_list.add(&player_bumper_left);

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		leftBumper->body->ApplyAngularImpulse(-50, true);
		leftUpBumper->body->ApplyAngularImpulse(-50, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		rightBumper->body->ApplyAngularImpulse(50, true);
	}


	p2List_item<PhysBody*>*c = player_bumpers.getFirst();
	p2List_item<SDL_Rect*>* d = player_bumpers_list.getFirst();
	while (c != NULL && d != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(sfx_spritesheet, x, y, d->data, 1.0f, c->data->GetRotation(), 0, 0);

		c = c->next;
		d = d->next;
	}



	if (App->player->lifes == 0) {
		LoseGame();
	}


	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate() {

	return UPDATE_CONTINUE;
}


void ModulePlayer::LoseGame() {

	App->audio->PlayFx(App->player->game_over);
	App->fade->FadeToBlack(2);

	App->UI->UpdatePuntuation();

	App->player->lifes = 3;
}





