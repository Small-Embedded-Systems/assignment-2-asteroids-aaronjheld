/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "view.h"

extern "C" uint8_t flip(uint8_t);

Display *graphics = Display::theDisplay();

static void drawShip(struct ship);
static void transformShape();
static void drawMissiles(struct missile *lst);
static void drawRocks(struct rock *lst);

uint8_t flip(uint8_t);
static void rock_flip();
static unsigned char flippedRock[200];

float currentHeadingForAllMissiles;


static const colour_t background = rgb(0,51,102); /* Midnight Blue */
static const colour_t missileColour = rgb(255,51,51); /* Red */
static const colour_t shipColour = rgb(0,255,0); /* Green */
static const colour_t asteroidColour = rgb(64,64,64); /* Grey */


static const coordinate_t shape[] = {
    {0.0f,-10.0f}, {5.0f,5.0f}, {-5.0f,5.0f}
};
static coordinate_t transformedShip[3];



/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}




void draw(void)
{
    graphics->fillScreen(background);
	

		drawMissiles(shots);
		drawRocks(asteroids);
		transformShape();
		drawShip(player);
    drawScore(lives, score, elapsed_time);
	
		if( paused ){
			graphics->setCursor(182,150);
			graphics->printf("paused"); 
		}  
    swap_DBuffer();
}

void drawRocks(struct rock *lst){
    rock_flip();
    while(lst){
        graphics->drawBitmap(lst->p.x-20, lst->p.y-20, flippedRock, asteroid_width, asteroid_height, asteroidColour);
        lst = lst->next;
    }	
}
void drawMissiles(struct missile *lst){
    while(lst){
        graphics->fillCircle(lst->p.x, lst->p.y, 1, missileColour);
        lst = lst->next;
    }
}
void drawShip(struct ship){
	switch (player.shield){
		case 3:
			graphics->drawCircle(player.p.x, player.p.y, 10, GREEN);
			break;
		case 2:
			graphics->drawCircle(player.p.x, player.p.y, 10, YELLOW);
			break;		
		case 1:
			graphics->drawCircle(player.p.x, player.p.y, 10, RED);
			break;		
		case 0:
			//do nothing
			break;	}
	graphics->drawLine(player.p.x+transformedShip[0].x, player.p.y+transformedShip[0].y, player.p.x+transformedShip[1].x, player.p.y+transformedShip[1].y, shipColour);
	graphics->drawLine(player.p.x+transformedShip[1].x, player.p.y+transformedShip[1].y, player.p.x+transformedShip[2].x, player.p.y+transformedShip[2].y, shipColour);
	graphics->drawLine(player.p.x+transformedShip[2].x, player.p.y+transformedShip[2].y, player.p.x+transformedShip[0].x, player.p.y+transformedShip[0].y, shipColour);
}




void rock_flip(){
    int i;
    for (i=0; i < 200; i++){
        flippedRock[i] = flip(asteroid_bits[i]);
    }
}
void transformShape(){
	int i;
	for(i = 0; i < 3 ; i++){
		transformedShip[i].x = (shape[i].x*cos(radians(player.heading))) - (shape[i].y*sin(radians(player.heading)));	
		transformedShip[i].y = (shape[i].x*sin(radians(player.heading))) + (shape[i].y*cos(radians(player.heading)));	
	}
}



void gameover(){
	graphics->setCursor(182,150);
	graphics->printf("GAME OVER - FINAL SCORE: %d", score); 
}
