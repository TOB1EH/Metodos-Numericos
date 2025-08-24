/**
 * @brief Programa que Localiza Raíces de una funcion a traves de diferentes metodos numericos
 * @file Raices.c
 * @author Tobias Funes
 * @version 0.1
 * 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "ctype.h"

/**
 * @brief Muestra el menú de opciones y obtiene la selección del usuario.
 * @param opcion Puntero donde se guarda la opción elegida.
 */
void opcionMenu(char *opcion);

/**
 * @brief Calcula el valor de la función objetivo.
 * @param x Valor en el que se evalúa la función.
 * @return Resultado de log(x) + exp(sin(x)) - x.
 */
double f(double x);

/**
 * @brief Ejecuta los métodos cerrados para encontrar raíces.
 */
void metodosCerrados();


int main(int argc, char const *argv[])
{
    char opcion = 0;
    int stopDoWhile = 0;
    
    /* Menu de opciones */
    do
    {
        printf("\n==================================================\n");
        printf("   MÉTODOS NUMÉRICOS - LOCALIZACIÓN DE RAÍCES\n");
        printf("==================================================\n");
        printf("Seleccione una opción:\n");
        printf("  a) Métodos Cerrados\n");
        printf("  c) Salir\n");
        printf("----------------------------------------\n");
        opcionMenu(&opcion);
        switch (opcion)
        {
        case 'a':
            metodosCerrados();
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
    return log(x) + exp(sin(x)) - x;
}

void metodosCerrados () 
{

    double a = 0.0; // Límite inferior
    double b = 0.0; // Límite superior
    double tol = 0.0; // Tolerancia para la convergencia
    double c = 0.0;
    double c_anterior = 0.0;
    double error = 0.0;
    int i = 0;

    char opcion = 0;
    int metodo = 0;

    do
    {
        do
        {
            metodo = 0; // Reiniciar metodo
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
            i++;

            /* Metodo de Biseccion */
            if (metodo == 1) c = (a + b) / 2.0; // Punto medio biseccion
            /* Metodo de Falsa Posicion */
            else if (metodo == 2) c = (a * f(b) - b * f(a)) / (f(b) - f(a)); // Punto de falsa posicion
            /* Metodo no valido */
            else {
                printf("Metodo no valido. Error Inesperado.\n");
                return;
            }

            /* Actualizar Intervalo de la raiz */
            if ((f(a) * f(c)) > 0) a = c;
            else if ((f(a) * f(c)) < 0) b = c;
            else {
                /* La raiz es c */
                error = 0.0; // Si f(c) es cero, la raiz es exacta
                break;
            }

            /* Actualizar el error */
            error = fabs(c - c_anterior);

            /* Actualizar el punto anterior */
            c_anterior = c;

        } while (error > tol);

        printf("\n========================================\n");
        printf("         RESULTADO DEL CÁLCULO\n");
        printf("========================================\n");
        printf("Raíz encontrada: %lf\n", c);
        printf("Error estimado: %lf\n", error);
        printf("Iteraciones: %d\n", i);
        printf("========================================\n");

    } while (opcion != 'c');
}