#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleUI.h"

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
	box = App->textures->Load("pinball/platform.png");
	rick = App->textures->Load("pinball/rick_head.png");
	tabletop = App->textures->Load("pinball/tabletop_no_bumpers.png");
	sfx_spritesheet = App->textures->Load("pinball/sfx_spritesheet.png");

	background_music = App->audio->PlayMusic("pinball/my_little_pony.wav", 3);
	bumpersound = App->audio->LoadFx("pinball/bonus.wav");
	score = App->audio->LoadFx("pinball/score.wav");
	lost_ball = App->audio->LoadFx("pinball/lost_ball.wav");
	



	//create map boundries
int tabletop_no_bumpers[82] = {
	69, 933,
	67, 495,
	74, 347,
	102, 267,
	159, 174,
	215, 128,
	255, 107,
	264, 171,
	150, 208,
	103, 284,
	77, 341,
	80, 363,
	141, 415,
	142, 543,
	97, 577,
	87, 653,
	89, 875,
	123, 874,
	123, 822,
	270, 902,
	271, 934,
	385, 934,
	384, 901,
	530, 823,
	532, 874,
	566, 874,
	565, 598,
	534, 556,
	548, 517,
	419, 437,
	398, 402,
	461, 316,
	484, 249,
	486, 206,
	395, 172,
	382, 18,
	202, 78,
	125, 139,
	51, 251,
	16, 384,
	14, 931
};
	App->physics->CreateChain(0, 0, tabletop_no_bumpers, 82, false); //map walls

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
	217, 832,
	130, 780,
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
	217 + 2 * (SCREEN_WIDTH /2 -217 + 20), 832,
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

	//Create small life saver sensors at ball pits
	lifesaver1 = App->physics->CreateRectangleSensor(105, 870, 30, 5);
	lifesaver2 = App->physics->CreateRectangleSensor(550, 870, 30, 5);

	//sensors of the activable platforms
	platform1 = App->physics->CreateRectangleSensor(107, 828, 36, 5);
	platform2 = App->physics->CreateRectangleSensor(550, 828, 36, 5);
	
	//create "ball death trigger"
	death_trigger = App->physics->CreateRectangleSensor(328, 930, 110, 5);
	

	//ball_kicker
	
	
	ball_kicker = App->physics->CreateRectangle(42, 885, 29, 30, DINAMIC); //Never change this
	ball_kicker_pivot = App->physics->CreateRectangle(42, 930, 29, 10, STATIC);
	
	b2PrismaticJointDef kicker_def;
	b2PrismaticJoint* kicker_joint = nullptr;
	App->physics->CreatePiston(ball_kicker, ball_kicker_pivot, kicker_def, kicker_joint);


	//first ball
	circles.add(App->physics->CreateCircle(30, 810, 15, DINAMIC));
	circles.getLast()->data->listener = this;
	
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
	

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		ball_kicker->body->ApplyForce({ 0,15 }, { 0, 0 }, true); //charge
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ball_kicker->body->ApplyForce({ 0,-1000 }, { 0, 0 }, true); //fire up ball
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15, DINAMIC));
		//circles.getLast()->data->listener = this;

		//back to original pos
		circles.getFirst()->data->body->SetTransform(b2Vec2(PIXEL_TO_METERS(30), PIXEL_TO_METERS(810)), 0);
		circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(0, 0));
		circles.getFirst()->data->body->SetAngularVelocity(0);
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		int x, y;
		platform1->GetPosition(x, y);
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), platform1->width, platform1->height, STATIC));
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

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
	//lose a life
		App->player->lifes--; 
	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		//reset velocity
		circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(0, 0));
		circles.getFirst()->data->body->SetAngularVelocity(0);
	}
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT) //lower volume
	{
		int x = Mix_VolumeMusic(-1); //-1 returns the actual value
		Mix_VolumeMusic(x-1);
		
	}
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT) //up volume
	{
		int x = Mix_VolumeMusic(-1); //-1 returns the actual value
		Mix_VolumeMusic(x+1);

	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	
	
	if (death_trigger->interacted == true) {
		
		death_trigger->interacted = false; //reset bool

		App->player->lifes--;

		if (App->player->lifes > 0) { App->audio->PlayFx(lost_ball); } //if its not game over, play a sound

		//send ball back to original pos
		circles.getFirst()->data->body->SetTransform(b2Vec2(PIXEL_TO_METERS(30), PIXEL_TO_METERS(810)), 0);
		circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(0, 0));
		circles.getFirst()->data->body->SetAngularVelocity(0);
	}
	

	//check if ball fell into pit
	if (lifesaver1->interacted == true) {
		
		circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(0, -25));
	}
	if (lifesaver2->interacted == true) {
		
		
		circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(0, -25));
	}
	
	if (platform1->interacted == true && lifesaver1->interacted == true) {

		platform1->interacted = lifesaver1->interacted = false;
		int x, y;
		platform1->GetPosition(x, y);
		PhysBody* thisbox = (App->physics->CreateAngledRectangle(x + platform1->width, y , platform1->width, platform1->height, KINEMATIC,60));
		
		//thisbox->body->SetTransform(b2Vec2(x, y), 45);
		boxes.add(thisbox);
	}
	if (platform2->interacted == true && lifesaver2->interacted == true) {

		platform2->interacted = lifesaver2->interacted = false;
		int x, y;
		platform2->GetPosition(x, y);
		boxes.add(App->physics->CreateAngledRectangle(x + platform2->width, y , platform2->width, platform2->height, KINEMATIC, -60));
	
	}
	
	// All draw functions ------------------------------------------------------


	//circles = our ball
	
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
		App->renderer->Blit(box, x, y,NULL,1.0f,c->data->GetRotation());
		/*
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		*/
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
		if (c->data->interacted) {
			App->audio->PlayFx(score);
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(sfx_spritesheet, x, y, d->data);

			App->UI->SumPuntuation(5);
		}
		c = c->next; 
		d = d->next;
	}
	//different because the do not stop to blit after first collision. First 3 are the hearts, others, the microlights.
	c = score_interactables.getFirst();
	d = score_interactables_list.getFirst();
	while (c != NULL && d != NULL)
	{
		if (c->data->interacted) {
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
		if (c->data->interacted) {
			App->audio->PlayFx(bumpersound);
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(sfx_spritesheet, x, y, d->data);
			x = circles.getFirst()->data->body->GetLinearVelocity().x * 2;
			y = circles.getFirst()->data->body->GetLinearVelocity().y * 2;

			circles.getFirst()->data->body->SetLinearVelocity(b2Vec2(x,y));
		
			
		}
		c = c->next;
		d = d->next;
	}
	
	//player bumpers. those move.
	

	
	// -----------------
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

		c->data->interacted = false;
		c = c->next;
	}
	c = interactable_bumpers.getFirst();

	while (c != NULL) {

		c->data->interacted = false;
		c = c->next;
	}

	c = circles.getFirst();
	while (c != NULL) {

		c->data->interacted = false;
		c = c->next;
	}

	//make sure that the block of lifesaver only happens if you're coming from it, not to it
	platform2->interacted = false;
	platform1->interacted = false;

	return UPDATE_CONTINUE;

	//we do not reset score_interactable: those are meant to stay shiny!



	
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	
	if (bodyA != nullptr) { bodyA->interacted = true; }
	if (bodyB != nullptr) { bodyB->interacted = true; }


	if (bodyA == death_trigger || bodyB == death_trigger) {
		
			
				
	}
}
