#!/usr/bin/env python3
"""
================================================================================
GRAFICADOR UNIVERSAL - MÉTODO ÚNICO vs SOLUCIÓN EXACTA
================================================================================
Compara UN método numérico contra la solución analítica exacta (si existe).
Genera gráfica de comparación y análisis de error.

AUTOR: Sistema de Métodos Numéricos
VERSIÓN: 2.0 - Universal
================================================================================
"""

import sys
import numpy as np
import matplotlib.pyplot as plt

# Configuración estética global
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (14, 10)
plt.rcParams['font.size'] = 11


def leer_datos_numericos(archivo):
    """
    Lee datos de archivo generado por programa en C.

    FORMATOS SOPORTADOS:
    - Formato A: i  x  y  (3 columnas: índice, x, y)
    - Formato B: x  y     (2 columnas: x, y)
    - Separadores: espacios, tabulaciones o mixtos

    Parámetros:
        archivo (str): Ruta del archivo de datos

    Retorna:
        tuple: (array_x, array_y) o (None, None) si hay error
    """
    x_datos = []
    y_datos = []

    try:
        with open(archivo, 'r', encoding='utf-8') as f:
            for linea in f:
                linea_limpia = linea.strip()

                # Saltar líneas vacías o comentarios
                if not linea_limpia or linea_limpia.startswith('#'):
                    continue

                # Detectar líneas con datos numéricos
                primer_char = linea_limpia[0]
                if primer_char.isdigit() or primer_char in ['-', '+', '.']:
                    partes = linea_limpia.split()

                    try:
                        if len(partes) >= 3:
                            # Formato: i  x  y
                            x_datos.append(float(partes[1]))
                            y_datos.append(float(partes[2]))
                        elif len(partes) == 2:
                            # Formato: x  y
                            x_datos.append(float(partes[0]))
                            y_datos.append(float(partes[1]))
                    except (ValueError, IndexError):
                        continue  # Ignorar líneas con formato inválido

        if not x_datos:
            print(f"❌ ERROR: No se encontraron datos numéricos en '{archivo}'")
            return None, None

        return np.array(x_datos), np.array(y_datos)

    except FileNotFoundError:
        print(f"❌ ERROR: Archivo '{archivo}' no encontrado")
        return None, None
    except (ValueError, IOError) as e:
        print(f"❌ ERROR inesperado: {e}")
        return None, None


def solicitar_solucion_exacta():
    """
    Pregunta al usuario si desea ingresar la solución exacta.

    Retorna:
        function o None: Función lambda de la solución exacta o None
    """
    print("\n" + "="*70)
    print("SOLUCIÓN EXACTA (Opcional)")
    print("="*70)
    print("¿Desea comparar con la solución analítica exacta?")
    print("  s) Sí, ingresaré la fórmula")
    print("  n) No, solo graficar método numérico")

    opcion = input("\nOpción [n]: ").strip().lower()

    if opcion == 's':
        print("\nEJEMPLOS de fórmulas válidas (usar sintaxis Python):")
        print("  • x**2 + 2*x + 1")
        print("  • np.exp(-x**2)")
        print("  • np.sqrt(x**2 + 1)")
        print("  • x**2 + 2*x + 1 - 0.5*np.exp(x)")
        print("\nNOTA: Use 'np.exp', 'np.sqrt', 'np.sin', 'np.cos', etc.")

        formula = input("\nIngrese y(x) = ").strip()

        if not formula:
            print("⚠️  No se ingresó fórmula. Continuando sin solución exacta.")
            return None

        try:
            # Crear función lambda y probarla
            # pylint: disable=eval-used
            solucion = eval(f"lambda x: {formula}")
            _ = solucion(1.0)  # Prueba
            print("✓ Fórmula aceptada")
            return solucion
        except (SyntaxError, NameError, TypeError, ZeroDivisionError) as e:
            print(f"⚠️  ERROR en la fórmula: {e}")
            print("   Continuando sin solución exacta.")
            return None

    return None


def graficar_metodo_unico(archivo, nombre_metodo, titulo_edo="EDO",
                          solucion_exacta=None, puntos_interes=None):
    """
    Genera gráfica comparativa de un método numérico.

    Parámetros:
        archivo (str): Archivo con resultados numéricos
        nombre_metodo (str): Nombre del método para títulos
        titulo_edo (str): Descripción de la EDO
        solucion_exacta (function): Función y(x) exacta o None
        puntos_interes (list): Lista de valores x para destacar
    """
    # Leer datos numéricos
    print(f"\n📂 Leyendo '{archivo}'...")
    x_num, y_num = leer_datos_numericos(archivo)

    if x_num is None:
        return

    print(f"✓ Datos leídos: {len(x_num)} puntos")
    print(f"  Rango: x ∈ [{x_num[0]:.4f}, {x_num[-1]:.4f}]")

    # Configurar figura
    if solucion_exacta:
        _, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10))
    else:
        _, ax1 = plt.subplots(1, 1, figsize=(14, 6))

    # ========================================
    # GRÁFICA PRINCIPAL
    # ========================================

    # Solución exacta (si existe)
    if solucion_exacta:
        x_exacta = np.linspace(x_num[0], x_num[-1], 500)
        try:
            y_exacta = solucion_exacta(x_exacta)
            ax1.plot(x_exacta, y_exacta, 'b-', linewidth=2.5,
                    label='Solución Exacta', alpha=0.8, zorder=1)
        except (ValueError, ZeroDivisionError, OverflowError) as e:
            print(f"⚠️  Error al evaluar solución exacta: {e}")
            solucion_exacta = None

    # Método numérico
    ax1.plot(x_num, y_num, 'ro-', linewidth=1.5, markersize=5,
            label=f'{nombre_metodo}', alpha=0.8, zorder=2)

    # Destacar puntos de interés
    if puntos_interes and solucion_exacta:
        for x_val in puntos_interes:
            if x_num[0] <= x_val <= x_num[-1]:
                idx = np.argmin(np.abs(x_num - x_val))
                y_aprox = y_num[idx]

                ax1.plot(x_val, y_aprox, 'go', markersize=12,
                        markeredgecolor='darkgreen', markeredgewidth=2,
                        zorder=3)

                ax1.annotate(f'({x_val:.2f}, {y_aprox:.4f})',
                            xy=(x_val, y_aprox),
                            xytext=(10, 10), textcoords='offset points',
                            fontsize=9, color='darkgreen',
                            bbox=dict(boxstyle='round,pad=0.5',
                                    facecolor='yellow', alpha=0.7),
                            arrowprops=dict(arrowstyle='->',
                                          connectionstyle='arc3,rad=0'))

    # Configuración de ejes
    ax1.set_xlabel('x', fontsize=13, fontweight='bold')
    ax1.set_ylabel('y(x)', fontsize=13, fontweight='bold')

    if solucion_exacta:
        titulo = f'{titulo_edo}\nComparación: {nombre_metodo} vs Solución Exacta'
    else:
        titulo = f'{titulo_edo}\nMétodo: {nombre_metodo}'

    ax1.set_title(titulo, fontsize=14, fontweight='bold', pad=15)
    ax1.legend(loc='best', fontsize=11, framealpha=0.95)
    ax1.grid(True, alpha=0.3)

    # ========================================
    # GRÁFICA DE ERROR (solo si hay solución exacta)
    # ========================================

    if solucion_exacta:
        try:
            y_exacta_puntos = solucion_exacta(x_num)
            errores_abs = np.abs(y_exacta_puntos - y_num)
            error_max = np.max(errores_abs)
            error_promedio = np.mean(errores_abs)

            ax2.plot(x_num, errores_abs, 'r-o', linewidth=2,
                    markersize=5, label='Error Absoluto', zorder=2)
            ax2.fill_between(x_num, 0, errores_abs, alpha=0.3,
                            color='red', zorder=1)

            ax2.axhline(y=error_max, color='darkred', linestyle='--',
                       linewidth=1.5, label=f'Error máximo = {error_max:.6e}')
            ax2.axhline(y=error_promedio, color='orange', linestyle='--',
                       linewidth=1.5, label=f'Error promedio = {error_promedio:.6e}')

            ax2.set_xlabel('x', fontsize=13, fontweight='bold')
            ax2.set_ylabel('Error Absoluto', fontsize=13, fontweight='bold')
            ax2.set_title('Análisis de Error', fontsize=14,
                         fontweight='bold', pad=15)
            ax2.legend(loc='best', fontsize=11, framealpha=0.95)
            ax2.grid(True, alpha=0.3)
            ax2.set_yscale('log')

            # Texto con estadísticas de error
            info_error = (f'Error máximo: {error_max:.6e}\n'
                         f'Error promedio: {error_promedio:.6e}')
            ax1.text(0.98, 0.02, info_error, transform=ax1.transAxes,
                    fontsize=10, verticalalignment='bottom',
                    horizontalalignment='right',
                    bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

            # Tabla de comparación
            print("\n" + "="*75)
            print(f"ANÁLISIS DE ERROR - {nombre_metodo}")
            print("="*75)
            print(f"{'x':>10} {'y_numérica':>18} {'y_exacta':>18} {'Error Abs.':>18}")
            print("-"*75)

            for i in range(0, len(x_num), max(1, len(x_num)//10)):
                x_val = x_num[i]
                y_aprox = y_num[i]
                y_exact = y_exacta_puntos[i]
                error = errores_abs[i]
                print(f"{x_val:10.6f} {y_aprox:18.10f} {y_exact:18.10f} "
                      f"{error:18.10e}")

            print("="*75)

        except (ValueError, ZeroDivisionError, OverflowError) as e:
            print(f"⚠️  No se pudo calcular error: {e}")

    # Ajustar y guardar
    plt.tight_layout()

    nombre_archivo = f'grafica_{nombre_metodo.lower().replace(" ", "_")}.png'
    plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
    print(f"\n✓ Gráfica guardada: '{nombre_archivo}'")

    plt.show()


def main():
    """Función principal interactiva."""
    print("\n" + "="*70)
    print("  GRAFICADOR UNIVERSAL - MÉTODO ÚNICO")
    print("="*70)

    # Solicitar archivo
    archivo = input("\nArchivo de datos: ").strip()
    if not archivo:
        print("❌ Debe especificar un archivo")
        sys.exit(1)

    # Solicitar nombre del método
    nombre_metodo = input("Nombre del método (ej: 'Euler', 'RK4'): ").strip()
    if not nombre_metodo:
        nombre_metodo = "Método Numérico"

    # Solicitar descripción de la EDO
    titulo_edo = input("Descripción de la EDO (opcional): ").strip()
    if not titulo_edo:
        titulo_edo = "Ecuación Diferencial Ordinaria"

    # Solicitar solución exacta
    solucion_exacta = solicitar_solucion_exacta()

    # Solicitar puntos de interés (solo si hay solución exacta)
    puntos_interes = None
    if solucion_exacta:
        puntos_str = input("\nPuntos a destacar (separados por coma, opcional): ").strip()
        if puntos_str:
            try:
                puntos_interes = [float(p.strip()) for p in puntos_str.split(',')]
            except ValueError:
                print("⚠️  Formato inválido. Continuando sin puntos destacados.")

    # Generar gráfica
    graficar_metodo_unico(archivo, nombre_metodo, titulo_edo,
                         solucion_exacta, puntos_interes)


if __name__ == "__main__":
    main()
