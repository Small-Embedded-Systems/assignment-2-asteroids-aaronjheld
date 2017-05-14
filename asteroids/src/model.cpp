/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"
#include "view.h"

static const float dragFactor = 0.02;
static const float accelerationFactor = 1.0;
static const size_t MAXSize = 10;
static rock_t rock_data[MAXSize];
static missile_t missile_data[MAXSize];

void new_missile(struct missile *r);
void new_asteroid(struct rock *r);
static missile_t *freeMissileNodes;
static rock_t *freeRockNodes;



static void move_ship(ship, float x);
struct rock * new_asteroids();
static void missiles_hit_rocks(struct missile *l, struct rock *k);
static bool ship_hit_rocks(struct rock *l);
static void update_missile_list(struct missile *l);
static void update_rock_list(struct rock *l);
static void move_missiles(struct missile *l);
static void move_asteroids(struct rock *l, float x);

void left(){
    player.heading -= 10.0f;
}
void right(){
    player.heading += 10.0f;
}
void fwd(){
    
    float xN = sin(radians(player.heading));
    float yN = cos(radians(player.heading));
    
    
    player.v.x += xN * accelerationFactor;
    player.v.y -= yN * accelerationFactor;
}

void drawScore(lives, score, elapsed_time)
{
    
    graphics->setCursor(2,12);
    grapics->printf("Lives: " + lives + "Score :" + score + "Time :" + elapsed_time);
}


void physics(void)
{
    if (!paused){
			elapsed_time += Dt;
			move_ship(player, Dt);
			move_missiles(shots);
			asteroids = new_asteroids();
			move_asteroids(asteroids, Dt);
			missiles_hit_rocks(shots, asteroids);
			if(ship_hit_rocks(asteroids)){
				if (player.shield <= 0){
					inPlay = false;
					score += (int) elapsed_time;
					lives--;
				}
				else {
					player.shield--;
				}
			}
			update_missile_list(shots);
			update_rock_list(asteroids);
		}
}


/* Movement of ship */


void shipBack(){
	
	float xN = sin(radians(player.heading));
    
    
	float yN = cos(radians(player.heading));

	
	player.v.x -= xN * accelerationFactor;
	player.v.y += yN * accelerationFactor;
}

void move_ship(ship, float x){
	
	player.p.x += player.v.x;
	player.p.y += player.v.y;
	
	
	player.v.x = player.v.x - (player.v.x * dragFactor);
	player.v.y = player.v.y - (player.v.y * dragFactor);
	
	
	if(player.p.x < 0) 		player.p.x += 400;
	if(player.p.x >= 400) player.p.x -= 400;
	if(player.p.y < 0) 		player.p.y += 272;
	if(player.p.y >= 272) player.p.y -= 272;
}


/* Stuff for missiles */

void fire(){
	struct missile *newMissile = allocate_missile_node();
	if(newMissile){
		newMissile->next = shots;
		shots = newMissile;
		new_missile(newMissile);
	}
	update_missile_list(shots);
}

/* Make a new missile*/
void new_missile(struct missile *r){
	r->heading = player.heading;
	
	r->p.x = player.p.x;
	r->p.y = player.p.y;
	
	r->v.x = sin(radians(r->heading));
	r->v.y = -cos(radians(r->heading));
	
	r->p.x += r->v.x*20;
	r->p.y += r->v.y*20;
	
	r->v.x *=50;
	r->v.y *=50;
	
	r->ttl = 20;

}

void initialise_missiles(){
    int n;
    for(n=0; n<(MAXSize-1) ; n++){
        missile_data[n].next = &missile_data[n+1];
    }
    missile_data[n].next = NULL;
    freeMissileNodes = &missile_data[0];
}

missile_t *allocate_missile_node(void){
    missile_t *node = NULL;
    if (freeMissileNodes){
        node = freeMissileNodes;
        freeMissileNodes = freeMissileNodes->next;
    }
    return node;
}

void free_missile_node(missile_t *n){
    n->next = freeMissileNodes;
    freeMissileNodes = n;
}

/* Physics for asteroids */
struct rock * new_asteroids(){
    int i;
    for(i = 0; i < MAXSize-1; i++){
        struct rock *newRock = allocate_rock_node();
        if(newRock){
            newRock->next = asteroids;
            asteroids = newRock;
            
            
            int initAstPos = randrange(1,5);
            switch (initAstPos){
                case 1 :
                
                newRock->p.x = randrange(0,100);
                newRock->p.y = randrange(0,272);
                newRock->v.x = 1;
                if(newRock->p.y > 136) newRock->v.y = -10;
                if(newRock->p.y <= 136) newRock->v.y = 10;
                break;
                case 2 :
                
                newRock->p.x = randrange(280,380);
                newRock->p.y = randrange(0,272);
                newRock->v.x = -1;
                if(newRock->p.y > 136) newRock->v.y = -10;
                if(newRock->p.y <= 136) newRock->v.y = 10;
                break;
                case 3 :
                
                newRock->p.x = randrange(0,380);
                newRock->p.y = randrange(0,100);
                newRock->v.y = 1;
                if(newRock->p.x > 200) newRock->v.x = -10;
                if(newRock->p.y <= 200) newRock->v.x = 10;
                break;	
                case 4 :
                
                newRock->p.x = randrange(0,380);
                newRock->p.y = randrange(172,272);
                newRock->v.y = 1;
                if(newRock->p.x > 200) newRock->v.x = -10;
                if(newRock->p.y <= 200) newRock->v.x = 10;
                break;				
            }
            newRock->ttl = 2000;
        }
        update_rock_list(asteroids);
    }
    return asteroids;
}



bool ship_hit_rocks(struct rock *l){
    for( ; l ; l =l->next){
        float r1 = 7.9;
        float r2 = 20;
        float dx = l->p.x - player.p.x; float dy = l->p.y - player.p.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance < (r1 + r2)){
            
            l->ttl = 0;
            return true;
        }
    }
    return false;
}

void update_missile_list(struct missile *l){
	for ( ; l ; l = l->next){		
		if (l->next->ttl <= 0){
			struct missile *expiredM = l->next;
			l->next = l->next->next;
			free_missile_node(expiredM);
		}		
	}
}

void move_missiles(struct missile *l){
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * Dt;
		l->p.y += l->v.y * Dt;
		
		
		if (l->p.y >= 272) 	l->ttl = 0;
		if (l->p.y < 0) 		l->ttl = 0;
		if (l->p.x >= 400) 	l->ttl = 0;
		if (l->p.x < 0) 		l->ttl = 0;
		
		l->ttl -= Dt;	
	}	
}



void initialise_asteroids(){
	int n;
	for(n=0; n<(MAXSize-1) ; n++){
		rock_data[n].next = &rock_data[n+1];
	}
	rock_data[n].next = NULL;
	freeRockNodes = &rock_data[0];	
}

rock_t *allocate_rock_node(void){
	rock_t *node = NULL;
	if (freeRockNodes){
		node = freeRockNodes;
		freeRockNodes = freeRockNodes->next;
	}
	return node;
}

void update_rock_list(struct rock *l){
	for ( ; l ; l = l->next){
		if (l->next->ttl <= 0){
			struct rock *expiredR = l->next;
			l->next = l->next->next;
			free_rock_node(expiredR);
		}		
	}
}

void free_rock_node(rock_t *n){
	n->next = freeRockNodes;
	freeRockNodes = n;
}
	
void move_asteroids(struct rock *l, float x){
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * x;
		l->p.y += l->v.y * x;
		
		
		if (l->p.y >= 272) 	l->p.y -= 272;
		if (l->p.y < 0) 		l->p.y += 272;
		if (l->p.x >= 380) 	l->p.x -= 380;
		if (l->p.x < 0) 		l->p.x += 380;
		
		l->ttl -= Dt;	
	}	
}


/* Detects collisions */

void missiles_hit_rocks(struct missile *l, struct rock *k){
	for ( ; l ; l = l->next){
		for( ; k ; k =k->next){
			float r1 = 1;
			float r2 = 20;
			float dx = l->p.x - k->p.x; float dy = l->p.y - k->p.y;
			float distance = sqrt(dx*dx + dy*dy);
			if (distance < (r1 + r2)){
				
					l->ttl = 0;
					k->ttl = 0;
					score += 5; /
			}
		}
	}
}
	

	



