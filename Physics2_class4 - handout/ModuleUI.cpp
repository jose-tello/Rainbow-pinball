#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"

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
	
	App->renderer->Blit(graphics, 50, 50, &puntuationUI);
	App->fonts->BlitText(50, 50, 0, "5");

	return UPDATE_CONTINUE;
}


void ModuleUI::GetPuntuations() {
	
}

void ModuleUI::BlitPuntuation() {
	
}

