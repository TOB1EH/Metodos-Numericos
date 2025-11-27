#!/usr/bin/env python3
"""
================================================================================
GRAFICADOR UNIVERSAL - COMPARACIÓN MÚLTIPLE DE MÉTODOS
================================================================================
Compara VARIOS métodos numéricos simultáneamente en una sola gráfica.
Opcionalmente incluye la solución exacta para referencia.

AUTOR: Sistema de Métodos Numéricos
VERSIÓN: 2.0 - Universal
================================================================================
"""

import sys
import os
import numpy as np
import matplotlib.pyplot as plt

# Configuración estética global
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (14, 8)
plt.rcParams['font.size'] = 11

# Colores y marcadores para diferenciar métodos
ESTILOS_METODOS = [
    ('red', 'o', '-'),      # Rojo, círculos, línea sólida
    ('blue', 's', '--'),    # Azul, cuadrados, línea discontinua
    ('green', '^', '-.'),   # Verde, triángulos, línea punto-guion
    ('orange', 'D', ':'),   # Naranja, diamantes, línea punteada
    ('purple', 'v', '-'),   # Púrpura, triángulos invertidos
    ('cyan', 'p', '--'),    # Cian, pentágonos
    ('magenta', '*', '-.'), # Magenta, estrellas
    ('brown', 'h', ':'),    # Café, hexágonos
]


def leer_datos_numericos(archivo):
    """
    Lee datos de archivo con formato flexible.

    Retorna:
        tuple: (array_x, array_y) o (None, None) si hay error
    """
    x_datos = []
    y_datos = []

    try:
        with open(archivo, 'r', encoding='utf-8') as f:
            for linea in f:
                linea_limpia = linea.strip()

                if not linea_limpia or linea_limpia.startswith('#'):
                    continue

                primer_char = linea_limpia[0]
                if primer_char.isdigit() or primer_char in ['-', '+', '.']:
                    partes = linea_limpia.split()

                    try:
                        if len(partes) >= 3:
                            x_datos.append(float(partes[1]))
                            y_datos.append(float(partes[2]))
                        elif len(partes) == 2:
                            x_datos.append(float(partes[0]))
                            y_datos.append(float(partes[1]))
                    except (ValueError, IndexError):
                        continue

        if not x_datos:
            return None, None

        return np.array(x_datos), np.array(y_datos)

    except FileNotFoundError:
        return None, None
    except (ValueError, IOError):
        return None, None


def solicitar_metodos():
    """
    Solicita información de los métodos a comparar.

    Retorna:
        list: Lista de tuplas (archivo, nombre_metodo)
    """
    metodos = []
    print("\n" + "="*70)
    print("CONFIGURACIÓN DE MÉTODOS A COMPARAR")
    print("="*70)
    print("Ingrese los métodos uno por uno (Enter vacío para terminar)\n")

    num_metodo = 1
    while True:
        print(f"--- Método {num_metodo} ---")
        archivo = input("  Archivo de datos: ").strip()

        if not archivo:
            break

        if not os.path.exists(archivo):
            print(f"  ⚠️  Archivo '{archivo}' no encontrado, saltando...")
            continue

        nombre = input("  Nombre del método: ").strip()
        if not nombre:
            nombre = f"Método {num_metodo}"

        metodos.append((archivo, nombre))
        num_metodo += 1
        print()

    return metodos


def solicitar_solucion_exacta():
    """
    Pregunta si se desea incluir solución exacta.

    Retorna:
        function o None: Función lambda de la solución o None
    """
    print("\n" + "="*70)
    print("SOLUCIÓN EXACTA (Opcional)")
    print("="*70)
    print("¿Incluir solución analítica exacta para comparación?")

    opcion = input("(s/n) [n]: ").strip().lower()

    if opcion == 's':
        print("\nEJEMPLOS de fórmulas (sintaxis Python con numpy):")
        print("  • x**2 + 2*x + 1")
        print("  • np.exp(-x**2)")
        print("  • x**2 + 2*x + 1 - 0.5*np.exp(x)")

        formula = input("\nIngrese y(x) = ").strip()

        if formula:
            try:
                # pylint: disable=eval-used
                solucion = eval(f"lambda x: {formula}")
                _ = solucion(1.0)
                print("✓ Fórmula aceptada")
                return solucion
            except (SyntaxError, NameError, TypeError, ZeroDivisionError) as e:
                print(f"⚠️  Error: {e}. Continuando sin solución exacta.")

    return None


def graficar_comparacion_multiple(metodos, titulo_edo="EDO",
                                  solucion_exacta=None):
    """
    Genera gráfica comparativa de múltiples métodos.

    Parámetros:
        metodos (list): Lista de tuplas (archivo, nombre)
        titulo_edo (str): Descripción de la EDO
        solucion_exacta (function): Función y(x) exacta o None
    """
    if not metodos:
        print("❌ No hay métodos para graficar")
        return

    print("\n📊 Generando gráfica comparativa...")

    # Crear figura
    _, ax = plt.subplots(figsize=(14, 8))

    # Variables para mantener el rango global
    x_min_global = float('inf')
    x_max_global = float('-inf')
    metodos_exitosos = []

    # Graficar cada método
    for idx, (archivo, nombre) in enumerate(metodos):
        x_datos, y_datos = leer_datos_numericos(archivo)

        if x_datos is None:
            print(f"⚠️  Omitiendo '{nombre}': no se pudo leer")
            continue

        # Actualizar rango global
        x_min_global = min(x_min_global, x_datos[0])
        x_max_global = max(x_max_global, x_datos[-1])

        # Seleccionar estilo
        color, marcador, linea = ESTILOS_METODOS[idx % len(ESTILOS_METODOS)]

        # Graficar
        ax.plot(x_datos, y_datos, color=color, marker=marcador,
               linestyle=linea, linewidth=1.5, markersize=5,
               label=nombre, alpha=0.8)

        metodos_exitosos.append(nombre)
        print(f"  ✓ {nombre}: {len(x_datos)} puntos")

    # Graficar solución exacta
    if solucion_exacta and metodos_exitosos:
        try:
            x_exacta = np.linspace(x_min_global, x_max_global, 500)
            y_exacta = solucion_exacta(x_exacta)

            ax.plot(x_exacta, y_exacta, 'k-', linewidth=2.5,
                   label='Solución Exacta', alpha=0.9, zorder=10)
            print("  ✓ Solución Exacta graficada")
        except (ValueError, ZeroDivisionError, OverflowError) as e:
            print(f"  ⚠️  Error con solución exacta: {e}")

    # Configuración de la gráfica
    ax.set_xlabel('x', fontsize=13, fontweight='bold')
    ax.set_ylabel('y(x)', fontsize=13, fontweight='bold')

    titulo = f'{titulo_edo}\nComparación de Métodos Numéricos'
    ax.set_title(titulo, fontsize=14, fontweight='bold', pad=15)

    ax.legend(loc='best', fontsize=10, framealpha=0.95,
             ncol=2 if len(metodos_exitosos) > 4 else 1)
    ax.grid(True, alpha=0.3)

    # Ajustar y guardar
    plt.tight_layout()

    nombre_archivo = 'grafica_comparacion_metodos.png'
    plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
    print(f"\n✓ Gráfica guardada: '{nombre_archivo}'")

    plt.show()

    print(f"\n✓ Se graficaron {len(metodos_exitosos)} métodos exitosamente")


def main():
    """Función principal interactiva."""
    print("\n" + "="*70)
    print("  GRAFICADOR UNIVERSAL - COMPARACIÓN MÚLTIPLE")
    print("="*70)

    # Solicitar métodos
    metodos = solicitar_metodos()

    if not metodos:
        print("\n❌ No se especificaron métodos. Saliendo...")
        sys.exit(1)

    print(f"\n✓ Se configuraron {len(metodos)} métodos")

    # Solicitar descripción de la EDO
    titulo_edo = input("\nDescripción de la EDO (opcional): ").strip()
    if not titulo_edo:
        titulo_edo = "Ecuación Diferencial Ordinaria"

    # Solicitar solución exacta
    solucion_exacta = solicitar_solucion_exacta()

    # Generar gráfica
    graficar_comparacion_multiple(metodos, titulo_edo, solucion_exacta)


if __name__ == "__main__":
    main()
