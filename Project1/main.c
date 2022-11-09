#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include "objects.h"


//Variáveis globais
const int WIDTH = 1500;
const int HEIGHT = 800;
int NUM_BULLETS = 60;
int NUM_COMETS = 35;
int NUM_COMETS2 = 5;
int NUM_COMETS3 = 5;
int FatSpeed = 13;
int HealthyFoodsSpeed = 8;
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false, false, false, false };

//Protótipos

void InitShip(SpaceShip* ship, ALLEGRO_BITMAP* image);
void DrawShip(SpaceShip* ship);
void MoveShipUp(SpaceShip* ship);
void MoveShipDown(SpaceShip* ship);
void MoveShipLeft(SpaceShip* ship);
void MoveShipRight(SpaceShip* ship);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip* ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip *ship);
void CollideBullet2(Bullet bullet[], int bSize, Comet2 comets2[], int c2Size, SpaceShip* ship);
void CollideBullet3(Bullet bullet[], int bSize, Comet3 comets3[], int c3Size, SpaceShip* ship);

void InitComet(Comet comets[], int size, ALLEGRO_BITMAP*fat);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
void CollideComet(Comet comets[], int cSize, SpaceShip* ship);

void InitComet2(Comet2 comets2[], int size, ALLEGRO_BITMAP*protein);
void DrawComet2(Comet2 comets2[], int size);
void StartComet2(Comet2 comets2[], int size);
void UpdateComet2(Comet2 comets2[], int size);
void CollideComet2(Comet2 comets2[], int c2Size, SpaceShip* ship);
void CollideComet3(Comet3 comets3[], int c3Size, SpaceShip* ship);

void DrawComet3(Comet comets3[], int size);
void InitComet3(Comet3 comets3[], int size, ALLEGRO_BITMAP* carbo);



int main(void)
{
	//Variável primitiva
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool isGameOver = false;
	bool gameWon = false;

	//Variável de objeto
	SpaceShip ship;
	Bullet bullets[60];
	Comet comets[35];
	Comet2 comets2[5];
	Comet comets3[5];

	//Variáveis do Allegro
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_FONT* font18 = NULL;
	ALLEGRO_BITMAP* shipimage;
	ALLEGRO_BITMAP* fat;
	ALLEGRO_BITMAP* protein;
	ALLEGRO_BITMAP* carbo;


	if (!al_init()) //Inicializar Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);

	if (!display)
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	shipimage = al_load_bitmap("shooter.bmp");
	fat = al_load_bitmap("icecream.bmp");
	protein = al_load_bitmap("chicken.bmp");
	carbo = al_load_bitmap("bread.bmp");

	

	srand(time(NULL));
	InitShip(&ship, shipimage);
	InitBullet(bullets, NUM_BULLETS);
	InitComet(comets, NUM_COMETS, fat);
	InitComet2(comets2, NUM_COMETS2, protein);
	InitComet3(comets3, NUM_COMETS3, carbo);

	font18 = al_load_font("moonrock.ttf", 30, 0);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (keys[UP])
				MoveShipUp(&ship);
			if (keys[DOWN])
				MoveShipDown(&ship);
			if (keys[LEFT])
				MoveShipLeft(&ship);
			if (keys[RIGHT])
				MoveShipRight(&ship);

			if (!isGameOver)
			{
				UpdateBullet(bullets, NUM_BULLETS);
				StartComet(comets, NUM_COMETS);
				StartComet2(comets2, NUM_COMETS2);
				StartComet(comets3, NUM_COMETS3);
				UpdateComet(comets, NUM_COMETS);
				UpdateComet2(comets2, NUM_COMETS2);
				UpdateComet(comets3, NUM_COMETS3);
				CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS, &ship);
				CollideBullet2(bullets, NUM_BULLETS, comets2, NUM_COMETS2, &ship);
				CollideBullet3(bullets, NUM_BULLETS, comets3, NUM_COMETS3, &ship);
				CollideComet(comets, NUM_COMETS, &ship);
				CollideComet2(comets2, NUM_COMETS2, &ship);
				CollideComet3(comets3, NUM_COMETS3, &ship);
			}

			if (ship.lives <= 0)
			{
				isGameOver = true;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				FireBullet(bullets, NUM_BULLETS, &ship);
				break;
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}

		
		

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if (!isGameOver)
			{
				DrawShip(&ship);
				DrawBullet(bullets, NUM_BULLETS);
				DrawComet(comets, NUM_COMETS, &ship);
				DrawComet2(comets2, NUM_COMETS2);
				DrawComet3(comets3, NUM_COMETS3);

				al_draw_textf(font18, al_map_rgb(0, 0, 255), 5, 5, 0, "PONTOS DE NUTRICAO: %i      GORDURAS DESTRUIDAS PELO LANCADOR DE AGUA: %i", ship.lives, ship.score);
			}
			else
			{
				al_draw_textf(font18, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Game Over!   Final Score: %i", ship.score);
				
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(255, 255, 255));
		}

	}

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font18);
	
	

	return 0;

}

void InitShip(SpaceShip* ship, ALLEGRO_BITMAP* image)
{
	ship->x = 40;
	ship->y = HEIGHT / 2;
	ship->ID = PLAYER;
	ship->lives = 3;
	ship->speed = 4;
	ship->boundx = 30;
	ship->boundy = 40;
	ship->score = 0;

	ship->maxframe = 3;
	ship->curframe = 0;
	ship->framecount = 0;
	ship->framedelay = 50;
	ship->framewidth = 50;
	ship->frameheight = 28;
	ship->animationcolumns = 3;
	ship->animationdirection = 1;

	ship->animationrow = 1;

	ship->image = image;
}

void DrawShip(SpaceShip* ship)
{
	al_draw_bitmap(ship->image, ship->x - 45, ship->y - 30, 0);
}
void MoveShipUp(SpaceShip* ship)
{
	ship->y -= ship->speed;
	if (ship->y < 0)
	{
		ship->y = 0;
	}
}
void MoveShipDown(SpaceShip* ship)
{
	ship->y += ship->speed;
	if (ship->y > HEIGHT)
	{
		ship->y = HEIGHT;
	}
}
void MoveShipLeft(SpaceShip* ship)
{
	ship->x -= ship->speed;
	if (ship->x < 0)
	{
		ship->x = 0;
	}
}
void MoveShipRight(SpaceShip* ship)
{
	ship->x += ship->speed;
	if (ship->x > 750)
	{
		ship->x = 750;
	}
}

void InitBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 10;
		bullet[i].live = false;
	}
}
void DrawBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y, 4, al_map_rgb(0, 0, 255));
	}
}
void FireBullet(Bullet bullet[], int size, SpaceShip* ship)
{
	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = ship->x + 17;
			bullet[i].y = ship->y;
			bullet[i].live = true;
			break;
		}

	}
}
void UpdateBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].x += bullet[i].speed;
			if (bullet[i].x > WIDTH)
			{
				bullet[i].live = false;
			}
		}
	}
}

void InitComet(Comet comets[], int size, ALLEGRO_BITMAP*fat)
{
	for (int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
		comets[i].live = false;
		comets[i].speed = 9;
		comets[i].boundx = 24;
		comets[i].boundy = 24;

		comets[i].cometimage = fat;
	}
}
void DrawComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			al_draw_bitmap(comets->cometimage, comets[i].x - 30, comets[i].y - 30, 0);
		}
	}
}
void StartComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!comets[i].live)
		{
			if (rand() % 500 == 0)
			{
				comets[i].live = true;
				comets[i].x = WIDTH ;
				comets[i].y = 30 + rand() % (HEIGHT - 60);
			}
		}
	}
}
void UpdateComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			comets[i].x -= comets[i].speed;


		}
	}
}
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip *ship)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (comets[j].live)
				{
					if (bullet[i].x > (comets[j].x - comets[j].boundx) &&
						bullet[i].x < (comets[j].x + comets[j].boundx) &&
						bullet[i].y >(comets[j].y - comets[j].boundy) &&
						bullet[i].y < (comets[j].y + comets[j].boundy))
					{
						bullet[i].live = false;
						comets[j].live = false;
						ship->score++;
						
					}
				}
			}
			
		}
	}
}
void CollideBullet2(Bullet bullet[], int bSize, Comet2 comets2[], int c2size, SpaceShip* ship)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int k = 0; k < c2size; k++)
			{
				if (comets2[k].live)
				{
					if (bullet[i].x > (comets2[k].x - comets2[k].boundx) &&
						bullet[i].x < (comets2[k].x + comets2[k].boundx) &&
						bullet[i].y >(comets2[k].y - comets2[k].boundy) &&
						bullet[i].y < (comets2[k].y + comets2[k].boundy))
					{
						bullet[i].live = false;
						comets2[k].live = false;
						ship->lives--;
						

					}
				}
			}
		}
	}
}
void CollideBullet3(Bullet bullet[], int bSize, Comet3 comets3[], int c3Size, SpaceShip* ship)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int k = 0; k < c3Size; k++)
			{
				if (comets3[k].live)
				{
					if (bullet[i].x > (comets3[k].x - comets3[k].boundx) &&
						bullet[i].x < (comets3[k].x + comets3[k].boundx) &&
						bullet[i].y >(comets3[k].y - comets3[k].boundy) &&
						bullet[i].y < (comets3[k].y + comets3[k].boundy))
					{
						bullet[i].live = false;
						comets3[k].live = false;
						ship->lives--;


					}
				}
			}
		}
	}
}
void CollideComet(Comet comets[], int cSize, SpaceShip* ship)
{
	for (int i = 0; i < cSize; i++)
	{
		if (comets[i].live)
		{
			if (comets[i].x - comets[i].boundx < ship->x + ship->boundx &&
				comets[i].x + comets[i].boundx > ship->x - ship->boundx &&
				comets[i].y - comets[i].boundy < ship->y + ship->boundy &&
				comets[i].y + comets[i].boundy > ship->y - ship->boundy)
			{
				ship->lives--;
				comets[i].live = false;
			}
			else if (comets[i].x < 0)
			{
				comets[i].live = false;
				//
				
			}
		}

	}
}

void InitComet2(Comet2 comets2[], int size, ALLEGRO_BITMAP*protein)
{
	for (int i = 0; i < size; i++)
	{
		comets2[i].ID = ENEMY;
		comets2[i].live = false;
		comets2[i].speed = 7;
		comets2[i].boundx = 22;
		comets2[i].boundy = 22;

		comets2[i].cometimage2 = protein;
	}
}
void InitComet3(Comet3 comets3[], int size, ALLEGRO_BITMAP* carbo)
{
	for (int i = 0; i < size; i++)
	{
		comets3[i].ID = ENEMY;
		comets3[i].live = false;
		comets3[i].speed = 7;
		comets3[i].boundx = 22;
		comets3[i].boundy = 22;

		comets3[i].cometimage3 = carbo;
	}
}
void DrawComet2(Comet2 comets2[], int size, SpaceShip *ship)
{
	for (int i = 0; i < size; i++)
	{
		if (comets2[i].live)
		{
			al_draw_bitmap(comets2->cometimage2, comets2[i].x - 30, comets2[i].y - 30, 0);
		}
	}
}
void StartComet2(Comet2 comets2[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!comets2[i].live)
		{
			if ((rand() % 500 == 0))
			{
				comets2[i].live = true;
				comets2[i].x = WIDTH;
				comets2[i].y = 30 + rand() % (HEIGHT - 60);
			}
		}
	}
}
void UpdateComet2(Comet2 comets2[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets2[i].live)
		{
			comets2[i].x -= comets2[i].speed;


		}
	}
}
void CollideComet2(Comet2 comets2[], int c2Size, SpaceShip* ship)
{
	for (int i = 0; i < c2Size; i++)
	{
		if (comets2[i].live)
		{
			if (comets2[i].x - comets2[i].boundx < ship->x + ship->boundx &&
				comets2[i].x + comets2[i].boundx > ship->x - ship->boundx &&
				comets2[i].y - comets2[i].boundy < ship->y + ship->boundy &&
				comets2[i].y + comets2[i].boundy > ship->y - ship->boundy)
			{
				ship->lives++;
				comets2[i].live = false;
			}
			else if (comets2[i].x < 0)
			{
				comets2[i].live = false;
				//

			}
		}

	}
}

void CollideComet3(Comet3 comets3[], int c3Size, SpaceShip* ship)
{
	for (int i = 0; i < c3Size; i++)
	{
		if (comets3[i].live)
		{
			if (comets3[i].x - comets3[i].boundx < ship->x + ship->boundx &&
				comets3[i].x + comets3[i].boundx > ship->x - ship->boundx &&
				comets3[i].y - comets3[i].boundy < ship->y + ship->boundy &&
				comets3[i].y + comets3[i].boundy > ship->y - ship->boundy)
			{
				ship->lives++;
				comets3[i].live = false;
			}
			else if (comets3[i].x < 0)
			{
				comets3[i].live = false;
				//

			}
		}

	}
}
void DrawComet3(Comet3 comets3[], int size, SpaceShip *ship)
{
	for (int i = 0; i < size; i++)
	{
		if (comets3[i].live)
		{
			al_draw_bitmap(comets3->cometimage3, comets3[i].x - 30, comets3[i].y - 30, 0);
		}
	}
}