#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#define LARGEUR 1230
#define HAUTEUR 780

#define DEAD 0
#define ALIVE 1

static SDL_Window* screen;
static SDL_Renderer* renderer;






/* remplit avec la couleur rgb un rectangle de largeur w et hauteur h,
   dont le coin superieur gauche est en (x, y) */
void CINI_fill_rect_rgb(int x, int y, int w, int h,	Uint8 r, Uint8 g, Uint8 b) {
	SDL_Rect rectangle = {x, y, w, h};
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rectangle);
    
}



int nbCells(){
    int res = 0;

    for(int i=0; i<LARGEUR; i+=11){
        for(int j=0; j<HAUTEUR; j+=11){
            res++;
        }
    }

    return res;
}


void drawGrid(int CurrentGame[LARGEUR/10][HAUTEUR/10]){

    for(int i=0; i<LARGEUR/10; i++){
        for(int j=0; j<HAUTEUR/10; j++){

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


void initialseGame(int CurrentGame[LARGEUR/10][HAUTEUR/10], int nbCell){
    for(int i =0; i<nbCell; i++){
        CurrentGame[random() % (LARGEUR/10)][random() % (HAUTEUR/10)] = ALIVE;
    }
}

void liveAndDead(int CurrentGame[LARGEUR/10][HAUTEUR/10], int i, int j, int* alive, int* dead){
    *alive = 0;
    *dead = 0;

    //No position error        
    if( (CurrentGame[i-1][j-1] == ALIVE || CurrentGame[i-1][j-1] == DEAD) && (CurrentGame[i+1][j+1] == ALIVE || CurrentGame[i+1][j+1] == DEAD) ){
        
        
        for(int i2 = i-1; i2< i+2; i2++){
            for(int j2= j-1; j2 < j+2; j2++){
                if(j2 != j || i2 != i){
                    if (i2 >= 0 && i2 < LARGEUR / 10 && j2 >= 0 && j2 < HAUTEUR / 10) {    
                        if(CurrentGame[i2][j2] == ALIVE) (*alive)++;
                        if(CurrentGame[i2][j2] == DEAD) (*dead)++;
                    }
                
                }
            }
        }
    }
}


void rules(int CurrentGame[LARGEUR/10][HAUTEUR/10], int PostGame[LARGEUR/10][HAUTEUR/10]){
    int alive = 0; 
    int dead = 0;

    for(int i=0; i<LARGEUR/10; i++){
        for(int j=0; j<HAUTEUR/10; j++){

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


void spawnSpaceShip(int CurrentGame[LARGEUR/10][HAUTEUR/10], int i, int j){
    
    CurrentGame[i+1][j] = ALIVE;
    CurrentGame[i+2][j+1] = ALIVE;
    CurrentGame[i][j+2] = ALIVE;
    CurrentGame[i+1][j+2] = ALIVE;
    CurrentGame[i+2][j+2] = ALIVE;

}

void spawnRPentomino(int CurrentGame[LARGEUR/10][HAUTEUR/10], int i, int j){
    CurrentGame[i][j+1] = ALIVE;
    CurrentGame[i][j+2] = ALIVE;
    CurrentGame[i+1][j] = ALIVE;
    CurrentGame[i+1][j+1] = ALIVE;
    CurrentGame[i+2][j+1] = ALIVE;
}



int main(int argc, char **argv){

    int speed;
    srand( time(NULL) );

    int type =0;
    int nb=0;

    if(argc > 1){
        for(int i=0; i<argc; i++){

            if(strcmp(argv[i], "-speed")==0){

                printf("%s \n", argv[i+1]);
                if(strcmp(argv[i+1], "slow")==0)   speed=30;
                if(strcmp(argv[i+1], "normal")==0) speed=20;
                if(strcmp(argv[i+1], "fast")==0)   speed=10;
            }

            if(strcmp(argv[i], "-randomGrid")==0){
                type=1;
                
                if(i+1<argc){
                    nb = atoi(argv[i+1]);
                }else{
                    printf("Usage: %s ... -randomGrid <nb> \n", argv[0]);
                    return 1;
                }

                
            }

            if(strcmp(argv[i], "-pentomino") == 0){
                type=2;

                if(i+1<argc){
                    nb = atoi(argv[i+1]);
                }else{
                    printf("Usage: %s ... -randomGrid <nb> \n", argv[0]);
                    return 1;
                }
            }
        }
    }


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
   

    screen = SDL_CreateWindow(
        "Game of Life",
        0,
        0,
        LARGEUR, 
        HAUTEUR,
        SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);


    int running = 1;
    SDL_Event e;

    //Game initialisation
    int nbCell = nbCells();

    int CurrentGame[LARGEUR/10][HAUTEUR/10];
    int PostGame[LARGEUR/10][HAUTEUR/10];
    int timing = 0;



    for(int i=0; i<LARGEUR/10; i++){
        for(int j=0; j<HAUTEUR/10; j++){
            CurrentGame[i][j] = DEAD;
        }
    }
    
    

    //Fill the screen white
    CINI_fill_rect_rgb(0,0,HAUTEUR,LARGEUR, 255, 255, 255);




    if(type==1){
        initialseGame(CurrentGame, nb);
    }

    if(type==2){
        for(int i=0; i<nb;i++){
            spawnRPentomino(CurrentGame, random() % (LARGEUR/10), random() % (HAUTEUR/10));
        }
        
    }


    drawGrid(CurrentGame);

    while(running){
        while (SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                running = 0;
            }
        }

        if(timing == speed){

            rules(CurrentGame, PostGame);
            
            memcpy(CurrentGame, PostGame, sizeof(PostGame));

            drawGrid(CurrentGame);

            timing=0;
        }

        //usleep( 500000 );
        //200 for normal speed
        SDL_Delay(10);
        timing++;
    }

    


    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);


    // Quit SDL subsystems
    SDL_Quit();

    return 0;

}