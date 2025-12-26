#include "physicalModel.h"
#include "DSL.h"

#include "general/debug.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>

const double EARTH_GRAVITY     = 9.81;
const double TIME_STEP_DEFAULT = 0.01;
const double CURVE_RADIUS      = 0.15;

static double countVectorAngle(double x1, double y1, 
                    double x2, double y2);
static double countDistance(double x1, double y1, 
                    double x2, double y2);

ball_t* ballCtor(ball_t* ball, 
                double weight, 
                double x, double y, 
                double vx, double vy,
                double ax, double ay){
    _WEIGHT(ball) = weight;

    _X(ball)  = x;
    _Y(ball)  = y;

    _VX(ball) = vx;
    _VY(ball) = vy;

    _AX(ball) = ax;
    _AY(ball) = ay;

    return ball;
}

physicalSystem_t* physicalSystemCtor(physicalSystem_t* physSystem){
    _GRAVITY(physSystem)   = EARTH_GRAVITY;
    _TIME_STEP(physSystem) = TIME_STEP_DEFAULT;
    
    return physSystem;
}

centralForceField_t* centralForceFieldCtor(centralForceField_t* centralForceField,
                                        double forceCoeff,
                                        double radius,
                                        double centerX, double centerY){
    _FORCE_COEFF(centralForceField) = forceCoeff;
    _RADIUS(centralForceField)      = radius;
    _CENTER_X(centralForceField)    = centerX;
    _CENTER_Y(centralForceField)    = centerY;

    return centralForceField;
}

size_t calculateModel(physicalSystem_t* physSystem, ball_t* ball, centralForceField_t* centralForceField,
                    double time, 
                    double* xData, double* yData){
    assert(physSystem);
    assert(ball);
    assert(centralForceField);

    double curTime = 0;

    size_t amountPoints = 0;
    while(curTime < time){

        if(_RADIUS(centralForceField) > _DISTANCE(ball, centralForceField)){
            LPRINTF("Попал в зону действия поля");

            _AX(ball) = _GRAVITY(physSystem) * (_DISTANCE(ball, centralForceField) / CURVE_RADIUS) * cos(_VECTOR_ANGLE(_X(ball), _Y(ball), _CENTER_X(centralForceField), _CENTER_Y(centralForceField)));
            _AY(ball) = _GRAVITY(physSystem) * (_DISTANCE(ball, centralForceField) / CURVE_RADIUS) * sin(_VECTOR_ANGLE(_X(ball), _Y(ball), _CENTER_X(centralForceField), _CENTER_Y(centralForceField)));

            _VX(ball) += _AX(ball) * _TIME_STEP(physSystem);
            _VY(ball) += _AY(ball) * _TIME_STEP(physSystem);
        }
        _AX(ball) = 0;
        _AY(ball) = 0;

        _X(ball) += _VX(ball) * _TIME_STEP(physSystem);
        _Y(ball) += _VY(ball) * _TIME_STEP(physSystem);

        xData[amountPoints] = _X(ball);
        yData[amountPoints] = _Y(ball);
        LPRINTF("Time: %.3fs | Y: %.3fm | X: %.3f | VY: %.3f | VX: %.3f | AY: %.3f | AX: %.3f | distance = %.3f\n", curTime, _Y(ball), _X(ball), _VY(ball), _VX(ball), _AY(ball), _AX(ball), _DISTANCE(ball, centralForceField));

        curTime += TIME_STEP_DEFAULT;
        amountPoints++;
    }

    LPRINTF("amountPoints = %d", amountPoints);

    return amountPoints;
}

static double countDistance(double x1, double y1, 
                    double x2, double y2){
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

static double countVectorAngle(double x1, double y1, 
                    double x2, double y2){
    return atan2(y2 - y1, x2 - x1);
}