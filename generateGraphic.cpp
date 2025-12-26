#include "generateGraphic.h"

#include "general/file.h"

#include <assert.h>
#include <stdlib.h>

const char* PY_GEN_GRAPHIC_FILE_NAME  = "testGraphic.py";

static FILE* pythonGenGraphicPreamble();
static void initPointsData(FILE* pyFilePtr, double* data, size_t amountPoints);

void pythonFuncGenAnimation(){
    FILE* pyFilePtr = pythonGenGraphicPreamble();

    fprintf(pyFilePtr, "def y(x, h):\n\treturn h*x**2\n\n"); 

    fprintf(pyFilePtr, "x_vals_1 = np.linspace(-10, 10, 1000)\n");
    fprintf(pyFilePtr, "y_vals_1 = y(x_vals_1, 2)\n\n");
    
    fprintf(pyFilePtr, "fig, ax = plt.subplots(figsize=(10, 6))\n");
    fprintf(pyFilePtr, "ax.set_xlim(-10, 10)\n");
    fprintf(pyFilePtr, "ax.set_ylim(0, 100)\n");
    fprintf(pyFilePtr, "ax.set_xlabel(\"x\")\n");
    fprintf(pyFilePtr, "ax.set_ylabel(\"y\")\n");
    fprintf(pyFilePtr, "ax.grid(True)\n\n");
    
    fprintf(pyFilePtr, "#Создание фигур\n");
    fprintf(pyFilePtr, "line1,  = ax.plot([], [], linewidth=2)\n");
    fprintf(pyFilePtr, "point1, = ax.plot([], [], 'ro', markersize=8)\n\n");

    fprintf(pyFilePtr, "#Инициализация\n");
    fprintf(pyFilePtr, "def init():\n"); 
    fprintf(pyFilePtr, "\tline1.set_data([], [])\n"); 
    fprintf(pyFilePtr, "\tpoint1.set_data([], [])\n\n"); 
    fprintf(pyFilePtr, "\treturn line1, point1\n\n"); 

    fprintf(pyFilePtr, "#Обновление кадра\n");
    fprintf(pyFilePtr, "def update(frame):\n"); 
    fprintf(pyFilePtr, "\tline1.set_data(x_vals_1[:frame], y_vals_1[:frame])\n"); 
    fprintf(pyFilePtr, "\tpoint1.set_data([x_vals_1[frame]], [y_vals_1[frame]])\n"); 
    fprintf(pyFilePtr, "\treturn line1, point1\n\n"); 

    fprintf(pyFilePtr, "#Создание анимации\n");
    fprintf(pyFilePtr, "ani = FuncAnimation(\n");
    fprintf(pyFilePtr, "\tfig,\n");
    fprintf(pyFilePtr, "\tupdate,\n");
    fprintf(pyFilePtr, "\tframes=len(x_vals_1),\n");
    fprintf(pyFilePtr, "\tinit_func=init,\n");
    fprintf(pyFilePtr, "\tinterval=1,\n");
    fprintf(pyFilePtr, "\tblit=True\n");
    fprintf(pyFilePtr, ")\n\n");

    fprintf(pyFilePtr, "#Сохранение анимации\n");
    fprintf(pyFilePtr, "ani.save(\"parabola.mp4\", writer=\"ffmpeg\", fps=30)\n");

    fclose(pyFilePtr);

    system("python testGraphic.py");
}

void pythonPointsGenAnimation(double* x, double* y, size_t amountPoints, size_t frameRate){
    FILE* pyFilePtr = pythonGenGraphicPreamble();

    fprintf(pyFilePtr, "x_vals_1 = ");
    initPointsData(pyFilePtr, x, amountPoints);

    fprintf(pyFilePtr, "y_vals_1 = ");
    initPointsData(pyFilePtr, y, amountPoints);

    fprintf(pyFilePtr, "fig, ax = plt.subplots(figsize=(10, 6))\n");
    fprintf(pyFilePtr, "ax.set_xlim(min(x_vals_1), max(x_vals_1))\n");
    fprintf(pyFilePtr, "ax.set_ylim(min(y_vals_1), max(y_vals_1))\n");
    fprintf(pyFilePtr, "ax.set_xlabel(\"x\")\n");
    fprintf(pyFilePtr, "ax.set_ylabel(\"y\")\n");
    fprintf(pyFilePtr, "ax.grid(True)\n\n");
    
    fprintf(pyFilePtr, "#Создание фигур\n");
    fprintf(pyFilePtr, "line1,  = ax.plot([], [], linewidth=2)\n");
    fprintf(pyFilePtr, "point1, = ax.plot([], [], 'ro', markersize=8)\n\n");

    fprintf(pyFilePtr, "#Инициализация\n");
    fprintf(pyFilePtr, "def init():\n"); 
    fprintf(pyFilePtr, "\tline1.set_data([], [])\n"); 
    fprintf(pyFilePtr, "\tpoint1.set_data([], [])\n\n"); 
    fprintf(pyFilePtr, "\treturn line1, point1\n\n"); 

    fprintf(pyFilePtr, "#Обновление кадра\n");
    fprintf(pyFilePtr, "def update(frame):\n"); 
    fprintf(pyFilePtr, "\tline1.set_data(x_vals_1[:frame+1], y_vals_1[:frame+1])\n"); 
    fprintf(pyFilePtr, "\tpoint1.set_data([x_vals_1[frame]], [y_vals_1[frame]])\n"); 
    fprintf(pyFilePtr, "\treturn line1, point1\n\n"); 

    fprintf(pyFilePtr, "#Создание анимации\n");
    fprintf(pyFilePtr, "ani = FuncAnimation(\n");
    fprintf(pyFilePtr, "\tfig,\n");
    fprintf(pyFilePtr, "\tupdate,\n");
    fprintf(pyFilePtr, "\tframes=len(x_vals_1),\n");
    fprintf(pyFilePtr, "\tinit_func=init,\n");
    fprintf(pyFilePtr, "\tinterval=10,\n");
    fprintf(pyFilePtr, "\tblit=True\n");
    fprintf(pyFilePtr, ")\n\n");

    fprintf(pyFilePtr, "#Сохранение анимации\n");
    fprintf(pyFilePtr, "ani.save(\"parabola.mp4\", writer=\"ffmpeg\", fps=%lu)\n", frameRate);

    fclose(pyFilePtr);

    system("python testGraphic.py");
}

static void initPointsData(FILE* pyFilePtr, double* data, size_t amountPoints){
    assert(pyFilePtr);
    assert(data);

    fprintf(pyFilePtr, "np.array([");
    
    size_t curPoint = 0;
    for(; curPoint < amountPoints - 1; curPoint++){
        fprintf(pyFilePtr, "%.4f, ", data[curPoint]);
    }

    fprintf(pyFilePtr, "%.4f])\n", data[curPoint]);
}   

static FILE* pythonGenGraphicPreamble(){
    fileDescription pyGenGraphic{
        PY_GEN_GRAPHIC_FILE_NAME,
        "wb"
    };

    FILE* pyFilePtr = myOpenFile(&pyGenGraphic);
    assert(pyFilePtr);

    fprintf(pyFilePtr, "import numpy as np\n");

    fprintf(pyFilePtr, "import matplotlib.pyplot as plt\n");
    fprintf(pyFilePtr, "from matplotlib.animation import FuncAnimation\n\n");

    return pyFilePtr;
}