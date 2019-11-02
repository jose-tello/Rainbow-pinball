#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void BlitMap();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> interactables; 
	p2List<PhysBody*> interactable_bumpers;
	p2List<PhysBody*> score_interactables;



	p2List<SDL_Rect*> interactables_list;
	p2List<SDL_Rect*> score_interactables_list;
	p2List<SDL_Rect*> interactable_bumpers_list;
	

	PhysBody* sensorheart1;
	PhysBody* sensorheart2;
	PhysBody* sensorheart3;
	PhysBody* micro_sensor1;
	PhysBody* micro_sensor2;
	PhysBody* micro_sensor3;
	PhysBody* micro_sensor4;
	PhysBody* micro_sensor5;
	PhysBody* micro_sensor6;
	PhysBody* micro_sensor7;
	PhysBody* micro_sensor8;
	PhysBody* micro_sensor9;
	PhysBody* micro_sensor10;
	PhysBody* micro_sensor11;
	PhysBody* micro_sensor12;
	PhysBody* micro_sensor13;
	PhysBody* micro_sensor14;

	PhysBody* land_mass1;
	PhysBody* land_mass2;

	PhysBody* trd_static_bumper;
	PhysBody* fst_static_bumper;
	PhysBody* snd_static_bumper;

	PhysBody* ruby_b;
	PhysBody* sapphire_b;
	PhysBody* emmerald_b;
	PhysBody* amethyst_b;
	PhysBody* magic_Well_b;

	PhysBody* death_trigger;
	PhysBody* lifesaver1;
	PhysBody* lifesaver2;
	PhysBody* platform1;
	PhysBody* platform2;


	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* tabletop;
	SDL_Texture* ball;
	SDL_Texture* sfx_spritesheet;

	SDL_Rect ruby;
	SDL_Rect sapphire;
	SDL_Rect emmerald;
	SDL_Rect amethyst;
	SDL_Rect magic_Well;
	SDL_Rect static_bumper;
	SDL_Rect static_bumper_2nd;
	SDL_Rect weird_bumper;
	SDL_Rect heart;
	SDL_Rect microlight;


	PhysBody* bumper1;
	PhysBody* circleBumper1;


	



	uint bumpersound;
	uint score;
	uint background_music;
	uint lost_ball;
	p2Point<int> ray;
	bool ray_on;
};
