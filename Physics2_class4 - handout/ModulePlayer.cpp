#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "ModuleUI.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
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

	sfx_spritesheet = App->textures->Load("assets/map/sfx_spritesheet.png");
	launcher = App->textures->Load("assets/map/launcher.png");

	game_over = App->audio->LoadFx("assets/music_and_sounds/game_over.wav");
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


	leftFliper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f - 96, 816, -10, 10, lpoints, 14, -0.30f, -0.02f);
	player_flipers.add(leftFliper);
	player_fliper_left.x = 120; player_fliper_left.y = 61; player_fliper_left.w = 98; player_fliper_left.h = 59;
	player_flipers_rects.add(&player_fliper_left);


	rightFliper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f + 128, 816, -90, 10, rpoints, 14, 0.05f, 0.50f);
	player_flipers.add(rightFliper);
	player_fliper_right.x = 120; player_fliper_right.y = 0; player_fliper_right.w = 98; player_fliper_right.h = 59;
	player_flipers_rects.add(&player_fliper_right);


	leftUpFliper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f - 220, 345, -10, 10, lpoints, 14, -0.30f, 0.15f);
	player_flipers.add(leftUpFliper);
	player_fliper_left.x = 120; player_fliper_left.y = 61; player_fliper_left.w = 98; player_fliper_left.h = 59;
	player_flipers_rects.add(&player_fliper_left);


	//ball_kicker

	ball_kicker = App->physics->CreateRectangle(31, 895, 33, 50, DINAMIC); //Never change this
	ball_kicker_pivot = App->physics->CreateRectangle(42, 930, 29, 10, STATIC);

	b2PrismaticJointDef kicker_def;
	b2PrismaticJoint* kicker_joint = nullptr;
	App->physics->CreatePiston(ball_kicker, ball_kicker_pivot, kicker_def, kicker_joint);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp() {

	App->textures->Unload(sfx_spritesheet);
	App->textures->Unload(launcher);
	
	sfx_spritesheet = nullptr;
	launcher = nullptr;

	player_flipers.clear();
	player_flipers_rects.clear();

	delete ball_kicker;
	ball_kicker = nullptr;
	delete ball_kicker_pivot;
	ball_kicker_pivot = nullptr;

	delete leftFliper;
	leftFliper = nullptr;
	delete rightFliper;
	rightFliper = nullptr;
	delete leftUpFliper;
	leftUpFliper = nullptr;

	LOG("Unloading player");

	return true;
}

update_status ModulePlayer::PreUpdate()
{
	//blit the kicker + keep it up......
	ball_kicker->body->ApplyForce({ 0,-10 }, { 0, 0 }, true);

	int x, y;
	ball_kicker->GetPosition(x, y);
	x -= ball_kicker->width;

	if (ball_kicker != NULL)
	{
		App->renderer->Blit(launcher, x, y, NULL, 1.0f);
	}

	return UPDATE_CONTINUE;

}


// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		leftFliper->body->ApplyAngularImpulse(-60, true);
		leftUpFliper->body->ApplyAngularImpulse(-60, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		rightFliper->body->ApplyAngularImpulse(60, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		ball_kicker->body->ApplyForce({ 0,15 }, { 0, 0 }, true); //charge
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ball_kicker->body->ApplyForce({ 0,-1000 }, { 0, 0 }, true); //fire up ball
	}

	DrawFlipers();


	if (App->player->lifes == 0) {
		LoseGame();
	}


	return UPDATE_CONTINUE;
}


update_status ModulePlayer::PostUpdate() {

	return UPDATE_CONTINUE;
}


void ModulePlayer::LoseGame() {

	App->scene_intro->circles.getFirst()->data->body->SetTransform(b2Vec2(PIXEL_TO_METERS(30), PIXEL_TO_METERS(810)), 0);
	App->scene_intro->circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(0, 0));
	App->scene_intro->circles.getFirst()->data->body->SetAngularVelocity(0);

	App->audio->PlayFx(App->player->game_over);
	App->scene_intro->mov_platform1->body->SetTransform(b2Vec2(0,-30), 51);
	App->scene_intro->mov_platform2->body->SetTransform(b2Vec2(0,-30), -51);

	p2List_item<PhysBody*> *c;

	c = App->scene_intro->hearts.getFirst();
	while (c != NULL) {

		c->data->interacted = false;
		c = c->next;
	}

	c = App->scene_intro->score_interactables.getFirst();
	while (c != NULL) {

		c->data->interacted = false;
		c = c->next;
	}

	App->fade->FadeToBlack(2);
	

	App->UI->UpdatePuntuation();

	App->player->lifes = 3;
}


void ModulePlayer::DrawFlipers() {

	p2List_item<PhysBody*>*c = player_flipers.getFirst();
	p2List_item<SDL_Rect*>* d = player_flipers_rects.getFirst();
	while (c != NULL && d != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(sfx_spritesheet, x, y, d->data, 1.0f, c->data->GetRotation(), 0, 0);

		c = c->next;
		d = d->next;
	}
}



