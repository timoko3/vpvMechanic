#include "generateGraphic.h"

#include "general/file.h"

#include <assert.h>
#include <stdlib.h>
#include <malloc.h>

const char* PY_GEN_GRAPHIC_FILE_NAME  = "testGraphic.py";

static FILE* pythonGenGraphicPreamble();
static void initPointsData(FILE* pyFilePtr, double* data, size_t amountPoints);
static void createFieldCircle(FILE* pyFilePtr, 
                            double centerX, double centerY, double radius);

graphicData_t* graphicsDataArrayCtor(graphicData_t** graphicsData, size_t amountGraphics, size_t expectedPointsAmount){
    *graphicsData = (graphicData_t*) calloc(amountGraphics, sizeof(graphicData_t));
    assert(graphicsData);

    for(size_t curGraphicData = 0; curGraphicData < amountGraphics; curGraphicData++){
        (*graphicsData)[curGraphicData].xData = (double*) calloc(expectedPointsAmount + 1, sizeof(double));
        (*graphicsData)[curGraphicData].yData = (double*) calloc(expectedPointsAmount + 1, sizeof(double));
    }

    return *graphicsData;
}

graphicData_t* graphicsDataArrayDtor(graphicData_t** graphicsData, size_t amountGraphics, size_t expectedPointsAmount){
    for(size_t curGraphicData = 0; curGraphicData < amountGraphics; curGraphicData++){
        free((*graphicsData)[curGraphicData].xData);
        free((*graphicsData)[curGraphicData].yData);
    }

    free(*graphicsData);

    return NULL;
}

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

void pythonPointsGenAnimation(graphicData_t* graphicsData, size_t amountGraphics,
                            size_t amountPoints, size_t frameRate,
                            double centerX, double centerY, double radius){
    FILE* pyFilePtr = pythonGenGraphicPreamble();

    createFieldCircle(pyFilePtr, centerX, centerY, radius);

    fprintf(pyFilePtr, "# Отрисовка окружности\n");
    fprintf(pyFilePtr,
    "circle_line, = ax.plot(x_circle, y_circle, "
    "linestyle='--', color='orange', linewidth=2)\n\n");

    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
        fprintf(pyFilePtr, "x_vals_%lu = ", curGraphic + 1);
        initPointsData(pyFilePtr, graphicsData[curGraphic].xData, amountPoints);

        fprintf(pyFilePtr, "y_vals_%lu = ", curGraphic + 1);
        initPointsData(pyFilePtr, graphicsData[curGraphic].yData, amountPoints);
    }
    fprintf(pyFilePtr, "\n");


    fprintf(pyFilePtr, "# Точки пересечения с осью X\n");
    fprintf(pyFilePtr, "x_cross = []\n");
    fprintf(pyFilePtr, "y_cross = []\n\n");

    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
        fprintf(pyFilePtr,
            "idx = np.where(np.diff(np.sign(y_vals_%lu)))[0]\n",
            curGraphic + 1
        );
        fprintf(pyFilePtr,
            "if len(idx) > 0:\n"
            "\ti = idx[0]\n"
            "\tx_cross.append(x_vals_1[i])\n"
            "\ty_cross.append(0)\n"
            "else:\n"
            "\tx_cross.append(None)\n"
            "\ty_cross.append(None)\n\n"
        );
    }


    fprintf(pyFilePtr, "# Оставляем только min и max пересечения\n");
    fprintf(pyFilePtr, "valid_x = [x for x in x_cross if x is not None]\n");
    fprintf(pyFilePtr, "if len(valid_x) >= 2:\n");
    fprintf(pyFilePtr, "\tx_min = min(valid_x)\n");
    fprintf(pyFilePtr, "\tx_max = max(valid_x)\n");
    fprintf(pyFilePtr, "\tx_sel = [x_min, x_max]\n");
    fprintf(pyFilePtr, "elif len(valid_x) == 1:\n");
    fprintf(pyFilePtr, "\tx_sel = [valid_x[0]]\n");
    fprintf(pyFilePtr, "else:\n");
    fprintf(pyFilePtr, "\tx_sel = []\n\n");

    fprintf(pyFilePtr,
        "cross_points = ax.plot(x_sel, [0]*len(x_sel), "
        "'ks', markersize=8, zorder=5)\n\n"
    );

    fprintf(pyFilePtr, "# Читаемые подписи (верх/низ)\n");
    fprintf(pyFilePtr, "offsets = [15, -25]\n");  // вверх, вниз
    fprintf(pyFilePtr, "for i, x in enumerate(x_sel):\n");
    fprintf(pyFilePtr,
        "\tax.annotate(\n"
        "\t\tf\"x = {x:.4f}\",\n"
        "\t\txy=(x, 0),\n"
        "\t\txytext=(0, offsets[i %% 2]),\n"
        "\t\ttextcoords='offset points',\n"
        "\t\tfontsize=10,\n"
        "\t\tcolor='black',\n"
        "\t\tbbox=dict(boxstyle='round,pad=0.3', fc='white', ec='black', alpha=0.85),\n"
        "\t\tarrowprops=dict(arrowstyle='->', color='black', lw=0.8),\n"
        "\t\tzorder=6\n"
        "\t)\n\n"
    );

    fprintf(pyFilePtr, "#Создание фигур\n");

    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
        fprintf(pyFilePtr, "line%lu,  = ax.plot([], [], linewidth=2, alpha = 0.5)\n", curGraphic + 1);
        fprintf(pyFilePtr, "point%lu, = ax.plot([], [], 'ro', markersize=8)\n\n", curGraphic + 1);
    }

    fprintf(pyFilePtr, "#Инициализация\n");
    fprintf(pyFilePtr, "def init():\n"); 
    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
            fprintf(pyFilePtr, "\tline%lu.set_data([], [])\n", curGraphic + 1); 
            fprintf(pyFilePtr, "\tpoint%lu.set_data([], [])\n\n", curGraphic + 1); 
    }

    fprintf(pyFilePtr, "\treturn");
    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
        fprintf(pyFilePtr, " line%lu, point%lu,", curGraphic + 1, curGraphic + 1);
    }
    fprintf(pyFilePtr, "\n\n");

    fprintf(pyFilePtr, "#Обновление кадра\n");
    fprintf(pyFilePtr, "def update(frame):\n"); 

    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
        fprintf(pyFilePtr, "\tline%lu.set_data(x_vals_1[:frame+1], y_vals_%lu[:frame+1])\n", curGraphic + 1, curGraphic + 1); 
        fprintf(pyFilePtr, "\tpoint%lu.set_data([x_vals_1[frame]], [y_vals_%lu[frame]])\n",  curGraphic + 1, curGraphic + 1); 
    }

    fprintf(pyFilePtr, "\treturn");
    for(size_t curGraphic = 0; curGraphic < amountGraphics; curGraphic++){
        fprintf(pyFilePtr, " line%lu, point%lu,", curGraphic + 1, curGraphic + 1);
    }
    fprintf(pyFilePtr, "\n\n");

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
        fprintf(pyFilePtr, "%.5f, ", data[curPoint]);
    }

    fprintf(pyFilePtr, "%.5f])\n", data[curPoint]);
}   

static void createFieldCircle(FILE* pyFilePtr, 
                            double centerX, double centerY, double radius){
    fprintf(pyFilePtr, "# Параметры окружности\n");
    fprintf(pyFilePtr, "cx = %lf\n",  centerX);
    fprintf(pyFilePtr, "cy = %lf\n",  centerY);
    fprintf(pyFilePtr, "R = %lf\n\n", radius);

    fprintf(pyFilePtr, "# Подсчет точек окружности\n");
    fprintf(pyFilePtr, "theta = np.linspace(0, 2*np.pi, 200)\n");
    fprintf(pyFilePtr, "x_circle = cx + R * np.cos(theta)\n");
    fprintf(pyFilePtr, "y_circle = cy + R * np.sin(theta)\n\n");

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

    fprintf(pyFilePtr, "fig, ax = plt.subplots(figsize=(10, 6))\n");
    fprintf(pyFilePtr, "ax.set_xlim(0, 0.2)\n");
    fprintf(pyFilePtr, "ax.set_ylim(-0.06, 0.06)\n");
    fprintf(pyFilePtr, "ax.set_xlabel(\"x\")\n");
    fprintf(pyFilePtr, "ax.set_ylabel(\"y\")\n");
    fprintf(pyFilePtr, "ax.grid(True)\n\n");

    return pyFilePtr;
}

