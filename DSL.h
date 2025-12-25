#define _WEIGHT(object) object->weight
#define _X(object)      object->pos.x
#define _Y(object)      object->pos.y
#define _VX(object)     object->velocity.vx
#define _VY(object)     object->velocity.vy

#define _GRAVITY(system)   system->gravityAcceleration
#define _TIME_STEP(system) system->dt