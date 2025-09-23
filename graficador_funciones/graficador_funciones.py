#!/usr/bin/env python3
"""
Graficador de Funciones Simple para Localización de Raíces
=====================================================

Este programa permite graficar funciones matemáticas simples para ayudar en la
localización visual de raíces en métodos numéricos.

Características básicas:
- Gráficas de funciones en ejes cartesianos
- Detección automática de raíces aproximadas
- Interfaz simple y directa

Autor: TOBIAS FUNES y BLACKBOXAI
Versión: 2.0
"""

import matplotlib.pyplot as plt
import numpy as np

class GraficadorSimple:
    """Clase simplificada para graficar funciones matemáticas."""

    def __init__(self):
        """Inicializa el graficador."""
        self.funciones_basicas = {
            'sin': np.sin, 'cos': np.cos, 'tan': np.tan,
            'exp': np.exp, 'log': np.log, 'sqrt': np.sqrt,
            'pi': np.pi, 'e': np.e, 'abs': np.abs
        }

    def evaluar_funcion_segura(self, expr: str, x_vals: np.ndarray) -> np.ndarray:
        """
        Evalúa una función de forma segura usando numpy.

        Args:
            expr: String con la expresión matemática
            x_vals: Array de valores x

        Returns:
            Array con los valores de la función
        """
        # Crear un entorno seguro
        safe_dict = {
            'x': x_vals,
            'sin': np.sin, 'cos': np.cos, 'tan': np.tan,
            'exp': np.exp, 'log': np.log, 'sqrt': np.sqrt,
            'pi': np.pi, 'e': np.e, 'abs': np.abs
        }

        # Evaluar usando numpy para vectorización
        # Usar evaluación más segura con excepciones específicas
        try:
            # Crear un entorno seguro con nombres permitidos
            allowed_names = {
                k: v for k, v in safe_dict.items() 
                if k not in ['__builtins__', 'eval', 'exec', 'import', 'open']
            }
            
            # Evaluar la expresión de forma segura
            # pylint: disable=eval-used
            y_vals = eval(expr, {"__builtins__": {}}, allowed_names)
            
            # Verificar que el resultado sea numérico
            if not isinstance(y_vals, np.ndarray):
                y_vals = np.array(y_vals)

            return y_vals
            
        except (ValueError, SyntaxError, NameError, ZeroDivisionError) as e:
            print(f"Error al evaluar la función '{expr}': {str(e)}")
            return np.full_like(x_vals, np.nan)

    def graficar(self, expr: str, x_min: float = -10, x_max: float = 10,
                 num_puntos: int = 1000) -> None:
        """
        Grafica una función y muestra las raíces aproximadas.

        Args:
            expr: String con la expresión matemática
            x_min: Valor mínimo de x
            x_max: Valor máximo de x
            num_puntos: Número de puntos para la gráfica
        """
        try:
            # Generar puntos x
            x_vals = np.linspace(x_min, x_max, num_puntos)

            # Evaluar la función
            y_vals = self.evaluar_funcion_segura(expr, x_vals)

            # Verificar si hay datos válidos
            if np.all(np.isnan(y_vals)):
                print("Error: No se pudo evaluar la función en el rango dado.")
                return

            # Crear la figura
            plt.figure(figsize=(10, 6))

            # Graficar la función
            plt.plot(x_vals, y_vals, 'b-', linewidth=2, label=f'f(x) = {expr}')

            # Configurar ejes
            plt.axhline(y=0, color='k', linestyle='-', alpha=0.3)
            plt.axvline(x=0, color='k', linestyle='-', alpha=0.3)

            # Encontrar raíces aproximadas (donde cruza el eje x)
            # Solo considerar valores no NaN
            validos = ~np.isnan(y_vals)
            if np.any(validos):
                y_validos = y_vals[validos]
                x_validos = x_vals[validos]

                # Buscar cambios de signo
                signos = np.sign(y_validos)
                cambios_signo = np.where(signos[:-1] * signos[1:] < 0)[0]

                if len(cambios_signo) > 0:
                    raices = []
                    for idx in cambios_signo:
                        # Interpolación lineal simple
                        x1, x2 = x_validos[idx], x_validos[idx+1]
                        y1, y2 = y_validos[idx], y_validos[idx+1]
                        if abs(y2 - y1) > 1e-10:  # Evitar división por cero
                            x_raiz = x1 - y1 * (x2 - x1) / (y2 - y1)
                            raices.append(x_raiz)

                    # Marcar raíces encontradas
                    if raices:
                        plt.scatter(raices, [0]*len(raices), color='red', s=100, zorder=5, marker='o')
                        for _, raiz in enumerate(raices):
                            plt.annotate(f'Raíz ≈ {raiz:.4f}',
                                       (raiz, 0),
                                       xytext=(0, 10),
                                       textcoords='offset points',
                                       fontsize=10,
                                       ha='center',
                                       bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.8))

            # Configurar título y etiquetas
            plt.title(f'Gráfica de f(x) = {expr}', fontsize=14, pad=20)
            plt.xlabel('x', fontsize=12)
            plt.ylabel('f(x)', fontsize=12)
            plt.grid(True, alpha=0.3)
            plt.legend(fontsize=10)

            # Ajustar límites
            y_min, y_max = np.nanmin(y_vals), np.nanmax(y_vals)
            if np.isfinite(y_min) and np.isfinite(y_max):
                margen = (y_max - y_min) * 0.1
                plt.ylim(y_min - margen, y_max + margen)

            plt.tight_layout()

            # Guardar la gráfica como imagen
            nombre_archivo = f"grafica_{expr.replace('**', 'pow').replace('/', 'div').replace('-', 'menos')}.png"
            plt.savefig(nombre_archivo, dpi=150, bbox_inches='tight')
            print(f"Gráfica guardada como: {nombre_archivo}")
            print("Nota: En este entorno, las gráficas se guardan como archivos PNG.")
            print("Para ver la gráfica, abre el archivo generado.")

            plt.close()  # Cerrar la figura para liberar memoria

        except (ValueError, TypeError, RuntimeError, OverflowError) as e:
            print(f"Error al graficar: {str(e)}")

def mostrar_instrucciones():
    """Muestra instrucciones básicas de uso."""
    print("\n" + "="*50)
    print("     GRAFICADOR DE FUNCIONES SIMPLE")
    print("="*50)
    print("Ejemplos de funciones:")
    print("  x**2 - 4          (raíz en x = 2)")
    print("  x**3 - x - 2      (raíz en x ≈ 1.52)")
    print("  exp(-x) - x       (raíz en x ≈ 0.57)")
    print("  cos(x)            (raíz en x ≈ 0.74)")
    print("  sin(x)            (raíz en x = 0)")
    print("="*50)

def main():
    """Función principal del programa."""
    graficador = GraficadorSimple()

    print("Graficador de Funciones Simple para Localización de Raíces")
    print("Este programa te ayuda a visualizar funciones y encontrar raíces.")

    mostrar_instrucciones()

    while True:
        try:
            # Obtener la función del usuario
            expr = input("\nIngresa la función f(x): ").strip()

            if not expr:
                print("Error: Debes ingresar una función.")
                continue

            # Obtener el rango
            try:
                x_min_str = input("Valor mínimo de x (default -10): ").strip()
                x_min = float(x_min_str) if x_min_str else -10.0

                x_max_str = input("Valor máximo de x (default 10): ").strip()
                x_max = float(x_max_str) if x_max_str else 10.0

                if x_min >= x_max:
                    print("Error: El valor mínimo debe ser menor que el máximo.")
                    continue

            except ValueError:
                print("Error: Ingresa números válidos para el rango.")
                continue

            print("Generando gráfica...")
            graficador.graficar(expr, x_min, x_max)

            # Preguntar si quiere continuar
            continuar = input("\n¿Graficar otra función? (s/n): ").strip().lower()
            if continuar not in ['s', 'si', 'sí', 'yes']:
                print("¡Hasta luego!")
                break

        except KeyboardInterrupt:
            print("\n\nPrograma interrumpido por el usuario.")
            break
        except (ValueError, TypeError) as e:
            print(f"Error inesperado: {e}")
            print("Intenta con una función más simple.")

if __name__ == "__main__":
    main()
