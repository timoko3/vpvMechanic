#define _WEIGHT(object) object->weight
#define _X(object)      object->pos.x
#define _Y(object)      object->pos.y
#define _VX(object)     object->velocity.vx
#define _VY(object)     object->velocity.vy
#define _AX(object)     object->acceleration.ax
#define _AY(object)     object->acceleration.ay

#define _GRAVITY(system)   system->gravityAcceleration
#define _TIME_STEP(system) system->dt

#define _FORCE_COEFF(field) field->forceCoeff
#define _RADIUS(field) field->radius
#define _CENTER_X(field) field->centerPos.x
#define _CENTER_Y(field) field->centerPos.y

#define _DISTANCE(object, field) countDistance(_X(object), _Y(object), _CENTER_X(field), _CENTER_Y(field))

#define _VECTOR_ANGLE(x1, y1, x2, y2) countVectorAngle(x1, y1, x2, y2)