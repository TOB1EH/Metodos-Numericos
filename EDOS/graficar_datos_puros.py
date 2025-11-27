#!/usr/bin/env python3
"""
================================================================================
GRAFICADOR UNIVERSAL - SOLO DATOS (SIN SOLUCIÓN EXACTA)
================================================================================
Grafica datos numéricos puros sin comparación con solución analítica.
Útil para visualizar resultados cuando no se conoce la solución exacta.

AUTOR: Sistema de Métodos Numéricos
VERSIÓN: 2.0 - Universal
================================================================================
"""

import sys
import numpy as np
import matplotlib.pyplot as plt

# Configuración estética
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (12, 7)
plt.rcParams['font.size'] = 11


def leer_datos_numericos(archivo):
    """
    Lee datos de archivo con formato flexible.

    Formatos soportados:
    - i  x  y  (3 columnas)
    - x  y     (2 columnas)

    Retorna:
        tuple: (array_x, array_y) o (None, None)
    """
    x_datos = []
    y_datos = []

    try:
        with open(archivo, 'r', encoding='utf-8') as f:
            for linea in f:
                linea_limpia = linea.strip()

                # Saltar vacías y comentarios
                if not linea_limpia or linea_limpia.startswith('#'):
                    continue

                # Detectar datos numéricos
                primer_char = linea_limpia[0]
                if primer_char.isdigit() or primer_char in ['-', '+', '.']:
                    partes = linea_limpia.split()

                    try:
                        if len(partes) >= 3:
                            # Formato: i x y
                            x_datos.append(float(partes[1]))
                            y_datos.append(float(partes[2]))
                        elif len(partes) == 2:
                            # Formato: x y
                            x_datos.append(float(partes[0]))
                            y_datos.append(float(partes[1]))
                    except (ValueError, IndexError):
                        continue

        if not x_datos:
            print(f"❌ No se encontraron datos en '{archivo}'")
            return None, None

        return np.array(x_datos), np.array(y_datos)

    except FileNotFoundError:
        print(f"❌ Archivo '{archivo}' no encontrado")
        return None, None
    except (ValueError, IOError) as e:
        print(f"❌ Error: {e}")
        return None, None


def graficar_datos_puros(archivo, titulo="Resultados Numéricos",
                        etiqueta="Datos", mostrar_puntos=True,
                        color='blue', marcador='o'):
    """
    Grafica datos numéricos sin comparación.

    Parámetros:
        archivo (str): Ruta del archivo con datos
        titulo (str): Título de la gráfica
        etiqueta (str): Etiqueta para la leyenda
        mostrar_puntos (bool): Si mostrar marcadores
        color (str): Color de la línea
        marcador (str): Tipo de marcador
    """
    print(f"\n📂 Leyendo '{archivo}'...")
    x_datos, y_datos = leer_datos_numericos(archivo)

    if x_datos is None:
        return

    print(f"✓ Datos leídos: {len(x_datos)} puntos")
    print(f"  Rango x: [{x_datos[0]:.6f}, {x_datos[-1]:.6f}]")
    print(f"  Rango y: [{np.min(y_datos):.6f}, {np.max(y_datos):.6f}]")

    # Crear figura
    _, ax = plt.subplots(figsize=(12, 7))

    # Graficar
    if mostrar_puntos:
        ax.plot(x_datos, y_datos, color=color, marker=marcador,
               linestyle='-', linewidth=2, markersize=6,
               label=etiqueta, alpha=0.8)
    else:
        ax.plot(x_datos, y_datos, color=color, linestyle='-',
               linewidth=2, label=etiqueta, alpha=0.8)

    # Configuración de ejes
    ax.set_xlabel('x', fontsize=13, fontweight='bold')
    ax.set_ylabel('y(x)', fontsize=13, fontweight='bold')
    ax.set_title(titulo, fontsize=14, fontweight='bold', pad=15)
    ax.legend(loc='best', fontsize=11, framealpha=0.95)
    ax.grid(True, alpha=0.3)

    # Información estadística
    info_texto = (f'Puntos: {len(x_datos)}\n'
                 f'x: [{x_datos[0]:.4f}, {x_datos[-1]:.4f}]\n'
                 f'y: [{np.min(y_datos):.4f}, {np.max(y_datos):.4f}]')

    ax.text(0.02, 0.98, info_texto, transform=ax.transAxes,
           fontsize=10, verticalalignment='top',
           bbox=dict(boxstyle='round', facecolor='lightyellow', alpha=0.8))

    # Ajustar y guardar
    plt.tight_layout()

    nombre_archivo = 'grafica_datos_puros.png'
    plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
    print(f"\n✓ Gráfica guardada: '{nombre_archivo}'")

    # Tabla de datos (primeros y últimos 5 puntos)
    print("\n" + "="*50)
    print("MUESTRA DE DATOS")
    print("="*50)
    print(f"{'i':>5} {'x':>15} {'y':>15}")
    print("-"*50)

    # Primeros 5
    for i in range(min(5, len(x_datos))):
        print(f"{i:5d} {x_datos[i]:15.8f} {y_datos[i]:15.8f}")

    if len(x_datos) > 10:
        print("  " + "."*44)

    # Últimos 5
    for i in range(max(len(x_datos)-5, 5), len(x_datos)):
        print(f"{i:5d} {x_datos[i]:15.8f} {y_datos[i]:15.8f}")

    print("="*50)

    plt.show()


def main():
    """Función principal interactiva."""
    print("\n" + "="*70)
    print("  GRAFICADOR UNIVERSAL - DATOS PUROS")
    print("="*70)

    # Solicitar archivo
    archivo = input("\nArchivo de datos: ").strip()
    if not archivo:
        print("❌ Debe especificar un archivo")
        sys.exit(1)

    # Solicitar título
    titulo = input("Título de la gráfica (opcional): ").strip()
    if not titulo:
        titulo = "Resultados Numéricos"

    # Solicitar etiqueta
    etiqueta = input("Etiqueta de los datos (opcional): ").strip()
    if not etiqueta:
        etiqueta = "Datos"

    # Solicitar si mostrar puntos
    mostrar_puntos_input = input("¿Mostrar puntos? (s/n) [s]: ").strip().lower()
    mostrar_puntos = mostrar_puntos_input != 'n'

    # Solicitar color
    print("\nColores disponibles: blue, red, green, orange, purple, cyan, black")
    color = input("Color de la línea [blue]: ").strip()
    if not color:
        color = 'blue'

    # Solicitar marcador
    print("\nMarcadores: o (círculo), s (cuadrado), ^ (triángulo), D (diamante)")
    marcador = input("Marcador [o]: ").strip()
    if not marcador:
        marcador = 'o'

    # Generar gráfica
    graficar_datos_puros(archivo, titulo, etiqueta,
                        mostrar_puntos, color, marcador)


if __name__ == "__main__":
    main()
