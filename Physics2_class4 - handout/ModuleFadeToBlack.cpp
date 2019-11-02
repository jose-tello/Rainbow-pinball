#include <math.h>
#include "Application.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

ModuleFadeToBlack::ModuleFadeToBlack(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	LOG("Contructor of Fade to Black");
}


ModuleFadeToBlack::~ModuleFadeToBlack()
{}

// Load assets
bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
update_status ModuleFadeToBlack::PostUpdate() {

	if (current_step == fade_step::none)
		return UPDATE_CONTINUE;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = min(1.0f, (float)now / (float)total_time); //¿MIN != min?

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			if (to_disable != nullptr) {
				to_disable->Disable();
			}
			//App->frames = 0;

			if (to_enable != nullptr)
			{
				to_enable->Enable();
			}
			
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time) {
			current_step = fade_step::none;
		}
	} break;
	}
	
	// Finally render the black square with alpha on the screen
	int screen_width = +(SCREEN_WIDTH);	//conversion needed since win uses these values as uint and SDL_Rect only works with int
	int screen_height = +(SCREEN_HEIGHT);
	screen = { 0, 0, screen_width, screen_height}; 
	SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->renderer->renderer, &screen);

	return UPDATE_CONTINUE;
}

// Fade to black the screen. At mid point, deactivate one module, then activate the other.
void ModuleFadeToBlack::FadeToBlack(Module* module_off, Module* module_on, float time)
{

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_disable = module_off;
		to_enable = module_on;
	}


}

//Fade without messing with modules. Used to "reset" positions in a visually smooth way
void ModuleFadeToBlack::FadeToBlack(float time) {

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		to_disable = nullptr;
		to_enable = nullptr;
	}
}

bool ModuleFadeToBlack::CleanUp() {
	
	to_disable = nullptr;
	to_enable = nullptr;

	return true;
}