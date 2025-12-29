#include <stdio.h>
#include <malloc.h>

#include "general/debug.h"

#include "generateGraphic.h"
#include "physicalModel.h"
#include "DSL.h"

const size_t AMOUNT_BALLS = 201;

int main(){
    physicalSystem_t physSystem;
    physicalSystemCtor(&physSystem);

    ball_t ball;
    ballCtor(&ball, 5, 0, -0.06, 0.01, 0, 0, 0);

    centralForceField_t field;
    centralForceFieldCtor(&field, 1, 0.05, 0.10, 0);

    int time = 1;

    size_t expectedPointsAmount = (size_t) (time / _TIME_STEP((&physSystem)));
    LPRINTF("expectedPointsAmount = %lu", expectedPointsAmount);

    graphicData_t* graphicsData;

    graphicsDataArrayCtor(&graphicsData, AMOUNT_BALLS, expectedPointsAmount);        

    size_t amountPoints = 0;
    for(size_t curBall = 0; curBall < AMOUNT_BALLS; curBall++){
        ballCtor(&ball, 5, 0, -0.05 + 0.0005 * (double) curBall, 0.42, 0, 0, 0);
        amountPoints = calculateModel(&physSystem, &ball, &field, time, graphicsData[curBall].xData, graphicsData[curBall].yData);  
    }

    size_t frameRate = (size_t) (1 / _TIME_STEP((&physSystem)));
    pythonPointsGenAnimation(graphicsData, AMOUNT_BALLS, amountPoints, frameRate, 
                            _CENTER_X((&field)), _CENTER_Y((&field)), _RADIUS((&field)));

    graphicsDataArrayDtor(&graphicsData, AMOUNT_BALLS, expectedPointsAmount);
}