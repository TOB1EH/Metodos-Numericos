#include <stdio.h>
#include <stdlib.h>

#define MATRIZ_TXT "matriz.txt"
#define N 10

int main(int argc, char const *argv[])
{
    FILE *file = fopen(MATRIZ_TXT, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escritura\n");
        return 1;
    }

    // Asignación de memoria correcta para una matriz 2D
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

    for (size_t i = 0; i < N; i++)
    {
        // Diagonal Principal
        A[i][i] = -2;

        // Vector independiente
        b[i] = 4;

        // Subdiagonal
        if (i > 0)
            A[i][i-1] = 1;

        // Superdiagonal
        if (i < N-1)
            A[i][i+1] = 1;
    }

    A[0][0] = 1;
    A[N-1][N-1] = 1;
    A[0][1] = 0;
    A[N-1][N-2] = 0;
    b[0] = 1;
    b[N-1] = 1;

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

    // Liberar memoria
    fclose(file);
    for (size_t i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(b);

    return 0;
}
