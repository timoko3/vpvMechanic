#include <stdio.h>
#include <malloc.h>

#include "general/debug.h"

#include "generateGraphic.h"
#include "physicalModel.h"
#include "DSL.h"

int main(){
    physicalSystem_t physSystem;
    physicalSystemCtor(&physSystem);

    ball_t ball;
    ballCtor(&ball, 5, 0, 0.01, 0.3, 0, 0, 0);

    centralForceField_t field;
    centralForceFieldCtor(&field, 1, 0.05, 0.17, 0);

    int time = 1;

    size_t expectedPointsAmount = (size_t) (time / _TIME_STEP((&physSystem)));
    LPRINTF("expectedPointsAmount = %lu", expectedPointsAmount);

    double* xData = (double*) calloc(expectedPointsAmount + 1, sizeof(double));
    double* yData = (double*) calloc(expectedPointsAmount + 1, sizeof(double));

    size_t amountPoints = calculateModel(&physSystem, &ball, &field, time, xData, yData);    

    size_t frameRate = (size_t) (1 / _TIME_STEP((&physSystem)));
    pythonPointsGenAnimation(xData, yData, amountPoints, frameRate);

    free(xData);
    free(yData);
}