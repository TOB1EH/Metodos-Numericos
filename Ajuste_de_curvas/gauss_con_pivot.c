/**
 * @file gauss_con_pivot.c
 * @brief Implementación del método de eliminación Gaussiana con pivoteo parcial.
 * @author Tobias Funes
 * @version 1.1
 *
 * =================================================================================
 * TEORÍA: ELIMINACIÓN GAUSSIANA CON PIVOTEO PARCIAL
 * =================================================================================
 * Este método resuelve sistemas de ecuaciones lineales A*x = b. Consta de dos fases:
 *
 * 1. ELIMINACIÓN HACIA ADELANTE:
 *    El objetivo es transformar la matriz A en una matriz triangular superior.
 *    Para cada columna `i` (de 0 a n-2):
 *    a) PIVOTEO PARCIAL: Se busca en la columna `i`, desde la fila `i` hacia abajo,
 *       el elemento con el mayor valor absoluto. La fila que contiene este elemento
 *       se intercambia con la fila `i`. Esto se hace para mejorar la estabilidad
 *       numérica, evitando divisiones por números pequeños que podrían amplificar
 *       errores de redondeo.
 *    b) ELIMINACIÓN: Se utilizan operaciones de fila para hacer cero todos los
 *       elementos debajo del pivote (A[i][i]). Para cada fila `j` debajo de `i`,
 *       se calcula un factor y se resta un múltiplo de la fila `i` de la fila `j`.
 *
 * 2. SUSTITUCIÓN HACIA ATRÁS (RETROSUSTITUCIÓN):
 *    Una vez que la matriz es triangular superior, el sistema es fácil de resolver.
 *    Se despeja la última incógnita (x[n-1]) y se sustituye su valor en la penúltima
 *    ecuación para encontrar x[n-2], y así sucesivamente hasta encontrar x[0].
 * =================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gauss_con_pivot.h"

void gaussPivot (double **A, double *b, int n, double *x)
{
    int pivote_fila = 0;
    double aux = 0.0;
    double factor = 0.0;
    double det = 1.0;

    // --- FASE 1: Eliminación hacia adelante ---
    for (size_t i = 0; i < (n-1); i++)
    {
        // a) Pivoteo Parcial: Encontrar la fila con el máximo pivote en la columna i.
        pivote_fila = i;
        for (size_t l = (i + 1); l < n; l++)
        {
            if (fabs(A[l][i]) > fabs(A[pivote_fila][i]))
            {
                pivote_fila = l;
            }
        }

        // Intercambiar la fila actual (i) con la fila del pivote si es necesario.
        if (pivote_fila != i) {
            // Intercambiar la fila completa en la matriz A.
            for (size_t m = i; m < n; m++)
            {
                aux = A[pivote_fila][m];
                A[pivote_fila][m] = A[i][m];
                A[i][m] = aux;
            }
            // Intercambiar el elemento correspondiente en el vector b.
            aux = b[pivote_fila];
            b[pivote_fila] = b[i];
            b[i] = aux;
        }

        // b) Eliminación: Hacer cero los elementos debajo del pivote A[i][i].
        for (size_t j = (i + 1); j < n; j++)
        {
            // Comprobar si el pivote es cero. Si lo es, la matriz es singular.
            if (A[i][i] == 0)
            {
                printf("[ERROR] Matriz singular detectada. El sistema no tiene solución única.\n");
                return;
            }
            // Calcular el factor para anular A[j][i].
            factor = (-A[j][i] / A[i][i]);

            // Actualizar el resto de la fila j en la matriz A.
            for (size_t k = i; k < n; k++)
            {
                A[j][k] += factor * A[i][k];
            }

            // Actualizar el elemento correspondiente en el vector b.
            b[j] += factor * b[i];
        }
    }

    // El determinante de una matriz triangular es el producto de su diagonal.
    det = 1.0;
    for (size_t i = 0; i < n; i++) {
        det *= A[i][i];
    }

    // Verificar si el determinante es cero
    // Usar una pequeña tolerancia para la comparación con cero.
    if (fabs(det) < 1e-12)
    {
        printf("\n[ERROR] El sistema no tiene solución única (determinante es cero).\n");
        // No se puede continuar con la sustitución hacia atrás.
        return;
    }
    printf("\nDeterminante de A: %.4lf\n", det);

    // --- FASE 2: Sustitución hacia atrás (Retrosustitución) ---
    for (int i = (n - 1); i >= 0; i--)
    {
        double suma = b[i];

        // Restar los términos A[i][j] * x[j] para j > i, donde los x[j] ya son conocidos.
        for (int j = (i + 1); j < n; j++) {
            suma -= A[i][j] * x[j];
        }

        // Despejar x[i].
        x[i] = suma / A[i][i];
    }

    // --- Presentación de la Solución ---
    printf("\n--------------------------------------------------\n");
    printf("Solución del sistema:\n");
    printf("(Coeficientes del polinomio a0, a1, ...)\n");
    for (size_t i = 0; i < n; i++)
        printf("  a%zu = %10.6lf\n", i, x[i]);
    printf("--------------------------------------------------\n");
}