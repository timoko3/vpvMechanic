#ifndef PHYSICAL_MODEL_H
#define PHYSICAL_MODEL_H

#include <stddef.h>

struct position_t{
    double x;
    double y;
};

struct velocity_t{
    double vx;
    double vy;
};

struct ball_t{
    double weight;
    position_t pos;
    velocity_t velocity;
};

// struct centralForceField{
//     position_t centerPos;
//     double radius;
// };

struct physicalSystem_t{
    double gravityAcceleration;
    double dt;
};

ball_t* ballCtor(ball_t* ball, 
                double weight, 
                double x, double y, 
                double vx, double vy);
physicalSystem_t* physicalSystemCtor(physicalSystem_t* physSystem);

size_t calculateModel(physicalSystem_t* physSystem, ball_t* ball, double time, double* x, double* y);

#endif /* PHYSICAL_MODEL_H */