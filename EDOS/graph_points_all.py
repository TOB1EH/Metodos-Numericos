"""
============================================================================
GRAFICADOR DE COMPARACIÓN MÚLTIPLE - EDOs
============================================================================

PROPÓSITO:
    Este script grafica y compara MÚLTIPLES métodos numéricos simultáneamente
    contra la solución exacta. Es ideal para visualizar todos los resultados
    en una sola gráfica.

USO:
    - Ejecutado automáticamente por EDOSResolutions.cpp cuando todos los
      archivos de resultados existen
    - También puede ejecutarse manualmente: python3 graph_points_all.py

ARCHIVOS QUE LEE:
    - method_of_exercise_results.txt (Método de Dos Pasos)
    - runge_kutta_4_results.txt (Método RK4)

SALIDA:
    - Gráfica comparativa con todos los métodos disponibles y solución exacta

NOTA:
    Si un archivo no existe, ese método simplemente no se grafica.
    No genera error.
============================================================================
"""

import os

import numpy as np
import matplotlib.pyplot as plt

def load_data(file_path):
    """
    Carga datos desde un archivo si existe.
    
    Parámetros:
        file_path (str): Nombre del archivo con formato X[i]  Y[i]
    
    Retorna:
        tuple: (x_values, y_values) o (None, None) si el archivo no existe
    """
    if os.path.exists(file_path):
        data = np.loadtxt(file_path)
        return data[:,0], data[:,1]
    return None, None

# --- Definir la solución exacta para comparación ---
# Problema: dy/dx = (x·e^(x²))/y
# Solución exacta: y(x) = e^(x²/2)

x_exact = np.linspace(0, 1, 200)

# y_exact = np.sqrt(np.exp(x_exact * x_exact))  # e^(x²/2) = √(e^(x²))
y_exact = (1 + (x_exact**2))**2

# --- Definir métodos disponibles para graficar ---
# Formato: "Nombre en leyenda": ("archivo.txt", "color", "marcador")
methods = {
    "Método del Ejercicio": ("method_of_exercise_results.txt", "r", "o"),
    "Runge-Kutta 4": ("runge_kutta_4_results.txt", "c", "D")
}

# Crear figura con tamaño apropiado
plt.figure(figsize=(9, 7))

# Graficar cada método numérico que tenga archivo disponible
for label, (filename, color, marker) in methods.items():
    x, y = load_data(filename)
    if x is not None:
        plt.plot(x, y, color=color, marker=marker, linestyle='-', label=label)

# Graficar la solución exacta como referencia
plt.plot(x_exact, y_exact, 'b-', linewidth=2, label="Solución Exacta")

# --- Configuración estética del gráfico ---
plt.title("Comparación de Métodos Numéricos vs Solución Exacta", fontsize=14)
plt.xlabel("x", fontsize=12)
plt.ylabel("y(x)", fontsize=12)
plt.legend(fontsize=11)
plt.grid(True, linestyle='--', alpha=0.7)
plt.xlim(0, 1)
plt.tight_layout()

plt.margins(x=0.05, y=0.05)  # Márgenes del 5% en ambos ejes

plt.show()
