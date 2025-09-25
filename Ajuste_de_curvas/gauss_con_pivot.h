/**
 * @file gauss_con_pivot.h
 * @brief Cabecera para la implementación de la eliminación Gaussiana con pivoteo parcial.
 * @author Tobias Funes
 * @version 1.1
 */
#ifndef GAUSS_CON_PIVOT_H
#define GAUSS_CON_PIVOT_H

/**
 * @brief Resuelve un sistema de ecuaciones lineales A*x = b usando eliminación Gaussiana con pivoteo parcial.
 * @details Este método modifica la matriz A y el vector b en el proceso.
 *          1. **Eliminación hacia adelante:** Transforma la matriz A en una matriz triangular superior.
 *             En cada paso, se utiliza pivoteo parcial para seleccionar el elemento de mayor
 *             valor absoluto en la columna actual como pivote, mejorando la estabilidad numérica.
 *          2. **Sustitución hacia atrás:** Resuelve el sistema triangular superior resultante para
 *             encontrar el vector de soluciones x.
 * @param A Matriz de coeficientes del sistema, de tamaño n x n. Será modificada.
 * @param b Vector de términos independientes, de tamaño n. Será modificado.
 * @param n Dimensión del sistema (número de ecuaciones/incógnitas).
 * @param x Vector de salida donde se almacenará la solución, de tamaño n.
 */
void gaussPivot(double **A, double *b, int n, double *x);

#endif // GAUSS_CON_PIVOT_H