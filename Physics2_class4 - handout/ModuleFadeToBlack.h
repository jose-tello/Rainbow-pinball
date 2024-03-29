#ifndef __ModuleFadeToBlack_H__
#define __ModuleFadeToBlack_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"
#include "Application.h"

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack(Application* app, bool start_enabled = true);
	~ModuleFadeToBlack();
	bool Start();
	update_status PostUpdate();
	bool CleanUp();
	void FadeToBlack(Module* module_off, Module* module_on, float time = 2.0f);
	void ModuleFadeToBlack::FadeToBlack(float time);
	Module* to_enable = nullptr;
	Module* to_disable = nullptr;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;


	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_H__