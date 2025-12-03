#include <stdio.h>
#include <stdlib.h>

#define MATRIZ_TXT "matriz.txt"
#define N 15  // ← CAMBIAR A 15

int main(int argc, char const *argv[])
{
    FILE *file = fopen(MATRIZ_TXT, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escritura\n");
        return 1;
    }

    double **A = (double **)malloc(N * sizeof(double *));
    double *b = (double *)malloc(N * sizeof(double));
    if (!A || !b) {
        printf("Error de memoria\n");
        fclose(file);
        return 1;
    }

    // Inicializar memoria para cada fila
    for (size_t i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
        if (!A[i]) {
            printf("Error de memoria en fila %zu\n", i);
            fclose(file);
            return 1;
        }
        // Inicializar en ceros
        for (size_t j = 0; j < N; j++) {
            A[i][j] = 0.0;
        }
    }

    // ============================================
    // MATRIZ DEL PROBLEMA 4 DEL EXAMEN
    // ============================================

    // PRIMERA FILA: [1, 0, 0, ..., 0]
    A[0][0] = 1.0;
    b[0] = 1.0;

    // FILAS 2 a ANTEUTIMA
    for (size_t i = 1; i < N - 1; i++) {
        A[i][i-1] = 1.0;   // Subdiagonal
        A[i][i] = -2.0;    // Diagonal principal
        A[i][i+1] = 1.0;   // Superdiagonal
        b[i] = 1.0;
    }

    // ÚLTIMA FILA: [0, 0, ..., 0, 1]
    A[N-1][N-1] = 1.0;
    b[N-1] = 1.0;

    // Guardar en archivo
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║  GENERANDO MATRIZ DEL PROBLEMA 4 (15x15)              ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\nMatriz: %dx%d\n", N, N);
    printf("Archivo: %s\n\n", MATRIZ_TXT);
    
    printf("Estructura de la matriz A:\n");
    printf("┌                                    ┐\n");
    printf("│  1   0   0   0  ...  0   0   0   0│\n");
    printf("│  1  -2   1   0  ...  0   0   0   0│\n");
    printf("│  0   1  -2   1  ...  0   0   0   0│\n");
    printf("│  ⋮   ⋮   ⋮   ⋮   ⋱   ⋮   ⋮   ⋮   ⋮│\n");
    printf("│  0   0   0   0  ...  1  -2   1   0│\n");
    printf("│  0   0   0   0  ...  0   0   0   1│\n");
    printf("└                                    ┘\n\n");
    
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            fprintf(file, "%.1lf ", A[i][j]);
        }
        fprintf(file, "%.1lf\n", b[i]);
    }
    
    printf("✓ Matriz guardada correctamente en '%s'\n\n", MATRIZ_TXT);

    // Liberar memoria
    fclose(file);
    for (size_t i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(b);

    return 0;
}