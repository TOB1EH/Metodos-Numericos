#include <stdio.h>
#include <stdlib.h>
#include "matriz_desde_archivo.h"

int leerSistemaDesdeArchivo(const char *filename, double ***A, double **b, int *n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[ERROR] No se pudo abrir el archivo\n");
        return 1;
    }

    // Contar el número de filas (n)
    *n = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }
    rewind(file);

    // Crear matriz A y vector b
    *A = (double **)malloc((*n) * sizeof(double *));
    *b = (double *)malloc((*n) * sizeof(double));
    if (!(*A) || !(*b)) {
        printf("[ERROR] Error de memoria\n");
        fclose(file);
        return 1;
    }

    // Leer los datos en una sola pasada
    for (int i = 0; i < *n; i++) {
        (*A)[i] = (double *)malloc((*n) * sizeof(double));
        if (!(*A)[i]) {
            printf("[ERROR] Error de memoria\n");
            fclose(file);
            return 1;
        }
        for (int j = 0; j < *n; j++) {
            fscanf(file, "%lf", &((*A)[i][j]));
        }
        fscanf(file, "%lf", &((*b)[i]));
    }
    fclose(file);

    // Imprimir para control
    printf("\n--------------------------------------------------\n");
    printf("Se obtuvo el siguiente sistema de ecuaciones del archivo:\n");
    printf("n = %d\n", *n);
    printf("Matriz A y vector b:\n");
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++)
            printf("%8.3lf ", (*A)[i][j]);
        printf("| %8.3lf\n", (*b)[i]);
    }
    printf("--------------------------------------------------\n");

    return 0;
}

void liberarMemoria(double **A, double *b, int n)
{
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
}