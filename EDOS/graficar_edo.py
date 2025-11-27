#!/usr/bin/env python3
"""
Graficador para comparar solución exacta vs aproximada de EDO
Problema n°3: dy/dx = y - x² + 1, y(0) = 0.5

Solución exacta: y(x) = x² + 2x + 1 - 0.5*e^x
"""

import numpy as np
import matplotlib.pyplot as plt

# Configurar estilo de gráficos
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (12, 8)
plt.rcParams['font.size'] = 11

def solucion_exacta(x, problema='problema3'):
    """
    Calcula la solución analítica exacta de la EDO según el problema
    
    Problemas disponibles:
    - 'problema3': dy/dx = y - x² + 1, y(0) = 0.5
                   Solución: y(x) = x² + 2x + 1 - 0.5*e^x
    
    - 'ejemplo1': dy/dx = -2xy, y(0) = 1
                  Solución: y(x) = e^(-x²)
    
    - 'manual': Retorna None (comparar solo métodos numéricos)
    
    Para AGREGAR TU PROPIA EDO:
    1. Agrega un nuevo caso en este if
    2. Escribe la fórmula de la solución exacta
    """
    if problema == 'problema3':
        # EDO: dy/dx = y - x² + 1, y(0) = 0.5
        return x**2 + 2*x + 1 - 0.5*np.exp(x)

    if problema == 'ejemplo1':
        # EDO: dy/dx = -2xy, y(0) = 1
        return np.exp(-x**2)

    if problema == 'manual':
        # Sin solución exacta programada
        return None

    # Problema no reconocido
    print(f"Advertencia: Problema '{problema}' no reconocido.")
    print("   Usando 'problema3' por defecto.")
    return x**2 + 2*x + 1 - 0.5*np.exp(x)

def leer_datos_archivo(archivo):
    """
    Lee los datos del archivo generado por el programa en C
    Formatos soportados:
    - Formato 1: i  x_i  y_i  (3 columnas con índice)
    - Formato 2: x  y  (2 columnas sin índice)
    Separadores: espacios o tabulaciones
    """
    x_aprox = []
    y_aprox = []

    try:
        with open(archivo, 'r', encoding='utf-8') as f:
            # Saltar encabezados hasta encontrar los datos
            linea = f.readline()
            while linea:
                linea_limpia = linea.strip()

                # Saltar líneas vacías
                if not linea_limpia:
                    linea = f.readline()
                    continue

                # Buscar donde empiezan los datos numéricos
                # Puede empezar con dígito (índice) o con dígito/signo negativo (x directo)
                if linea_limpia[0].isdigit() or linea_limpia[0] in ['-', '+', '.']:
                    # Dividir por espacios o tabulaciones
                    partes = linea_limpia.split()

                    if len(partes) >= 3:
                        # Formato: i  x_i  y_i
                        try:
                            x_val = float(partes[1])
                            y_val = float(partes[2])
                            x_aprox.append(x_val)
                            y_aprox.append(y_val)
                        except ValueError:
                            pass  # Línea no contiene datos válidos

                    elif len(partes) == 2:
                        # Formato: x  y
                        try:
                            x_val = float(partes[0])
                            y_val = float(partes[1])
                            x_aprox.append(x_val)
                            y_aprox.append(y_val)
                        except ValueError:
                            pass  # Línea no contiene datos válidos

                linea = f.readline()

        if len(x_aprox) == 0:
            print(f"⚠️ Advertencia: No se encontraron datos numéricos en '{archivo}'")
            print("   Formatos soportados:")
            print("   - Formato 1: i  x_i  y_i  (3 columnas)")
            print("   - Formato 2: x  y  (2 columnas)")
            return None, None

        return np.array(x_aprox), np.array(y_aprox)

    except FileNotFoundError:
        print(f"❌ Error: No se encontró el archivo '{archivo}'")
        print("   Asegúrate de ejecutar el programa en C primero.")
        return None, None

def graficar_comparacion(archivo_datos='rk4_variante_resultados.txt', nombre_metodo='RK4 Variante'):
    """
    Crea la gráfica comparativa entre solución exacta y aproximada
    
    Parámetros:
    - archivo_datos: Nombre del archivo con los resultados
    - nombre_metodo: Nombre del método numérico para los títulos
    """
    # Leer datos del archivo
    print(f"📂 Leyendo datos de '{archivo_datos}'...")
    x_aprox, y_aprox = leer_datos_archivo(archivo_datos)

    if x_aprox is None:
        return

    if len(x_aprox) == 0:
        print("❌ Error: No se pudieron leer datos del archivo.")
        print(f"   Verifica que '{archivo_datos}' tenga el formato correcto.")
        return

    print(f"✓ Datos leídos: {len(x_aprox)} puntos")

    # Generar puntos para la solución exacta (más densos para curva suave)
    x_min = x_aprox[0]
    x_max = x_aprox[-1]
    x_exacta = np.linspace(x_min, x_max, 500)
    y_exacta = solucion_exacta(x_exacta)

    # Calcular y_exacta en los puntos de x_aprox para calcular errores
    y_exacta_puntos = solucion_exacta(x_aprox)
    errores_abs = np.abs(y_exacta_puntos - y_aprox)
    error_max = np.max(errores_abs)
    error_promedio = np.mean(errores_abs)

    # Crear figura con dos subplots
    _, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))

    # ========================================
    # SUBPLOT 1: Comparación de soluciones
    # ========================================
    ax1.plot(x_exacta, y_exacta, 'b-', linewidth=2.5,
             label='Solución Exacta: $y(x) = x^2 + 2x + 1 - 0.5e^x$',
             alpha=0.8)

    ax1.plot(x_aprox, y_aprox, 'ro--', linewidth=1.5, markersize=6,
             label=f'Solución Aproximada ({nombre_metodo})', alpha=0.8)

    # Resaltar puntos específicos solicitados
    for x_val in [1.5, 2.0]:
        if x_val <= x_max:
            idx = np.argmin(np.abs(x_aprox - x_val))
            y_aprox_val = y_aprox[idx]
            y_exacta_val = solucion_exacta(x_val)

            ax1.plot(x_val, y_aprox_val, 'go', markersize=12,
                    markeredgecolor='darkgreen', markeredgewidth=2,
                    label=f'y({x_val}) ≈ {y_aprox_val:.4f}')

            ax1.annotate(f'({x_val}, {y_aprox_val:.4f})',
                        xy=(x_val, y_aprox_val),
                        xytext=(10, 10), textcoords='offset points',
                        fontsize=10, color='darkgreen',
                        bbox=dict(boxstyle='round,pad=0.5',
                                facecolor='yellow', alpha=0.7),
                        arrowprops=dict(arrowstyle='->',
                                      connectionstyle='arc3,rad=0'))

    ax1.set_xlabel('x', fontsize=13, fontweight='bold')
    ax1.set_ylabel('y', fontsize=13, fontweight='bold')
    ax1.set_title('EDO: dy/dx = y - x² + 1, y(0) = 0.5\n'
                  f'Comparación: Solución Exacta vs Aproximada ({nombre_metodo})',
                  fontsize=14, fontweight='bold', pad=15)
    ax1.legend(loc='upper left', fontsize=10, framealpha=0.95)
    ax1.grid(True, alpha=0.3)

    # Agregar información del error en el gráfico
    textstr = f'Error máximo: {error_max:.6e}\nError promedio: {error_promedio:.6e}'
    ax1.text(0.98, 0.02, textstr, transform=ax1.transAxes,
            fontsize=11, verticalalignment='bottom', horizontalalignment='right',
            bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

    # ========================================
    # SUBPLOT 2: Error absoluto
    # ========================================
    ax2.plot(x_aprox, errores_abs, 'r-o', linewidth=2, markersize=5,
            label='Error Absoluto: |y_exacta - y_aproximada|')
    ax2.fill_between(x_aprox, 0, errores_abs, alpha=0.3, color='red')

    ax2.axhline(y=error_max, color='darkred', linestyle='--', linewidth=1.5,
               label=f'Error máximo = {error_max:.6e}')
    ax2.axhline(y=error_promedio, color='orange', linestyle='--', linewidth=1.5,
               label=f'Error promedio = {error_promedio:.6e}')

    ax2.set_xlabel('x', fontsize=13, fontweight='bold')
    ax2.set_ylabel('Error Absoluto', fontsize=13, fontweight='bold')
    ax2.set_title('Análisis de Error Absoluto', fontsize=14, fontweight='bold', pad=15)
    ax2.legend(loc='upper left', fontsize=10, framealpha=0.95)
    ax2.grid(True, alpha=0.3)
    ax2.set_yscale('log')  # Escala logarítmica para ver mejor los errores pequeños

    # Ajustar espaciado entre subplots
    plt.tight_layout()

    # Guardar figura con nombre específico del método
    nombre_archivo = f'grafica_edo_{nombre_metodo.lower().replace(" ", "_")}.png'
    plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
    print(f"\n✓ Gráfica guardada como '{nombre_archivo}'")

    # Mostrar gráfica
    plt.show()

    # Imprimir tabla de comparación para puntos clave
    print("\n" + "="*70)
    print(f"TABLA DE COMPARACIÓN EN PUNTOS ESPECÍFICOS - {nombre_metodo}")
    print("="*70)
    print(f"{'x':>8} {'y_aproximada':>15} {'y_exacta':>15} {'Error Abs.':>15}")
    print("-"*70)

    for x_val in [0.0, 0.5, 1.0, 1.5, 2.0]:
        if x_val <= x_max:
            idx = np.argmin(np.abs(x_aprox - x_val))
            if abs(x_aprox[idx] - x_val) < 0.01:  # Verificar que sea el punto correcto
                y_aprox_val = y_aprox[idx]
                y_exacta_val = solucion_exacta(x_val)
                error = abs(y_exacta_val - y_aprox_val)
                error_msg = f"{x_val:8.1f} {y_aprox_val:15.10f} "
                error_msg += f"{y_exacta_val:15.10f} {error:15.10e}"
                print(error_msg)

    print("="*70)

def main():
    """
    Función principal
    """
    print("\n" + "="*70)
    print("  GRAFICADOR DE EDO - Solución Exacta vs Aproximada")
    print("  Problema n°3: dy/dx = y - x² + 1, y(0) = 0.5")
    print("="*70 + "\n")

    # Permitir al usuario elegir el archivo
    print("Archivos disponibles:")
    print("  1) rk4_variante_resultados.txt (RK4 Variante)")
    print("  2) euler_resultados.txt (Método de Euler)")
    print("  3) heun_resultados.txt (Método de Heun)")
    print("  4) rk4_resultados.txt (RK4 Clásico)")
    print("  5) Otro archivo")

    opcion = input("\nSeleccione una opción (1-5) [1]: ").strip()

    archivos = {
        '1': ('rk4_variante_resultados.txt', 'RK4 Variante'),
        '2': ('euler_resultados.txt', 'Método de Euler'),
        '3': ('heun_resultados.txt', 'Método de Heun'),
        '4': ('rk4_resultados.txt', 'RK4 Clásico'),
        '5': None
    }

    if not opcion or opcion == '':
        opcion = '1'  # Valor por defecto

    if opcion == '5':
        archivo = input("Ingrese el nombre del archivo: ").strip()
        nombre_metodo = input(
            "Ingrese el nombre del método (ej: 'Método de Punto Medio'): "
        ).strip()
        if not nombre_metodo:
            nombre_metodo = "Método Numérico"
    elif opcion in archivos:
        archivo, nombre_metodo = archivos[opcion]
    else:
        print(f"⚠️ Opción '{opcion}' no válida. Usando opción 1 por defecto.")
        archivo = 'rk4_variante_resultados.txt'
        nombre_metodo = 'RK4 Variante'

    # Generar gráfica
    graficar_comparacion(archivo, nombre_metodo)

if __name__ == "__main__":
    main()
