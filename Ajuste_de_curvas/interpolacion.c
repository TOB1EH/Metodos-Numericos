/**
 * @file interpolacion.c
 * @brief Programa para encontrar el polinomio interpolador usando la forma de Lagrange.
 * @author Tobias Funes
 * @version 2.0
 *
 * =================================================================================
 * TEORÍA: INTERPOLACIÓN POLINOMIAL
 * ============================================================================
 * La interpolación es un método para construir una función (generalmente un
 * polinomio) que pasa exactamente por un conjunto de puntos de datos dados.
 * Dado un conjunto de n+1 puntos (x_0, y_0), (x_1, y_1), ..., (x_n, y_n),
 * el objetivo es encontrar un polinomio P(x) de grado a lo sumo n tal que
 * P(x_k) = y_k para todo k = 0, 1, ..., n.
 *
 * FORMA DE LAGRANGE:
 * El polinomio interpolador de Lagrange es una forma directa de escribir dicho
 * polinomio. Se define como una suma ponderada de "polinomios base" L_k(x):
 *
 *   P(x) = Σ_{k=0}^{n} [y_k * L_k(x)]
 *
 * donde cada polinomio base L_k(x) tiene la propiedad de que L_k(x_k) = 1 y
 * L_k(x_j) = 0 para j ≠ k. Se construye de la siguiente manera:
 *
 *   L_k(x) = Π_{j=0, j≠k}^{n} [ (x - x_j) / (x_k - x_j) ]
 *
 * Esta forma es elegante para la teoría, pero puede ser computacionalmente
 * costosa si se necesita evaluar el polinomio en muchos puntos o si se añaden
 * nuevos nodos de interpolación.
 * ---
 *
 * EJEMPLO DE CÁLCULO MANUAL
 * ============================================================================
 * Dados 3 puntos: (1, 2), (2, 3), (3, 6).
 * El polinomio será de grado 2.
 *
 * 1. Calcular los polinomios base L_k(x):
 *
 *    L_0(x) = [(x - x_1) / (x_0 - x_1)] * [(x - x_2) / (x_0 - x_2)]
 *           = [(x - 2) / (1 - 2)] * [(x - 3) / (1 - 3)]
 *           = [(x - 2) / -1] * [(x - 3) / -2]
 *           = (x^2 - 5x + 6) / 2
 *
 *    L_1(x) = [(x - x_0) / (x_1 - x_0)] * [(x - x_2) / (x_1 - x_2)]
 *           = [(x - 1) / (2 - 1)] * [(x - 3) / (2 - 3)]
 *           = [(x - 1) / 1] * [(x - 3) / -1]
 *           = - (x^2 - 4x + 3) = -x^2 + 4x - 3
 *
 *    L_2(x) = [(x - x_0) / (x_2 - x_0)] * [(x - x_1) / (x_2 - x_1)]
 *           = [(x - 1) / (3 - 1)] * [(x - 2) / (3 - 2)]
 *           = [(x - 1) / 2] * [(x - 2) / 1]
 *           = (x^2 - 3x + 2) / 2
 *
 * 2. Construir el polinomio P(x):
 *
 *    P(x) = y_0*L_0(x) + y_1*L_1(x) + y_2*L_2(x)
 *    P(x) = 2 * [(x^2 - 5x + 6) / 2] + 3 * [-x^2 + 4x - 3] + 6 * [(x^2 - 3x + 2) / 2]
 *    P(x) = (x^2 - 5x + 6) + (-3x^2 + 12x - 9) + (3x^2 - 9x + 6)
 *
 * 3. Expandir y simplificar para obtener los coeficientes:
 *
 *    P(x) = (1 - 3 + 3)x^2 + (-5 + 12 - 9)x + (6 - 9 + 6)
 *    P(x) = 1x^2 - 2x + 3
 * ---
 *
 * NOTAS ADICIONALES
 * ============================================================================
 * - El polinomio interpolador es único, sin importar el método usado para encontrarlo (Lagrange, Newton, etc.).
 * - La interpolación de Lagrange es computacionalmente intensiva si se agregan nuevos puntos, ya que todos los polinomios base deben recalcularse.
 * - Para un gran número de nodos equiespaciados, los polinomios de alto grado pueden oscilar salvajemente entre los puntos (Fenómeno de Runge).
 * - La opción 'b' (eliminación Gaussiana) resuelve un sistema de ecuaciones de Vandermonde, que es otra forma de encontrar los coeficientes del polinomio.
 * ---
 *
 * TEORÍA: INTERPOLACIÓN SEGMENTARIA (SPLINES)
 * ============================================================================
 * En lugar de usar un único polinomio de alto grado, los splines utilizan
 * polinomios de bajo grado (lineales, cúbicos) en cada subintervalo entre
 * puntos consecutivos. Esto evita el fenómeno de Runge y proporciona curvas
 * más suaves y estables.
 *
 * SPLINES LINEALES:
 * Es la forma más simple. Simplemente conecta los puntos de datos consecutivos
 * con segmentos de recta. Para un intervalo [x_i, x_{i+1}], el spline es:
 *
 *   S_i(x) = y_i + m_i * (x - x_i)
 *
 * donde m_i = (y_{i+1} - y_i) / (x_{i+1} - x_i) es la pendiente de la recta.
 * El resultado es una función continua, pero su derivada no lo es (la curva
 * tiene "esquinas" en los nodos).
 *
 * SPLINES CÚBICOS:
 * Utiliza polinomios de tercer grado en cada intervalo [x_i, x_{i+1}]:
 *
 *   S_i(x) = a_i + b_i(x - x_i) + c_i(x - x_i)^2 + d_i(x - x_i)^3
 *
 * Para asegurar que la curva sea suave, se imponen condiciones de continuidad
 * no solo en la función, sino también en su primera y segunda derivada en cada
 * nodo interior. Esto genera un sistema de ecuaciones lineales que permite
 * calcular los coeficientes. Los splines cúbicos "naturales" (usados aquí)
 * asumen que la segunda derivada en los extremos (x_0 y x_n) es cero, lo que
 * corresponde a que no hay curvatura en los extremos del trazado.
 *
 * El proceso para encontrar los coeficientes implica:
 * 1. Saber que a_i = y_i.
 * 2. Construir un sistema de ecuaciones tridiagonal para encontrar los
 *    coeficientes c_i (relacionados con la segunda derivada).
 * 3. Con los c_i conocidos, calcular los b_i y d_i.
 *
 *   d_i = (c_{i+1} - c_i) / (3 * h_i)
 *   b_i = (y_{i+1} - y_i) / h_i - h_i * (2*c_i + c_{i+1}) / 3
 *
 * donde h_i = x_{i+1} - x_i.
 * Este método produce curvas visualmente muy suaves y es ampliamente utilizado en gráficos por computadora y diseño.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../libreria_de_aditamentos/aditamentos_ui.h"
#include "gauss_con_pivot.h"

// Define el nombre del archivo que contiene los nodos de interpolación.
#define NODOS_TXT "nodos.txt"

/**
 * @brief Lee un conjunto de puntos (x, y) desde un archivo de texto.
 * @param filename Nombre del archivo a leer.
 * @param x_puntos Puntero al array donde se almacenarán las coordenadas x.
 * @param y_puntos Puntero al array donde se almacenarán las coordenadas y.
 * @param n Puntero al número de puntos leídos.
 */
void leerPuntosDesdeArchivo(const char *filename, double **x_puntos, double **y_puntos, int *n);

/**
 * @brief Libera la memoria asignada para los arreglos de puntos.
 * @param x_puntos Arreglo de coordenadas x.
 * @param y_puntos Arreglo de coordenadas y.
 */
void liberarPuntos(double *x_puntos, double *y_puntos);

/**
 * @brief Implementa el método de interpolación de Lagrange.
 * @details Pide al usuario un punto 'x' para interpolar, calcula el valor P(x)
 *          usando la fórmula de Lagrange y muestra el resultado y el error.
 *          También ofrece la opción de mostrar el polinomio expandido.
 * @param x_puntos Arreglo con las coordenadas x de los puntos.
 * @param y_puntos Arreglo con las coordenadas y de los puntos.
 * @param n Número de puntos.
 */
void lagrange(double *x_puntos, double *y_puntos, int n);

/**
 * @brief Calcula y muestra los coeficientes del polinomio de Lagrange expandido.
 * @details Expande la fórmula P(x) = Σ y_k * L_k(x) para encontrar los coeficientes
 *          del polinomio en su forma estándar P(x) = a_n*x^n + ... + a_1*x + a_0.
 * @param x_puntos Arreglo con las coordenadas x de los puntos.
 * @param y_puntos Arreglo con las coordenadas y de los puntos.
 * @param n Número de puntos.
 */
void mostrarLagrangeExpandido(double *x_puntos, double *y_puntos, int n);

/**
 * @brief Imprime un polinomio en formato legible a partir de sus coeficientes.
 * @param coeficientes Array con los coeficientes del polinomio (coeficientes[i] es el coeficiente de x^i).
 * @param grado Grado del polinomio.
 */
void imprimirPolinomio(double *coeficientes, int grado);

/**
 * @brief Implementa la interpolación por splines lineales.
 * @details Conecta puntos consecutivos con rectas. Pide un valor x, encuentra
 *          el intervalo [x_i, x_{i+1}] que lo contiene y calcula el valor
 *          interpolado usando la ecuación de la recta entre (x_i, y_i) y (x_{i+1}, y_{i+1}).
 * @param x_puntos Arreglo con las coordenadas x de los puntos.
 * @param y_puntos Arreglo con las coordenadas y de los puntos.
 * @param n Número de puntos.
 */
void splinesLineales(double *x_puntos, double *y_puntos, int n);

/**
 * @brief Implementa la interpolación por splines cúbicos naturales.
 * @details Calcula un polinomio de tercer grado para cada intervalo entre nodos,
 *          asegurando continuidad en la función y sus dos primeras derivadas.
 *          Resuelve un sistema tridiagonal para encontrar los coeficientes.
 */

/**
 * @brief Encuentra el polinomio interpolador resolviendo el sistema de Vandermonde.
 * @details Construye la matriz de Vandermonde A y el vector b a partir de los puntos,
 *          y resuelve el sistema A*c=b para encontrar los coeficientes 'c' del polinomio.
 * @param x_puntos Arreglo con las coordenadas x de los puntos.
 * @param y_puntos Arreglo con las coordenadas y de los puntos.
 * @param n Número de puntos.
 */
void splinesCubicas(double *x_puntos, double *y_puntos, int n);
void funcional (double *x_puntos, double *y_puntos, int n);

int main(void)
{
    char opcion = 0; // Opción del menú
    int stopDoWhile = 0; // Controla la salida del bucle principal

    double *x_puntos = NULL;
    double *y_puntos = NULL;
    int n = 0;

    // Bucle principal del menú.
    do
    {
        system("clear"); // Limpiar la pantalla (Linux/Mac)
        printf("\n=========================================================\n");
        printf("  AJUSTES DE CURVAS POR MÉTODOS DE INTERPOLACION\n");
        printf("===========================================================\n");
        printf("Seleccione una opción:\n");
        printf("  a) Forma de Lagrange del Polinomio Interpolador\n");
        printf("  b) Polinomio Interpolador (vía Sistema de Ecuaciones de Vandermonde)\n");
        printf("\nInterpolacion Segmentaria (Curvas Spline):\n");
        printf("  c) Splines Lineales\n");
        printf("  d) Splines Cubicas\n");
        printf("  e) Salir\n");
        printf("--------------------------------------------------\n");
        opcionMenu(&opcion);

        switch (opcion)
        {
        case 'a':
            system("clear");
            printf("------------------------------------------------------------\n");
            printf("         FORMA DE LAGRANGE DEL POLINOMIO INTERPOLADOR\n");
            printf("------------------------------------------------------------\n");
            leerPuntosDesdeArchivo(NODOS_TXT, &x_puntos, &y_puntos, &n);
            pausa();
            system("clear");
            lagrange(x_puntos, y_puntos, n);
            liberarPuntos(x_puntos, y_puntos);
            pausa();
            break;
        case 'b':
            system("clear");
            printf("------------------------------------------------------------\n");
            printf("      POLINOMIO INTERPOLADOR (SISTEMA DE VANDERMONDE)\n");
            printf("------------------------------------------------------------\n");
            leerPuntosDesdeArchivo(NODOS_TXT, &x_puntos, &y_puntos, &n);
            pausa();
            system("clear");
            funcional(x_puntos, y_puntos, n);
            liberarPuntos(x_puntos, y_puntos);
            pausa();
            break;
        case 'c':
            system("clear");
            printf("------------------------------------------------------------\n");
            printf("              INTERPOLACIÓN POR SPLINES LINEALES\n");
            printf("------------------------------------------------------------\n");
            leerPuntosDesdeArchivo(NODOS_TXT, &x_puntos, &y_puntos, &n);
            pausa();
            system("clear");
            splinesLineales(x_puntos, y_puntos, n);
            liberarPuntos(x_puntos, y_puntos);
            pausa();
            break;
        case 'd':
            system("clear");
            printf("------------------------------------------------------------\n");
            printf("              INTERPOLACIÓN POR SPLINES CÚBICOS\n");
            printf("------------------------------------------------------------\n");
            leerPuntosDesdeArchivo(NODOS_TXT, &x_puntos, &y_puntos, &n);
            pausa();
            system("clear");
            splinesCubicas(x_puntos, y_puntos, n);
            liberarPuntos(x_puntos, y_puntos);
            pausa();
            break;
        case 'e':
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

/**
 * @brief Función de ejemplo para calcular el valor "real" y el error.
 * @details Esta función se usa para comparar el valor interpolado P(x) con un
 *          valor teórico f(x) y así calcular el error de interpolación.
 * @param x El punto en el que se evalúa la función.
 * @return El valor de f(x).
 */
double f (double x)
{
    // Esta es la función teórica usada para generar los puntos en 'nodos.txt'
    // y para calcular el error de la interpolación.
    return 3 * pow(x, 4) - 2 * pow(x, 3) + pow(x, 2) - x + 1;
}

/**
 * @brief Calcula el error absoluto entre dos valores.
 * @param fx El valor teórico o exacto.
 * @param Pn El valor aproximado o interpolado.
 * @return El error absoluto |fx - Pn|.
 */
double calcularError(double fx, double Pn)
{
    return fabs(fx - Pn);
}

// Libera la memoria de los arreglos de puntos.
void liberarPuntos(double *x_puntos, double *y_puntos)
{
    free(x_puntos);
    free(y_puntos);
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

// Implementación del método de Lagrange.
void lagrange(double *x_puntos, double *y_puntos, int n)
{
    double x_i = 0.0;
    double suma = 0.0;
    double producto = 0.0;
    double error = 0.0;
    double fx = 0.0;

    // Solicitar el punto donde se desea interpolar.
    printf("\nIngrese el valor a interpolar: ");
    scanf("%lf", &x_i);
    while (getchar() != '\n'); // Limpiar el búfer de entrada

    system("clear");

    // Bucle principal de Lagrange: P_n(x_i) = Σ [y_k * L_k(x_i)]
    for (int k = 0; k < n; k++) // Sumatoria sobre k
    {
        producto = 1.0; // L_k(x_i)
        for (int j = 0; j < n; j++)
        {
            if (j != k)
            {
                double denominador = x_puntos[k] - x_puntos[j];
                // Verificación para evitar división por cero si hay puntos x duplicados.
                if (denominador == 0.0) {
                    printf("[ERROR] División por cero detectada. Los puntos x_%d y x_%d son iguales.\n", k, j);
                    return;
                } else {
                    producto *= (x_i - x_puntos[j]) / denominador;
                }
            }
        }

        // Mostrar el valor del polinomio base L_k(x) evaluado en x_i.
        printf("\nL_%d(%.3f) = %.6f\n", k, x_i, producto);
        
        // Acumular el término y_k * L_k(x_i) a la suma total.
        suma += producto * y_puntos[k];
    }
    pausa();
    system("clear");

    // Calcular el error absoluto comparando con la función teórica f(x).
    fx = f(x_i);
    error = calcularError(fx, suma);

    // Imprimir resultados de la interpolación.
    printf("\nEl valor interpolado en X = %lf es: %lf\n", x_i, suma);
    printf("Error absoluto (vs f(x) real): %lf\n", error);
    pausa(); // Esperar a que el usuario presione ENTER.

    char opcion = 0;
    do
    {
        system("clear");
        printf("\n¿Desea ver el polinomio de Lagrange expandido? (S/n) ");
        opcionMenu(&opcion);
        switch (opcion)
        {
        case 's': case 'S':
            mostrarLagrangeExpandido(x_puntos, y_puntos, n);
            opcion = 'n'; // Para salir del bucle
            break;
        case 'n': case 'N':
            printf("Saliendo...\n");
            break;
        default:
            printf("\nOpción no válida. Intente de nuevo.\n");
            break;
        }
    } while (opcion != 'n' && opcion != 'N');
}

// Implementación de la función para mostrar el polinomio expandido.
void mostrarLagrangeExpandido(double *x_puntos, double *y_puntos, int n)
{
    if (n <= 0) return;

    int grado = n - 1;
    // Array para almacenar los coeficientes del polinomio final P(x).
    // Se inicializa a cero con calloc.
    double *coeficientes_finales = (double *)calloc(n, sizeof(double));
    if (!coeficientes_finales) {
        printf("[ERROR] Fallo de memoria para coeficientes_finales.\n");
        return;
    }

    // Array temporal para calcular los coeficientes de cada polinomio base L_k(x).
    double *coef_Lk = (double *)malloc(n * sizeof(double));
    if (!coef_Lk) {
        printf("[ERROR] Fallo de memoria para coef_Lk.\n");
        free(coeficientes_finales);
        return;
    }

    // Bucle principal: para cada k, calcular los coeficientes de y_k * L_k(x)
    // y sumarlos a los coeficientes finales.
    for (int k = 0; k < n; k++)
    {
        // Inicializar coef_Lk para calcular el numerador de L_k(x)
        // Empezamos con el polinomio 1 (coef[0]=1, resto 0)
        for(int i=0; i<n; i++) coef_Lk[i] = 0.0;
        coef_Lk[0] = 1.0;

        double denominador = 1.0;

        // Bucle para construir el polinomio numerador de L_k(x) y el denominador.
        // El numerador es Π (x - x_j) para j != k.
        for (int j = 0; j < n; j++)
        {
            if (k == j) continue;

            // Multiplicar el polinomio actual (almacenado en coef_Lk) por el binomio (x - r),
            // donde r = nodos[j][0].
            // Si P(x) = c_m*x^m + ..., entonces P(x)*(x-r) = c_m*x^{m+1} + (c_{m-1}-r*c_m)*x^m + ...
            // Esto se implementa iterando desde el grado más alto hacia abajo.
            for (int i = grado; i > 0; i--) {
                coef_Lk[i] = coef_Lk[i-1] - coef_Lk[i] * x_puntos[j];
            }
            coef_Lk[0] *= -x_puntos[j];

            // Acumular el producto en el denominador: Π (x_k - x_j).
            denominador *= (x_puntos[k] - x_puntos[j]);
        }

        // Dividir los coeficientes del numerador por el denominador
        // y multiplicar por y_k, luego sumar al polinomio final
        for (int i = 0; i <= grado; i++)
        {
            coeficientes_finales[i] += (coef_Lk[i] / denominador) * y_puntos[k];
        }
    }

    // Imprimir el resultado final.
    printf("\n------------------------------------------------------------\n");
    printf("      Polinomio Interpolador de Lagrange (Expandido)\n");
    printf("------------------------------------------------------------\n");
    printf("P(x) = ");
    imprimirPolinomio(coeficientes_finales, grado);
    printf("\n------------------------------------------------------------\n");

    // Liberar memoria.
    free(coeficientes_finales);
    free(coef_Lk);
}

// Implementación de la función para imprimir un polinomio.
void imprimirPolinomio(double *coeficientes, int grado)
{
    int primer_termino = 1;
    for (int i = grado; i >= 0; i--)
    {
        if (fabs(coeficientes[i]) > 1e-9) // No imprimir términos con coeficiente cero
        {
            // Imprimir el signo (+ o -) si no es el primer término.
            if (!primer_termino) {
                if (coeficientes[i] > 0) printf(" + ");
                else printf(" - ");
            } else if (coeficientes[i] < 0) {
                // Imprimir signo negativo si es el primer término y es negativo.
                printf("-");
            }

            // Imprimir el valor absoluto del coeficiente.
            if (fabs(fabs(coeficientes[i]) - 1.0) > 1e-9 || i == 0) {
                printf("%.4f", fabs(coeficientes[i]));
                if (i > 0) printf("*");
            }

            // Imprimir la parte de la variable (x, x^2, etc.).
            if (i > 0) {
                printf("*x");
                if (i > 1) printf("^%d", i);
            }
            primer_termino = 0; // Ya no es el primer término.
        }
    }
    // Si no se imprimió ningún término (polinomio nulo), imprimir "0.0".
    if (primer_termino) printf("0.0");
    printf("\n");
}

void splinesLineales(double *x_puntos, double *y_puntos, int n)
{
    if (n < 2) {
        printf("[ERROR] Se necesitan al menos 2 puntos para la interpolación lineal.\n");
        return;
    }

    double x_val;
    printf("\nIngrese el valor a interpolar: ");
    scanf("%lf", &x_val);
    while (getchar() != '\n');

    // 1. Encontrar el intervalo [x_i, x_{i+1}] que contiene a x_val.
    int i = -1;
    for (int j = 0; j < n - 1; j++) {
        if (x_val >= x_puntos[j] && x_val <= x_puntos[j+1]) {
            i = j;
            break;
        }
    }

    // Verificar si el punto está fuera del rango de interpolación.
    if (i == -1) {
        printf("\n[ADVERTENCIA] El valor %.4f está fuera del rango de interpolación [%.4f, %.4f].\n",
               x_val, x_puntos[0], x_puntos[n-1]);
        printf("La extrapolación puede no ser precisa.\n");
        // Se puede decidir si extrapolar o no. Aquí extrapolamos con el primer o último segmento.
        if (x_val < x_puntos[0]) i = 0;
        else i = n - 2;
    }

    // 2. Calcular la pendiente (m) en ese intervalo.
    double h = x_puntos[i+1] - x_puntos[i];
    if (fabs(h) < 1e-9) {
        printf("[ERROR] División por cero. Los puntos x_%d y x_%d son iguales.\n", i, i+1);
        return;
    }
    double m = (y_puntos[i+1] - y_puntos[i]) / h;

    // 3. Aplicar la fórmula del spline lineal S_i(x) = y_i + m * (x - x_i).
    double y_val = y_puntos[i] + m * (x_val - x_puntos[i]);

    // 4. Mostrar el resultado.
    printf("\nEl punto se encuentra en el intervalo [%.4f, %.4f].\n", x_puntos[i], x_puntos[i+1]);
    printf("La ecuación del spline en este tramo es: S_%d(x) = %.4f + %.4f * (x - %.4f)\n", i, y_puntos[i], m, x_puntos[i]);
    printf("\n------------------------------------------------------------\n");
    printf("El valor interpolado en X = %.4f es: %.6f\n", x_val, y_val);
    printf("Error absoluto (vs f(x) real): %.6f\n", calcularError(f(x_val), y_val));
    printf("------------------------------------------------------------\n");
}

void splinesCubicas(double *x_puntos, double *y_puntos, int n)
{
    if (n < 2) {
        printf("[ERROR] Se necesitan al menos 2 puntos para splines cúbicos.\n");
        return;
    }
    int num_splines = n - 1;
    int num_incognitas = 4 * num_splines; // 4 coeficientes (a,b,c,d) por cada spline

    // --- Paso 1: Asignar memoria para el sistema de ecuaciones A*sol = b ---
    double **A = (double **)malloc(num_incognitas * sizeof(double *)); // Matriz de condiciones
    double *b = (double *)malloc(num_incognitas * sizeof(double));
    double *solucion = (double *)malloc(num_incognitas * sizeof(double)); // Coeficientes [a0,b0,c0,d0, a1,b1,c1,d1, ...]

    if (!A || !b || !solucion) {
        printf("[ERROR] Falla de memoria al crear el sistema para splines cúbicos.\n");
        free(A); free(b); free(solucion);
        return;
    }

    for (int i = 0; i < num_incognitas; i++) {
        A[i] = (double *)calloc(num_incognitas, sizeof(double)); // Inicializar con ceros
        if (!A[i]) {
            printf("[ERROR] Falla de memoria en fila %d de la matriz A.\n", i);
            // Liberar memoria asignada hasta ahora
            for(int k=0; k<i; k++) free(A[k]);
            free(A); free(b); free(solucion);
            return;
        }
        b[i] = 0.0;
    }

    // --- Paso 2: Construir la matriz A y el vector b según las condiciones ---
    int fila_actual = 0;

    // Condición 1: Cada spline S_k pasa por sus puntos (x_k, y_k) y (x_{k+1}, y_{k+1})
    // S_k(x) = a_k*x^3 + b_k*x^2 + c_k*x + d_k. Esto genera 2*(n-1) ecuaciones.
    for (int k = 0; k < num_splines; k++) {
        // Ecuación S_k(x_k) = y_k
        A[fila_actual][4*k + 0] = pow(x_puntos[k], 3); // Coeficiente de a_k (x^3)
        A[fila_actual][4*k + 1] = pow(x_puntos[k], 2); // Coeficiente de b_k (x^2)
        A[fila_actual][4*k + 2] = x_puntos[k];         // Coeficiente de c_k (x)
        A[fila_actual][4*k + 3] = 1;                   // Coeficiente de d_k (1)
        b[fila_actual] = y_puntos[k];
        fila_actual++;

        // Ecuación S_k(x_{k+1}) = y_{k+1}
        A[fila_actual][4*k + 0] = pow(x_puntos[k+1], 3);
        A[fila_actual][4*k + 1] = pow(x_puntos[k+1], 2);
        A[fila_actual][4*k + 2] = x_puntos[k+1];
        A[fila_actual][4*k + 3] = 1;
        b[fila_actual] = y_puntos[k+1];
        fila_actual++;
    }

    // Condición 2: Continuidad de la primera derivada en nodos interiores.
    // S'_k(x_{k+1}) = S'_{k+1}(x_{k+1}) => S'_k(x_{k+1}) - S'_{k+1}(x_{k+1}) = 0. S'(x) = 3ax^2 + 2bx + c
    // Esto genera n-2 ecuaciones.
    for (int k = 0; k < num_splines - 1; k++) {
        // Coeficientes para S'_k(x_{k+1})
        A[fila_actual][4*k + 0] = 3 * pow(x_puntos[k+1], 2); // 3*a_k
        A[fila_actual][4*k + 1] = 2 * x_puntos[k+1];         // 2*b_k
        A[fila_actual][4*k + 2] = 1;                         // c_k
        // Coeficientes para -S'_{k+1}(x_{k+1})
        A[fila_actual][4*(k+1) + 0] = -3 * pow(x_puntos[k+1], 2); // -3*a_{k+1}
        A[fila_actual][4*(k+1) + 1] = -2 * x_puntos[k+1];         // -2*b_{k+1}
        A[fila_actual][4*(k+1) + 2] = -1;                         // -c_{k+1}
        b[fila_actual] = 0.0;
        fila_actual++;
    }

    // Condición 3: Continuidad de la segunda derivada en nodos interiores.
    // S''_k(x_{k+1}) = S''_{k+1}(x_{k+1}) => S''_k(x_{k+1}) - S''_{k+1}(x_{k+1}) = 0. S''(x) = 6ax + 2b
    // Esto genera n-2 ecuaciones.
    for (int k = 0; k < num_splines - 1; k++) {
        // Coeficientes para S''_k(x_{k+1})
        A[fila_actual][4*k + 0] = 6 * x_puntos[k+1]; // 6*a_k
        A[fila_actual][4*k + 1] = 2;                 // 2*b_k
        // Coeficientes para -S''_{k+1}(x_{k+1})
        A[fila_actual][4*(k+1) + 0] = -6 * x_puntos[k+1]; // -6*a_{k+1}
        A[fila_actual][4*(k+1) + 1] = -2;                 // -2*b_{k+1}
        b[fila_actual] = 0.0;
        fila_actual++;
    }

    // Condición 4: Condiciones de frontera para spline natural.
    // S''_0(x_0) = 0  y  S''_{n-2}(x_{n-1}) = 0 (segunda derivada nula en los extremos)
    // Esto genera 2 ecuaciones.
    // Ecuación S''_0(x_0) = 0  =>  6*a_0*x_0 + 2*b_0 = 0
    A[fila_actual][0] = 6 * x_puntos[0]; // 6*a_0
    A[fila_actual][1] = 2;               // 2*b_0
    b[fila_actual] = 0.0;
    fila_actual++;

    // Ecuación S''_{n-2}(x_{n-1}) = 0  =>  6*a_{n-2}*x_{n-1} + 2*b_{n-2} = 0
    A[fila_actual][4*(num_splines-1) + 0] = 6 * x_puntos[n-1]; // 6*a_{n-2}
    A[fila_actual][4*(num_splines-1) + 1] = 2;                 // 2*b_{n-2}
    b[fila_actual] = 0.0;

    // --- Paso 3: Resolver el sistema de ecuaciones A*sol = b ---
    printf("Resolviendo sistema de %d x %d ecuaciones para encontrar los coeficientes...\n", num_incognitas, num_incognitas);
    gaussPivot(A, b, num_incognitas, solucion);

    // --- Paso 4: Imprimir los polinomios resultantes ---
    printf("\nPolinomios cúbicos para cada tramo S_k(x) = a_k*x^3 + b_k*x^2 + c_k*x + d_k:\n");
    printf("------------------------------------------------------------------------------------------\n");
    for (int k = 0; k < num_splines; k++) {
        double a_k = solucion[4*k + 0];
        double b_k = solucion[4*k + 1];
        double c_k = solucion[4*k + 2];
        double d_k = solucion[4*k + 3];
        printf("Tramo %d, x en [%.2f, %.2f]:\n", k, x_puntos[k], x_puntos[k+1]);
        printf("S_%d(x) = (%.4f)*x^3 + (%.4f)*x^2 + (%.4f)*x + (%.4f)\n\n",
               k, a_k, b_k, c_k, d_k);
    }
    printf("------------------------------------------------------------------------------------------\n");
    pausa();
    system("clear");

    // --- Paso 5: Pedir un punto al usuario y evaluar el spline correspondiente ---
    double x_val;
    printf("\nIngrese el valor a interpolar: ");
    scanf("%lf", &x_val);
    while (getchar() != '\n');

    // Encontrar el tramo (k) que contiene a x_val.
    int k = -1;
    for (int j = 0; j < num_splines; j++) {
        if (x_val >= x_puntos[j] && x_val <= x_puntos[j+1]) {
            k = j;
            break;
        }
    }

    if (k == -1) {
        printf("\n[ADVERTENCIA] El valor %.4f está fuera del rango de interpolación [%.4f, %.4f].\n",
               x_val, x_puntos[0], x_puntos[n-1]);
        // Decidir si extrapolar usando el primer o último tramo.
        if (x_val < x_puntos[0]) k = 0;
        else k = num_splines - 1;
    }

    // Evaluar el polinomio S_k(x) en x_val.
    double a_k = solucion[4*k + 0];
    double b_k = solucion[4*k + 1];
    double c_k = solucion[4*k + 2];
    double d_k = solucion[4*k + 3];
    double y_val = a_k * pow(x_val, 3) + b_k * pow(x_val, 2) + c_k * x_val + d_k;

    printf("\nEl punto se encuentra en el intervalo [%.4f, %.4f] (Tramo %d).\n", x_puntos[k], x_puntos[k+1], k);
    printf("\n------------------------------------------------------------\n");
    printf("El valor interpolado en X = %.4f es: %.6f\n", x_val, y_val);
    printf("Error absoluto (vs f(x) real): %.6f\n", calcularError(f(x_val), y_val));
    printf("------------------------------------------------------------\n");

    // --- Paso 6: Liberar toda la memoria dinámica ---
    for (int i = 0; i < num_incognitas; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(solucion);
}

void funcional (double *x_puntos, double *y_puntos, int n)
{
    // 1. Asignar memoria para la matriz de Vandermonde A y el vector b.
    double **A = (double **)malloc(n * sizeof(double *));
    double *b = (double *)malloc(n * sizeof(double));
    double *coeficientes = (double *)malloc(n * sizeof(double)); // Vector para los coeficientes a_i

    if (!A || !b || !coeficientes) {
        printf("[ERROR] Error de memoria\n");
        free(A);
        free(b);
        free(coeficientes);
        return;
    }

    // 2. Armar la Matriz de Vandermonde A y el vector independiente b.
    for (size_t i = 0; i < (size_t)n; i++)
    {
        // Asignar memoria para las columnas de cada fila
        A[i] = (double *)malloc(n * sizeof(double));
        if (!A[i])
        {
            printf("[ERROR] Error de memoria en la fila %zu\n", i);
            // Liberar toda la memoria asignada antes de salir para evitar fugas.
            for(size_t k = 0; k < i; k++) {
                free(A[k]);
            }
            free(A);
            free(b);
            free(coeficientes);
            return;
        }

        for (size_t j = 0; j < (size_t)n; j++)
        {
            A[i][j] = pow(x_puntos[i], j); // A[i][j] = x_i^j
        }
        b[i] = y_puntos[i];
    }

    // 3. Resolver el sistema de ecuaciones A*coef = b para encontrar los coeficientes.
    gaussPivot(A, b, n, coeficientes);

    // 4. Imprimir el polinomio resultante.
    printf("\n------------------------------------------------------------\n");
    printf("      Polinomio Interpolador (Vía Sistema de Vandermonde)\n");
    printf("------------------------------------------------------------\n");
    printf("P(x) = ");
    imprimirPolinomio(coeficientes, n-1);
    printf("------------------------------------------------------------\n\n");

    // Solicitar el punto donde se desea interpolar.
    double x_i = 0.0;
    printf("\nIngrese el valor a interpolar: ");
    scanf("%lf", &x_i);
    while (getchar() != '\n'); // Limpiar el búfer de entrada

    // 5. Evaluar el polinomio en el punto x_i.
    double suma = 0.0;
    for (size_t i = 0; i < (size_t)n; i++)
    {
        suma += coeficientes[i] * pow(x_i, i);
    }
    
    // 6. Calcular el valor real y el error.
    double fx = f(x_i);
    printf("\nEl valor interpolado en X = %lf es: %lf\n", x_i, suma);
    printf("Error absoluto (vs f(x) real): %lf\n", calcularError(fx, suma));

    // 7. Liberar toda la memoria dinámica utilizada en esta función.
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(coeficientes);
}