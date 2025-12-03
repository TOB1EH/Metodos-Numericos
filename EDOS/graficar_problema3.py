#!/usr/bin/env python3
"""
Graficador para Problema 3 - Comparación Solución Exacta vs Numérica
====================================================================

Este programa grafica:
- Solución exacta: y(x) = e^(-x)·sin(2x)
- Solución numérica: Runge-Kutta 4to orden

Autor: Tobias Funes
"""

import matplotlib.pyplot as plt
import numpy as np
import sys

def leer_datos_rk4(archivo):
    """Lee los datos del archivo de Runge-Kutta"""
    datos = []
    with open(archivo, 'r') as f:
        for linea in f:
            linea = linea.strip()
            if not linea or linea.startswith('#'):
                continue
            partes = linea.split()
            if len(partes) >= 3:
                # i x y_numerico y_exacto error_abs error_pct
                i = int(partes[0])
                x = float(partes[1])
                y_num = float(partes[2])
                y_ex = float(partes[3])
                datos.append([i, x, y_num, y_ex])
    return np.array(datos)

def graficar_problema3(archivo_datos):
    """Crea la gráfica comparativa"""
    
    # Leer datos
    print(f"Leyendo datos de: {archivo_datos}")
    datos = leer_datos_rk4(archivo_datos)
    
    if len(datos) == 0:
        print("Error: No se encontraron datos en el archivo")
        return
    
    i = datos[:, 0]
    x = datos[:, 1]
    y_numerico = datos[:, 2]
    y_exacto = datos[:, 3]
    
    print(f"✓ Datos leídos: {len(x)} puntos")
    print(f"  Intervalo: [{x[0]:.2f}, {x[-1]:.2f}]")
    
    # Crear figura con dos subgráficas
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
    
    # =========================================================================
    # GRÁFICA 1: Comparación Solución Exacta vs Numérica
    # =========================================================================
    ax1.plot(x, y_exacto, 'b-', linewidth=2.5, label='Solución Exacta: $y(x) = e^{-x}\\sin(2x)$', alpha=0.8)
    ax1.plot(x, y_numerico, 'r--', linewidth=1.5, label='Runge-Kutta 4to Orden (h=0.01)', alpha=0.7)
    
    # Marcar últimos 6 puntos (como pide el inciso b)
    ultimos_6 = len(x) - 6
    ax1.plot(x[ultimos_6:], y_numerico[ultimos_6:], 'go', markersize=8, 
             label='Últimos 6 puntos (Inciso b)', zorder=5)
    
    # Marcar condiciones iniciales
    ax1.plot(x[0], y_numerico[0], 'ko', markersize=10, label='C.I.: y(0)=0, y\'(0)=2', zorder=6)
    
    ax1.set_xlabel('x', fontsize=13, fontweight='bold')
    ax1.set_ylabel('y', fontsize=13, fontweight='bold')
    ax1.set_title('Problema 3 - Solución de EDO de Segundo Orden\n$\\frac{d^2y}{dx^2} + 2\\frac{dy}{dx} + 5y = 0$', 
                  fontsize=14, fontweight='bold')
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.legend(fontsize=10, loc='upper right')
    ax1.axhline(y=0, color='k', linewidth=0.5, alpha=0.3)
    ax1.axvline(x=0, color='k', linewidth=0.5, alpha=0.3)
    
    # =========================================================================
    # GRÁFICA 2: Error Absoluto
    # =========================================================================
    error_abs = np.abs(y_numerico - y_exacto)
    
    ax2.semilogy(x, error_abs, 'r-', linewidth=2, label='Error Absoluto: $|y_{num} - y_{exact}|$')
    ax2.fill_between(x, 1e-12, error_abs, alpha=0.3, color='red')
    
    # Línea de referencia
    error_max = np.max(error_abs)
    error_promedio = np.mean(error_abs)
    ax2.axhline(y=error_max, color='orange', linestyle='--', linewidth=1.5, 
                label=f'Error máximo: {error_max:.2e}')
    ax2.axhline(y=error_promedio, color='green', linestyle='--', linewidth=1.5, 
                label=f'Error promedio: {error_promedio:.2e}')
    
    ax2.set_xlabel('x', fontsize=13, fontweight='bold')
    ax2.set_ylabel('Error Absoluto (escala log)', fontsize=13, fontweight='bold')
    ax2.set_title('Error del Método de Runge-Kutta 4to Orden', fontsize=14, fontweight='bold')
    ax2.grid(True, alpha=0.3, linestyle='--', which='both')
    ax2.legend(fontsize=10, loc='upper right')
    
    # Ajustar espaciado
    plt.tight_layout()
    
    # Guardar
    nombre_archivo = 'problema3_grafica_completa.png'
    plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
    print(f"\n✅ Gráfica guardada como: {nombre_archivo}")
    
    # Mostrar estadísticas
    print("\n" + "="*70)
    print("ESTADÍSTICAS DE ERROR:")
    print("="*70)
    print(f"  Error máximo:    {error_max:.10e}")
    print(f"  Error promedio:  {error_promedio:.10e}")
    print(f"  Error mínimo:    {np.min(error_abs):.10e}")
    print(f"  Error final:     {error_abs[-1]:.10e}")
    print("="*70)
    
    # Mostrar
    plt.show()

def main():
    """Función principal"""
    print("\n" + "="*70)
    print("GRAFICADOR - PROBLEMA 3")
    print("Solución Exacta vs Runge-Kutta 4to Orden")
    print("="*70 + "\n")
    
    # Archivo por defecto o usuario especifica
    if len(sys.argv) > 1:
        archivo = sys.argv[1]
    else:
        print("Ingrese el nombre del archivo con los datos de RK4:")
        print("(Presione Enter para usar: problema3_rk4_completo.txt)")
        archivo = input("Archivo: ").strip()
        if not archivo:
            archivo = "problema3_rk4_completo.txt"
    
    try:
        graficar_problema3(archivo)
    except FileNotFoundError:
        print(f"\n❌ Error: No se encontró el archivo '{archivo}'")
        print("   Asegúrate de que el archivo existe en el directorio actual.")
    except Exception as e:
        print(f"\n❌ Error al procesar: {e}")

if __name__ == "__main__":
    main()
