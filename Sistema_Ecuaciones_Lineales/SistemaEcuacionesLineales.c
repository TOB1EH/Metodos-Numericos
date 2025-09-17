#include <stdio.h>
#include <math.h>    // Para funciones matemáticas
#include <stdlib.h>  // Para funciones de memoria dinámica
#include "ctype.h"   // Para tolower()

#define RUTA_MATRIZ "matriz.txt"

/**
 * @brief Pausa la ejecución hasta que el usuario presione ENTER.
 */
void pausa()
{
    printf("\nPresione ENTER para continuar...");
    getchar();
}

/**
 * @brief Solicita al usuario que confirme la matriz cargada desde el archivo.
 * @param A Puntero a la matriz A (salida).
 * @param b Puntero al vector b (salida).
 * @param n Puntero al tamaño del sistema (salida).
 */
void confirmarMatriz(double ***A, double **b, int *n);

/**
 * @brief Muestra el menú de opciones y obtiene la selección del usuario.
 * @param opcion Puntero donde se guarda la opción elegida.
 */
void opcionMenu(char *opcion);

/**
 * @brief Lee un sistema de ecuaciones lineales desde un archivo.
 * @param filename Nombre del archivo.
 * @param A Puntero a la matriz A (salida).
 * @param b Puntero al vector b (salida).
 * @param n Puntero al tamaño del sistema (salida).
 * @return 0 si todo salió bien, 1 si hubo error.
 *
 * El archivo debe tener n filas, cada una con n coeficientes y el término independiente.
 */
int leerSistemaDesdeArchivo(const char *filename, double ***A, double **b, int *n);

/**
 * @brief Libera la memoria reservada para la matriz y el vector.
 * @param A Matriz de coeficientes.
 * @param b Vector de términos independientes.
 * @param n Tamaño del sistema.
 */
void liberarMemoria(double **A, double *b, int n);

/**
 * @brief Resuelve el sistema de ecuaciones usando el método de eliminación de Gauss.
 * @param A Matriz de coeficientes (modificada en el proceso).
 * @param b Vector de términos independientes (modificado en el proceso).
 * @param n Tamaño del sistema.
 *
 * Este método transforma la matriz en triangular superior y luego realiza
 * retrosustitución para encontrar la solución del sistema.
 * Imprime la matriz triangular superior, el determinante y la solución del sistema.
 */
void eliminacionGauss(double **A, double *b, int n);

int main(int argc, char const *argv[])
{
    char opcion = 0; // Opción del menú
    int stopDoWhile = 0; // Controla la salida del bucle principal

    double **A = NULL;
    double *b = NULL;
    int n = 0;

    do
    {
        system("clear"); // Limpiar la pantalla (Linux/Mac)
        printf("\n=========================================================\n");
        printf("  MÉTODOS DE SOLUCIÓN DE ECUACIONES ALGEBRAICAS LINEALES\n");
        printf("===========================================================\n");
        printf("Seleccione una opción:\n");
        printf("  a) Método Eliminación de Gauss\n");
        printf("  b) Método de Jacobi\n");
        printf("  c) Método de Gauss-Seidel\n");
        printf("  d) Salir\n");
        printf("--------------------------------------------------\n");
        opcionMenu(&opcion);

        switch (opcion)
        {
        case 'a':
            system("clear");
            printf("--------------------------------------------------\n");
            printf("         MÉTODO DE ELIMINACIÓN DE GAUSS\n");
            printf("--------------------------------------------------\n");
            confirmarMatriz(&A, &b, &n);
            pausa();
            system("clear");
            eliminacionGauss(A, b, n);
            pausa();
            liberarMemoria(A, b, n);
            break;
        case 'b':
            system("clear");
            printf("--------------------------------------------------\n");
            printf("         MÉTODO DE JACOBI (No implementado)\n");
            printf("--------------------------------------------------\n");
            pausa();
            break;
        case 'c':
            system("clear");
            printf("--------------------------------------------------\n");
            printf("         MÉTODO DE GAUSS-SEIDEL (No implementado)\n");
            printf("--------------------------------------------------\n");
            pausa();
            break;
        case 'd':
            printf("\nSaliendo del programa...\n");
            stopDoWhile = 1;
            break;
        default:
            printf("\nOpción no válida. Intente de nuevo.\n");
            pausa();
            break;
        }
    } while (stopDoWhile == 0);

    return 0;
}

void confirmarMatriz(double ***A, double **b, int *n)
{
    printf("--------------------------------------------------\n");
    printf("Antes de continuar, verifique que la matriz y el vector\n");
    printf("cargados en el archivo '%s' son correctos.\n", RUTA_MATRIZ);
    printf("En caso contrario, modifique el archivo con la nueva matriz A y vector b.\n");
    printf("--------------------------------------------------\n");
    printf("Al presionar ENTER confirma que la matriz cargada es correcta.\n");
    pausa();

    if (leerSistemaDesdeArchivo(RUTA_MATRIZ, A, b, n) != 0)
    {
        printf("[ERROR] Error al leer el sistema desde el archivo.\n");
        pausa();
        return;
    }
}

void opcionMenu(char *opcion)
{
    printf("Ingrese una opción: ");
    scanf(" %c", opcion);
    *opcion = tolower(*opcion); // Convierte el caracter a minúscula
    while (getchar() != '\n'); // Limpia el buffer
}

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
    pausa();

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

/**
 * @brief Implementación del método de eliminación de Gauss.
 * @param A Matriz de coeficientes (modificada en el proceso).
 * @param b Vector de términos independientes (modificado en el proceso).
 * @param n Tamaño del sistema.
 *
 * Este método transforma la matriz en triangular superior y luego realiza
 * retrosustitución para encontrar la solución del sistema.
 * Imprime la matriz triangular superior, el determinante y la solución del sistema.
 */
void eliminacionGauss(double **A, double *b, int n)
{
    int pivote_fila = 0;
    double aux = 0.0;
    double factor = 0.0;
    double det = 1.0;
    double suma = 0.0;

    // Transformar la matriz A a una matriz triangular superior
    for (size_t i = 0; i < (n-1); i++)
    {
        // Pivoteo Parcial
        pivote_fila = i;

        // Si el pivote es cero o muy pequeño, buscar un pivote mayor en las filas inferiores
        if (fabs(A[i][i]) < 1e-2)
        {
            // Buscar el mayor pivote en la columna i desde la fila i hasta la fila n
            for (size_t l = (i + 1); l < n; l++)
                if (fabs(A[l][i]) > fabs(A[pivote_fila][i]))
                    pivote_fila = l;

            // Intercambiar filas si se encontró un pivote mayor
            for (size_t m = i; m < n; m++)
            {
                aux = A[pivote_fila][m];
                A[pivote_fila][m] = A[i][m];
                A[i][m] = aux;
            }
            aux = b[pivote_fila];
            b[pivote_fila] = b[i];
            b[i] = aux;
        }

        // Buscar el máximo en la columna i desde la fila i hasta la fila n
        for (size_t j = (i + 1); j < n; j++)
        {
            // Evitar división por cero
            if (A[i][i] == 0)
            {
                printf("[ERROR] División por cero en la fila %zu\n", i);
                exit(EXIT_FAILURE);
            }
            // Calcular el factor multiplicativo
            factor = (-A[j][i] / A[i][i]);

            // Actualizar la fila j de A
            for (size_t k = i; k < n; k++)
                A[j][k] += factor * A[i][k];

            // Actualizar el vector b
            b[j] += factor * b[i];
        }
    }

    // Imprimir la matriz triangular superior y el vector modificado para control
    printf("\n--------------------------------------------------\n");
    printf("Matriz Triangular Superior A:\n");
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++) printf("%10.4lf\t", A[i][j]);

        // Imprimir el vector b junto a la matriz A
        printf("| %10.4lf\n", b[i]);
    }
    printf("--------------------------------------------------\n");

    // Determinante de A
    for (size_t i = 0; i < n; i++) det *= A[i][i];

    // Verificar si el determinante es cero
    if (det == 0)
    {
        printf("\n[ERROR] El sistema no tiene solución única (determinante es cero).\n");
        exit(EXIT_FAILURE);
    }
    printf("\nDeterminante de A: %.4lf\n", det);

    // Sustitución hacia atrás para encontrar la solución (Retrosustitución)
    double *x = (double *)malloc(n * sizeof(double)); // Vector solución
    if (x == NULL)
    {
        printf("[ERROR] No se pudo asignar memoria para el vector solución.\n");
        exit(EXIT_FAILURE);
    }

    // Calcular los valores de x desde la última fila hasta la primera
    for (int i = (n - 1); i >= 0; i--)
    {
        suma = b[i];

        // Calcular la suma de A[i][j] * x[j] para j = i+1 hasta n
        for (int j = (i + 1); j < n; j++)
            suma -= A[i][j] * x[j];

        // Calcular el valor de x[i]
        x[i] = suma / A[i][i];
    }

    // Imprimir la solución de forma amigable
    printf("\n--------------------------------------------------\n");
    printf("Solución del sistema:\n");
    printf("--------------------------------------------------\n");
    for (size_t i = 0; i < n; i++)
        printf("  x[%zu] = %10.4lf\n", i, x[i]);
    printf("--------------------------------------------------\n");

    // Liberar memoria
    free(x);
}