import matplotlib.pyplot as plt
import numpy as np

# Leer la tabla original
print("Ingrese el nombre del archivo de la tabla ORIGINAL: ")
archivo_original = input().strip()
data_original = np.loadtxt(archivo_original)
x_original = data_original[:, 0]
y_original = data_original[:, 1]

# Leer la tabla generada con splines
print("Ingrese el nombre del archivo de la tabla GENERADA: ")
archivo_generado = input().strip()
data_generada = np.loadtxt(archivo_generado)
x_generada = data_generada[:, 0]
y_generada = data_generada[:, 1]

# Crear la gráfica
plt.figure(figsize=(12, 7))

# Graficar la tabla generada (spline)
plt.plot(x_generada, y_generada, 'b-', linewidth=2, label='Spline Cúbico')

# Graficar los puntos originales
plt.plot(x_original, y_original, 'ro', markersize=8, label='Puntos Originales', zorder=5)

# Configuración de la gráfica
plt.xlabel('x', fontsize=12)
plt.ylabel('y', fontsize=12)
plt.title('Interpolación con Splines Cúbicos', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3, linestyle='--')
plt.legend(fontsize=10)

# Guardar y mostrar
plt.savefig('splines_cubicos.png', dpi=300, bbox_inches='tight')
print("\n✓ Gráfica guardada como 'splines_cubicos.png'")
plt.show()