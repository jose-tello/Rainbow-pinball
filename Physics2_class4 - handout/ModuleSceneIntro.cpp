#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = tabletop= ball= launcher= NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball = App->textures->Load("pinball/the_ball.png"); 
	circle = App->textures->Load("pinball/the_ball.png");
	launcher = App->textures->Load("pinball/launcher.png");
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	tabletop = App->textures->Load("pinball/tabletop_no_bumpers.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");


	//create map boundries
	int tabletop_no_bumpers[120] = {
		269, 927,
		270, 901,
		126, 822,
		122, 874,
		89, 871,
		84, 638,
		87, 605,
		101, 575,
		125, 557,
		141, 551,
		140, 411,
		149, 398,
		89, 334,
		101, 286,
		122, 243,
		147, 206,
		253, 174,
		264, 167,
		265, 120,
		254, 107,
		204, 131,
		153, 177,
		126, 209,
		102, 248,
		86, 287,
		71, 351,
		63, 414,
		53, 464,
		55, 554,
		59, 849,
		21, 853,
		19, 553,
		21, 466,
		25, 418,
		41, 316,
		75, 224,
		107, 176,
		168, 114,
		264, 64,
		327, 53,
		394, 77,
		385, 103,
		390, 171,
		483, 203,
		461, 311,
		393, 400,
		413, 422,
		407, 447,
		449, 460,
		497, 477,
		544, 520,
		532, 550,
		550, 567,
		566, 601,
		567, 676,
		567, 874,
		532, 874,
		528, 824,
		386, 900,
		383, 928
	};
	App->physics->CreateChain(0, 0, tabletop_no_bumpers, 120, false); //map walls

	//create interactive static bodies
	App->physics->CreateCircle(299, 230, 28, STATIC); //Sapphire
	App->physics->CreateCircle(285, 315, 28, STATIC); //Ruby
	App->physics->CreateCircle(386, 430, 28, STATIC); //Emmerald
	App->physics->CreateCircle(380, 262, 28, STATIC); //Amethyst
	App->physics->CreateCircle(207, 467, 28, STATIC); //Magic Well


	//Static bumpers
	int static_bumper_1[6] = {
		428, 756,
		477, 640,
		483, 734
	};
	App->physics->CreateChain(0, 0, static_bumper_1, 6, false);
	
	int static_bumper_2[6] = {
		227, 756,
		172, 731,
		177, 638
	};
	App->physics->CreateChain(0, 0, static_bumper_2, 6, false);


	int static_bumper_3[12] = {
		257, 322,
		234, 329,
		236, 339,
		321, 365,
		327, 356,
		306, 329
	};
	App->physics->CreateChain(0, 0, static_bumper_3, 12, false);




	//Static land masses

	int landmass1[18] = {
	121, 648,
	129, 648,
	132, 730,
	142, 757,
	224, 809,
	211, 818,
	215, 835,
	135, 784,
	120, 747
	};
	App->physics->CreateChain(0, 0, landmass1, 18, false);

	int landmass2[18] = {
	521, 646,
	532, 648,
	533, 750,
	519, 785,
	441, 833,
	441, 814,
	427, 811,
	507, 760,
	522, 734
	};
	App->physics->CreateChain(0, 0, landmass2, 18, false);


	//create interactive sensors
	sensorheart1 = App->physics->CreateRectangleSensor(282, 140, 25, 25); //heart n1
	sensorheart2 = App->physics->CreateRectangleSensor(326, 140, 25, 25); //heart n2
	sensorheart3 = App->physics->CreateRectangleSensor(371, 140, 25, 25); //heart n3

	//create all micro_sensors
	sensorheart1 = App->physics->CreateRectangleSensor(192, 218, 15, 15); //top_left
	sensorheart2 = App->physics->CreateRectangleSensor(222, 210, 15, 15);
	sensorheart3 = App->physics->CreateRectangleSensor(252, 200, 15, 15);

	micro_sensor4 = App->physics->CreateRectangleSensor(400, 200, 15, 15); //top_right
	micro_sensor5 = App->physics->CreateRectangleSensor(430, 210, 15, 15);
	micro_sensor6 = App->physics->CreateRectangleSensor(460, 218, 15, 15);

	micro_sensor7 = App->physics->CreateRectangleSensor(247, 362, 15, 15); //middle
	micro_sensor8 = App->physics->CreateRectangleSensor(277, 370, 15, 15);
	micro_sensor9 = App->physics->CreateRectangleSensor(305, 379, 15, 15);

	micro_sensor10 = App->physics->CreateRectangleSensor(162, 435, 15, 15); //left
	micro_sensor11= App->physics->CreateRectangleSensor(162, 465, 15, 15);
	micro_sensor12= App->physics->CreateRectangleSensor(162, 497, 15, 15);

	micro_sensor13 = App->physics->CreateRectangleSensor(152, 712, 15, 15); //isolated in bottom lane
	micro_sensor14 = App->physics->CreateRectangleSensor(503, 712, 15, 15);


	leftBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f - 105, 820, 10, 10, 80, 20, false);
	rightBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f + 105, 820, 10, 10, 80, 20, true);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::PreUpdate() {
	// blit the background......
	if (tabletop != NULL)
	{
		App->renderer->Blit(tabletop, 0, 0);
	}

	return UPDATE_CONTINUE;
}


// Update: draw figures
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		leftBumper->body->ApplyAngularImpulse(-50, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		rightBumper->body->ApplyAngularImpulse(50, true);
	}
	
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15, DINAMIC));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, DINAMIC));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			170, 40
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();
	
	
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}



	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
