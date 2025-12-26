#ifndef GENERATE_GRAPHIC_H
#define GENERATE_GRAPHIC_H

#include <stddef.h>

struct graphicData_t{
    double* xData;
    double* yData;
};

graphicData_t* graphicsDataArrayCtor(graphicData_t** graphicsData, size_t amountGraphics, size_t expectedPointsAmount);
graphicData_t* graphicsDataArrayDtor(graphicData_t** graphicsData, size_t amountGraphics, size_t expectedPointsAmount);

void pythonFuncGenAnimation();

void pythonPointsGenAnimation(graphicData_t* graphicsData, size_t amountGraphics,
                            size_t amountPoints, size_t frameRate,
                            double centerX, double centerY, double radius);

#endif /* GENERATE_GRAPHIC_H */

