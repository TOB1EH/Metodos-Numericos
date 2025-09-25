/**
 * @file crearMatriz.c
 * @brief Generador de matrices de prueba para sistemas de ecuaciones lineales.
 * @author Tobias Funes
 * @version 1.1
 *
 * @details
 * Este programa crea una matriz tridiagonal de tamaño N x N y un vector de
 * términos independientes. Luego, guarda el sistema de ecuaciones resultante
 * en el archivo "matriz.txt" en un formato compatible con otros programas
 * del proyecto que resuelven sistemas lineales.
 *
 * La matriz generada es:
 * - Diagonal principal: 2
 * - Superdiagonal: 1
 * - Subdiagonal: 1
 *
 * Los términos independientes se inicializan a 6, pero los valores del primer
 * y último elemento se modifican a 4.5 para simular condiciones de borde.
 */
#include <stdio.h>
#include <stdlib.h>

#define MATRIZ_TXT "matriz.txt" // Nombre del archivo de salida.
#define N 10 // Dimensión de la matriz a generar.

int main(int argc, char const *argv[])
{
    // Abrir el archivo en modo escritura ('w'). Si existe, se sobrescribe.
    FILE *file = fopen(MATRIZ_TXT, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escritura\n");
        return 1;
    }

    // Asignación de memoria correcta para una matriz 2D
    // Primero, se asigna un arreglo de punteros (las filas).
    double **A = (double **)malloc(N * sizeof(double *));
    double *b = (double *)malloc(N * sizeof(double));
    if (!A || !b)
    {
        printf("Error de memoria\n");
        fclose(file);
        return 1;
    }

    for (size_t i = 0; i < N; i++)
    {
        // Luego, para cada fila, se asigna un arreglo de doubles (las columnas).
        A[i] = (double *)malloc(N * sizeof(double));
        if (!A[i]) {
            printf("Error de memoria en fila %zu\n", i);
            // Aquí faltaría liberar la memoria ya asignada antes de salir
            fclose(file);
            return 1;
        }
        for (size_t j = 0; j < N; j++)
        {
            A[i][j] = 0;
        }
    }

    // Construcción de la matriz tridiagonal y el vector b.
    for (size_t i = 0; i < N; i++)
    {
        // Superdiagonal
        if (i < N-1)
            A[i][i+1] = 1;

        // Diagonal Principal
        A[i][i] = 2;

        // Subdiagonal
        if (i > 0)
        A[i][i-1] = 1;

        // Vector independiente
        b[i] = 6;
    }

    // Modificación de las condiciones de borde en el vector b.
    // A[0][0] = 1;
    // A[N-1][N-1] = 1;
    // A[0][1] = 0;
    // A[N-1][N-2] = 0;
    b[0] = 4.5;
    b[N-1] = 4.5;

    // Guardar la matriz y el vector en el archivo
    printf("Guardando matriz de %dx%d en %s...\n", N, N, MATRIZ_TXT);
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            fprintf(file, "%.1lf ", A[i][j]);
        }
        // Cuando lo quiera leer el programa recordar quitar el | y la tabulacion
        fprintf(file, "%.1lf\n", b[i]);
    }
    printf("Matriz guardada correctamente.\n");

    // Liberar la memoria asignada para evitar fugas (memory leaks).
    // Primero se liberan las columnas de cada fila.
    fclose(file);
    for (size_t i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(b);

    return 0;
}
