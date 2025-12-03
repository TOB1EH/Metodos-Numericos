/**
 * @file Raices.c
 * @brief Programa que localiza raíces de una función usando métodos numéricos clásicos.
 * @author Tobias Funes
 * @version 1.0
 * 
 * ============================================================================
 * TEORÍA GENERAL DE MÉTODOS NUMÉRICOS PARA RAÍCES
 * ============================================================================
 * Los métodos numéricos permiten encontrar aproximaciones a las raíces de funciones
 * cuando no es posible obtenerlas de forma analítica. Los métodos implementados aquí
 * son:
 *   - Bisección
 *   - Falsa Posición
 *   - Punto Fijo
 *   - Newton-Raphson
 *   - Secante
 * 
 * Cada método tiene ventajas y limitaciones. La elección depende de la función,
 * el intervalo y la precisión deseada.
 * 
 * ============================================================================
 * EJEMPLOS DE ITERACIÓN MANUAL
 * ============================================================================
 * MÉTODO DE BISECCIÓN
 * Suponga f(x) = x^2 - 4, intervalo [1, 3], tolerancia 0.01
 * Iteración 1: a=1, b=3, c=(1+3)/2=2, f(c)=0
 * Raíz encontrada: x=2
 * 
 * MÉTODO DE FALSA POSICIÓN
 * Suponga f(x) = x^2 - 4, intervalo [1, 3]
 * Iteración 1:
 *   f(1) = -3, f(3) = 5
 *   c = (1*5 - 3*(-3)) / (5 - (-3)) = (5 + 9) / 8 = 1.75
 *   f(1.75) = -0.9375
 *   Nuevo intervalo: [1.75, 3] (porque f(1.75) y f(3) tienen signos opuestos)
 * Iteración 2:
 *   Repetir el cálculo con los nuevos extremos.
 * 
 * MÉTODO DE PUNTO FIJO
 * Suponga f(x) = cos(x), buscamos la raíz de x = cos(x)
 * Iteración 1: x0 = 0.5, x1 = cos(0.5) ≈ 0.8776
 * Iteración 2: x2 = cos(0.8776) ≈ 0.6360
 * Iteración 3: x3 = cos(0.6360) ≈ 0.8047
 * Iterar hasta que |x_{n+1} - x_n| < tolerancia.
 * 
 * MÉTODO DE NEWTON-RAPHSON
 * Suponga f(x) = x^3 - x - 2, f'(x) = 3x^2 - 1, x0 = 1.5
 * Iteración 1:
 *   f(1.5) = 1.5^3 - 1.5 - 2 = -0.125
 *   f'(1.5) = 3*(1.5)^2 - 1 = 5.75
 *   x1 = 1.5 - (-0.125 / 5.75) ≈ 1.5217
 * Iteración 2:
 *   f(1.5217) ≈ 0.0021
 *   f'(1.5217) ≈ 5.949
 *   x2 = 1.5217 - (0.0021 / 5.949) ≈ 1.5214
 * Iterar hasta que |x_{n+1} - x_n| < tolerancia.
 * 
 * MÉTODO DE LA SECANTE
 * Suponga f(x) = exp(-x) - x, x0 = 0, x1 = 1
 * Iteración 1:
 *   f(0) = 1, f(1) ≈ -0.6321
 *   x2 = 1 - [f(1)*(1-0)] / [f(1)-f(0)] = 1 - [-0.6321*1] / [-0.6321-1] ≈ 0.6131
 * Iteración 2:
 *   f(0.6131) ≈ 0.0716
 *   x3 = 0.6131 - [0.0716*(0.6131-1)] / [0.0716-(-0.6321)] ≈ 0.5455
 * Iterar hasta que |x_{n+1} - x_n| < tolerancia.
 * 
 * ============================================================================
 * SECCIÓN DE NOTAS TEÓRICAS PERSONALES
 * ============================================================================
 * - El método de bisección es robusto pero lento.
 * - La falsa posición puede ser más rápida, pero depende de la elección inicial.
 * - El método de Newton-Raphson es rápido, pero requiere derivadas y una buena aproximación inicial.
 * - La secante no requiere derivadas, pero necesita dos puntos iniciales.
 * - El método de punto fijo requiere que |g'(x)| < 1 cerca de la raíz para garantizar convergencia.
 * ============================================================================
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "ctype.h"

#define MAX_ITER 10000 // Número máximo de iteraciones para evitar bucles infinitos

/**
 * @brief Muestra el menú de opciones y obtiene la selección del usuario.
 * @param opcion Puntero donde se guarda la opción elegida.
 */
void opcionMenu(char *opcion);

/**
 * @brief Calcula el valor de la función objetivo.
 * @param x Valor en el que se evalúa la función.
 * @return Resultado de la funcion f(x).
 */
double f(double x);

/**
 * @brief Calcula la derivada de la función objetivo.
 * @param x Valor en el que se evalúa la derivada de la función.
 * @return Resultado de la derivada de f en x.
 */
double f_prima(double x);

/**
 * @brief Calcula el valor de la función de iteración g(x) para el método de punto fijo.
 * @param x Valor en el que se evalúa la función.
 * @return Resultado de la funcion g(x).
 */
double g(double x);

/**
 * @brief Calcula la derivada de la función de iteración g(x).
 * @param x Valor en el que se evalúa la derivada de la función.
 * @return Resultado de la derivada de g en x.
 */
double g_prima(double x);


/**
 * @brief Ejecuta los métodos cerrados para encontrar raíces.
 * 
 * Métodos implementados:
 * - Bisección: Divide el intervalo y selecciona el subintervalo donde cambia el signo de f(x).
 * - Falsa Posición: Usa una interpolación lineal para aproximar la raíz.
 */
void metodosCerrados();

/**
 * @brief Ejecuta los métodos abiertos para encontrar raíces.
 * 
 * Métodos implementados:
 * - Punto Fijo
 * - Newton-Raphson
 * - Secante
 */
void metodosAbiertos();

/**
 * @brief Método de Bisección para encontrar raíces.
 * @details
 * Teoría: El método de bisección es un algoritmo de búsqueda de raíces que parte de un intervalo [a, b]
 * donde la función cambia de signo (f(a)*f(b)<0). En cada iteración, se calcula el punto medio c=(a+b)/2.
 * Si f(c) es cero, c es la raíz. Si no, se selecciona el subintervalo [a,c] o [c,b] donde la función sigue cambiando de signo.
 * El proceso se repite hasta que el intervalo sea suficientemente pequeño (según la tolerancia).
 */
void metodoBiseccion();

/**
 * @brief Método de Falsa Posición (Regla de la Secante) para encontrar raíces.
 * @details
 * Teoría: Similar a la bisección, pero en vez de tomar el punto medio, se calcula el punto de intersección
 * de la recta que une (a, f(a)) y (b, f(b)) con el eje x. Esto suele acelerar la convergencia si la función es casi lineal.
 * El nuevo punto c se calcula como: c = (a*f(b) - b*f(a)) / (f(b) - f(a)).
 * Se repite el proceso en el subintervalo donde la función cambia de signo.
 */
void metodoFalsaPosicion();

/**
 * @brief Método de Punto Fijo para encontrar raíces.
 * @details
 * Teoría: Reescribe la ecuación f(x)=0 como x=g(x). Se parte de una aproximación inicial x0 y se calcula x1=g(x0).
 * El proceso se repite: x_{n+1}=g(x_n). El método converge si |g'(x)|<1 cerca de la raíz.
 */
void puntoFijo();

/**
 * @brief Método de Newton-Raphson para encontrar raíces.
 * @details
 * Teoría: Usa la aproximación de Taylor para encontrar la raíz. Dado x_n, se calcula x_{n+1} = x_n - f(x_n)/f'(x_n).
 * El método converge rápidamente si la función es suave y la derivada no es cero cerca de la raíz.
 */
void newtonRaphson();

/**
 * @brief Método de la Secante para encontrar raíces.
 * @details
 * Teoría: Es una variante del método de Newton-Raphson que no requiere calcular la derivada.
 * Usa dos aproximaciones iniciales x0 y x1, y calcula la siguiente como:
 * x_{n+1} = x_n - f(x_n)*(x_n - x_{n-1})/(f(x_n) - f(x_{n-1})).
 * El proceso se repite hasta alcanzar la tolerancia.
 */
void secante();

int main(int argc, char const *argv[])
{
    char opcion = 0; // Opción del menú
    int stopDoWhile = 0; // Controla la salida del bucle principal

    /* Menu de opciones */
    do
    {
        system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
        printf("\n==================================================\n");
        printf("   MÉTODOS NUMÉRICOS - LOCALIZACIÓN DE RAÍCES\n");
        printf("==================================================\n");
        printf("Seleccione una opción:\n");
        printf("  a) Métodos Cerrados\n");
        printf("  b) Métodos Abiertos\n");
        printf("  c) Salir\n");
        printf("----------------------------------------\n");
        opcionMenu(&opcion);
        switch (opcion)
        {
        case 'a':
            metodosCerrados();
            break;
        case 'b':
            metodosAbiertos();
            break;
        case 'c':
            /* Salir del programa */
            printf("Saliendo del programa...\n");
            stopDoWhile = 1;
            break;
        default:
            printf("Opción no válida. Intente de nuevo.\n");
            break;
        }
    } while (stopDoWhile == 0);

    return 0;
}


void opcionMenu (char *opcion)
{
    printf("Ingrese una opcion: ");
    scanf(" %c", opcion);
    *opcion = tolower(*opcion); // coonvierte los caracteres a minusculas
}


double f(double x)
{
    // return sqrt(sin(sqrt(x)));
    // return (x * x - sin(sqrt(x)));
    //return log(x) + exp(sin(x)) - x;
    // return (2*x + log(x) - sin(3*x));
    //  return (exp(x*x) - 2);
    return (30.9504 * pow(x, 3) - 74.2202 * pow(x, 2) + 58.6036 * x - 15.3315);
}

double f_prima(double x)
{
    // Derivada numérica central para mayor precisión
    // double h = 1e-5;
    double h = 0.01;
    return (f(x + h) - f(x - h)) / (2 * h);
    // return (3*f(x) - 4*f(x - h) + f(x - 2*h)) / (2 * h);
}

double g(double x)
{
    // Ecuación f(x) = 0 -> 2*x + log(x) - sin(3*x) = 0
    // Despejando x: x = (sin(3*x) - log(x)) / 2
    return (sin(3 * x) - log(x)) / 2.0;
}

double g_prima(double x)
{
    // Derivada numérica de g(x)
    double h = 1e-5;
    // return (g(x + h) - g(x - h)) / (2 * h);
    return (3*g(x) - 4*g(x - h) + g(x - 2*h)) / (2 * h);
}

void metodosCerrados ()
{

    double a = 0.0; // Límite inferior
    double b = 0.0; // Límite superior
    double tol = 0.0; // Tolerancia para la convergencia
    double c = 0.0; // Raíz aproximada
    double c_anterior = 0.0; // Raíz aproximada en la iteración anterior
    double error_abs = 0.0; // Error absoluto
    double error_ptual = 0.0; // Error porcentual
    int i = 0; // Contador de iteraciones

    char opcion = 0; // Opción del menú
    int metodo = 0; // Método seleccionado: 1-Bisección, 2-Falsa Posición

    do
    {
        do
        {
            metodo = 0; // Reiniciar metodo
            system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
            printf("\n----------------------------------------\n");
            printf("   MÉTODOS CERRADOS\n");
            printf("----------------------------------------\n");
            printf("Seleccione un método:\n");
            printf("  a) Método de Bisección\n");
            printf("  b) Método de Falsa Posición\n");
            printf("  c) Volver al menú anterior\n");
            printf("----------------------------------------\n");
            opcionMenu(&opcion);
            switch (opcion)
            {
                case 'a':
                    metodo = 1; // Biseccion
                    break;
                case 'b':
                    metodo = 2; // Falsa Posicion
                    break;
                case 'c':
                    printf("Saliendo del menu de metodos cerrados...\n");
                    return;
                default:
                    printf("Opción no válida. Intente de nuevo.\n");
                    break;
            }
        } while (opcion != 'c' && metodo == 0);

        /* Solicitar los valores de a, b y tol */
        printf("\nIngrese el valor del limite inferior a: ");
        scanf("%lf", &a);
        printf("Ingrese el valor del limite superior b: ");
        scanf("%lf", &b);
        printf("Ingrese la tolerancia: ");
        scanf("%lf", &tol);

        // Verificar si f(a) y f(b) tienen signos opuestos (Existe la Raiz):
        if (f(a) * f(b) >= 0)
        {
            printf("\n[ERROR] No se puede aplicar los métodos en el intervalo [%.6lf, %.6lf], no hay raiz.\n", a, b);
            break; // Error: no se puede aplicar el método
        }

        c_anterior = a; // Inicializar c_anterior con el valor de a
        i = 0; // Reiniciar contador de iteraciones

        do
        {
            i++; // Incrementar contador de iteraciones

            /* Metodo de Biseccion */
            if (metodo == 1) c = (a + b) / 2.0; // Punto medio biseccion
            /* Metodo de Falsa Posicion */
            else if (metodo == 2) c = (a * f(b) - b * f(a)) / (f(b) - f(a)); // Punto de falsa posicion
            /* Metodo no valido */
            else {

                printf("Metodo no valido. Error Inesperado.\n");
                printf("Presione ENTER para continuar...");
                getchar(); // Captura el ENTER pendiente del scanf anterior
                getchar(); // Espera que el usuario presione ENTER
                
                system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
                return;
            }

            /* Actualizar Intervalo de la raiz */
            if ((f(a) * f(c)) > 0) a = c;
            else if ((f(a) * f(c)) < 0) b = c;
            else {
                /* La raiz es c */
                error_abs = 0.0; // Si f(c) es cero, la raiz es exacta
                error_ptual = 0.0;
                break;
            }

            /* Actualizar el error */
            error_abs = fabs(c - c_anterior); // Absoluto
            error_ptual = fabs((c - c_anterior) / c) * 100.0; // Porcentual

            /* Actualizar el punto anterior */
            c_anterior = c;

        } while (error_abs > tol);

        printf("\n========================================\n");
        printf("         RESULTADO DEL CÁLCULO\n");
        printf("========================================\n");
        printf("Raíz encontrada: %lf\n", c);
        printf("Error absoluto estimado: %lf\n", error_abs);
        printf("Error porcentual estimado: %lf %%\n", error_ptual);
        printf("Iteraciones: %d\n", i);
        printf("========================================\n");
        printf("Presione ENTER para continuar...");
        getchar(); // Captura el ENTER pendiente del scanf anterior
        getchar(); // Espera que el usuario presione ENTER
        
        system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"

    } while (opcion != 'c');
}

void metodosAbiertos ()
{
    char opcion = 0;

    do
    {
        system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
        printf("\n----------------------------------------\n");
        printf("   MÉTODOS ABIERTOS\n");
        printf("----------------------------------------\n");
        printf("Seleccione un método:\n");
        printf("  a) Método de Punto Fijo.\n");
        printf("  b) Método de Newton-Raphson.\n");
        printf("  c) Metodo de la Secante.\n");
        printf("  d) Volver al menú anterior.\n");
        printf("----------------------------------------\n");
        opcionMenu(&opcion);
        switch (opcion)
        {
            case 'a':
                puntoFijo();
                break;
            case 'b':
                newtonRaphson();
                break;
            case 'c':
                secante();
                break;
            case 'd':
                printf("Saliendo del menu de metodos abiertos...\n");
                return;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 'd');
}

void puntoFijo ()
{
    double x_0 = 0, x_1 = 0, error_abs = 0, error_ptual = 0, tol = 0;
    int i = 0;

    /* Solicitar los valores de x0 y tol */
    printf("\nIngrese el valor inicial x0: ");
    scanf("%lf", &x_0);
    printf("Ingrese la tolerancia: ");
    scanf("%lf", &tol);

    // Verificar la condición de convergencia UNA SOLA VEZ al inicio.
    if (fabs(g_prima(x_0)) >=1)
    {
        printf("\n[ERROR] No se puede aplicar el método de punto fijo en x0 = %.6lf, no converge.\n", x_0);
        printf("Presione ENTER para continuar...");
        getchar(); // Captura el ENTER pendiente del scanf anterior
        getchar(); // Espera que el usuario presione ENTER
        
        system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
        return; // Error: no se puede aplicar el método
    }

    i = 0; // Reiniciar contador de iteraciones

    do
    {
        if (fabs(g_prima(x_0)) >=1)
        {
            printf("\n[ERROR] No se puede aplicar el método de punto fijo en x0 = %.6lf, no converge.\n", x_0);
            printf("Presione ENTER para continuar...");
            getchar(); // Captura el ENTER pendiente del scanf anterior
            getchar(); // Espera que el usuario presione ENTER
            
            system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
            return; // Error: no se puede aplicar el método
        }
        i++; // Incrementar contador de iteraciones

        /* Calcular el siguiente punto */
        x_1 = g(x_0);

        /* Actualizar el error */
        error_abs = fabs(x_1 - x_0);
        error_ptual = fabs((x_1 - x_0) / x_1) * 100.0;

        /* Actualizar el punto anterior */
        x_0 = x_1;

    } while (error_abs > tol);

    printf("\n========================================\n");
    printf("         RESULTADO DEL CÁLCULO\n");
    printf("========================================\n");
    printf("Raíz encontrada: %lf\n", x_1);
    printf("Error absoluto estimado: %lf\n", error_abs);
    printf("Error porcentual estimado: %lf %%\n", error_ptual);
    printf("Iteraciones: %d\n", i);
    printf("========================================\n");
    printf("Presione ENTER para continuar...");
    getchar(); // Captura el ENTER pendiente del scanf anterior
    getchar(); // Espera que el usuario presione ENTER

    system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
}

void newtonRaphson ()
{
    double x_0 = 0, x_1 = 0, error_abs = 0, error_ptual = 0, tol = 0;
    int i = 0;

    /* Solicitar los valores de x0 y tol */
    printf("\nIngrese el valor inicial x0: ");
    scanf("%lf", &x_0);
    printf("Ingrese la tolerancia: ");
    scanf("%lf", &tol);

    i = 0; // Reiniciar contador de iteraciones

    do
    {
        if (f(x_0) == 0)
        {
            error_abs = 0.0; // Si f(x0) es cero, la raiz es exacta
            error_ptual = 0.0;
            break;;
        }
        else if (fabs(f_prima(x_0)) < 10e-4)
        {
            printf("\n[WARNING] No se puede aplicar el método de Newton-Raphson en x0 = %.6lf, la derivada tiende a cero.\n", x_0);
            printf("Presione ENTER para continuar...");
            getchar(); // Captura el ENTER pendiente del scanf anterior
            getchar(); // Espera que el usuario presione ENTER

            system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
            return; // Error: no se puede aplicar el método
        }
        else if (f_prima(x_0) == 0)
        {
            printf("\n[ERROR] No se puede aplicar el método de Newton-Raphson en x0 = %.6lf, derivada es cero.\n", x_0);
            printf("Presione ENTER para continuar...");
            getchar(); // Captura el ENTER pendiente del scanf anterior
            getchar(); // Espera que el usuario presione ENTER

            system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
            return; // Error: no se puede aplicar el método
        }

        i++; // Incrementar contador de iteraciones

        /* Calcular el siguiente punto */
        x_1 = x_0 - f(x_0) / f_prima(x_0);

        /* Actualizar el error */
        error_abs = fabs(x_1 - x_0); // Absoluto
        error_ptual = fabs((x_1 - x_0) / x_1) * 100.0; // Porcentual

        /* Actualizar el punto anterior */
        x_0 = x_1;

    } while (error_abs > tol && i < MAX_ITER);

    printf("\n========================================\n");
    printf("         RESULTADO DEL CÁLCULO\n");
    printf("========================================\n");
    printf("Raíz encontrada: %lf\n", x_1);
    printf("Error absoluto estimado: %lf\n", error_abs);
    printf("Error porcentual estimado: %lf %%\n", error_ptual);
    printf("Iteraciones: %d\n", i);
    if (fabs(f(x_1)) < 0.01)
        printf("Criterio de parada: tolerancia alcanzada.\n");
    else
        printf("Criterio de parada: máximo de iteraciones alcanzado.\n");
    printf("========================================\n");
    printf("Presione ENTER para continuar...");
    getchar(); // Captura el ENTER pendiente del scanf anterior
    getchar(); // Espera que el usuario presione ENTER

    system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
}

void secante ()
{
    double x_0 = 0, x_1 = 0, x_2 = 0, error_abs = 0, error_ptual = 0, tol = 0;
    int i = 0;

    /* Solicitar los valores de x0 y tol */
    printf("\nIngrese el valor inicial x0: ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor inicial x1: ");
    scanf("%lf", &x_1);
    printf("Ingrese la tolerancia: ");
    scanf("%lf", &tol);

    do
    {
        if (f(x_1) - f(x_0) == 0 || fabs(f(x_1) - f(x_0)) < 10e-4)
        {
            printf("\n[ERROR] No se puede aplicar el método de la secante en x0 = %.6lf y x1 = %.6lf, división por cero.\n", x_0, x_1);
            printf("Presione ENTER para continuar...");
            getchar(); // Captura el ENTER pendiente del scanf anterior
            getchar(); // Espera que el usuario presione ENTER
            
            system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
            return; // Error: no se puede aplicar el método
        }

        /* Calcular el siguiente punto */
        x_2 = x_1 - (f(x_1) * (x_1 - x_0) / (f(x_1) - f(x_0)));

        /* Actualizar el error */
        error_abs = fabs(x_2 - x_1); // Absoluto
        error_ptual = fabs((x_2 - x_1) / x_2 ) * 100.0; // Porcentual

        /* Actualizar los puntos */
        x_0 = x_1;
        x_1 = x_2;

        i++; // Incrementar contador de iteraciones

    } while (error_abs > tol && i < MAX_ITER);
    
    printf("\n========================================\n");
    printf("         RESULTADO DEL CÁLCULO\n");
    printf("========================================\n");
    printf("Raíz encontrada: %lf\n", x_2);
    printf("Error absoluto estimado: %lf\n", error_abs);
    printf("Error porcentual estimado: %lf %%\n", error_ptual);
    printf("Iteraciones: %d\n", i);
    if (fabs(f(x_2)) < 0.01)
        printf("Criterio de parada: tolerancia alcanzada. El metodo correctamente.\n");
    else
        printf("Criterio de parada: máximo de iteraciones alcanzado.\n");
    printf("========================================\n");
    printf("Presione ENTER para continuar...");
    getchar(); // Captura el ENTER pendiente del scanf anterior
    getchar(); // Espera que el usuario presione ENTER

    system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
}

/**
 * Funciones ejemplo para probar cada método:
 * 
 * Bisección y Falsa Posición
 *  f(x) = x^2 - 4
 *  Raíz en x = 2
 *  Intervalo: [1, 3]
 * 
 * Punto Fijo
 *  f(x) = cos(x)
 *  Raíz cerca de x = 0.739
 *  Puedes usar g(x) = cos(x) como función de iteración.
 * 
 * Newton-Raphson
 *  f(x) = x^3 - x - 2
 *  Raíz cerca de x = 1.521
 *  Derivada: f'(x) = 3x^2 - 1
 * 
 * Secante
 *  f(x) = exp(-x) - x
 *  Raíz cerca de x = 0.567
 *  Usa x0 = 0, x1 = 1
 * 
 */