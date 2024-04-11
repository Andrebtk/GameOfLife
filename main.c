#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define HAUTEUR 800
#define LARGEUR 600

#define DEAD 0
#define ALIVE 1

static SDL_Window* screen;
static SDL_Renderer* renderer;


/*

Implementation of the game of life 


each cell is 10x10


nb of cells : 4015


TO DO

in liveAndDead:
    -DO when i-1 or  j+1 is not defined


*/




/* remplit avec la couleur rgb un rectangle de largeur w et hauteur h,
   dont le coin superieur gauche est en (x, y) */
void CINI_fill_rect_rgb(int x, int y, int w, int h,	Uint8 r, Uint8 g, Uint8 b) {
	SDL_Rect rectangle = {x, y, w, h};
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rectangle);
    
}



int nbCells(){
    int res = 0;

    for(int i=0; i<HAUTEUR; i+=11){
        for(int j=0; j<LARGEUR; j+=11){
            res++;
        }
    }

    return res;
}


void drawGrid(int CurrentGame[HAUTEUR/10][LARGEUR/10]){

    for(int i=0; i<HAUTEUR/10; i++){
        for(int j=0; j<LARGEUR/10; j++){

            if(CurrentGame[i][j]==DEAD) {
                CINI_fill_rect_rgb(i*10,j*10,10,10, 255,255,255);
            }

            if(CurrentGame[i][j]==ALIVE){
                CINI_fill_rect_rgb(i*10,j*10,10,10, 0,0,0);
            } 
            if(CurrentGame[i][j] == 7){
                CINI_fill_rect_rgb(i*10,j*10,10,10, 255,0,0);
            }
        }
    }
    SDL_RenderPresent(renderer);
    
}


void initialseGame(int CurrentGame[HAUTEUR/10][LARGEUR/10], int nbCell){
    for(int i =0; i<nbCell; i++){
        CurrentGame[random() % (HAUTEUR/10)][random() % (LARGEUR/10)] = ALIVE;
    }
}

void liveAndDead(int CurrentGame[HAUTEUR/10][LARGEUR/10], int i, int j, int* alive, int* dead){
    *alive = 0;
    *dead = 0;

    //No position error        
    if( (CurrentGame[i-1][j-1] == ALIVE || CurrentGame[i-1][j-1] == DEAD) && (CurrentGame[i+1][j+1] == ALIVE || CurrentGame[i+1][j+1] == DEAD) ){
        
        
        for(int i2 = i-1; i2< i+2; i2++){
            for(int j2= j-1; j2 < j+2; j2++){
                if(j2 != j || i2 != i){
                    
                    if(CurrentGame[i2][j2] == ALIVE) (*alive)++;
                    if(CurrentGame[i2][j2] == DEAD) (*dead)++;
                }
                
            }
        }

    }



}


void rules(int CurrentGame[HAUTEUR/10][LARGEUR/10], int PostGame[HAUTEUR/10][LARGEUR/10]){
    int alive = 0; 
    int dead = 0;

    for(int i=0; i<HAUTEUR/10; i++){
        for(int j=0; j<LARGEUR/10; j++){

            liveAndDead(CurrentGame, i, j, &alive, &dead);          

            if(CurrentGame[i][j] == ALIVE){
                if(alive<2) PostGame[i][j] = DEAD;
                if(alive == 2 || alive == 3) PostGame[i][j] = ALIVE;
                if(alive>3) PostGame[i][j] = DEAD;
            }


            if(CurrentGame[i][j] == DEAD){
                if(alive == 3) PostGame[i][j] = ALIVE;
                else PostGame[i][j] = DEAD;
            }

        }
    }
}



/*
Postion

i,j
|
0X0
00X
XXX

*/

void spawnSpaceShip(int CurrentGame[HAUTEUR/10][LARGEUR/10], int i, int j){
    
    CurrentGame[i+1][j] = ALIVE;
    CurrentGame[i+2][j+1] = ALIVE;
    CurrentGame[i][j+2] = ALIVE;
    CurrentGame[i+1][j+2] = ALIVE;
    CurrentGame[i+2][j+2] = ALIVE;

}

void spawnRPentomino(int CurrentGame[HAUTEUR/10][LARGEUR/10], int i, int j){
    CurrentGame[i][j+1] = ALIVE;
    CurrentGame[i][j+2] = ALIVE;
    CurrentGame[i+1][j] = ALIVE;
    CurrentGame[i+1][j+1] = ALIVE;
    CurrentGame[i+2][j+1] = ALIVE;
}



int main(){


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
   

    screen = SDL_CreateWindow(
        "Game of Life",
        0,
        0,
        HAUTEUR,
        LARGEUR,
        SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);


    int running = 1;
    SDL_Event e;

    //Game initialisation
    int nbCell = nbCells();

    int CurrentGame[HAUTEUR/10][LARGEUR/10];
    int PostGame[HAUTEUR/10][LARGEUR/10];



    for(int i=0; i<HAUTEUR/10; i++){
        for(int j=0; j<LARGEUR/10; j++){
            CurrentGame[i][j] = DEAD;
        }
    }
    
    

    //Fill the screen white
    CINI_fill_rect_rgb(0,0,HAUTEUR,LARGEUR, 255, 255, 255);
    
    //inital grid 
    
    //initialseGame(CurrentGame, 500);
    spawnRPentomino(CurrentGame, 20, 20);
    //CurrentGame[5][5] = 7;
    //liveAndDead(CurrentGame, 5,5, &alive, &dead);
    drawGrid(CurrentGame);

    while(running){

        while (SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                running = 0;
            }
        }

        rules(CurrentGame, PostGame);
        
        memcpy(CurrentGame, PostGame, sizeof(PostGame));

        drawGrid(CurrentGame);

        usleep( 100000 );
    }

    


    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;

}
