#include "physicalModel.h"
#include "DSL.h"

#include "general/debug.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>

const double EARTH_GRAVITY     = 9.81;
const double TIME_STEP_DEFAULT = 0.01;

ball_t* ballCtor(ball_t* ball, 
                double weight, 
                double x, double y, 
                double vx, double vy){
    _WEIGHT(ball) = weight;

    _X(ball)  = x;
    _Y(ball)  = y;

    _VX(ball) = vx;
    _VY(ball) = vy;

    return ball;
}

physicalSystem_t* physicalSystemCtor(physicalSystem_t* physSystem){
    _GRAVITY(physSystem)   = EARTH_GRAVITY;
    _TIME_STEP(physSystem) = TIME_STEP_DEFAULT;
    
    return physSystem;
}

size_t calculateModel(physicalSystem_t* physSystem, ball_t* ball, double time, 
                    double* xData, double* yData){
    assert(physSystem);
    assert(ball);

    double curTime = 0;

    size_t amountPoints = 0;
    while(curTime < time){
        _VY(ball) -= _GRAVITY(physSystem) * TIME_STEP_DEFAULT;

        _Y(ball) += _VY(ball) * TIME_STEP_DEFAULT;

        _X(ball) += _VX(ball) * TIME_STEP_DEFAULT;

        if (_Y(ball) <= 0) {
            _Y(ball)  = 0;
            _VY(ball) = 0;
            printf("тело коснулось земли\n");
            break;
        }

        xData[amountPoints] = _X(ball);
        yData[amountPoints] = _Y(ball);
        printf("Time: %.2fs | Height: %.2fm | Range: %.2f\n", curTime, _Y(ball), _X(ball));

        curTime += TIME_STEP_DEFAULT;
        amountPoints++;
    }
    amountPoints++;

    LPRINTF("amountPoints = %d", amountPoints);

    return amountPoints;
}