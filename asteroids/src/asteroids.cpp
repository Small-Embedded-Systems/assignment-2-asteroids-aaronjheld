/* Asteroids
    W14018240 - Aaron Held
    */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time = 0.0f; 
int   score = 0;
int   lives = 5;
struct ship player;
struct missile *shots = NULL;
struct rock *asteroids = NULL;
void pauseToStart();
void initialiseGame();
bool inPlay;

float Dt = 0.01f;

Ticker model, view, controller;

bool paused = false;
/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P5_3,PullUp);
int main()
{

    init_DBuffer();
    

    view.attach( draw, 0.055 );
    model.attach( physics, Dt );
    controller.attach( controls, 0.1 );
    
    	
    while(true) {			
			initialiseGame();
			while (lives > 0){
				pauseToStart();
				inPlay = true;
					while(inPlay){
						
					}
			}			
			gameover();
    }
}

void initialiseGame()



{
  player.p.x = 200;
	player.p.y = 136;
	player.v.x = 0; 
	player.v.y = 0; 
	player.heading = 0.0;
	player.shield = 3;
	initialise_missiles();
	initialise_asteroids();
}

void pauseToStart()

{
    while( userbutton.read() )
    
    {
        paused=true;
        wait_ms(100);
    }
    paused = false;	
}

