import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def y(x, h):
    return h*x**2

# Данные первая кривая
x_vals_1 = np.linspace(-10, 10, 1000)
y_vals_1 = y(x_vals_1, 2)

x_vals_2 = np.linspace(-10, 10, 1000)
y_vals_2 = y(x_vals_2, 1)

# Фигура
fig, ax = plt.subplots(figsize=(10, 6))
ax.set_xlim(-10, 10)
ax.set_ylim(0, 100)
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.grid(True)

# Пустая линия
line1, = ax.plot([], [], linewidth=2)
point1, = ax.plot([], [], 'ro', markersize=8)  # r = red, o = circle

line2, = ax.plot([], [], linewidth=2)
point2, = ax.plot([], [], 'ro', markersize=8)  # r = red, o = circle

# Инициализация
def init():
    line1.set_data([], [])
    point1.set_data([], [])

    line2.set_data([], [])
    point2.set_data([], [])

    return line1, point1, line2, point2

# Обновление кадра
def update(frame):
    line1.set_data(x_vals_1[:frame], y_vals_1[:frame])
    point1.set_data([x_vals_1[frame]], [y_vals_1[frame]])

    line2.set_data(x_vals_2[:frame], y_vals_2[:frame])
    point2.set_data([x_vals_2[frame]], [y_vals_2[frame]])

    return line1, point1, line2, point2

# Анимация
ani = FuncAnimation(
	fig,
    update,
    frames=len(x_vals_1),
    init_func=init,
    interval=10,
    blit=True
)

ani.save("parabola.mp4", writer="ffmpeg", fps=120)
