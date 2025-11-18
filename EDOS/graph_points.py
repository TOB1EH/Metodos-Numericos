"""
============================================================================
GRAFICADOR INDIVIDUAL - EDOs
============================================================================

PROPÓSITO:
    Este script grafica UN SOLO método numérico contra la solución exacta.
    Es útil para analizar en detalle un método específico.

USO:
    - Se ejecuta manualmente cuando quieres ver solo un método:
      python3 graph_points.py
    - Lee el archivo 'results.txt' que debe existir en el mismo directorio

ARCHIVOS QUE LEE:
    - results.txt (generado por el programa en C)
      Formato: X[i]    Y[i]  (dos columnas separadas por tabulador)

SALIDA:
    - Gráfica comparativa del método vs solución exacta

PROBLEMA QUE RESUELVE:
    dy/dx = (x·e^(x²))/y
    Solución exacta: y(x) = e^(x²/2) = √(e^(x²))

DIFERENCIA CON graph_points_all.py:
    - graph_points.py: Grafica UN método individual (results.txt)
    - graph_points_all.py: Grafica TODOS los métodos simultáneamente
============================================================================
"""

import numpy as np
import matplotlib.pyplot as plt

# --- Leer datos del archivo results.txt (generado por el código en C) ---
# Formato del archivo: cada fila contiene X[i]    Y[i]
data = np.loadtxt('results.txt')

x_euler = data[:, 0]  # Primera columna: valores de x
y_euler = data[:, 1]  # Segunda columna: valores de y calculados

# --- Calcular la solución analítica exacta y(x) = e^(x²/2) ---
x_exact = np.linspace(min(x_euler), max(x_euler), 200)  # 200 puntos para curva suave
y_exact = np.sqrt(np.exp(x_exact*x_exact))  # √(e^(x²)) = e^(x²/2)

# --- Graficar ambas curvas para comparación ---
plt.figure(figsize=(8, 6))

# Puntos del método numérico
plt.plot(x_euler, y_euler, 'ro-', label="Método del Ejercicio", markersize=5)

# Curva de la solución exacta
plt.plot(x_exact, y_exact, 'b-', label="Solución Exacta", linewidth=2)

# --- Formateo del gráfico ---
plt.title("Comparación entre Método del Ejercicio y Solución Exacta", fontsize=13)
plt.xlabel("x", fontsize=12)
plt.ylabel("y(x)", fontsize=12)
plt.legend()
plt.grid(True)

plt.margins(x=0.05, y=0.05)  # Márgenes del 5% en ambos ejes

# Límites de ejes ligeramente extendidos (opcional, comentado)
# plt.xlim(min(x_euler) - 0.5, max(x_euler) + 0.5)
# plt.ylim(min(y_euler) - 0.5, max(y_exact) + 0.5)

# Si quieres graficar usando límites de datos de la solución exacta, descomenta:
# plt.xlim(min(x_exact)-0.1, max(x_exact)+0.1)
# plt.ylim(min(y_exact)-0.1, max(y_exact)+0.1)

# Mostrar gráfico
plt.show()

# REFERENCIAS DE MATPLOTLIB:
# - Tipos de marcadores:
#   https://matplotlib.org/stable/api/markers_api.html
# - Colores disponibles:
#   https://matplotlib.org/stable/gallery/color/named_colors.html
# - Estilos de línea:
#   https://matplotlib.org/stable/api/_as_gen/matplotlib.lines.Line2D.html
