#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI assets");
	bool ret = true;

	typography1 = App->fonts->Load("pinball/Font_name.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~·!@#$%^&*()-+=[]{}|:;ç'<>,./? ", 1);
	numbers = App->fonts->Load("pinball/puntuationNumbers.png", "0123456789", 1);
	graphics = App->textures->Load("pinball/ui_pinball.png");
	

	puntuationUI.x = 0;
	puntuationUI.y = 0;
	puntuationUI.w = 270;
	puntuationUI.h = 100;


	ballsRemaining.x = 29;
	ballsRemaining.y = 115;
	ballsRemaining.w = 17;
	ballsRemaining.h = 16;

	int currentPuntuation = 0;

	return ret;
}

// Load assets
bool ModuleUI::CleanUp() {

	App->textures->Unload(graphics);
	App->fonts->UnLoad(numbers);
	App->fonts->UnLoad(typography1);

	graphics = nullptr;

	return true;
}

// Update: draw background
update_status ModuleUI::Update() {
	
	
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		SumPuntuation(5);
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		//lose a life
		App->player->lifes--;
	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		//lose a life
		App->player->lifes++;
	}

	App->renderer->Blit(graphics, 5, 10, &puntuationUI);
	BlitPuntuation();
	BlitLives();

	return UPDATE_CONTINUE;
}


void ModuleUI::UpdatePuntuation() {
	
	strcpy_s(previousPuntuation, playerPuntuation);

	if (atoi(playerPuntuation) > atoi(maxPuntuation))
	{
		strcpy_s(maxPuntuation, playerPuntuation);
	}
	
	memset(playerPuntuation, NULL, MAX_PUNTUATION_LENGHT);
	playerPuntuation[0] = '0';
}

void ModuleUI::SumPuntuation(int points) {

	int aux;
	aux = atoi(playerPuntuation) + points;  //atoi converts a string to int

	_itoa_s(aux, playerPuntuation, MAX_PUNTUATION_LENGHT, 10); //itoa converts an int to string

}

void ModuleUI::BlitPuntuation() {
	int x = strlen(playerPuntuation); //Get the current array lenght

	App->fonts->BlitText(240 - x * App->fonts->fonts[numbers].char_w, 60, numbers, playerPuntuation);

	App->fonts->BlitText(30, 110, typography1, "MAX POINTS:");
	App->fonts->BlitText(160, 110, typography1, maxPuntuation);

	App->fonts->BlitText(30, 130, typography1, "PREV. POINTS:");
	App->fonts->BlitText(183, 130, typography1, previousPuntuation);

}

void ModuleUI::BlitLives() {
	for (int i = 0; i < App->player->lifes-1; i++)
	{
		App->renderer->Blit(graphics, 239 - 24 * i, 33, &ballsRemaining);
	}
}
