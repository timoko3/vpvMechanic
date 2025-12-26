#ifndef GENERATE_GRAPHIC_H
#define GENERATE_GRAPHIC_H

#include <stddef.h>

void pythonFuncGenAnimation();

void pythonPointsGenAnimation(double* x, double* y, 
                            size_t amountPoints, size_t frameRate,
                            double centerX, double centerY, double radius);

#endif /* GENERATE_GRAPHIC_H */

