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
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> micro_sensors; //probably usefull to check for collisions and special blits

	PhysBody* leftBumper;
	PhysBody* rightBumper;

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
	PhysBody* weird_static_bumper;
	PhysBody* fst_static_bumper;
	PhysBody* snd_static_bumper;
	PhysBody* ruby_b;
	PhysBody* sapphire_b;
	PhysBody* emmerald_b;
	PhysBody* amethyst_b;
	PhysBody* magic_Well_b;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* tabletop;
	SDL_Texture* ball;
	SDL_Texture* launcher;
	SDL_Texture* sfx_spritesheet;

	SDL_Rect ruby;
	SDL_Rect sapphire;
	SDL_Rect emmerald;
	SDL_Rect amethyst;
	SDL_Rect magic_Well;
	SDL_Rect static_bumper;
	SDL_Rect weird_bumper;
	SDL_Rect heart;
	SDL_Rect microlight;

	PhysBody* bumper1;
	PhysBody* circleBumper1;



	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
