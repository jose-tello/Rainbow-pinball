#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
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
	sfx_spritesheet = App->textures->Load("pinball/sfx_spritesheet.png");

	bumpersound = App->audio->LoadFx("pinball/bonus.wav");
	score = App->audio->LoadFx("pinball/score.wav");


	//create map boundries
	int tabletop_no_bumpers[120] = {
		269, 935,
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
		59, 930,
		21, 930,
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
		383, 935
	};
	App->physics->CreateChain(0, 0, tabletop_no_bumpers, 120, false); //map walls

	//create interactive static bodies
	interactables.add(sapphire_b=App->physics->CreateCircle(299, 230, 28, STATIC)); //sapphire
	sapphire.x = 120; sapphire.y = 187; sapphire.w = sapphire.h = 56;
	interactables_list.add(&sapphire);

	interactables.add(ruby_b=App->physics->CreateCircle(285, 315, 28, STATIC)); //ruby
	ruby.x = 0; ruby.y = 187; ruby.w = ruby.h = 56; 
	interactables_list.add(&ruby);

	interactables.add(emmerald_b=App->physics->CreateCircle(386, 430, 28, STATIC)); //emmerald
	emmerald.x = 360; emmerald.y = 67; emmerald.w = emmerald.h = 56;
	interactables_list.add(&emmerald);

	interactables.add(amethyst_b=App->physics->CreateCircle(380, 262, 28, STATIC)); //amethyst
	amethyst.x = 240; amethyst.y = 187; amethyst.w = amethyst.h = 56;
	interactables_list.add(&amethyst);

	interactables.add(magic_Well_b=App->physics->CreateCircle(207, 467, 28, STATIC)); //magic_Well
	magic_Well.x = 240; magic_Well.y = 67; magic_Well.w = magic_Well.h = 56;
	interactables_list.add(&magic_Well);


	//Static bumpers
	int static_bumper_1[6] = {
		0, 116,
		49, 0,
		55, 94
	};
	interactable_bumpers.add( fst_static_bumper = App->physics->CreateChain(428, 640, static_bumper_1, 6, false));
	static_bumper.x = static_bumper.y = 0; static_bumper.w = 56; static_bumper.h = 120;
	interactable_bumpers_list.add(&static_bumper);
	
	
	int static_bumper_2[6] = {
		55, 118,
		0, 93,
		5, 0
	};
	interactable_bumpers.add(snd_static_bumper = App->physics->CreateChain(172, 638, static_bumper_2, 6, false));
	static_bumper_2nd.x = 56; static_bumper_2nd.y = 0; static_bumper_2nd.w = 56; static_bumper_2nd.h = 120;
	interactable_bumpers_list.add(&static_bumper_2nd);

	int static_bumper_3[12] = {
		23, 0,
		0, 7,
		2, 17,
		87, 43,
		93, 34,
		72, 7
	};
	interactable_bumpers.add(trd_static_bumper = App->physics->CreateChain(234, 322, static_bumper_3, 12, false));
	weird_bumper.x = 480; weird_bumper.y = 78; weird_bumper.w = 93; weird_bumper.h = 41;
	interactable_bumpers_list.add(&weird_bumper);




	//Static land masses

	int landmass1[18] = {
	121, 648,
	129, 648,
	132, 730,
	142, 757,
	224, 809,
	211, 818,
	217, 840,
	135, 784,
	120, 747
	};
	land_mass1 = App->physics->CreateChain(0, 0, landmass1, 18, false);
	
	
	int landmass2[18] = {
	121 + 2*(SCREEN_WIDTH/2 -121 +15), 648, // +15 => adjusting to the fact that this pinball is not perfectly symmetrycal
	129 + 2*(SCREEN_WIDTH /2 -129 + 15), 648,
	132 + 2*(SCREEN_WIDTH /2 -132 + 15), 730,
	142 + 2 * (SCREEN_WIDTH /2 -142 + 15), 757,
	224 + 2 * (SCREEN_WIDTH /2 -224 + 15), 809,
	211 + 2 * (SCREEN_WIDTH /2 -211 + 15), 818,
	217 + 2 * (SCREEN_WIDTH /2 -217 + 15), 840,
	135 + 2 * (SCREEN_WIDTH /2 -135 + 15), 784,
	120 + 2 * (SCREEN_WIDTH /2 -120 + 15), 747
	};
	
	land_mass2 = App->physics->CreateChain(0, 0, landmass2, 18, false);


	//create interactive sensors
	heart.x = 360; heart.y = 217; heart.w = 28; heart.h = 23;
	for (int i = 0; i < 3; i++) { score_interactables_list.add(&heart); }

	score_interactables.add(sensorheart1 = App->physics->CreateRectangleSensor(282, 141, 15, 15)); //heart n1
	score_interactables.add(sensorheart2 = App->physics->CreateRectangleSensor(326, 141, 15, 15)); //heart n2
	score_interactables.add(sensorheart3 = App->physics->CreateRectangleSensor(371, 141, 15, 15)); //heart n3

	//create all micro_sensors
	microlight.x = 480; microlight.y = 217; microlight.w = 22; microlight.h = 23;
	for (int i = 0; i < 14; i++) { score_interactables_list.add(&microlight); }

	score_interactables.add(sensorheart1 = App->physics->CreateRectangleSensor(197, 221, 15, 15)); //top_left
	score_interactables.add(sensorheart2 = App->physics->CreateRectangleSensor(227, 213, 15, 15));
	score_interactables.add(sensorheart3 = App->physics->CreateRectangleSensor(256, 203, 15, 15));

	score_interactables.add(micro_sensor4 = App->physics->CreateRectangleSensor(404, 202, 15, 15)); //top_right
	score_interactables.add(micro_sensor5 = App->physics->CreateRectangleSensor(433, 212, 15, 15));
	score_interactables.add(micro_sensor6 = App->physics->CreateRectangleSensor(463, 220, 15, 15));

	score_interactables.add(micro_sensor7 = App->physics->CreateRectangleSensor(249, 366, 15, 15)); //middle
	score_interactables.add(micro_sensor8 = App->physics->CreateRectangleSensor(279, 374, 15, 15));
	score_interactables.add(micro_sensor9 = App->physics->CreateRectangleSensor(307, 383, 15, 15));

	score_interactables.add(micro_sensor10 = App->physics->CreateRectangleSensor(165, 439, 15, 15)); //left
	score_interactables.add(micro_sensor11= App->physics->CreateRectangleSensor(165, 469, 15, 15));
	score_interactables.add(micro_sensor12= App->physics->CreateRectangleSensor(165, 501, 15, 15));

	score_interactables.add(micro_sensor13 = App->physics->CreateRectangleSensor(155, 716, 15, 15)); //isolated in bottom lane
	score_interactables.add(micro_sensor14 = App->physics->CreateRectangleSensor(506, 716, 15, 15));

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


	leftBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f -82, 828, -10, 10, lpoints, 14, -0.30f, -0.02f);
	rightBumper = App->physics->CreateBumper(SCREEN_WIDTH * 0.5f + 117, 828, -90, 10, rpoints, 14, 0.02f, 0.40f);
	


	//ball_kicker
	
	
	ball_kicker = App->physics->CreateRectangle(42, 885, 29, 30, DINAMIC); //Never change this
	ball_kicker_pivot = App->physics->CreateRectangle(42, 930, 29, 10, STATIC);
	
	b2PrismaticJointDef kicker_def;
	b2PrismaticJoint* kicker_joint = nullptr;
	App->physics->CreatePiston(ball_kicker, ball_kicker_pivot, kicker_def, kicker_joint);
	
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

	//blit the kicker + keep it up......
	ball_kicker->body->ApplyForce({ 0,-10 }, { 0, 0 }, true);
	
	int x, y;
	ball_kicker->GetPosition(x, y);
	x -= ball_kicker->width;
	
	if (ball_kicker != NULL)
	{
		App->renderer->Blit(launcher, x, y,NULL,1.0f);
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

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		ball_kicker->body->ApplyForce({ 0,15 }, { 0, 0 }, true); //charge
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ball_kicker->body->ApplyForce({ 0,-1000 }, { 0, 0 }, true); //fire up baby
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
		//if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
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


	//Drawing shiny elements on collision
	c = interactables.getFirst();
	p2List_item<SDL_Rect*>* d;
	d = interactables_list.getFirst();

	while (c != NULL && d != NULL)
	{
		if (c->data->shiny) {
			App->audio->PlayFx(score);
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(sfx_spritesheet, x, y, d->data);
		}
		c = c->next; 
		d = d->next;
	}
	//different because the do not stop to blit after first collision. First 3 are the hearts, others, the microlights.
	c = score_interactables.getFirst();
	d = score_interactables_list.getFirst();
	while (c != NULL && d != NULL)
	{
		if (c->data->shiny) {
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(sfx_spritesheet, x, y, d->data);
		}
		c = c->next;
		d = d->next;
	}


	//different so we can add velocity calculus next.
	c = interactable_bumpers.getFirst();
	d = interactable_bumpers_list.getFirst();
	while (c != NULL && d != NULL)
	{
		if (c->data->shiny) {
			App->audio->PlayFx(bumpersound);
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(sfx_spritesheet, x, y, d->data);
		}
		c = c->next;
		d = d->next;
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

update_status ModuleSceneIntro::PostUpdate() {
	
	p2List_item<PhysBody*>* c;
	c = interactables.getFirst();

	
	while (c != NULL) {

		c->data->shiny = false;
		c = c->next;
	}
	c = interactable_bumpers.getFirst();

	while (c != NULL) {

		c->data->shiny = false;
		c = c->next;
	}
	return UPDATE_CONTINUE;

	//we do not reset score_interactable: those are meant to stay shiny!
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	
	if (bodyA != nullptr) { bodyA->shiny = true; }
	if (bodyB != nullptr) { bodyB->shiny = true; }






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
