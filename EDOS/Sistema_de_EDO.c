/**
 * @file Sistema_de_EDO.c
 * @brief Programa para resolver sistemas de ecuaciones diferenciales ordinarias (EDOs)
 *        de primer orden usando métodos numéricos.
 * @author Tobias Funes
 * @version 1.0
 * 
 * ============================================================================
 * TEORÍA DE SISTEMAS DE EDOs
 * ============================================================================
 * Un sistema de ecuaciones diferenciales ordinarias de primer orden tiene la forma:
 * 
 *   dy1/dx = f1(x, y1, y2)
 *   dy2/dx = f2(x, y1, y2)
 * 
 * con condiciones iniciales:
 *   y1(x0) = y1_0
 *   y2(x0) = y2_0
 * 
 * Los métodos numéricos para EDOs simples se pueden extender naturalmente para
 * resolver sistemas de EDOs aplicando el mismo esquema de actualización a cada
 * ecuación del sistema.
 * 
 * MÉTODOS IMPLEMENTADOS:
 * 
 * 1. MÉTODO DE EULER PARA SISTEMAS
 *    Fórmulas:
 *      y1[i+1] = y1[i] + h · f1(x[i], y1[i], y2[i])
 *      y2[i+1] = y2[i] + h · f2(x[i], y1[i], y2[i])
 *    
 *    Características:
 *    - Extensión directa del método de Euler para EDOs simples
 *    - Ambas ecuaciones se actualizan usando los valores en el mismo punto
 *    - Orden de error: O(h²) local, O(h) global
 *    - Simple pero menos preciso
 * 
 * 2. MÉTODO DE RUNGE-KUTTA 4 PARA SISTEMAS (RK4)
 *    Para cada variable y1 y y2 se calculan cuatro coeficientes k:
 *    
 *    k1_1 = f1(x[i], y1[i], y2[i])
 *    k1_2 = f2(x[i], y1[i], y2[i])
 *    
 *    k2_1 = f1(x[i] + h/2, y1[i] + h/2·k1_1, y2[i] + h/2·k1_2)
 *    k2_2 = f2(x[i] + h/2, y1[i] + h/2·k1_1, y2[i] + h/2·k1_2)
 *    
 *    k3_1 = f1(x[i] + h/2, y1[i] + h/2·k2_1, y2[i] + h/2·k2_2)
 *    k3_2 = f2(x[i] + h/2, y1[i] + h/2·k2_1, y2[i] + h/2·k2_2)
 *    
 *    k4_1 = f1(x[i] + h, y1[i] + h·k3_1, y2[i] + h·k3_2)
 *    k4_2 = f2(x[i] + h, y1[i] + h·k3_1, y2[i] + h·k3_2)
 *    
 *    Actualización:
 *    y1[i+1] = y1[i] + (h/6)·(k1_1 + 2k2_1 + 2k3_1 + k4_1)
 *    y2[i+1] = y2[i] + (h/6)·(k1_2 + 2k2_2 + 2k3_2 + k4_2)
 *    
 *    Características:
 *    - Los coeficientes k se calculan para AMBAS variables simultáneamente
 *    - Cada etapa usa los valores actualizados de TODAS las variables
 *    - Orden de error: O(h⁵) local, O(h⁴) global
 *    - Mucho más preciso que Euler
 * 
 * EJEMPLO DEL SISTEMA IMPLEMENTADO:
 *   dy1/dx = 3x + y2
 *   dy2/dx = x² - y1 - 1
 * 
 * Este es un sistema acoplado porque:
 * - La ecuación de y1 depende de y2
 * - La ecuación de y2 depende de y1
 * - Deben resolverse simultáneamente
 * 
 * SALIDA DEL PROGRAMA:
 * - Archivo 'sistema_y1.txt': contiene x[i] vs y1[i]
 * - Archivo 'sistema_y2.txt': contiene x[i] vs y2[i]
 * - Tabla en consola con ambas variables
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Prototipos de funciones */
double f1(double x, double y1, double y2);
double f2(double x, double y1, double y2);

/**
 * @brief Función principal que resuelve el sistema de EDOs.
 * @details
 * Esta función:
 * 1. Solicita al usuario las condiciones iniciales y parámetros de integración
 * 2. Permite elegir entre el método de Euler o RK4
 * 3. Resuelve el sistema de EDOs usando el método seleccionado
 * 4. Guarda los resultados en dos archivos separados (uno por variable)
 * 5. Muestra una tabla con los resultados en consola
 */
int main(void)
{
    char opcion;

    /* ==========================================
       DEFINIR VARIABLES DEL PROBLEMA
       ========================================== */
    
    /* Límites del intervalo de integración */
    double x_0 = 0.0;  /* Punto inicial */
    double x_f = 0.0;  /* Punto final */

    /* Condiciones iniciales para las variables dependientes */
    double y1_0 = 0.0;  /* y1(x0) */
    double y2_0 = 0.0;  /* y2(x0) */

    /* Arreglos para almacenar la solución */
    double * x = NULL;   /* Variable independiente */
    double * y1 = NULL;  /* Primera variable dependiente */
    double * y2 = NULL;  /* Segunda variable dependiente */

    int n = 0;      /* Número de subintervalos */
    double h = 0.0; /* Tamaño del paso: h = (xf - x0) / n */

    /* ==========================================
       ENTRADA DE DATOS
       ========================================== */
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   SISTEMA DE ECUACIONES DIFERENCIALES     ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y1 (y1_0): ");
    scanf("%lf", &y1_0);
    printf("Ingrese el valor inicial de y2 (y2_0): ");
    scanf("%lf", &y2_0);
    
    /* ==========================================
       SELECCIÓN DE MÉTODO DE ENTRADA (h o n)
       ========================================== */
    char opcion_entrada;
    printf("\n¿Cómo desea especificar el paso de integración?\n");
    printf("  a) Ingresar el número de pasos (n)\n");
    printf("  b) Ingresar el tamaño del paso (h)\n");
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion_entrada);
    
    if (opcion_entrada == 'a' || opcion_entrada == 'A') {
        /* Usuario ingresa n, calculamos h */
        printf("Ingrese el número de pasos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; /* Calcular el tamaño del paso */
        printf("→ Tamaño del paso calculado: h = %.6lf\n", h);
    } else if (opcion_entrada == 'b' || opcion_entrada == 'B') {
        /* Usuario ingresa h, calculamos n */
        printf("Ingrese el tamaño del paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); /* Calcular el número de pasos */
        
        /* Ajustar h para que sea exacto */
        h = (x_f - x_0) / n;
        printf("→ Número de pasos calculado: n = %d\n", n);
        printf("→ Tamaño del paso ajustado: h = %.6lf\n", h);
    } else {
        printf("Opción no válida. Usando opción por defecto: ingresar n\n");
        printf("Ingrese el número de pasos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; /* Calcular el tamaño del paso */
        printf("→ Tamaño del paso calculado: h = %.6lf\n", h);
    }

    /* ==========================================
       RESERVAR MEMORIA DINÁMICA
       ========================================== */
    /* Se necesita (n+1) elementos para incluir x0 y xf */
    x = (double *)malloc((n + 1) * sizeof(double));
    y1 = (double *)malloc((n + 1) * sizeof(double));
    y2 = (double *)malloc((n + 1) * sizeof(double));

    /* ==========================================
       ESTABLECER CONDICIONES INICIALES
       ========================================== */
    x[0] = x_0;
    y1[0] = y1_0;
    y2[0] = y2_0;

    /* ==========================================
       MENÚ DE SELECCIÓN DE MÉTODO
       ========================================== */
    do
    {
        printf("\nSeleccione el método para resolver el sistema de EDOs:\n");
        printf("a) Método de Euler\n");
        printf("b) Método de Runge-Kutta 4to Orden (RK4)\n");
        printf("c) Salir\n");
        printf("Ingrese su opción: ");
        scanf("%c", &opcion);
        switch (opcion)
        {
        case 'a':
            /* ==========================================
               MÉTODO DE EULER PARA SISTEMAS
               ========================================== */
            /* 
             * Esquema de Euler aplicado a cada ecuación del sistema:
             * y1[i+1] = y1[i] + h · f1(x[i], y1[i], y2[i])
             * y2[i+1] = y2[i] + h · f2(x[i], y1[i], y2[i])
             * 
             * Nota: Ambas ecuaciones se evalúan en el MISMO punto (x[i], y1[i], y2[i])
             *       antes de actualizar a los nuevos valores.
             */
            for (int i = 0; i < n; i++)
            {
                x[i + 1] = x[i] + h;  /* Avanzar en la variable independiente */

                /* Actualizar y1 usando f1 */
                y1[i + 1] = y1[i] + h * f1(x[i], y1[i], y2[i]);

                /* Actualizar y2 usando f2 */
                y2[i + 1] = y2[i] + h * f2(x[i], y1[i], y2[i]);
            }

            printf("\n✓ Método de Euler completado exitosamente.\n");
            opcion = 'c';  /* Salir después de completar Euler */
            break;
        case 'b':
            /* ==========================================
               MÉTODO DE RUNGE-KUTTA 4 PARA SISTEMAS
               ========================================== */
            /*
             * RK4 aplicado a sistemas de EDOs requiere calcular coeficientes k
             * para CADA variable del sistema (k_11, k_12 para y1 y y2).
             * 
             * Los subíndices indican:
             * k_ij donde i = etapa (1,2,3,4) y j = variable (1 o 2)
             * 
             * Ejemplo: k_21 = coeficiente k2 para la variable y1
             */
            double k_11, k_12, k_21, k_22, k_31, k_32, k_41, k_42;

            for (int i = 0; i < n; i++)
            {
                /* ===== ETAPA 1: Pendientes al inicio del intervalo ===== */

                /* k1: Pendiente al inicio del intervalo */
                k_11 = f1(x[i], y1[i], y2[i]);
                k_12 = f2(x[i], y1[i], y2[i]);

                /* k2: Pendiente en el punto medio usando k1 */
                k_21 = f1(x[i] + h / 2.0, y1[i] + (h / 2.0) * k_11, y2[i] + (h / 2.0) * k_12);
                k_22 = f2(x[i] + h / 2.0, y1[i] + (h / 2.0) * k_11, y2[i] + (h / 2.0) * k_12);

                /* k3: Pendiente en el punto medio usando k2 (más preciso) */
                k_31 = f1(x[i] + h / 2.0, y1[i] + (h / 2.0) * k_21, y2[i] + (h / 2.0) * k_22);
                k_32 = f2(x[i] + h / 2.0, y1[i] + (h / 2.0) * k_21, y2[i] + (h / 2.0) * k_22);

                /* k4: Pendiente al final del intervalo usando k3 */
                k_41 = f1(x[i] + h, y1[i] + h * k_31, y2[i] + h * k_32);
                k_42 = f2(x[i] + h, y1[i] + h * k_31, y2[i] + h * k_32);

                /* Avanzar en x */
                x[i + 1] = x[i] + h;

                /* Actualizar y1 y y2 */
                y1[i + 1] = y1[i] + (h / 6.0) * (k_11 + 2.0 * k_21 + 2.0 * k_31 + k_41);
                y2[i + 1] = y2[i] + (h / 6.0) * (k_12 + 2.0 * k_22 + 2.0 * k_32 + k_42);
            }
            
            printf("\n✓ Método de Runge-Kutta 4 completado exitosamente.\n");
            opcion = 'c';  /* Salir después de completar RK4 */
            break;
            
        default:
            printf("Opción no válida. Intente de nuevo.\n");
            break;
        }
    } while (opcion != 'c');

    /* ==========================================
       GUARDAR RESULTADOS EN ARCHIVOS
       ========================================== */
    /*
     * Se crean dos archivos separados:
     * 1. sistema_y1.txt: Contiene la evolución de y1 vs x
     * 2. sistema_y2.txt: Contiene la evolución de y2 vs x
     * 
     * Esto facilita el análisis y graficación independiente de cada variable.
     */

    /* ===== ARCHIVO 1: Evolución de y1 ===== */
    FILE *archivo1 = fopen("sistema_y1.txt", "w");
    if (archivo1 == NULL)
    {
        printf("\n✗ Error: No se pudo crear el archivo 'sistema_y1.txt'\n");
    }
    else
    {
        /* Escribir encabezado con información del problema */
        fprintf(archivo1, "# Resultados del Sistema de EDOs - Variable y1\n");
        fprintf(archivo1, "# dy1/dx = f1(x, y1, y2)\n");
        fprintf(archivo1, "# dy2/dx = f2(x, y1, y2)\n");
        fprintf(archivo1, "# Condiciones iniciales: y1(%.6lf) = %.6lf, y2(%.6lf) = %.6lf\n", 
                x_0, y1_0, x_0, y2_0);
        fprintf(archivo1, "# Intervalo: [%.6lf, %.6lf]\n", x_0, x_f);
        fprintf(archivo1, "# Paso h: %.6lf\n", h);
        fprintf(archivo1, "# Número de pasos: %d\n", n);
        fprintf(archivo1, "#\n");
        fprintf(archivo1, "# i\tx_i\ty1_i\n");

        /* Escribir datos tabulados */
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo1, "%d\t%.10lf\t%.10lf\n", i, x[i], y1[i]);
        }

        fclose(archivo1);
        printf("\n✓ Resultados de y1 guardados en 'sistema_y1.txt'\n");
    }

    /* ===== ARCHIVO 2: Evolución de y2 ===== */
    FILE *archivo2 = fopen("sistema_y2.txt", "w");
    if (archivo2 == NULL)
    {
        printf("\n✗ Error: No se pudo crear el archivo 'sistema_y2.txt'\n");
    }
    else
    {
        /* Escribir encabezado con información del problema */
        fprintf(archivo2, "# Resultados del Sistema de EDOs - Variable y2\n");
        fprintf(archivo2, "# dy1/dx = f1(x, y1, y2)\n");
        fprintf(archivo2, "# dy2/dx = f2(x, y1, y2)\n");
        fprintf(archivo2, "# Condiciones iniciales: y1(%.6lf) = %.6lf, y2(%.6lf) = %.6lf\n", 
                x_0, y1_0, x_0, y2_0);
        fprintf(archivo2, "# Intervalo: [%.6lf, %.6lf]\n", x_0, x_f);
        fprintf(archivo2, "# Paso h: %.6lf\n", h);
        fprintf(archivo2, "# Número de pasos: %d\n", n);
        fprintf(archivo2, "#\n");
        fprintf(archivo2, "# i\tx_i\ty2_i\n");

        /* Escribir datos tabulados */
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo2, "%d\t%.10lf\t%.10lf\n", i, x[i], y2[i]);
        }

        fclose(archivo2);
        printf("✓ Resultados de y2 guardados en 'sistema_y2.txt'\n");
    }

    /* ==========================================
       MOSTRAR RESULTADOS EN PANTALLA
       ========================================== */
    /*
     * Se muestra una tabla con las tres columnas:
     * - Índice i
     * - Valor de x en el paso i
     * - Valor de y1 en el paso i
     * - Valor de y2 en el paso i
     */
    printf("\n════════════════════════════════════════════════════════════\n");
    printf("  RESULTADOS - SISTEMA DE EDOs\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("Paso h: %.6lf\n", h);
    printf("Número de pasos: %d\n\n", n);
    printf("  i      x_i          y1_i            y2_i\n");
    printf("────────────────────────────────────────────────────────────\n");

    /* Imprimir cada fila de la tabla */
    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf | %12.8lf\n", i, x[i], y1[i], y2[i]);
    }
    printf("════════════════════════════════════════════════════════════\n");

    /* ==========================================
       LIBERAR MEMORIA DINÁMICA
       ========================================== */
    /*
     * Es importante liberar toda la memoria reservada con malloc
     * para evitar fugas de memoria (memory leaks).
     */
    free(x);
    free(y1);
    free(y2);

    /* Pausar antes de salir para que el usuario pueda ver los resultados */
    printf("\nPresione ENTER para continuar...");
    getchar();  /* Captura el ENTER pendiente del scanf anterior */
    getchar();  /* Espera que el usuario presione ENTER */

    return 0;
}

/**
 * @brief Primera ecuación del sistema de EDOs.
 * @details
 * Define la derivada de y1 con respecto a x:
 *   dy1/dx = f1(x, y1, y2) = 3x + y2
 * 
 * Esta ecuación muestra que la tasa de cambio de y1 depende de:
 * - La variable independiente x (término 3x)
 * - La otra variable dependiente y2 (acoplamiento entre ecuaciones)
 * 
 * El término y1 no aparece en esta ecuación (parámetro no usado),
 * pero se mantiene en la firma de la función para mantener
 * consistencia con el formato estándar f(x, y1, y2).
 * 
 * @param x Valor de la variable independiente
 * @param y1 Valor de la primera variable dependiente (no usado en esta ecuación)
 * @param y2 Valor de la segunda variable dependiente
 * @return El valor de dy1/dx en el punto (x, y1, y2)
 */
double f1(double x, double y1, double y2)
{
    // (void)y1;  /* Suprimir warning de parámetro no usado */
    // return 3 * x + y2;

    (void)x;   // No se usa x
    (void)y1;  // No se usa y1
    return y2;  // dy₁/dx = y₂
}

/**
 * @brief Segunda ecuación del sistema de EDOs.
 * @details
 * Define la derivada de y2 con respecto a x:
 *   dy2/dx = f2(x, y1, y2) = x² - y1 - 1
 * 
 * Esta ecuación muestra que la tasa de cambio de y2 depende de:
 * - La variable independiente x (término x²)
 * - La otra variable dependiente y1 (acoplamiento entre ecuaciones)
 * - Una constante (-1)
 * 
 * El término y2 no aparece en esta ecuación (parámetro no usado),
 * pero se mantiene en la firma de la función para mantener
 * consistencia con el formato estándar f(x, y1, y2).
 * 
 * ACOPLAMIENTO DEL SISTEMA:
 * - f1 depende de y2
 * - f2 depende de y1
 * Por lo tanto, las ecuaciones están acopladas y deben resolverse
 * simultáneamente. No pueden resolverse de forma independiente.
 * 
 * @param x Valor de la variable independiente
 * @param y1 Valor de la primera variable dependiente
 * @param y2 Valor de la segunda variable dependiente (no usado en esta ecuación)
 * @return El valor de dy2/dx en el punto (x, y1, y2)
 */
double f2(double x, double y1, double y2)
{
    // (void)y2;  /* Suprimir warning de parámetro no usado */
    // return pow(x, 2) - y1 - 1;

    (void)x;  // No se usa x
    return -2.0 * y2 - 5.0 * y1;  // dy₂/dx = -2y₂ - 5y₁
}
