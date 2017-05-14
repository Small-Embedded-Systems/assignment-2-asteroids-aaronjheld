/* utils methods */

float norm(float value, float min, float max);
float lerp(float min, float max, float value);
float map(float value, float lower, float upper, float min, float max);


typedef uint16_t colour_t;

#define rgb(r,g,b)
float dist(coordinate_t p1, coordinate_t p2);
extern const float pi;
float radians(float degrees);

int randrange(int from, int to);



