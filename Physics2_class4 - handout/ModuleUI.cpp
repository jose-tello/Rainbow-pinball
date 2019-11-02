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

	numbers = App->fonts->Load("pinball/puntuationNumbers.png", "0123456789", 1);
	graphics = App->textures->Load("pinball/ui_pinball.png");

	

	puntuationUI.x = 0;
	puntuationUI.y = 0;
	puntuationUI.w = 270;
	puntuationUI.h = 100;


	int currentPuntuation = 0;

	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{

	App->textures->Unload(graphics);
	App->fonts->UnLoad(numbers);
	

	graphics = nullptr;

	return true;
}

// Update: draw background
update_status ModuleUI::Update() {
	
	int aux;
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		aux = atoi(playerPuntuation) + 5;  //atoi converts a string to int
		
		_itoa_s(aux, playerPuntuation, 10, 10); //itoa converts an int to string
	}

	aux;

	App->renderer->Blit(graphics, 5, 10, &puntuationUI);
	App->fonts->BlitText(240, 60, 0, playerPuntuation);

	return UPDATE_CONTINUE;
}


void ModuleUI::GetPuntuations() {
	
}

void ModuleUI::BlitPuntuation() {
	
}

