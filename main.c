#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

/* Constates sous forme de macros */
#define GRID_SIZE 	10
#define NB_COLUMNS 	90
#define NB_LINES 	60
#define WIDTH 		NB_COLUMNS 	* GRID_SIZE
#define HEIGHT 		NB_LINES 	* GRID_SIZE
#define START_X		0
#define START_Y		0

/* Prototypes des  structures (si necessaire) */
typedef struct 	Mouse {
	Sint32 x;
	Sint32 y;
}Mouse;

/*************************
 * N'oublie pas que tu as modifié les regles alors va les changer
 * 
 */

/* Prototypes des fonctions */
void 	sdl_print_error(char *msg);
void 	Destroy_Window(SDL_Renderer *renderer, SDL_Window *window);
void	Draw_Grid(SDL_Renderer *renderer);


void	Init_Matrice(uint8_t (*matrice)[NB_LINES][NB_COLUMNS]);
void	Affiche_Matrice(uint8_t (*matrice)[NB_LINES][NB_COLUMNS]);
void	Draw_Matrice(SDL_Renderer *renderer, uint8_t (*matrice)[NB_LINES][NB_COLUMNS]);
void 	Calcule_Next_Gen(uint8_t (*cur)[NB_LINES][NB_COLUMNS], uint8_t (*next)[NB_LINES][NB_COLUMNS]);
void	Swap_Gen(uint8_t (**cur)[NB_LINES][NB_COLUMNS], uint8_t (**next)[NB_LINES][NB_COLUMNS]);
void 	Edit_Matrice(SDL_MouseButtonEvent *mouse, uint8_t (*matrice)[NB_LINES][NB_COLUMNS]);
void 	Edit_Matrice_2(uint8_t (*matrice)[NB_LINES][NB_COLUMNS], Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);


uint8_t 	Calcule_Somme_Voisin(uint8_t (*matrice)[NB_LINES][NB_COLUMNS], uint8_t x, uint8_t y);

/* Prototypes des Fonctions qui seront appellées dans le main */
/* Fonction qui initialise les données*/
void 	Load_Game(); 
// fonction qui met a jour les données
void 	Update_Game(); 
// fonction qui affiche a l'écran
void 	Draw_Game(SDL_Renderer *renderer); 

/* Variables goblales */
float 	delay = .5*1000;
int 	nb_gen = 0;

bool 	grid_is_visible = true;
bool	on_play = false;
bool 	mouse_edit = false;

uint8_t	next_gen[NB_LINES][NB_COLUMNS] ;

uint8_t	curr_gen[NB_LINES][NB_COLUMNS] ;


uint8_t (*curr)[NB_LINES][NB_COLUMNS]  = &curr_gen;
uint8_t (*next)[NB_LINES][NB_COLUMNS]  = &next_gen;


/* Fonction principale */
int 	main(int argc, char *argv[])
{
	/* Initialisation de la sdl */
	if (0 != SDL_Init(SDL_INIT_VIDEO))
	{
		sdl_print_error("Initialisation de la SDL");
	}

	/* Création des renderer et window */
	SDL_Window 		*window   = NULL;
	SDL_Renderer 	*renderer = NULL;

	/* Initialisation de la window */
	window 	= SDL_CreateWindow("GAME OF LIFE ( M.A.Souaré )", 
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
								WIDTH, HEIGHT, 0);
	
	/* Gestion de l'erreur si l'alloc échoue */
	if (!window)
	{
		sdl_print_error("Initialisation de la window");
		SDL_Quit();
	}

	/* Initialisation du renderer */
	renderer = SDL_CreateRenderer(window, -1, 0);
	
	/* Gestion de l'erreur si l'alloc échoue */
	if (!renderer)
	{
		sdl_print_error("Initialisation du renderer");
		Destroy_Window(renderer, window);
	}

	/* ************************************************** */
	bool game_loop = true;
	SDL_Event event;
	Load_Game();

	Mouse mouse_init;
	Mouse mouse_fin;

	while (game_loop)
	{
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				game_loop = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_g:
					grid_is_visible = !grid_is_visible;
					break;
				
				case SDLK_p:
					on_play = !on_play;
					break;
				case SDLK_r:
					if (!on_play)
						Init_Matrice(curr);
					break;

				default:
					break;
				}
			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				//Edit_Matrice(&event.button, curr);
				if (!on_play)
				{
					mouse_init = (Mouse){event.button.x, event.button.y};
					mouse_edit = true;
				}
			}
			
			
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				//Edit_Matrice(&event.button, curr);
				if (!on_play)
				{
					mouse_fin = (Mouse){event.button.x, event.button.y};
					mouse_edit = false;
					Edit_Matrice_2(curr, mouse_init.x, mouse_init.y, mouse_fin.x, mouse_fin.y);
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 250, 250, 255);
		SDL_RenderClear(renderer);
		
		if (on_play)
		{
			Update_Game();
		}
		Draw_Game(renderer);
		SDL_RenderPresent(renderer);
		
		
		SDL_Delay(delay);
	}
	
	/* ************************************************** */

	/* Libération de la mémoire et sortie de la SDL */
	Destroy_Window(renderer, window);
}












/* Implémentation des fonctions qui seront appellées dans le main 
 * Load_Game() 		=> Initialisation du jeu (appellée une seule fois);
 * Update_Game() 	=> Mise à jour des données du jeu ;
 * Draw_Game() 		=> Affichage à l'écran ;
*/

void 	Load_Game()
{
	srand(time(NULL));
	printf("Init\n");
	Init_Matrice(curr);
	//Affiche_Matrice(curr);
}

void 	Update_Game()
{
	Calcule_Next_Gen(curr, next);
	printf("Changemnt gen =>%d\n", nb_gen);	
	
	Swap_Gen(&curr, &next);
	printf("echange matrice\n");
}

void 	Draw_Game(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 45, 10, 55, 255);
	Draw_Matrice(renderer, curr);
	
	if (grid_is_visible)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		Draw_Grid(renderer);
	}
}






/* Cette fonction permet d'afficher une erreur */
void 	sdl_print_error(char *msg)
{
	fprintf(stderr, "Error : %s => %s.\n", msg, SDL_GetError());
}

/* Cette fonction detruit et libère les window et renderer */
void 	Destroy_Window(SDL_Renderer *renderer, SDL_Window *window)
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
	}

	if (window)
	{
		SDL_DestroyWindow(window);
	}
	fprintf(stdout, "Destruction de la fenetre reussie.\n");
	SDL_Quit();
}

/* Cette fontion affiche la grille */
void	Draw_Grid(SDL_Renderer *renderer)
{
	for (unsigned line = 0; line < WIDTH; line+= GRID_SIZE)
	{
		SDL_RenderDrawLine(renderer, line, START_Y, line, HEIGHT);
	}

	for (unsigned column = 0; column < HEIGHT; column+= GRID_SIZE)
	{
		SDL_RenderDrawLine(renderer, START_X, column, WIDTH, column);
	}
}

/* ************************************************************** */
uint8_t 	Calcule_Somme_Voisin(uint8_t (*m)[NB_LINES][NB_COLUMNS], uint8_t x, uint8_t y)
{
	uint8_t 	Xm1 = (x-1+ NB_COLUMNS) % NB_COLUMNS;
	uint8_t 	Xp1 = (x+1+ NB_COLUMNS) % NB_COLUMNS;
	
	uint8_t 	Ym1 = (y-1+ NB_LINES) % NB_LINES;
	uint8_t 	Yp1 = (y+1+ NB_LINES) % NB_LINES;

	uint8_t 	somme = 0;
	for (unsigned l = 0; l < NB_LINES; ++l)
	{
		(*m)[l][0] = 0;
		(*m)[l][NB_COLUMNS-1] = 0;
	}
	for (unsigned c = 0; c < NB_COLUMNS; ++c)
	{
		(*m)[0][c] = 0;
		(*m)[NB_LINES-1][c] = 0;
	}
	
	somme += (*m)[Ym1][Xm1] + (*m)[Ym1][x] + (*m)[Ym1][Xp1];
	somme += (*m)[y][Xm1]   + (*m)[y][Xp1];
	somme += (*m)[Yp1][Xm1] + (*m)[Yp1][x] + (*m)[Yp1][Xp1];

	return (somme);
}

/* *************************************************************  */
void	Init_Matrice(uint8_t (*matrice)[NB_LINES][NB_COLUMNS])
{
	nb_gen = 0;
	for (unsigned y = 0; y < NB_LINES; y++)
	{
		for (unsigned x = 0; x < NB_COLUMNS; x++)
		{
			(*matrice)[y][x] = 0;//rand() % 2 ;
		}
	}
}

/* *************************************************************  */
void 	Edit_Matrice(SDL_MouseButtonEvent *mouse, uint8_t (*matrice)[NB_LINES][NB_COLUMNS])
{
	unsigned x = (mouse->x) / GRID_SIZE;
	unsigned y = (mouse->y) / GRID_SIZE;

	if (!on_play)
	{
		(*matrice)[y][x] = ((*matrice)[y][x]) ? 0 : 1;
	}
}

void 	Edit_Matrice_2(uint8_t (*matrice)[NB_LINES][NB_COLUMNS], Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2)
{	
	if (!on_play)
	{
		x1 /= GRID_SIZE;
		y1 /= GRID_SIZE;
		x2 /= GRID_SIZE;
		y2 /= GRID_SIZE;

		Sint32 max_x = (x1 > x2) ? x1: x2;
		Sint32 max_y = (y1 > y2) ? y1: y2;

		for (Sint32 y = (y1 < max_y) ? y1: y2; y <= max_y; ++y)
		{
			for (Sint32 x = (x1 < max_x) ? x1: x2; x <= max_x; ++x)
			{
				(*matrice)[y][x] = ((*matrice)[y][x]) ? 0 : 1;
			}
		}
	}
}

/* *************************************************************  */
void	Affiche_Matrice(uint8_t (*matrice)[NB_LINES][NB_COLUMNS])
{
	printf("\ndebut affichage\n\n");
	for (uint8_t y = 0; y < NB_LINES; y++)
	{
		printf("y=%d.\t", y);
		for (uint8_t x = 0; x < NB_COLUMNS; x++)
		{
			printf("%d  ", (*matrice)[y][x]);
		}
		printf("\n");
	}
	printf("\nFin affichage\n\n");
}

/* *************************************************************  */
void 	Calcule_Next_Gen(uint8_t (*cur)[NB_LINES][NB_COLUMNS], uint8_t (*next)[NB_LINES][NB_COLUMNS])
{
	uint8_t somme = 0;
	for (unsigned y = 0; y < NB_LINES; ++y)
	{
		for (unsigned x = 0; x < NB_COLUMNS; ++x)
		{
			somme = Calcule_Somme_Voisin(cur, x, y);
			
			(*next)[y][x] = 0;
			/* Regles du jeu 
			* Une cellule morte possédant exactement trois cellules voisines vivantes devient vivante (elle naît) ;
			* Une cellule vivante possédant moins deux ou plus de trois cellules voisines vivantes meurt ;
			*/
			
			if (somme == 3 && (*cur)[y][x] == 0) (*next)[y][x] = 1;
			if ((somme == 3 || somme == 2) && (*cur)[y][x] == 1) (*next)[y][x] = 1;
		}
	}
	nb_gen++;
}

/* *************************************************************  */
void	Swap_Gen(uint8_t (**cur)[NB_LINES][NB_COLUMNS], uint8_t (**next)[NB_LINES][NB_COLUMNS])
{
	uint8_t (*temp)[NB_LINES][NB_COLUMNS];
	
	temp = *cur;
	*cur  = *next;
	*next = temp;

	printf("Changemnt effectue\n");
}


/* *************************************************************  */
void	Draw_Matrice(SDL_Renderer *renderer, uint8_t (*matrice)[NB_LINES][NB_COLUMNS])
{
	for (uint8_t l = 0; l < NB_LINES; l++)
	{
		for (uint8_t c = 0; c < NB_COLUMNS; c++)
		{
			if ((*matrice)[l][c])
			{
				SDL_Rect r = {c*GRID_SIZE, l*GRID_SIZE, GRID_SIZE, GRID_SIZE};
				SDL_RenderFillRect(renderer, &r);
			}
		}
	}
}
