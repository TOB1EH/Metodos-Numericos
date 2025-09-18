#include <stdio.h>
#include <stdlib.h>  // Para funciones de memoria dinámica
#include <math.h>    // Para funciones matemáticas
#include "ctype.h"   // Para tolower()
#include "matriz_desde_archivo/matriz_desde_archivo.h"
#include "../libreria_de_aditamentos/aditamentos_ui.h"

#define RUTA_MATRIZ "matriz.txt"
#define MAX_ITER 10000 // Número máximo de iteraciones para métodos iterativos

/**
 * @brief Solicita al usuario que confirme la matriz cargada desde el archivo.
 * @param A Puntero a la matriz A (salida).
 * @param b Puntero al vector b (salida).
 * @param n Puntero al tamaño del sistema (salida).
 */
void confirmarMatriz(double ***A, double **b, int *n);


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

/**
 * @brief Resuelve el sistema de ecuaciones usando el método de Jacobi.
 * @param A Matriz de coeficientes.
 * @param b Vector de términos independientes.
 * @param n Tamaño del sistema.
 *
 * Este método es iterativo y se basa en la descomposición de la matriz A
 * en sus componentes diagonal, inferior y superior.
 * Imprime la solución aproximada del sistema.
 */
void jacobi (double **A, double *b, int n);

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
            confirmarMatriz(&A, &b, &n);
            pausa();
            system("clear");
            jacobi(A, b, n);
            pausa();
            liberarMemoria(A, b, n);
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
    system("clear");

    if (leerSistemaDesdeArchivo(RUTA_MATRIZ, A, b, n) != 0)
    {
        printf("[ERROR] Error al leer el sistema desde el archivo.\n");
        pausa();
        return;
    }
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

/**
 * @brief Resuelve el sistema de ecuaciones usando el método de Jacobi.
 * 
 * Este método es iterativo y se basa en la descomposición de la matriz A
 * en sus componentes diagonal, inferior y superior. Se verifica que la matriz
 * sea diagonalmente dominante para asegurar la convergencia del método.
 * 
 * El método itera hasta que el error entre iteraciones sea menor que la tolerancia
 * especificada por el usuario o hasta alcanzar el número máximo de iteraciones.
 * 
 * @param A Matriz de coeficientes.
 * @param b Vector de términos independientes.
 * @param n Tamaño del sistema.
 * 
 * Imprime la evolución del proceso iterativo, la convergencia y la solución aproximada
 * de forma clara y amigable, similar al método de eliminación de Gauss.
 */
void jacobi (double **A, double *b, int n)
{
    double suma = 0.0;

    // Verificar si es Diagonalmente Dominante
    for (int i = 0; i < n; i++)
    {
        suma = 0.0;

        // Calcular la suma de los valores absolutos de los elementos no diagonales en la fila i
        for (int j = 0; j < n; j++)
        {
            // Sumar los valores absolutos de los elementos no diagonales
            if (i != j)
                suma += fabs(A[i][j]);
        }

        // Verificar la condición de diagonalmente dominante
        if (fabs(A[i][i]) < suma)
        {
            printf("[ADVERTENCIA] La matriz no es diagonalmente dominante en la fila %d.\n", i);
            printf("El método de Jacobi puede no converger.\n");
            exit(EXIT_FAILURE);
        }
        // Verificar que el elemento diagonal no sea cero
        if (A[i][i] == 0)
        {
            printf("[ERROR] La matriz tiene un elemento diagonal cero en la fila %d.\n", i);
            printf("El método de Jacobi no puede aplicarse.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Implementación del método de Jacobi
    double *x_nuevo = (double *)malloc(n * sizeof(double)); // Vector solución nuevo
    double *x_viejo = (double *)malloc(n * sizeof(double)); // Vector solución anterior
    if (x_nuevo == NULL || x_viejo == NULL)
    {
        printf("[ERROR] No se pudo asignar memoria para los vectores solución.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializar x_viejo con ceros
    for (int i = 0; i < n; i++)
        x_viejo[i] = 0.0;

    int iteracion = 0; // Contador de iteraciones
    double tol = 0.0;  // Tolerancia para la convergencia
    double error = 0.0; // Error de la iteración
    double error_viejo = 1000.0; // Error para control

    printf("Ingrese la tolerancia deseada (ej. 0.0001 = 1e-4): ");
    scanf("%lf", &tol);
    while (getchar() != '\n'); // Limpia el buffer

    printf("\n--------------------------------------------------\n");
    printf("Iteración | Error          | Valores aproximados\n");
    printf("--------------------------------------------------\n");

    do
    {
        for (int i = 0; i < n; i++)
        {
            suma = 0.0;
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                    suma += A[i][j] * x_viejo[j];
            }
            x_nuevo[i] = (b[i] - suma) / A[i][i];
        }

        error = 0.0; // Reiniciar el error para la siguiente iteración
        for (size_t i = 0; i < n; i++)
            error += pow(x_nuevo[i] - x_viejo[i], 2);
        error = sqrt(error);

        // Imprimir el estado actual de la iteración
        printf("%9d | %14.6e |", iteracion + 1, error);
        for (size_t i = 0; i < n; i++)
            printf(" %10.6lf", x_nuevo[i]);
        printf("\n");

        if (error > error_viejo)
        {
            printf("[ADVERTENCIA] El método de Jacobi no está convergiendo.\n");
            free(x_nuevo);
            free(x_viejo);
            exit(EXIT_FAILURE);
        }
        error_viejo = error;

        // Actualizar x_viejo para la siguiente iteración
        for (size_t i = 0; i < n; i++)
            x_viejo[i] = x_nuevo[i];

        iteracion++;

    } while (error > tol && iteracion < MAX_ITER);

    printf("--------------------------------------------------\n");
    printf("\nEl método de Jacobi convergió en %d iteraciones con un error de %.6e\n", iteracion, error);
    printf("Solución aproximada:\n");
    printf("--------------------------------------------------\n");
    for (size_t i = 0; i < n; i++)
        printf("  x[%zu] = %10.6lf\n", i, x_nuevo[i]);
    printf("--------------------------------------------------\n");

    // Liberar memoria
    free(x_nuevo);
    free(x_viejo);
}
