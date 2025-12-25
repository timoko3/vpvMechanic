#include <stdio.h>
#include <malloc.h>

#include "generateGraphic.h"
#include "physicalModel.h"
#include "DSL.h"

int main(){
    physicalSystem_t physSystem;
    physicalSystemCtor(&physSystem);

    ball_t ball;
    ballCtor(&ball, 5, 0, 0, 0.5, 40);

    int time = 10;

    size_t expectedPointsAmount = (size_t) (time / _TIME_STEP((&physSystem)));
    double* xData = (double*) calloc(expectedPointsAmount, sizeof(double));
    double* yData = (double*) calloc(expectedPointsAmount, sizeof(double));

    size_t amountPoints = calculateModel(&physSystem, &ball, 10, xData, yData);    

    size_t frameRate = (size_t) (1 / _TIME_STEP((&physSystem)));
    pythonPointsGenAnimation(xData, yData, amountPoints, frameRate);

    free(xData);
    free(yData);
}