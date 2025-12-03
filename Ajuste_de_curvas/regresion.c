/**
 * @file regresion.c
 * @brief Implementa métodos de regresión lineal simple y polinomial por mínimos cuadrados.
 * @author Tobias Funes
 * @version 3.0
 *
 * =================================================================================
 * TEORÍA: REGRESIÓN POR MÍNIMOS CUADRADOS
 * =================================================================================
 * 
 * 1. REGRESIÓN LINEAL SIMPLE:
 * ---------------------------
 * La regresión lineal simple ajusta una recta y = a + b*x a un conjunto de puntos.
 * Los coeficientes se calculan mediante las fórmulas:
 *   b = [n*Σ(x*y) - Σx*Σy] / [n*Σ(x²) - (Σx)²]
 *   a = ȳ - b*x̄
 * donde x̄ y ȳ son las medias de x e y respectivamente.
 * 
 * 2. REGRESIÓN POLINOMIAL:
 * ------------------------
 * La regresión polinomial es una técnica para encontrar un polinomio de grado 'm'
 * que mejor se ajuste a un conjunto de 'n' puntos de datos (x_i, y_i), donde n > m.
 * El "mejor ajuste" se define como aquel que minimiza la suma de los cuadrados de
 * los residuos (errores verticales) entre los valores 'y' observados y los valores
 * predichos por el polinomio.
 *
 * El polinomio tiene la forma:
 *   f(x) = a_0 + a_1*x + a_2*x^2 + ... + a_m*x^m
 *
 * El error total (Sr) a minimizar es:
 *   Sr = Σ [y_i - f(x_i)]^2 = Σ [y_i - (a_0 + a_1*x_i + ... + a_m*x_i^m)]^2
 *
 * Para encontrar los coeficientes (a_0, a_1, ..., a_m) que minimizan Sr, se toman
 * las derivadas parciales de Sr con respecto a cada coeficiente y se igualan a cero:
 *   ∂Sr/∂a_0 = 0, ∂Sr/∂a_1 = 0, ..., ∂Sr/∂a_m = 0
 *
 * Esto genera un sistema de (m+1) ecuaciones lineales, conocido como las
 * "ecuaciones normales":
 *
 *   n         * a_0 + (Σx_i)    * a_1 + ... + (Σx_i^m)   * a_m = Σy_i
 *   (Σx_i)    * a_0 + (Σx_i^2)  * a_1 + ... + (Σx_i^{m+1}) * a_m = Σ(x_i*y_i)
 *   ...
 *   (Σx_i^m)  * a_0 + (Σx_i^{m+1})* a_1 + ... + (Σx_i^{2m})  * a_m = Σ(x_i^m*y_i)
 *
 * Este sistema se puede escribir en forma matricial A*a = b, donde:
 * - 'a' es el vector de coeficientes a encontrar.
 * - 'A' es una matriz simétrica donde A[i][j] = Σ(x_k^{i+j}).
 * - 'b' es un vector donde b[i] = Σ(y_k * x_k^i).
 *
 * Este programa construye y resuelve este sistema usando eliminación Gaussiana
 * para encontrar los coeficientes del polinomio de mejor ajuste.
 * =================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../libreria_de_aditamentos/aditamentos_ui.h"
#include "gauss_con_pivot.h"

#define ARCHIVO_PUNTOS "nodos.txt"

/**
 * @brief Lee un conjunto de puntos (x, y) desde un archivo de texto.
 * @param filename Nombre del archivo a leer.
 * @param x_puntos Puntero al array donde se almacenarán las coordenadas x.
 * @param y_puntos Puntero al array donde se almacenarán las coordenadas y.
 * @param n Puntero al número de puntos leídos.
 */
void leerPuntosDesdeArchivo(const char *filename, double **x_puntos, double **y_puntos, int *n);

/**
 * @brief Imprime un polinomio de forma legible a partir de sus coeficientes.
 * @param coeficientes Array con los coeficientes del polinomio (a_0, a_1, ...).
 * @param grado Grado del polinomio (m).
 */
void imprimirPolinomioRegresion(double *coeficientes, int grado);

/**
 * @brief Realiza una regresión lineal simple (y = a + b*x) mediante mínimos cuadrados.
 * @param x_puntos Array con las coordenadas x de los puntos.
 * @param y_puntos Array con las coordenadas y de los puntos.
 * @param n Número de puntos.
 * @param a Puntero donde se almacenará el coeficiente independiente.
 * @param b Puntero donde se almacenará la pendiente.
 */
void regresionLinealSimple(double *x_puntos, double *y_puntos, int n, double *a, double *b);

void menuRegresionPolinomial(double *x_puntos, double *y_puntos, int n);
void menuRegresionLinealSimple(double *x_puntos, double *y_puntos, int n);

int main(void)
{
    double *x_puntos = NULL;
    double *y_puntos = NULL;
    int n = 0;
    int opcion = 0;

    system("clear");
    printf("\n=========================================================\n");
    printf("  METODOS DE REGRESION POR MINIMOS CUADRADOS\n");
    printf("===========================================================\n");
    printf("Recuerde modificar el archivo '%s' que cuenta con los puntos a utilizar.\n", ARCHIVO_PUNTOS);
    
    leerPuntosDesdeArchivo(ARCHIVO_PUNTOS, &x_puntos, &y_puntos, &n);

    do
    {
        printf("\nSeleccione el tipo de regresión:\n");
        printf("  1. Regresión Lineal Simple (y = a + b*x)\n");
        printf("  2. Regresión Polinomial (grado m)\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        while (getchar() != '\n'); // Limpiar el búfer de entrada
        
        if (opcion < 1 || opcion > 2) {
            printf("[ERROR] Opción inválida. Seleccione 1 o 2.\n");
        }
    } while (opcion < 1 || opcion > 2);

    if (opcion == 1) {
        menuRegresionLinealSimple(x_puntos, y_puntos, n);
    } else {
        menuRegresionPolinomial(x_puntos, y_puntos, n);
    }

    // --- Liberación de memoria ---
    free(x_puntos);
    free(y_puntos);
    return 0;
}

void menuRegresionPolinomial(double *x_puntos, double *y_puntos, int n)
{
    int grado = 0;
    
    do
    {
        printf("Ingrese el grado del polinomio: ");
        scanf("%d", &grado);
        while (getchar() != '\n'); // Limpiar el búfer de entrada
        if (grado <= 0) {
            printf("[ERROR] El grado debe ser un entero positivo.\n");
        } else if (grado >= n) {
            printf("[ERROR] El grado debe ser menor que el número de puntos (%d).\n", n);
            printf("         Para grado >= n-1, use interpolación.\n");
        }

    } while (grado <= 0 || grado >= n);
    
    int m = grado + 1; // El tamaño del sistema de ecuaciones es grado + 1

    // Asignar memoria para la matriz A, el vector b y el vector de coeficientes a_i
    double **A = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
        A[i] = (double *)malloc(m * sizeof(double));
    
    double *b = (double *)malloc(m * sizeof(double));
    double *a_i = (double *)malloc(m * sizeof(double));

    // --- Construcción de las Ecuaciones Normales ---
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            double sumax = 0.0;
            for (int k = 0; k < n; k++)
            {
                sumax += pow(x_puntos[k], i + j);
            }
            A[i][j] = sumax;
        }
    }

    for (int i = 0; i < m; i++)
    {
        double sumaxy = 0.0;
        for (int j = 0; j < n; j++)
        {
            sumaxy += y_puntos[j] * pow(x_puntos[j], i);
        }
        b[i] = sumaxy;
    }
    
    // --- Resolución del Sistema ---
    gaussPivot(A, b, m, a_i);

    // --- Presentación de Resultados ---
    printf("\n------------------------------------------------------------\n");
    printf("  Polinomio de Regresión f(x) de Grado %d\n", grado);
    printf("------------------------------------------------------------\n");
    printf("f(x) = ");
    imprimirPolinomioRegresion(a_i, grado);
    printf("\n------------------------------------------------------------\n");

    // --- Cálculo de Métricas de Error del Ajuste ---
    // 1. Calcular la media de los valores 'y'
    double y_media = 0.0;
    for (int i = 0; i < n; i++) {
        y_media += y_puntos[i];
    }
    y_media /= n;

    // 2. Calcular St (Suma total de cuadrados) y Sr (Suma de cuadrados de residuos)
    double st = 0.0;
    double sr = 0.0;
    for (int i = 0; i < n; i++) {
        // Evaluar el polinomio f(x) en el punto x_i
        double fx = 0.0;
        for (int j = 0; j < m; j++) {
            fx += a_i[j] * pow(x_puntos[i], j);
        }
        // Suma total de cuadrados: Σ(y_i - y_media)^2
        st += pow(y_puntos[i] - y_media, 2);
        // Suma de cuadrados de los residuos: Σ(y_i - f(x_i))^2
        sr += pow(y_puntos[i] - fx, 2);
    }

    // 3. Calcular métricas finales
    double syx = sqrt(sr / (n - m)); // Error estándar de la estimación
    double r2 = (st - sr) / st;      // Coeficiente de determinación R^2

    printf("\nEvaluación del ajuste:\n");
    printf("Suma de cuadrados de los residuos (Sr): %.6f\n", sr);
    printf("Error estándar de la estimación (Sy/x): %.6f\n", syx);
    printf("Coeficiente de determinación (R^2):     %.6f (%.2f %%)\n", r2, r2 * 100);
    printf("\n* Sy/x: Error absoluto promedio del ajuste.\n* R^2: Proporción de la varianza de 'y' explicada por el modelo (cercano a 1 es mejor).\n");
    printf("------------------------------------------------------------\n");

    // --- Liberación de memoria del sistema de ecuaciones ---
    for (int i = 0; i < m; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(a_i);
}

void menuRegresionLinealSimple(double *x_puntos, double *y_puntos, int n)
{
    double a = 0.0, b = 0.0;
    
    printf("\n============================================================\n");
    printf("  REGRESIÓN LINEAL SIMPLE: y = a + b*x\n");
    printf("============================================================\n");
    
    // Realizar la regresión lineal simple
    regresionLinealSimple(x_puntos, y_puntos, n, &a, &b);
    
    // --- Presentación de Resultados ---
    printf("\n------------------------------------------------------------\n");
    printf("  Ecuación de Regresión Lineal\n");
    printf("------------------------------------------------------------\n");
    printf("y = %.6f + %.6f*x\n", a, b);
    printf("------------------------------------------------------------\n");
    
    // --- Cálculo de Métricas de Error del Ajuste ---
    // 1. Calcular la media de los valores 'y'
    double y_media = 0.0;
    for (int i = 0; i < n; i++) {
        y_media += y_puntos[i];
    }
    y_media /= n;
    
    // 2. Calcular St (Suma total de cuadrados) y Sr (Suma de cuadrados de residuos)
    double st = 0.0;
    double sr = 0.0;
    for (int i = 0; i < n; i++) {
        double y_pred = a + b * x_puntos[i];
        st += pow(y_puntos[i] - y_media, 2);
        sr += pow(y_puntos[i] - y_pred, 2);
    }
    
    // 3. Calcular métricas finales
    double syx = sqrt(sr / (n - 2)); // Error estándar (n-2 grados de libertad para regresión lineal)
    double r2 = (st - sr) / st;      // Coeficiente de determinación R^2
    double r = sqrt(r2);             // Coeficiente de correlación
    if (b < 0) r = -r;               // r tiene el mismo signo que b
    
    printf("\nEvaluación del ajuste:\n");
    printf("Suma de cuadrados de los residuos (Sr): %.6f\n", sr);
    printf("Error estándar de la estimación (Sy/x): %.6f\n", syx);
    printf("Coeficiente de correlación (r):         %.6f\n", r);
    printf("Coeficiente de determinación (R^2):     %.6f (%.2f %%)\n", r2, r2 * 100);
    printf("\n* Sy/x: Error absoluto promedio del ajuste.\n");
    printf("* r: Correlación entre x e y (cercano a ±1 indica fuerte relación lineal).\n");
    printf("* R^2: Proporción de la varianza de 'y' explicada por el modelo.\n");
    printf("------------------------------------------------------------\n");
    
    // Mostrar tabla de valores predichos
    printf("\nTabla de valores predichos:\n");
    printf("------------------------------------------------------------\n");
    printf("    x_i         y_i       y_pred      residuo\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        double y_pred = a + b * x_puntos[i];
        double residuo = y_puntos[i] - y_pred;
        printf("%10.4f  %10.4f  %10.4f  %10.4f\n", 
               x_puntos[i], y_puntos[i], y_pred, residuo);
    }
    printf("------------------------------------------------------------\n");
}

// Implementación de la función para leer puntos desde un archivo.
void leerPuntosDesdeArchivo(const char *filename, double **x_puntos, double **y_puntos, int *n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[ERROR] No se pudo abrir el archivo\n");
        *n = 0;
        return;
    }

    // Contar el número de filas (puntos) en el archivo.
    *n = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }
    rewind(file);

    // Asignar memoria para los arreglos de coordenadas x e y.
    *x_puntos = (double *)malloc((*n) * sizeof(double));
    *y_puntos = (double *)malloc((*n) * sizeof(double));
    if (!(*x_puntos) || !(*y_puntos))
    {
        printf("[ERROR] Error de memoria\n");
        free(*x_puntos);
        free(*y_puntos);
        fclose(file);
        return;
    }

    // Leer los datos de cada punto.
    for (int i = 0; i < *n; i++)
    {
        // Leer los dos valores double de la línea actual en x e y.
        fscanf(file, "%lf %lf", &((*x_puntos)[i]), &((*y_puntos)[i]));
    }
    fclose(file);

    // Imprimir los puntos leídos para verificación del usuario.
    printf("\n----------------------------------------------------\n");
    printf("Se obtuvo el siguiente sistema de puntos:\n");
    for (int i = 0; i < *n; i++)
    {
        printf("x%d = %10.4f, y%d = %10.4f", i, (*x_puntos)[i], i, (*y_puntos)[i]);
        printf("\n");
    }
    printf("----------------------------------------------------\n");
}

void imprimirPolinomioRegresion(double *coeficientes, int grado)
{
    int primer_termino = 1;
    for (int i = grado; i >= 0; i--)
    {
        double coef = coeficientes[i];
        // No imprimir términos con coeficiente muy cercano a cero
        if (fabs(coef) > 1e-9)
        {
            // Imprimir el signo (+ o -) si no es el primer término
            if (!primer_termino) {
                if (coef > 0) printf(" + ");
                else printf(" - ");
            } else if (coef < 0) {
                // Imprimir signo negativo si es el primer término y es negativo
                printf("-");
            }

            // Imprimir el valor absoluto del coeficiente
            printf("%.4f", fabs(coef));

            // Imprimir la parte de la variable (x, x^2, etc.)
            if (i > 0) {
                printf("*x");
                if (i > 1) printf("^%d", i);
            }
            primer_termino = 0; // Ya no es el primer término
        }
    }
    // Si no se imprimió ningún término (polinomio nulo), imprimir "0.0"
    if (primer_termino) printf("0.0");
}

void regresionLinealSimple(double *x_puntos, double *y_puntos, int n, double *a, double *b)
{
    // Calcular las sumas necesarias
    double suma_x = 0.0;
    double suma_y = 0.0;
    double suma_xy = 0.0;
    double suma_x2 = 0.0;
    
    for (int i = 0; i < n; i++) {
        suma_x += x_puntos[i];
        suma_y += y_puntos[i];
        suma_xy += x_puntos[i] * y_puntos[i];
        suma_x2 += x_puntos[i] * x_puntos[i];
    }
    
    // Calcular las medias
    double x_media = suma_x / n;
    double y_media = suma_y / n;
    
    // Calcular la pendiente (b) y el término independiente (a)
    // b = [n*Σ(xy) - Σx*Σy] / [n*Σ(x²) - (Σx)²]
    *b = (n * suma_xy - suma_x * suma_y) / (n * suma_x2 - suma_x * suma_x);
    
    // a = ȳ - b*x̄
    *a = y_media - (*b) * x_media;
}