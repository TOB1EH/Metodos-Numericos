#!/usr/bin/env python3
"""
Graficador Genérico de Comparación de Métodos Numéricos
========================================================

Este programa compara CUALQUIER par de métodos o soluciones.

Casos de uso:
- Comparar dos métodos numéricos (Euler vs RK4, RK2 vs Heun, etc.)
- Comparar método numérico vs solución exacta
- Comparar diferentes configuraciones (h diferente, etc.)
- Cualquier par de tablas de valores (x, y)

Compatible con:
- Euler
- Runge-Kutta (RK2, RK4)
- Heun
- Punto Medio
- Adams-Bashforth
- Cualquier otro método que genere tabla (x, y)

Autor: Tobias Funes
Versión: 2.0
"""

import matplotlib.pyplot as plt
import numpy as np
import sys

def leer_tabla(archivo, columnas='auto'):
    """
    Lee una tabla de datos desde un archivo de texto.
    
    Args:
        archivo: Ruta al archivo
        columnas: 'auto' para detectar, o tuple (col_x, col_y) para especificar
    
    Returns:
        x, y: Arrays de numpy con los datos
    """
    x_data = []
    y_data = []
    
    with open(archivo, 'r') as f:
        for linea in f:
            linea = linea.strip()
            
            # Ignorar líneas vacías o comentarios
            if not linea or linea.startswith('#'):
                continue
            
            # Dividir por espacios/tabulaciones
            partes = linea.split()
            
            if len(partes) < 2:
                continue
            
            try:
                if columnas == 'auto':
                    # Asumir que las primeras dos columnas numéricas son x, y
                    x = float(partes[0])
                    y = float(partes[1])
                else:
                    col_x, col_y = columnas
                    x = float(partes[col_x])
                    y = float(partes[col_y])
                
                x_data.append(x)
                y_data.append(y)
            except (ValueError, IndexError):
                # Ignorar líneas que no se puedan convertir
                continue
    
    return np.array(x_data), np.array(y_data)

def graficar_comparacion(archivo_1, archivo_2, nombre_metodo_1="Método 1", nombre_metodo_2="Método 2"):
    """
    Crea gráfica comparativa entre dos métodos o soluciones.
    
    Args:
        archivo_1: Archivo con resultados del primer método
        archivo_2: Archivo con resultados del segundo método
        nombre_metodo_1: Nombre del primer método (para la leyenda)
        nombre_metodo_2: Nombre del segundo método (para la leyenda)
    """
    
    print("\n" + "="*70)
    print(f"GRAFICADOR GENÉRICO - {nombre_metodo_1.upper()} vs {nombre_metodo_2.upper()}")
    print("="*70 + "\n")
    
    # Leer datos
    print(f"📂 Leyendo {nombre_metodo_1}: {archivo_1}")
    x_num, y_num = leer_tabla(archivo_1)
    print(f"   ✓ {len(x_num)} puntos leídos")
    
    print(f"📂 Leyendo {nombre_metodo_2}: {archivo_2}")
    x_ex, y_ex = leer_tabla(archivo_2)
    print(f"   ✓ {len(x_ex)} puntos leídos")
    
    # Validar que tengan el mismo rango
    if abs(x_num[0] - x_ex[0]) > 1e-6 or abs(x_num[-1] - x_ex[-1]) > 1e-6:
        print("\n⚠️  ADVERTENCIA: Los intervalos no coinciden exactamente")
        print(f"   {nombre_metodo_1}: [{x_num[0]:.6f}, {x_num[-1]:.6f}]")
        print(f"   {nombre_metodo_2}: [{x_ex[0]:.6f}, {x_ex[-1]:.6f}]")
    
    # Interpolar si tienen diferente número de puntos
    if len(x_num) != len(x_ex):
        print(f"\n⚠️  Los archivos tienen diferente número de puntos")
        print(f"   Interpolando {nombre_metodo_2} a los puntos de {nombre_metodo_1}...")
        y_ex_interp = np.interp(x_num, x_ex, y_ex)
        x_ex_plot = x_ex
        y_ex_plot = y_ex
        x_num_plot = x_num
        y_num_plot = y_num
        x_error = x_num
        y_ex_error = y_ex_interp
    else:
        x_ex_plot = x_ex
        y_ex_plot = y_ex
        x_num_plot = x_num
        y_num_plot = y_num
        x_error = x_num
        y_ex_error = y_ex
    
    # Calcular error
    error_abs = np.abs(y_num_plot - y_ex_error)
    error_max = np.max(error_abs)
    error_promedio = np.mean(error_abs)
    error_min = np.min(error_abs)
    
    # Crear figura con dos subgráficas
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10))
    
    # =========================================================================
    # GRÁFICA 1: Comparación Solución Exacta vs Numérica
    # =========================================================================
    ax1.plot(x_ex_plot, y_ex_plot, 'b-', linewidth=2.5, 
             label=nombre_metodo_2, alpha=0.8)
    ax1.plot(x_num_plot, y_num_plot, 'r--', linewidth=2, 
             label=nombre_metodo_1, alpha=0.7, marker='o', markersize=4, markevery=max(1, len(x_num)//20))
    
    # Marcar puntos inicial y final
    ax1.plot(x_num_plot[0], y_num_plot[0], 'go', markersize=12, 
             label='Condición Inicial', zorder=6, markeredgecolor='black', markeredgewidth=1.5)
    ax1.plot(x_num_plot[-1], y_num_plot[-1], 'rs', markersize=12, 
             label='Punto Final', zorder=6, markeredgecolor='black', markeredgewidth=1.5)
    
    ax1.set_xlabel('x', fontsize=14, fontweight='bold')
    ax1.set_ylabel('y', fontsize=14, fontweight='bold')
    ax1.set_title(f'Comparación: {nombre_metodo_2} vs {nombre_metodo_1}', 
                  fontsize=15, fontweight='bold')
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.legend(fontsize=11, loc='best', framealpha=0.9)
    ax1.axhline(y=0, color='k', linewidth=0.5, alpha=0.3)
    ax1.axvline(x=0, color='k', linewidth=0.5, alpha=0.3)
    
    # =========================================================================
    # GRÁFICA 2: Error Absoluto
    # =========================================================================
    
    # Determinar si usar escala logarítmica
    usar_log = error_min > 0 and (error_max / error_min > 100)
    
    if usar_log:
        ax2.semilogy(x_error, error_abs, 'r-', linewidth=2.5, 
                     label=f'Error Absoluto: $|{nombre_metodo_1} - {nombre_metodo_2}|$')
    else:
        ax2.plot(x_error, error_abs, 'r-', linewidth=2.5, 
                 label=f'Error Absoluto: $|{nombre_metodo_1} - {nombre_metodo_2}|$')
    
    ax2.fill_between(x_error, 0, error_abs, alpha=0.3, color='red')
    
    # Líneas de referencia
    ax2.axhline(y=error_max, color='orange', linestyle='--', linewidth=2, 
                label=f'Error máximo: {error_max:.2e}', alpha=0.7)
    ax2.axhline(y=error_promedio, color='green', linestyle='--', linewidth=2, 
                label=f'Error promedio: {error_promedio:.2e}', alpha=0.7)
    
    ax2.set_xlabel('x', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Error Absoluto' + (' (escala log)' if usar_log else ''), 
                   fontsize=14, fontweight='bold')
    ax2.set_title(f'Diferencia: {nombre_metodo_1} vs {nombre_metodo_2}', fontsize=15, fontweight='bold')
    ax2.grid(True, alpha=0.3, linestyle='--', which='both' if usar_log else 'major')
    ax2.legend(fontsize=11, loc='best', framealpha=0.9)
    
    # Ajustar espaciado
    plt.tight_layout()
    
    # Guardar
    nombre_salida = f'comparacion_{nombre_metodo_1.replace(" ", "_").lower()}_vs_{nombre_metodo_2.replace(" ", "_").lower()}.png'
    plt.savefig(nombre_salida, dpi=300, bbox_inches='tight')
    print(f"\n✅ Gráfica guardada como: {nombre_salida}")
    
    # Mostrar estadísticas
    print("\n" + "="*70)
    print("ESTADÍSTICAS DE ERROR:")
    print("="*70)
    print(f"  Error máximo:    {error_max:.10e}")
    print(f"  Error promedio:  {error_promedio:.10e}")
    print(f"  Error mínimo:    {error_min:.10e}")
    print(f"  Error final:     {error_abs[-1]:.10e}")
    print(f"  Puntos:          {len(x_num)}")
    print(f"  Intervalo:       [{x_num[0]:.6f}, {x_num[-1]:.6f}]")
    print("="*70 + "\n")
    
    # Mostrar
    plt.show()

def main():
    """Función principal - Interfaz del usuario"""
    
    print("\n" + "╔" + "="*68 + "╗")
    print("║" + " "*10 + "GRAFICADOR GENÉRICO DE MÉTODOS NUMÉRICOS" + " "*18 + "║")
    print("╚" + "="*68 + "╝\n")
    
    print("Este graficador puede comparar:")
    print("  • Dos métodos numéricos diferentes (Euler vs RK4, RK2 vs Heun, etc.)")
    print("  • Método numérico vs Solución exacta")
    print("  • Cualquier par de tablas de valores\n")
    
    # Solicitar archivos y nombres
    print("Ingrese el nombre del archivo del PRIMER método/solución:")
    print("(Ejemplo: euler.txt, rk4.txt, solucion_exacta.txt)")
    archivo_1 = input("Archivo 1: ").strip()
    
    print("\nIngrese el NOMBRE del primer método/solución:")
    print("(Ejemplo: Euler, RK4, Solución Exacta)")
    nombre_metodo_1 = input("Nombre 1: ").strip()
    
    if not nombre_metodo_1:
        nombre_metodo_1 = "Método 1"
    
    print("\n" + "-"*70 + "\n")
    
    print("Ingrese el nombre del archivo del SEGUNDO método/solución:")
    print("(Ejemplo: rk4.txt, heun.txt, solucion_exacta.txt)")
    archivo_2 = input("Archivo 2: ").strip()
    
    print("\nIngrese el NOMBRE del segundo método/solución:")
    print("(Ejemplo: RK4, Heun, Solución Exacta)")
    nombre_metodo_2 = input("Nombre 2: ").strip()
    
    if not nombre_metodo_2:
        nombre_metodo_2 = "Método 2"
    
    # Generar gráfica
    try:
        graficar_comparacion(archivo_1, archivo_2, nombre_metodo_1, nombre_metodo_2)
    except FileNotFoundError as e:
        print(f"\n❌ Error: No se encontró el archivo")
        print(f"   {e}")
    except Exception as e:
        print(f"\n❌ Error al procesar: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()
