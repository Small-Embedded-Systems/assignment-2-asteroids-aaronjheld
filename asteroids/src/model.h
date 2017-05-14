/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

typedef struct rock {
    float heading;
    coordinate_t p;
    vector_t     v;
    float ttl;
    struct rock *next;
} rock_t;

struct ship {
	  float heading;
    coordinate_t p;
    vector_t     v;
		int		 shield;
	  int shipScore;
};




typedef struct missile {
	  float heading;
    coordinate_t p;
	  vector_t     v;
	  float ttl;
    struct missile *next;
} missile_t;

void physics(void);

void shipBack();
void initialise_missiles();
void initialise_asteroids();

void free_missile_node(missile_t *n);
void fire();
void left();
void light();
void fwd();

rock_t *allocate_rock_node(void);
missile_t *allocate_missile_node(void);
void free_rock_node(rock_t *n);
