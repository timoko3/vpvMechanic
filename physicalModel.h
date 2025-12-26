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

struct acceleration_t{
    double ax;
    double ay;
};

struct ball_t{
    double weight;
    position_t pos;
    velocity_t velocity;
    acceleration_t acceleration;
};

struct centralForceField_t{
    position_t centerPos;
    double     radius;
    double     forceCoeff;
};

struct physicalSystem_t{
    double gravityAcceleration;
    double dt;
};

ball_t* ballCtor(ball_t* ball, 
                double weight, 
                double x, double y, 
                double vx, double vy,
                double ax, double ay);
physicalSystem_t* physicalSystemCtor(physicalSystem_t* physSystem);
centralForceField_t* centralForceFieldCtor(centralForceField_t* centralForceField,
                                        double forceCoeff,
                                        double radius,
                                        double centerX, double centerY);

size_t calculateModel(physicalSystem_t* physSystem, ball_t* ball, centralForceField_t* centralForceField,
                    double time, 
                    double* xData, double* yData);

#endif /* PHYSICAL_MODEL_H */