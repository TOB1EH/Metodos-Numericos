/**
 * @file EDOS.c
 * @brief Programa para resolver Ecuaciones Diferenciales Ordinarias (EDO) de primer orden
 *        usando diversos métodos numéricos.
 * @author Tobias Funes
 * @version 1.0
 * 
 * ============================================================================
 * TEORÍA GENERAL DE MÉTODOS NUMÉRICOS PARA EDOs
 * ============================================================================
 * Los métodos numéricos permiten resolver ecuaciones diferenciales ordinarias
 * de primer orden de la forma:
 *     dy/dx = f(x, y)
 * con condición inicial y(x₀) = y₀
 * 
 * cuando no es posible obtener una solución analítica exacta.
 * 
 * Los métodos implementados en este programa son:
 * 
 * 1. MÉTODO DE EULER (Orden 1)
 *    - El más simple de todos los métodos
 *    - Fórmula: y[i+1] = y[i] + h·f(x[i], y[i])
 *    - Error global: O(h²)
 *    - Ventaja: Muy fácil de implementar y entender
 *    - Desventaja: Menos preciso, requiere pasos pequeños
 * 
 * 2. MÉTODO DE HEUN (Euler Mejorado, Orden 2)
 *    - Método predictor-corrector
 *    - Predictor: y* = y[i] + h·f(x[i], y[i])
 *    - Corrector: y[i+1] = y[i] + (h/2)·[f(x[i], y[i]) + f(x[i+1], y*)]
 *    - Error global: O(h³)
 *    - Ventaja: Más preciso que Euler, usa evaluaciones múltiples
 *    - Desventaja: Requiere dos evaluaciones de f por paso
 * 
 * 3. MÉTODO DEL PUNTO MEDIO (Orden 2)
 *    - Evalúa la pendiente en el punto medio del intervalo
 *    - k1 = f(x[i], y[i])
 *    - k2 = f(x[i] + h/2, y[i] + (h/2)·k1)
 *    - y[i+1] = y[i] + h·k2
 *    - Error global: O(h³)
 *    - Ventaja: Buena precisión con dos evaluaciones
 *    - Desventaja: Necesita calcular pendientes intermedias
 * 
 * 4. MÉTODO DE RUNGE-KUTTA 4TO ORDEN (RK4, Orden 4)
 *    - El más preciso de los métodos clásicos
 *    - Usa cuatro evaluaciones de la función por paso
 *    - k1 = f(x[i], y[i])
 *    - k2 = f(x[i] + h/2, y[i] + (h/2)·k1)
 *    - k3 = f(x[i] + h/2, y[i] + (h/2)·k2)
 *    - k4 = f(x[i] + h, y[i] + h·k3)
 *    - y[i+1] = y[i] + (h/6)·(k1 + 2k2 + 2k3 + k4)
 *    - Error global: O(h⁵)
 *    - Ventaja: Excelente precisión, estándar en la industria
 *    - Desventaja: Requiere cuatro evaluaciones de f por paso
 * 
 * ============================================================================
 * ANÁLISIS DE ERROR Y CONVERGENCIA
 * ============================================================================
 * 
 * TIPOS DE ERROR:
 * 1. Error de Truncamiento Local: Error cometido en un solo paso
 * 2. Error de Truncamiento Global: Error acumulado en todo el intervalo
 * 3. Error de Redondeo: Error debido a la precisión finita de la computadora
 * 
 * ANÁLISIS DE ERROR (implementado en método de Euler):
 * - Error Absoluto: |y_exacta - y_aproximada|
 * - Error Relativo: |y_exacta - y_aproximada| / |y_exacta| × 100%
 * - Error Máximo: max(|y_exacta[i] - y_aproximada[i]|) para todo i
 * - Error Promedio: promedio de errores absolutos
 * 
 * FACTOR DE CONVERGENCIA (Q):
 * Fórmula: Q = (1/ln(2)) · ln(|y₁-y₂| / |y₂-y₃|)
 * donde:
 *   y₁ = aproximación con paso h
 *   y₂ = aproximación con paso h/2
 *   y₃ = aproximación con paso h/4
 * 
 * Interpretación:
 *   Q ≈ 1 → Método de orden 1 (Euler)
 *   Q ≈ 2 → Método de orden 2 (Heun, Punto Medio)
 *   Q ≈ 4 → Método de orden 4 (Runge-Kutta 4)
 * 
 * Esta técnica NO requiere conocer la solución exacta y permite
 * verificar experimentalmente el orden teórico del método.
 * 
 * ============================================================================
 * EJEMPLO DE EDO RESUELTA: dy/dx = -2xy
 * ============================================================================
 * 
 * SOLUCIÓN ANALÍTICA (Método de Separación de Variables):
 * 
 * Paso 1: Separar variables
 *   dy/dx = -2xy
 *   dy/y = -2x dx
 * 
 * Paso 2: Integrar ambos lados
 *   ∫(dy/y) = ∫(-2x dx)
 *   ln|y| = -x² + C
 * 
 * Paso 3: Despejar y
 *   y = e^(-x² + C) = e^C · e^(-x²) = A · e^(-x²)
 * 
 * Paso 4: Aplicar condición inicial y(x₀) = y₀
 *   y₀ = A · e^(-x₀²)
 *   A = y₀ · e^(x₀²)
 * 
 * Paso 5: Solución final
 *   y(x) = y₀ · e^(-x² + x₀²)
 * 
 * EJEMPLO NUMÉRICO:
 * Si x₀ = 0, y₀ = 1, entonces:
 *   y(x) = e^(-x²)
 * 
 * Valores particulares:
 *   y(0.5) = e^(-0.25) ≈ 0.7788
 *   y(1.0) = e^(-1) ≈ 0.3679
 *   y(1.5) = e^(-2.25) ≈ 0.1054
 * 
 * ============================================================================
 * EJEMPLO DE ITERACIÓN MANUAL - MÉTODO DE EULER
 * ============================================================================
 * Problema: dy/dx = -2xy, y(0) = 1, calcular y(0.2) con h = 0.1
 * 
 * Iteración 0:
 *   x₀ = 0, y₀ = 1
 *   f(x₀, y₀) = -2(0)(1) = 0
 * 
 * Iteración 1:
 *   y₁ = y₀ + h·f(x₀, y₀) = 1 + 0.1·0 = 1
 *   x₁ = x₀ + h = 0 + 0.1 = 0.1
 *   f(x₁, y₁) = -2(0.1)(1) = -0.2
 * 
 * Iteración 2:
 *   y₂ = y₁ + h·f(x₁, y₁) = 1 + 0.1·(-0.2) = 0.98
 *   x₂ = 0.2
 * 
 * Resultado: y(0.2) ≈ 0.98
 * Solución exacta: y(0.2) = e^(-0.04) ≈ 0.9608
 * Error absoluto: |0.9608 - 0.98| ≈ 0.0192
 * 
 * ============================================================================
 * EJEMPLO DE ITERACIÓN MANUAL - MÉTODO DE HEUN
 * ============================================================================
 * Problema: dy/dx = -2xy, y(0) = 1, calcular y(0.1) con h = 0.1
 * 
 * Iteración 1:
 *   x₀ = 0, y₀ = 1
 *   
 *   PREDICTOR:
 *   f(x₀, y₀) = -2(0)(1) = 0
 *   y* = y₀ + h·f(x₀, y₀) = 1 + 0.1·0 = 1
 *   
 *   CORRECTOR:
 *   x₁ = 0.1
 *   f(x₁, y*) = -2(0.1)(1) = -0.2
 *   y₁ = y₀ + (h/2)·[f(x₀, y₀) + f(x₁, y*)]
 *      = 1 + (0.1/2)·[0 + (-0.2)]
 *      = 1 + 0.05·(-0.2)
 *      = 1 - 0.01 = 0.99
 * 
 * Resultado: y(0.1) ≈ 0.99
 * Solución exacta: y(0.1) = e^(-0.01) ≈ 0.9900
 * Error: ≈ 0 (muy preciso)
 * 
 * ============================================================================
 * EJEMPLO DE ITERACIÓN MANUAL - MÉTODO DE RUNGE-KUTTA 4
 * ============================================================================
 * Problema: dy/dx = -2xy, y(0) = 1, calcular y(0.1) con h = 0.1
 * 
 * Iteración 1:
 *   x₀ = 0, y₀ = 1, h = 0.1
 *   
 *   k1 = f(0, 1) = -2(0)(1) = 0
 *   
 *   k2 = f(0.05, 1 + 0.05·0) = f(0.05, 1)
 *      = -2(0.05)(1) = -0.1
 *   
 *   k3 = f(0.05, 1 + 0.05·(-0.1)) = f(0.05, 0.995)
 *      = -2(0.05)(0.995) = -0.0995
 *   
 *   k4 = f(0.1, 1 + 0.1·(-0.0995)) = f(0.1, 0.99005)
 *      = -2(0.1)(0.99005) = -0.1980
 *   
 *   y₁ = y₀ + (h/6)·(k1 + 2k2 + 2k3 + k4)
 *      = 1 + (0.1/6)·(0 + 2(-0.1) + 2(-0.0995) + (-0.1980))
 *      = 1 + (0.1/6)·(-0.597)
 *      = 1 - 0.00995 ≈ 0.99005
 * 
 * Resultado: y(0.1) ≈ 0.99005
 * Solución exacta: y(0.1) = e^(-0.01) ≈ 0.990049834
 * Error: ≈ 0.000000166 (extremadamente preciso)
 * 
 * ============================================================================
 * NOTAS SOBRE IMPLEMENTACIÓN
 * ============================================================================
 * - Todos los métodos usan memoria dinámica (malloc/free)
 * - Los resultados se guardan en archivos de texto
 * - El método de Euler incluye análisis completo de errores
 * - Se proporciona función factorConvergencia() para verificar orden
 * - Para cambiar la EDO, modificar la función f(x, y)
 * - Para sistemas con solución exacta conocida, actualizar solucionExacta()
 * 
 * ADVERTENCIAS:
 * - Pasos muy grandes (h) causan errores significativos
 * - Métodos de orden bajo requieren muchos pasos para buena precisión
 * - RK4 es recomendado para la mayoría de aplicaciones prácticas
 * - Verificar siempre la convergencia con diferentes valores de h
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Prototipos de funciones */
double f(double x, double y);
double solucionExacta(double x, double x_0, double y_0);
void euler();
void heun();
void puntoMedio();
void rk4();
void rk4_variante();
void metodoDospasos();
void factorConvergencia(int n, double h, double *x, double *y);

int main(void)
{
    char opcion;

    do
    {
        printf("\n╔════════════════════════════════════════════════════╗\n");
        printf("║   Métodos Numéricos para resolver EDO de 1° orden ║\n");
        printf("╚════════════════════════════════════════════════════╝\n");
        printf("Seleccione una opción:\n");
        printf("  a) Método de Euler\n");
        printf("  b) Método de Heun (Euler Mejorado)\n");
        printf("  c) Método del Punto Medio\n");
        printf("  d) Método de Runge-Kutta 4to Orden (Clásico)\n");
        printf("  v) Método de Runge-Kutta 4to Orden (Variante)\n");
        printf("  e) Método de Dos Pasos (Multipaso)\n");
        printf("  f) Salir\n");
        printf("────────────────────────────────────────────────────\n");
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        
        switch (opcion)
        {
        case 'a':
            euler();
            break;
        case 'b':
            heun();
            break;
        case 'c':
            puntoMedio();
            break;
        case 'd':
            rk4();
            break;
        case 'v':
            rk4_variante();
            break;
        case 'e':
            metodoDospasos();
            break;
        case 'f':
            printf("\n✓ Saliendo del programa...\n");
            break;
        default:
            printf("\n✗ Opción no válida. Intente de nuevo.\n");
            break;
        }
    } while (opcion != 'f');
    
    return 0;
}

/**
 * @brief Función que define la EDO de primer orden a resolver.
 * 
 * FORMA GENERAL:
 *   dy/dx = f(x, y)
 * 
 * PROBLEMA ACTUAL (Problema n°3):
 *   dy/dx = y - x² + 1
 *   con y(0) = 0.5, x ∈ [0,2]
 * 
 * Esta función representa el lado derecho de la ecuación diferencial.
 * Para resolver una EDO diferente, simplemente modifique el cuerpo
 * de esta función.
 * 
 * EJEMPLOS DE OTRAS EDOs:
 *   - dy/dx = -2xy: return -2 * x * y;
 *   - Crecimiento exponencial: return k * y;
 *   - Logística: return r * y * (1 - y/K);
 *   - Oscilador: return -omega * omega * y; (requiere sistema 2x2)
 * 
 * @param x Valor de la variable independiente
 * @param y Valor de la variable dependiente en x
 * @return El valor de dy/dx en el punto (x, y)
 */
double f(double x, double y)
{
    // Problema n°3: dy/dx = y - x² + 1
    // return (y - x * x + 1.0);

    return (y * (4 * y - x * x));
}

/**
 * @brief Calcula la solución analítica exacta de la EDO: dy/dx = -2xy
 * 
 * DESARROLLO MATEMÁTICO:
 * ----------------------
 * Ecuación: dy/dx = -2xy
 * 
 * Paso 1: Separación de variables
 *   dy/y = -2x dx
 * 
 * Paso 2: Integración
 *   ∫(dy/y) = ∫(-2x dx)
 *   ln|y| = -x² + C
 * 
 * Paso 3: Solución general
 *   y = e^(-x² + C) = A·e^(-x²)
 * 
 * Paso 4: Aplicar condición inicial y(x₀) = y₀
 *   y₀ = A·e^(-x₀²)
 *   A = y₀·e^(x₀²)
 * 
 * Paso 5: Solución particular
 *   y(x) = y₀·e^(x₀²)·e^(-x²) = y₀·e^(x₀² - x²)
 * 
 * RESULTADO FINAL:
 *   y(x) = y₀ · e^(-x² + x₀²)
 * 
 * USO:
 * Esta función se utiliza para calcular el error de los métodos
 * numéricos comparando la aproximación con la solución exacta.
 * 
 * @param x Valor de x donde se desea evaluar la solución
 * @param x_0 Valor inicial de x (condición inicial)
 * @param y_0 Valor inicial de y (condición inicial)
 * @return El valor exacto de y(x)
 */
double solucionExacta(double x, double x_0, double y_0)
{
    // return y_0 * exp(-x * x + x_0 * x_0);
    return (y_0 * (4 * y_0 - x * x));
}

/**
 * @brief Método de Euler para resolver EDO de primer orden.
 * 
 * TEORÍA:
 * -------
 * El método de Euler es el método numérico más simple para resolver EDOs.
 * Se basa en la aproximación de Taylor de primer orden.
 * 
 * FÓRMULA RECURSIVA:
 *   y[i+1] = y[i] + h · f(x[i], y[i])
 *   x[i+1] = x[i] + h
 * 
 * donde:
 *   h = (xf - x0) / n  (tamaño del paso)
 *   n = número de subintervalos
 * 
 * DERIVACIÓN:
 * Partiendo de la serie de Taylor:
 *   y(x + h) = y(x) + h·y'(x) + (h²/2!)·y''(x) + ...
 * 
 * Truncando después del término lineal:
 *   y(x + h) ≈ y(x) + h·y'(x)
 * 
 * Como y'(x) = f(x, y), obtenemos:
 *   y(x + h) ≈ y(x) + h·f(x, y)
 * 
 * ERROR:
 *   - Error de truncamiento local: O(h²)
 *   - Error de truncamiento global: O(h)
 * 
 * ANÁLISIS DE ERROR IMPLEMENTADO:
 * Este método incluye cálculo completo de errores:
 *   1. Error absoluto: |y_exacta - y_aproximada|
 *   2. Error relativo: (error_absoluto / |y_exacta|) × 100%
 *   3. Error máximo en todo el intervalo
 *   4. Error promedio en todo el intervalo
 * 
 * VENTAJAS:
 *   - Muy simple de implementar
 *   - Requiere solo una evaluación de f por paso
 *   - Bajo costo computacional
 * 
 * DESVENTAJAS:
 *   - Precisión limitada (orden 1)
 *   - Requiere pasos pequeños para buena precisión
 *   - Acumula errores rápidamente
 * 
 * SALIDA:
 *   - Tabla en consola con: i, x[i], y_aprox, y_exacta, error_abs, error_rel
 *   - Estadísticas: error máximo y promedio
 *   - Archivo 'euler_resultados.txt' con todos los datos
 */
void euler ()
{
    double x_0 = 0.0; // Valor inicial de x
    double x_f = 0.0; // Valor final de x

    double y_0 = 0.0; // Valor inicial de y
    // double y_f = 0.0; // Valor final de y

    double h = 0.0;   // Paso de integración

    int n = 0;        // Número de pasos

    double * x = NULL; // Array para almacenar los valores de x
    double * y = NULL; // Array para almacenar los valores de y

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║          MÉTODO DE EULER                   ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y (y_0): ");
    scanf("%lf", &y_0);
    
    // Preguntar si se ingresará n o h
    char opcion;
    printf("\n¿Qué desea ingresar?\n");
    printf("  n) Número de subintervalos (n)\n");
    printf("  h) Tamaño de paso (h)\n");
    printf("Opción: ");
    scanf(" %c", &opcion);
    
    if (opcion == 'n' || opcion == 'N') {
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; // Calcular el paso de integración
    } else if (opcion == 'h' || opcion == 'H') {
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); // Calcular el número de pasos
    } else {
        printf("\n✗ Opción inválida. Usando n por defecto.\n");
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n;
    }

    x = (double *)malloc((n + 1) * sizeof(double));
    y = (double *)malloc((n + 1) * sizeof(double));

    /* Calcular Solucion */
    x[0] = x_0;
    y[0] = y_0;

    for (int i = 0; i < n; i++)
    {
        x[i + 1] = x[i] + h;
        y[i + 1] = y[i] + h * f(x[i], y[i]); // f(x, y) es la función que define la EDO
    }

    /* ==========================================
       CALCULAR ERRORES
       ========================================== */
    double *y_exacta = (double *)malloc((n + 1) * sizeof(double));
    double *error_abs = (double *)malloc((n + 1) * sizeof(double));
    double *error_rel = (double *)malloc((n + 1) * sizeof(double));
    
    double error_max = 0.0;
    double error_promedio = 0.0;
    
    for (int i = 0; i <= n; i++)
    {
        y_exacta[i] = solucionExacta(x[i], x_0, y_0);
        error_abs[i] = fabs(y_exacta[i] - y[i]);
        
        if (fabs(y_exacta[i]) > 1e-10)
            error_rel[i] = (error_abs[i] / fabs(y_exacta[i])) * 100.0;
        else
            error_rel[i] = 0.0;
        
        if (error_abs[i] > error_max)
            error_max = error_abs[i];
        
        error_promedio += error_abs[i];
    }
    error_promedio /= (n + 1);
    
    /* ==========================================
       MOSTRAR RESULTADOS EN PANTALLA
       ========================================== */
    printf("\n════════════════════════════════════════════════════════════════════════════\n");
    printf("  RESULTADOS - MÉTODO DE EULER\n");
    printf("════════════════════════════════════════════════════════════════════════════\n");
    printf("Paso h: %.6lf\n", h);
    printf("Número de pasos: %d\n\n", n);
    printf("  i      x_i        y_aprox      y_exacta    Error Abs.  Error Rel.(%%)\n");
    printf("────────────────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf | %12.8lf | %10.2e | %8.4lf%%\n", 
               i, x[i], y[i], y_exacta[i], error_abs[i], error_rel[i]);
    }
    printf("════════════════════════════════════════════════════════════════════════════\n");
    printf("\n📊 ESTADÍSTICAS DE ERROR:\n");
    printf("   Error máximo:    %.6e\n", error_max);
    printf("   Error promedio:  %.6e\n", error_promedio);
    printf("════════════════════════════════════════════════════════════════════════════\n");

    /* ==========================================
       GUARDAR RESULTADOS EN ARCHIVO
       ========================================== */
    FILE *archivo = fopen("euler_resultados.txt", "w");

    if (archivo == NULL)
    {
        printf("\n✗ Error: No se pudo crear el archivo 'euler_resultados.txt'\n");
    }
    else
    {
        // Escribir encabezado
        fprintf(archivo, "# Resultados del Método de Euler con Análisis de Error\n");
        // fprintf(archivo, "# EDO: dy/dx = -2xy\n");
        // fprintf(archivo, "# Solución exacta: y(x) = y0 * exp(-x^2 + x0^2)\n");
        fprintf(archivo, "# Condición inicial: y(%.6lf) = %.6lf\n", x_0, y_0);
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", x_0, x_f);
        fprintf(archivo, "# Paso h: %.6lf\n", h);
        fprintf(archivo, "# Número de pasos: %d\n", n);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# Estadísticas de Error:\n");
        fprintf(archivo, "# Error máximo:    %.6e\n", error_max);
        fprintf(archivo, "# Error promedio:  %.6e\n", error_promedio);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_aproximado\ty_exacto\terror_absoluto\terror_relativo(%%)\n");

        // Escribir datos
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\t%.10lf\t%.10e\t%.6lf\n", 
                    i, x[i], y[i], y_exacta[i], error_abs[i], error_rel[i]);
        }

        fclose(archivo);
        printf("\n✓ Resultados guardados en 'euler_resultados.txt'\n");
    }

    /* ==========================================
       LIBERAR MEMORIA
       ========================================== */
    free(x);
    free(y);
    free(y_exacta);
    free(error_abs);
    free(error_rel);
    
    printf("\nPresione ENTER para continuar...");
    getchar();
    getchar();
}

/**
 * @brief Método de Heun (Euler Mejorado) para resolver EDO de primer orden.
 * 
 * TEORÍA:
 * -------
 * El método de Heun es un método predictor-corrector de segundo orden.
 * También conocido como método de Euler Mejorado o método de Euler Modificado.
 * 
 * ESQUEMA PREDICTOR-CORRECTOR:
 * 
 * Paso Predictor:
 *   y* = y[i] + h · f(x[i], y[i])
 * 
 * Paso Corrector:
 *   y[i+1] = y[i] + (h/2) · [f(x[i], y[i]) + f(x[i+1], y*)]
 * 
 * El método primero predice un valor usando Euler, luego lo corrige
 * promediando las pendientes en x[i] y x[i+1].
 * 
 * INTERPRETACIÓN GEOMÉTRICA:
 * En lugar de usar solo la pendiente en el punto inicial (como Euler),
 * Heun usa el promedio de la pendiente inicial y la pendiente en el
 * punto predicho. Esto produce una aproximación tipo regla del trapecio.
 * 
 * ERROR:
 *   - Error de truncamiento local: O(h³)
 *   - Error de truncamiento global: O(h²)
 * 
 * VENTAJAS:
 *   - Mayor precisión que Euler (orden 2 vs orden 1)
 *   - Solo requiere dos evaluaciones de f por paso
 *   - Implementación relativamente simple
 *   - Buen balance entre precisión y eficiencia
 * 
 * DESVENTAJAS:
 *   - Más costoso que Euler (dos evaluaciones vs una)
 *   - Menos preciso que RK4
 * 
 * COMPARACIÓN CON EULER:
 * Para el mismo paso h, Heun es significativamente más preciso.
 * Para la misma precisión, Heun puede usar pasos más grandes.
 * 
 * SALIDA:
 *   - Tabla en consola con: i, x[i], y[i]
 *   - Archivo 'heun_resultados.txt' con los datos
 */
void heun()
{
    double x_0 = 0.0; // Valor inicial de x
    double x_f = 0.0; // Valor final de x

    double y_0 = 0.0; // Valor inicial de y

    double h = 0.0;   // Paso de integración

    int n = 0;        // Número de pasos

    double * x = NULL; // Array para almacenar los valores de x
    double * y = NULL; // Array para almacenar los valores de y

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║     MÉTODO DE HEUN (EULER MEJORADO)        ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y (y_0): ");
    scanf("%lf", &y_0);
    
    // Preguntar si se ingresará n o h
    char opcion;
    printf("\n¿Qué desea ingresar?\n");
    printf("  n) Número de subintervalos (n)\n");
    printf("  h) Tamaño de paso (h)\n");
    printf("Opción: ");
    scanf(" %c", &opcion);
    
    if (opcion == 'n' || opcion == 'N') {
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; // Calcular el paso de integración
    } else if (opcion == 'h' || opcion == 'H') {
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); // Calcular el número de pasos
    } else {
        printf("\n✗ Opción inválida. Usando n por defecto.\n");
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n;
    }

    /* Reservar memoria para los arrays */
    x = (double *)malloc((n + 1) * sizeof(double));
    y = (double *)malloc((n + 1) * sizeof(double));

    /* ==========================================
       ALGORITMO DE HEUN
       ========================================== */
    x[0] = x_0;
    y[0] = y_0;

    for (int i = 0; i < n; i++)
    {
        double y_predictor;  /* Valor predicho de y usando Euler */
        
        /* Avanzar en x */
        x[i + 1] = x[i] + h;
        
        /* PASO PREDICTOR: Aproximación inicial usando Euler */
        y_predictor = y[i] + h * f(x[i], y[i]);
        
        /* PASO CORRECTOR: Promedio de pendientes */
        /* Pendiente inicial: f(x[i], y[i]) */
        /* Pendiente final: f(x[i+1], y_predictor) */
        /* Promedio: (pendiente_inicial + pendiente_final) / 2 */
        y[i + 1] = y[i] + (h / 2.0) * (f(x[i], y[i]) + f(x[i + 1], y_predictor));
    }

    /* ==========================================
       MOSTRAR RESULTADOS EN PANTALLA
       ========================================== */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS - MÉTODO DE HEUN\n");
    printf("════════════════════════════════════════════\n");
    printf("Paso h: %.6lf\n", h);
    printf("Número de pasos: %d\n\n", n);
    printf("  i      x_i          y_i\n");
    printf("────────────────────────────────────────────\n");

    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf\n", i, x[i], y[i]);
    }
    printf("════════════════════════════════════════════\n");

    /* ==========================================
       GUARDAR RESULTADOS EN ARCHIVO
       ========================================== */
    FILE *archivo = fopen("heun_resultados.txt", "w");

    if (archivo == NULL)
    {
        printf("\n✗ Error: No se pudo crear el archivo 'heun_resultados.txt'\n");
    }
    else
    {
        // Escribir encabezado
        fprintf(archivo, "# Resultados del Método de Heun (Euler Mejorado)\n");
        fprintf(archivo, "# EDO: dy/dx = f(x,y)\n");
        fprintf(archivo, "# Condición inicial: y(%.6lf) = %.6lf\n", x_0, y_0);
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", x_0, x_f);
        fprintf(archivo, "# Paso h: %.6lf\n", h);
        fprintf(archivo, "# Número de pasos: %d\n", n);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_i\n");

        // Escribir datos
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\n", i, x[i], y[i]);
        }

        fclose(archivo);
        printf("\n✓ Resultados guardados en 'heun_resultados.txt'\n");
    }

    /* ==========================================
       LIBERAR MEMORIA
       ========================================== */
    free(x);
    free(y);
    
    printf("\nPresione ENTER para continuar...");
    getchar();
    getchar();
}

/**
 * @brief Método del Punto Medio para resolver EDO de primer orden.
 * 
 * TEORÍA:
 * -------
 * El método del Punto Medio es un método de segundo orden que evalúa
 * la función en el punto medio del intervalo [x[i], x[i+1]].
 * 
 * FÓRMULAS:
 *   k1 = f(x[i], y[i])
 *   k2 = f(x[i] + h/2, y[i] + (h/2)·k1)
 *   y[i+1] = y[i] + h·k2
 * 
 * INTERPRETACIÓN:
 * 1. k1 es la pendiente en el punto inicial (x[i], y[i])
 * 2. Usamos k1 para estimar y en el punto medio: x[i] + h/2
 * 3. k2 es la pendiente en ese punto medio
 * 4. Avanzamos todo el paso h usando la pendiente del punto medio k2
 * 
 * DERIVACIÓN:
 * Se basa en la expansión de Taylor de segundo orden, pero evaluando
 * la derivada en el punto medio del intervalo en lugar del extremo.
 * 
 * VENTAJA SOBRE EULER:
 * Al evaluar la pendiente en el punto medio, se obtiene una mejor
 * aproximación de la pendiente promedio en todo el intervalo.
 * 
 * ERROR:
 *   - Error de truncamiento local: O(h³)
 *   - Error de truncamiento global: O(h²)
 * 
 * COMPARACIÓN CON HEUN:
 * Ambos son métodos de orden 2, pero usan estrategias diferentes:
 * - Heun: promedia pendientes en inicio y fin
 * - Punto Medio: usa pendiente en el centro
 * 
 * En la práctica, ambos métodos tienen precisión similar.
 * 
 * VENTAJAS:
 *   - Orden 2 (más preciso que Euler)
 *   - Solo dos evaluaciones de f por paso
 *   - Conceptualmente simple
 * 
 * DESVENTAJAS:
 *   - Menos preciso que RK4
 *   - Requiere calcular punto intermedio
 * 
 * SALIDA:
 *   - Tabla en consola con: i, x[i], y[i]
 *   - Archivo 'puntomedio_resultados.txt' con los datos
 */
void puntoMedio()
{
    double x_0 = 0.0; // Valor inicial de x
    double x_f = 0.0; // Valor final de x

    double y_0 = 0.0; // Valor inicial de y

    double h = 0.0;   // Paso de integración

    int n = 0;        // Número de pasos

    double * x = NULL; // Array para almacenar los valores de x
    double * y = NULL; // Array para almacenar los valores de y

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║          MÉTODO DEL PUNTO MEDIO            ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y (y_0): ");
    scanf("%lf", &y_0);
    
    // Preguntar si se ingresará n o h
    char opcion;
    printf("\n¿Qué desea ingresar?\n");
    printf("  n) Número de subintervalos (n)\n");
    printf("  h) Tamaño de paso (h)\n");
    printf("Opción: ");
    scanf(" %c", &opcion);
    
    if (opcion == 'n' || opcion == 'N') {
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; // Calcular el paso de integración
    } else if (opcion == 'h' || opcion == 'H') {
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); // Calcular el número de pasos
    } else {
        printf("\n✗ Opción inválida. Usando n por defecto.\n");
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n;
    }

    /* Reservar memoria para los arrays */
    x = (double *)malloc((n + 1) * sizeof(double));
    y = (double *)malloc((n + 1) * sizeof(double));

    /* ==========================================
       ALGORITMO DEL PUNTO MEDIO
       ========================================== */
    x[0] = x_0;
    y[0] = y_0;

    for (int i = 0; i < n; i++)
    {
        double k1, k2;
        
        /* Avanzar en x */
        x[i + 1] = x[i] + h;
        
        /* k1: Pendiente en el punto actual */
        k1 = f(x[i], y[i]);
        
        /* k2: Pendiente en el punto medio */
        /* Primero estimamos y en el punto medio: y[i] + (h/2)*k1 */
        /* Luego evaluamos f en ese punto medio */
        k2 = f(x[i] + h / 2.0, y[i] + (h / 2.0) * k1);
        
        /* Avanzar usando la pendiente del punto medio */
        y[i + 1] = y[i] + h * k2;
    }

    /* ==========================================
       MOSTRAR RESULTADOS EN PANTALLA
       ========================================== */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS - MÉTODO DEL PUNTO MEDIO\n");
    printf("════════════════════════════════════════════\n");
    printf("Paso h: %.6lf\n", h);
    printf("Número de pasos: %d\n\n", n);
    printf("  i      x_i          y_i\n");
    printf("────────────────────────────────────────────\n");

    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf\n", i, x[i], y[i]);
    }
    printf("════════════════════════════════════════════\n");

    /* ==========================================
       GUARDAR RESULTADOS EN ARCHIVO
       ========================================== */
    FILE *archivo = fopen("puntomedio_resultados.txt", "w");

    if (archivo == NULL)
    {
        printf("\n✗ Error: No se pudo crear el archivo 'puntomedio_resultados.txt'\n");
    }
    else
    {
        // Escribir encabezado
        fprintf(archivo, "# Resultados del Método del Punto Medio\n");
        fprintf(archivo, "# EDO: dy/dx = f(x,y)\n");
        fprintf(archivo, "# Condición inicial: y(%.6lf) = %.6lf\n", x_0, y_0);
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", x_0, x_f);
        fprintf(archivo, "# Paso h: %.6lf\n", h);
        fprintf(archivo, "# Número de pasos: %d\n", n);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_i\n");

        // Escribir datos
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\n", i, x[i], y[i]);
        }

        fclose(archivo);
        printf("\n✓ Resultados guardados en 'puntomedio_resultados.txt'\n");
    }

    /* ==========================================
       LIBERAR MEMORIA
       ========================================== */
    free(x);
    free(y);
    
    printf("\nPresione ENTER para continuar...");
    getchar();
    getchar();
}

/**
 * @brief Método de Runge-Kutta de 4to Orden (RK4) para resolver EDO.
 * 
 * TEORÍA:
 * -------
 * RK4 es el método clásico de la familia Runge-Kutta y uno de los más
 * utilizados en la práctica por su excelente balance entre precisión
 * y costo computacional.
 * 
 * FÓRMULAS:
 *   k1 = f(x[i], y[i])
 *   k2 = f(x[i] + h/2, y[i] + (h/2)·k1)
 *   k3 = f(x[i] + h/2, y[i] + (h/2)·k2)
 *   k4 = f(x[i] + h, y[i] + h·k3)
 *   
 *   y[i+1] = y[i] + (h/6)·(k1 + 2k2 + 2k3 + k4)
 * 
 * INTERPRETACIÓN DE LOS COEFICIENTES k:
 * 
 * k1: Pendiente al inicio del intervalo
 *     - Evaluada en el punto (x[i], y[i])
 *     - Es la misma pendiente que usaría Euler
 * 
 * k2: Pendiente en el punto medio (primera estimación)
 *     - Usa k1 para estimar y en x[i] + h/2
 *     - Mejora sobre la pendiente inicial
 * 
 * k3: Pendiente en el punto medio (segunda estimación)
 *     - Usa k2 (más preciso que k1) para estimar y en x[i] + h/2
 *     - Refinamiento de k2
 * 
 * k4: Pendiente al final del intervalo
 *     - Usa k3 para estimar y en x[i] + h
 *     - Pendiente en el punto final predicho
 * 
 * PROMEDIO PONDERADO:
 * La fórmula final es un promedio ponderado de las cuatro pendientes:
 *   Pendiente_promedio = (k1 + 2k2 + 2k3 + k4) / 6
 * 
 * Los coeficientes 1:2:2:1 dan más peso a las pendientes centrales,
 * similar a la regla de Simpson para integración.
 * 
 * ERROR:
 *   - Error de truncamiento local: O(h⁵)
 *   - Error de truncamiento global: O(h⁴)
 * 
 * VENTAJAS:
 *   - Excelente precisión (orden 4)
 *   - Método estándar en la industria
 *   - Requiere solo f(x,y), no derivadas
 *   - Muy estable numéricamente
 *   - Ampliamente estudiado y probado
 * 
 * DESVENTAJAS:
 *   - Cuatro evaluaciones de f por paso
 *   - Más complejo que métodos de orden inferior
 * 
 * CUÁNDO USAR RK4:
 *   - Cuando se necesita buena precisión
 *   - Cuando el costo de evaluar f no es prohibitivo
 *   - Como método predeterminado para la mayoría de problemas
 * 
 * COMPARACIÓN:
 *   Euler:  1 evaluación, orden 1
 *   Heun:   2 evaluaciones, orden 2
 *   PM:     2 evaluaciones, orden 2
 *   RK4:    4 evaluaciones, orden 4  ← Mejor relación costo/precisión
 * 
 * SALIDA:
 *   - Tabla en consola con: i, x[i], y[i]
 *   - Archivo 'rk4_resultados.txt' con los datos
 */
void rk4()
{
    double x_0 = 0.0; // Valor inicial de x
    double x_f = 0.0; // Valor final de x

    double y_0 = 0.0; // Valor inicial de y

    double h = 0.0;   // Paso de integración

    int n = 0;        // Número de pasos

    double * x = NULL; // Array para almacenar los valores de x
    double * y = NULL; // Array para almacenar los valores de y

    double k1, k2, k3, k4; // Coeficientes del método RK4

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║     MÉTODO DE RUNGE-KUTTA 4TO ORDEN        ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y (y_0): ");
    scanf("%lf", &y_0);
    
    // Preguntar si se ingresará n o h
    char opcion;
    printf("\n¿Qué desea ingresar?\n");
    printf("  n) Número de subintervalos (n)\n");
    printf("  h) Tamaño de paso (h)\n");
    printf("Opción: ");
    scanf(" %c", &opcion);
    
    if (opcion == 'n' || opcion == 'N') {
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; // Calcular el paso de integración
    } else if (opcion == 'h' || opcion == 'H') {
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); // Calcular el número de pasos
    } else {
        printf("\n✗ Opción inválida. Usando n por defecto.\n");
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n;
    }

    /* Reservar memoria para los arrays */
    x = (double *)malloc((n + 1) * sizeof(double));
    y = (double *)malloc((n + 1) * sizeof(double));

    /* ==========================================
       ALGORITMO DE RUNGE-KUTTA 4TO ORDEN
       ========================================== */
    x[0] = x_0;
    y[0] = y_0;

    for (int i = 0; i < n; i++)
    {
        /* Calcular los cuatro coeficientes k */
        
        /* k1: Pendiente al inicio del intervalo */
        k1 = f(x[i], y[i]);
        
        /* k2: Pendiente en el punto medio usando k1 */
        k2 = f(x[i] + h / 2.0, y[i] + (h / 2.0) * k1);
        
        /* k3: Pendiente en el punto medio usando k2 (más preciso) */
        k3 = f(x[i] + h / 2.0, y[i] + (h / 2.0) * k2);
        
        /* k4: Pendiente al final del intervalo usando k3 */
        k4 = f(x[i] + h, y[i] + h * k3);
        
        /* Avanzar en x */
        x[i + 1] = x[i] + h;
        
        /* Promedio ponderado de las pendientes (1:2:2:1) */
        /* Este es el núcleo del método RK4 */
        y[i + 1] = y[i] + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    }

    /* ==========================================
       MOSTRAR RESULTADOS EN PANTALLA
       ========================================== */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS - MÉTODO DE RUNGE-KUTTA 4\n");
    printf("════════════════════════════════════════════\n");
    printf("Paso h: %.6lf\n", h);
    printf("Número de pasos: %d\n\n", n);
    printf("  i      x_i          y_i\n");
    printf("────────────────────────────────────────────\n");

    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf\n", i, x[i], y[i]);
    }
    printf("════════════════════════════════════════════\n");

    /* ==========================================
       GUARDAR RESULTADOS EN ARCHIVO
       ========================================== */
    FILE *archivo = fopen("rk4_resultados.txt", "w");

    if (archivo == NULL)
    {
        printf("\n✗ Error: No se pudo crear el archivo 'rk4_resultados.txt'\n");
    }
    else
    {
        // Escribir encabezado
        fprintf(archivo, "# Resultados del Método de Runge-Kutta 4to Orden\n");
        fprintf(archivo, "# EDO: dy/dx = f(x,y)\n");
        fprintf(archivo, "# Condición inicial: y(%.6lf) = %.6lf\n", x_0, y_0);
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", x_0, x_f);
        fprintf(archivo, "# Paso h: %.6lf\n", h);
        fprintf(archivo, "# Número de pasos: %d\n", n);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_i\n");

        // Escribir datos
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\n", i, x[i], y[i]);
        }

        fclose(archivo);
        printf("\n✓ Resultados guardados en 'rk4_resultados.txt'\n");
    }

    /* ==========================================
       LIBERAR MEMORIA
       ========================================== */
    free(x);
    free(y);
    
    printf("\nPresione ENTER para continuar...");
    getchar();
    getchar();
}

/**
 * @brief Método de Runge-Kutta 4to Orden - Variante específica
 * 
 * TEORÍA:
 * -------
 * Esta es una variante del método RK4 con coeficientes diferentes.
 * Se diferencia del RK4 clásico en los puntos de evaluación y los pesos.
 * 
 * FÓRMULA DE LA VARIANTE:
 *   k₁ = f(xᵢ, yᵢ)
 *   k₂ = f(xᵢ + (1/2)h, yᵢ + (1/4)hk₁)
 *   k₃ = f(xᵢ + (2/3)h, yᵢ - (1/3)hk₁ + hk₂)
 *   k₄ = f(xᵢ + h, yᵢ + hk₁ - hk₂ + hk₃)
 *   yᵢ₊₁ = yᵢ + (h/8)(k₁ + 3k₂ + 3k₃ + k₄)
 * 
 * COMPARACIÓN CON RK4 CLÁSICO:
 *   RK4 Clásico: yᵢ₊₁ = yᵢ + (h/6)(k₁ + 2k₂ + 2k₃ + k₄)
 *   Variante:    yᵢ₊₁ = yᵢ + (h/8)(k₁ + 3k₂ + 3k₃ + k₄)
 * 
 * DIFERENCIAS CLAVE:
 *   - Coeficiente final: 1/8 en lugar de 1/6
 *   - Pesos intermedios: 3-3 en lugar de 2-2
 *   - Punto k₃: evaluado en 2/3 en lugar de 1/2
 *   - Combinaciones más complejas en k₃ y k₄
 * 
 * ERROR:
 *   - Error de truncamiento local: O(h⁵)
 *   - Error de truncamiento global: O(h⁴)
 * 
 * APLICACIÓN:
 * Esta variante puede ofrecer mejor estabilidad o precisión para
 * ciertos tipos de EDOs, dependiendo de las características del problema.
 * 
 * SALIDA:
 *   - Tabla en consola con: i, x[i], y[i]
 *   - Muestra valores específicos solicitados (y(1.5), y(2.0))
 *   - Archivo 'rk4_variante_resultados.txt' con los datos paso a paso
 */
void rk4_variante()
{
    double x_0 = 0.0; // Valor inicial de x
    double x_f = 0.0; // Valor final de x

    double y_0 = 0.0; // Valor inicial de y

    double h = 0.0;   // Paso de integración

    int n = 0;        // Número de pasos

    double * x = NULL; // Array para almacenar los valores de x
    double * y = NULL; // Array para almacenar los valores de y

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║   MÉTODO RK4 - VARIANTE (Problema n°3)    ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y (y_0): ");
    scanf("%lf", &y_0);
    
    // Preguntar si se ingresará n o h
    char opcion;
    printf("\n¿Qué desea ingresar?\n");
    printf("  n) Número de subintervalos (n)\n");
    printf("  h) Tamaño de paso (h)\n");
    printf("Opción: ");
    scanf(" %c", &opcion);
    
    if (opcion == 'n' || opcion == 'N') {
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        h = (x_f - x_0) / n; // Calcular el paso de integración
    } else if (opcion == 'h' || opcion == 'H') {
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); // Calcular el número de pasos
        h = (x_f - x_0) / n;  // Recalcular h para exactitud
    } else {
        printf("Opción no válida. Usando h por defecto.\n");
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h);
        h = (x_f - x_0) / n;
    }

    x = (double *)malloc((n + 1) * sizeof(double));
    y = (double *)malloc((n + 1) * sizeof(double));

    /* Condiciones iniciales */
    x[0] = x_0;
    y[0] = y_0;

    /* ==========================================
       APLICAR MÉTODO RK4 VARIANTE
       ========================================== */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("  ITERACIONES PASO A PASO - RK4 VARIANTE\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("Fórmula: yᵢ₊₁ = yᵢ + (h/8)(k₁ + 3k₂ + 3k₃ + k₄)\n\n");

    for (int i = 0; i < n; i++)
    {
        /* Calcular las pendientes k */
        double k1 = f(x[i], y[i]);
        double k2 = f(x[i] + 0.5 * h, y[i] + 0.25 * h * k1);
        double k3 = f(x[i] + (2.0/3.0) * h, y[i] - (1.0/3.0) * h * k1 + h * k2);
        double k4 = f(x[i] + h, y[i] + h * k1 - h * k2 + h * k3);

        /* Calcular siguiente valor de y */
        y[i + 1] = y[i] + (h / 8.0) * (k1 + 3.0 * k2 + 3.0 * k3 + k4);
        x[i + 1] = x[i] + h;

        /* Mostrar solo algunos pasos clave o todos si n es pequeño */
        if (n <= 20 || i == 0 || i == n-1 || (i+1) % 5 == 0) {
            printf("Paso %d:\n", i);
            printf("  x[%d] = %.4lf\n", i, x[i]);
            printf("  k₁ = %.10lf\n", k1);
            printf("  k₂ = %.10lf\n", k2);
            printf("  k₃ = %.10lf\n", k3);
            printf("  k₄ = %.10lf\n", k4);
            printf("  y[%d] = %.10lf\n\n", i+1, y[i + 1]);
        }
    }

    /* ==========================================
       MOSTRAR RESULTADOS FINALES
       ========================================== */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("  RESULTADOS FINALES - RK4 VARIANTE\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("Paso h: %.10lf\n", h);
    printf("Número de pasos: %d\n\n", n);
    printf("  i      x_i           y_i (10 decimales)\n");
    printf("────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i <= n; i++)
    {
        printf(" %3d   %.4lf      %.10lf\n", i, x[i], y[i]);
    }
    printf("════════════════════════════════════════════════════════════════\n");

    /* ==========================================
       VALORES ESPECÍFICOS SOLICITADOS
       ========================================== */
    printf("\n📊 VALORES ESPECÍFICOS (con 10 cifras decimales):\n");
    
    // Buscar y(1.5)
    for (int i = 0; i <= n; i++) {
        if (fabs(x[i] - 1.5) < 1e-6) {
            printf("   y(1.5) = %.10lf\n", y[i]);
            break;
        }
    }
    
    // Buscar y(2.0)
    for (int i = 0; i <= n; i++) {
        if (fabs(x[i] - 2.0) < 1e-6) {
            printf("   y(2.0) = %.10lf\n", y[i]);
            break;
        }
    }
    
    printf("\n");

    /* ==========================================
       GUARDAR RESULTADOS EN ARCHIVO
       ========================================== */
    FILE *archivo = fopen("rk4_variante_resultados.txt", "w");

    if (archivo == NULL)
    {
        printf("\n✗ Error al crear el archivo.\n");
    }
    else
    {
        fprintf(archivo, "MÉTODO DE RUNGE-KUTTA 4TO ORDEN - VARIANTE\n");
        fprintf(archivo, "==========================================\n");
        fprintf(archivo, "EDO: dy/dx = y - x² + 1\n");
        fprintf(archivo, "Condición inicial: y(%.4lf) = %.4lf\n", x_0, y_0);
        fprintf(archivo, "Intervalo: [%.4lf, %.4lf]\n", x_0, x_f);
        fprintf(archivo, "Paso h: %.10lf\n", h);
        fprintf(archivo, "Número de pasos: %d\n\n", n);
        
        fprintf(archivo, "Fórmula utilizada:\n");
        fprintf(archivo, "k₁ = f(xᵢ, yᵢ)\n");
        fprintf(archivo, "k₂ = f(xᵢ + h/2, yᵢ + (h/4)k₁)\n");
        fprintf(archivo, "k₃ = f(xᵢ + (2/3)h, yᵢ - (1/3)hk₁ + hk₂)\n");
        fprintf(archivo, "k₄ = f(xᵢ + h, yᵢ + hk₁ - hk₂ + hk₃)\n");
        fprintf(archivo, "yᵢ₊₁ = yᵢ + (h/8)(k₁ + 3k₂ + 3k₃ + k₄)\n\n");
        
        fprintf(archivo, "RESULTADOS PASO A PASO:\n");
        fprintf(archivo, "%-5s %-12s %-20s\n", "i", "x_i", "y_i");
        fprintf(archivo, "─────────────────────────────────────\n");

        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%-5d %-12.4lf %-20.10lf\n", i, x[i], y[i]);
        }
        
        fprintf(archivo, "\nVALORES ESPECÍFICOS:\n");
        
        // Buscar y(1.5)
        for (int i = 0; i <= n; i++) {
            if (fabs(x[i] - 1.5) < 1e-6) {
                fprintf(archivo, "y(1.5) = %.10lf\n", y[i]);
                break;
            }
        }
        
        // Buscar y(2.0)
        for (int i = 0; i <= n; i++) {
            if (fabs(x[i] - 2.0) < 1e-6) {
                fprintf(archivo, "y(2.0) = %.10lf\n", y[i]);
                break;
            }
        }

        fclose(archivo);
        printf("\n✓ Resultados guardados en 'rk4_variante_resultados.txt'\n");
    }

    /* ==========================================
       LIBERAR MEMORIA
       ========================================== */
    free(x);
    free(y);
    
    printf("\nPresione ENTER para continuar...");
    getchar();
    getchar();
}

/**
 * @brief Calcula el factor de convergencia Q para verificar el orden de un método numérico.
 * 
 * TEORÍA DEL ANÁLISIS DE CONVERGENCIA:
 * -------------------------------------
 * El factor de convergencia Q permite determinar experimentalmente el orden
 * de un método numérico SIN necesidad de conocer la solución exacta.
 * 
 * FÓRMULA:
 *   Q = (1 / ln(2)) · ln(|y₁ - y₂| / |y₂ - y₃|)
 * 
 * donde:
 *   y₁ = aproximación con paso h (la que recibe como parámetro)
 *   y₂ = aproximación con paso h/2 (se calcula internamente)
 *   y₃ = aproximación con paso h/4 (se calcula internamente)
 * 
 * INTERPRETACIÓN:
 *   Q ≈ 1 → Método de orden 1 (como Euler)
 *   Q ≈ 2 → Método de orden 2 (como Heun o Punto Medio)
 *   Q ≈ 4 → Método de orden 4 (como RK4)
 * 
 * FUNDAMENTO TEÓRICO:
 * Si un método es de orden p, entonces el error global es proporcional a h^p:
 *   E(h) ≈ C·h^p
 * 
 * Por lo tanto:
 *   E(h/2) ≈ C·(h/2)^p = C·h^p/2^p = E(h)/2^p
 *   E(h/4) ≈ E(h)/4^p
 * 
 * Asumiendo que las aproximaciones convergen a la solución exacta:
 *   |y₁ - y₂| ≈ E(h) - E(h/2) ≈ E(h)·(1 - 1/2^p)
 *   |y₂ - y₃| ≈ E(h/2) - E(h/4) ≈ E(h/2)·(1 - 1/2^p)
 * 
 * Tomando el cociente:
 *   |y₁ - y₂| / |y₂ - y₃| ≈ E(h) / E(h/2) = 2^p
 * 
 * Aplicando logaritmo:
 *   ln(|y₁ - y₂| / |y₂ - y₃|) ≈ p·ln(2)
 *   p ≈ ln(|y₁ - y₂| / |y₂ - y₃|) / ln(2)
 * 
 * VENTAJAS DE ESTE MÉTODO:
 * 1. NO requiere la solución exacta
 * 2. Verifica experimentalmente el orden teórico
 * 3. Detecta errores de implementación
 * 4. Ayuda a validar convergencia
 * 
 * USO PRÁCTICO:
 * Si Q difiere significativamente del orden teórico esperado:
 * - Puede haber un error en la implementación
 * - El paso h puede ser demasiado grande
 * - La función puede tener discontinuidades
 * - Efectos de redondeo pueden dominar
 * 
 * NOTA IMPORTANTE:
 * Esta función usa el método de Euler para calcular y₂ y y₃.
 * Para verificar otros métodos, modifique las fórmulas de integración
 * en los bucles de cálculo.
 * 
 * @param n Número de pasos usados en la aproximación original
 * @param h Tamaño del paso usado en la aproximación original
 * @param x Array con los valores de x (tamaño n+1)
 * @param y Array con los valores de y aproximados (tamaño n+1)
 */
void factorConvergencia(int n, double h, double *x, double *y)
{
    /* Extraer datos de la aproximación original (y₁) */
    double x_0 = x[0];
    double y_0 = y[0];
    double x_f = x[n];
    
    /* ==========================================
       CALCULAR APROXIMACIÓN CON PASO h/2 (y₂)
       ========================================== */
    int n2 = 2 * n;  /* Doble número de pasos */
    double *x2 = (double *)malloc((n2 + 1) * sizeof(double));
    double *y2 = (double *)malloc((n2 + 1) * sizeof(double));
    double h2 = h / 2.0;
    
    /* ==========================================
       CALCULAR APROXIMACIÓN CON PASO h/4 (y₃)
       ========================================== */
    int n3 = 4 * n;  /* Cuádruple número de pasos */
    double *x3 = (double *)malloc((n3 + 1) * sizeof(double));
    double *y3 = (double *)malloc((n3 + 1) * sizeof(double));
    double h3 = h / 4.0;
    
    /* Inicializar condiciones iniciales */
    x2[0] = x_0; y2[0] = y_0;
    x3[0] = x_0; y3[0] = y_0;
    
    /* Integración con paso h/2 usando Euler */
    for (int i = 0; i < n2; i++)
    {
        x2[i + 1] = x2[i] + h2;
        y2[i + 1] = y2[i] + h2 * f(x2[i], y2[i]);
    }
    
    /* Integración con paso h/4 usando Euler */
    for (int i = 0; i < n3; i++)
    {
        x3[i + 1] = x3[i] + h3;
        y3[i + 1] = y3[i] + h3 * f(x3[i], y3[i]);
    }
    
    /* ==========================================
       CALCULAR FACTOR DE CONVERGENCIA Q
       ========================================== */
    
    /* Valores finales de las tres aproximaciones */
    double y1_final = y[n];
    double y2_final = y2[n2];
    double y3_final = y3[n3];
    
    /* Diferencias entre aproximaciones consecutivas */
    double diff_y1_y2 = fabs(y1_final - y2_final);
    double diff_y2_y3 = fabs(y2_final - y3_final);
    
    /* Calcular Q usando la fórmula */
    double Q = 0.0;
    if (diff_y2_y3 > 1e-15 && diff_y1_y2 > 1e-15)
    {
        Q = (1.0 / log(2.0)) * log(diff_y1_y2 / diff_y2_y3);
    }
    
    /* ==========================================
       MOSTRAR RESULTADOS DEL ANÁLISIS
       ========================================== */
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║       ANÁLISIS DEL FACTOR DE CONVERGENCIA         ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");
    
    printf("Fórmula: Q = (1/ln(2)) · ln(|y₁-y₂| / |y₂-y₃|)\n\n");
    
    printf("DATOS:\n");
    printf("────────────────────────────────────────────────────\n");
    printf("Paso h:       %.6e  (n = %d)\n", h, n);
    printf("Paso h/2:     %.6e  (n = %d)\n", h2, n2);
    printf("Paso h/4:     %.6e  (n = %d)\n\n", h3, n3);
    
    printf("VALORES EN x = %.6lf:\n", x_f);
    printf("────────────────────────────────────────────────────\n");
    printf("y₁ (con h):   %.12lf\n", y1_final);
    printf("y₂ (con h/2): %.12lf\n", y2_final);
    printf("y₃ (con h/4): %.12lf\n\n", y3_final);
    
    printf("DIFERENCIAS:\n");
    printf("────────────────────────────────────────────────────\n");
    printf("|y₁ - y₂|:    %.6e\n", diff_y1_y2);
    printf("|y₂ - y₃|:    %.6e\n", diff_y2_y3);
    printf("Razón:        %.4lf\n\n", diff_y1_y2 / diff_y2_y3);
    
    printf("FACTOR DE CONVERGENCIA:\n");
    printf("════════════════════════════════════════════════════\n");
    printf("Q = %.4lf\n", Q);
    printf("════════════════════════════════════════════════════\n\n");
    
    printf("INTERPRETACIÓN:\n");
    printf("   Q ≈ 1 → Método de orden 1 (Euler)\n");
    printf("   Q ≈ 2 → Método de orden 2 (Heun, Punto Medio)\n");
    printf("   Q ≈ 4 → Método de orden 4 (Runge-Kutta 4)\n");
    
    /* Liberar memoria de las aproximaciones adicionales */
    free(x2);
    free(y2);
    free(x3);
    free(y3);
}
/**
 * ============================================================================
 * MÉTODO DE DOS PASOS (MULTIPASO)
 * ============================================================================
 * 
 * DESCRIPCIÓN:
 *   Método multipaso que calcula cada nuevo valor usando los DOS valores
 *   anteriores. Es más preciso que métodos de un solo paso porque usa
 *   información histórica de la solución.
 * 
 * FÓRMULA:
 *   y_{i+1} = y_i + h(2f(x_i, y_i) - f(x_{i-1}, y_{i-1}))
 * 
 * CARACTERÍSTICAS:
 *   - Requiere dos valores iniciales: y_0 e y_1
 *   - Para y_1 se usa un método de un paso (Euler) como "arrancador"
 *   - Orden del método: O(h²)
 *   - Más eficiente que RK4 porque solo evalúa f una vez por paso
 * 
 * PROBLEMA ESPECÍFICO A RESOLVER:
 *   dy/dx = (x·e^(x²))/y   con y(0) = 1   en x ∈ [0, 1]
 * 
 * SOLUCIÓN ANALÍTICA (por variables separables):
 *   y·dy = x·e^(x²)·dx
 *   ∫y·dy = ∫x·e^(x²)·dx
 *   y²/2 = (1/2)e^(x²) + C
 *   
 *   Con y(0) = 1: 1/2 = 1/2·e^0 + C → C = 0
 *   Por lo tanto: y(x) = e^(x²/2)
 * 
 * PSEUDOCÓDIGO:
 *   1. Leer condición inicial y_0 y parámetros
 *   2. Calcular y_1 usando método de Euler (arrancador)
 *   3. Para i = 1, 2, ..., n-1:
 *      a) Calcular f(x_i, y_i)
 *      b) Calcular f(x_{i-1}, y_{i-1})
 *      c) Aplicar fórmula: y_{i+1} = y_i + h(2f_i - f_{i-1})
 *   4. Mostrar resultados y errores
 * 
 * VENTAJAS:
 *   ✓ Usa información de pasos anteriores
 *   ✓ Una sola evaluación de f por paso (después del arranque)
 *   ✓ Buena precisión para métodos multipaso
 * 
 * DESVENTAJAS:
 *   ✗ Necesita método arrancador para y_1
 *   ✗ No es auto-arrancable
 *   ✗ Menos preciso que RK4
 * ============================================================================
 */
void metodoDospasos()
{
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║        MÉTODO DE DOS PASOS (MULTIPASO)            ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");
    
    printf("FÓRMULA: y_{i+1} = y_i + h(2f(x_i,y_i) - f(x_{i-1},y_{i-1}))\n\n");
    
    printf("PROBLEMA GENERAL A RESOLVER:\n");
    printf("  dy/dx = (x·e^(x²))/y\n");
    printf("  Solución exacta: y(x) = e^(x²/2)\n");
    printf("════════════════════════════════════════════════════\n\n");
    
    /* Parámetros del problema */
    double x_0;          // Condición inicial x_0
    double y_0;          // Condición inicial y_0
    double x_f;          // Punto final
    int n;               // Número de pasos
    double h;            // Tamaño del paso
    
    /* Solicitar condiciones iniciales y finales */
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x_0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &x_f);
    printf("Ingrese el valor inicial de y, y(x_0) = y_0: ");
    scanf("%lf", &y_0);
    
    /* Preguntar si se ingresará n o h */
    char opcion;
    printf("¿Qué desea ingresar?\n");
    printf("  n) Número de subintervalos (n)\n");
    printf("  h) Tamaño de paso (h)\n");
    printf("Opción: ");
    scanf(" %c", &opcion);
    
    if (opcion == 'n' || opcion == 'N') {
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        
        if (n < 2) {
            printf("\n✗ Error: Se necesitan al menos 2 pasos para método de dos pasos.\n");
            printf("Presione ENTER para continuar...");
            getchar();
            getchar();
            return;
        }
        
        h = (x_f - x_0) / n; // Calcular el paso
    } else if (opcion == 'h' || opcion == 'H') {
        printf("Ingrese el tamaño de paso (h): ");
        scanf("%lf", &h);
        n = (int)((x_f - x_0) / h); // Calcular el número de pasos
        
        if (n < 2) {
            printf("\n✗ Error: El paso h es muy grande. Se necesitan al menos 2 pasos.\n");
            printf("Presione ENTER para continuar...");
            getchar();
            getchar();
            return;
        }
    } else {
        printf("\n✗ Opción inválida. Usando n por defecto.\n");
        printf("Ingrese el número de subintervalos (n): ");
        scanf("%d", &n);
        
        if (n < 2) {
            printf("\n✗ Error: Se necesitan al menos 2 pasos para método de dos pasos.\n");
            printf("Presione ENTER para continuar...");
            getchar();
            getchar();
            return;
        }
        
        h = (x_f - x_0) / n;
    }
    
    printf("\n📊 PARÁMETROS:\n");
    printf("────────────────────────────────────────────────────\n");
    printf("  Condición inicial: y(%.4lf) = %.4lf\n", x_0, y_0);
    printf("  Intervalo: [%.4lf, %.4lf]\n", x_0, x_f);
    printf("  Número de pasos: %d\n", n);
    printf("  Tamaño de paso h: %.6lf\n\n", h);
    
    /* Reservar memoria para arrays */
    double *x = (double *)malloc((n + 1) * sizeof(double));
    double *y = (double *)malloc((n + 1) * sizeof(double));
    double *y_exacta = (double *)malloc((n + 1) * sizeof(double));
    double *error_abs = (double *)malloc((n + 1) * sizeof(double));
    double *error_rel = (double *)malloc((n + 1) * sizeof(double));
    double *f_actual = (double *)malloc((n + 1) * sizeof(double));
    double *f_anterior = (double *)malloc((n + 1) * sizeof(double));
    
    if (!x || !y || !y_exacta || !error_abs || !error_rel || !f_actual || !f_anterior) {
        printf("✗ Error al asignar memoria.\n");
        return;
    }
    
    /* ========================================
       PASO 1: INICIALIZACIÓN
       ======================================== */
    
    /* Valor inicial */
    x[0] = x_0;
    y[0] = y_0;
    
    /* Solución exacta para f(x) = (x·e^(x²))/y: y(x) = e^(x²/2) */
    y_exacta[0] = exp(x[0] * x[0] / 2.0);
    error_abs[0] = fabs(y_exacta[0] - y[0]);
    error_rel[0] = fabs(error_abs[0] / y_exacta[0]) * 100.0;
    
    /* Calcular f_0 = f(x_0, y_0) para usar en el siguiente paso */
    f_anterior[0] = (x[0] * exp(x[0] * x[0])) / y[0];
    
    printf("🚀 ARRANQUE DEL MÉTODO:\n");
    printf("────────────────────────────────────────────────────\n");
    printf("Usando EULER para calcular y_1 (primer paso)...\n\n");
    
    /* ========================================
       PASO 2: MÉTODO ARRANCADOR (EULER)
       Calculamos y_1 usando Euler: y_1 = y_0 + h·f(x_0, y_0)
       ======================================== */
    
    x[1] = x_0 + h;
    y[1] = y[0] + h * f_anterior[0];
    
    /* Valores exactos y errores para i=1 */
    y_exacta[1] = exp(x[1] * x[1] / 2.0);
    error_abs[1] = fabs(y_exacta[1] - y[1]);
    error_rel[1] = fabs(error_abs[1] / y_exacta[1]) * 100.0;
    
    /* Calcular f_1 para usar en el siguiente paso */
    f_actual[1] = (x[1] * exp(x[1] * x[1])) / y[1];
    
    printf("  i=0: x=%.4lf, y=%.6lf (inicial)\n", x[0], y[0]);
    printf("  i=1: x=%.4lf, y=%.6lf (Euler)\n\n", x[1], y[1]);
    
    /* ========================================
       PASO 3: MÉTODO DE DOS PASOS
       y_{i+1} = y_i + h(2f(x_i,y_i) - f(x_{i-1},y_{i-1}))
       ======================================== */
    
    printf("📈 APLICANDO MÉTODO DE DOS PASOS:\n");
    printf("────────────────────────────────────────────────────\n\n");
    
    for (int i = 1; i < n; i++)
    {
        /* Calcular nuevo punto x */
        x[i + 1] = x_0 + (i + 1) * h;
        
        /* Guardar f_anterior para este paso */
        f_anterior[i] = (i == 1) ? f_anterior[0] : f_actual[i-1];
        
        /* Calcular f_actual en el punto actual */
        f_actual[i] = (x[i] * exp(x[i] * x[i])) / y[i];
        
        /* FÓRMULA DEL MÉTODO DE DOS PASOS */
        y[i + 1] = y[i] + h * (2.0 * f_actual[i] - f_anterior[i]);
        
        /* Calcular solución exacta y errores */
        y_exacta[i + 1] = exp(x[i + 1] * x[i + 1] / 2.0);
        error_abs[i + 1] = fabs(y_exacta[i + 1] - y[i + 1]);
        error_rel[i + 1] = fabs(error_abs[i + 1] / y_exacta[i + 1]) * 100.0;
    }
    
    /* ========================================
       PASO 4: MOSTRAR RESULTADOS
       ======================================== */
    
    printf("✅ TABLA DE RESULTADOS:\n");
    printf("════════════════════════════════════════════════════════════════════════════\n");
    printf("  i       x_i        y_aprox      y_exacta    Error Abs.   Error Rel.(%%)\n");
    printf("────────────────────────────────────────────────────────────────────────────\n");
    
    for (int i = 0; i <= n; i++)
    {
        printf("%3d  %10.6lf  %12.8lf  %12.8lf  %11.2e   %10.6lf\n",
               i, x[i], y[i], y_exacta[i], error_abs[i], error_rel[i]);
    }
    
    printf("════════════════════════════════════════════════════════════════════════════\n\n");
    
    /* ========================================
       PASO 5: ANÁLISIS DE ERROR
       ======================================== */
    
    /* Calcular error máximo y promedio */
    double error_max = error_abs[0];
    double error_prom = 0.0;
    
    for (int i = 0; i <= n; i++)
    {
        if (error_abs[i] > error_max)
            error_max = error_abs[i];
        error_prom += error_abs[i];
    }
    error_prom /= (n + 1);
    
    printf("📊 ANÁLISIS DE ERROR:\n");
    printf("════════════════════════════════════════════════════\n");
    printf("Error máximo:   %.6e\n", error_max);
    printf("Error promedio: %.6e\n", error_prom);
    printf("Error final:    %.6e (en x=%.1lf)\n", error_abs[n], x_f);
    printf("════════════════════════════════════════════════════\n\n");
    
    /* Valor final */
    printf("🎯 RESULTADO FINAL:\n");
    printf("════════════════════════════════════════════════════\n");
    printf("y(%.1lf) ≈ %.10lf\n", x_f, y[n]);
    printf("Exacto:  %.10lf\n", y_exacta[n]);
    printf("Error:   %.6e (%.6lf%%)\n", error_abs[n], error_rel[n]);
    printf("════════════════════════════════════════════════════\n\n");
    
    /* Guardar resultados en archivo */
    FILE *archivo = fopen("metodo_dospasos_resultados.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# MÉTODO DE DOS PASOS - RESULTADOS\n");
        fprintf(archivo, "# Problema: dy/dx = (x·e^(x²))/y, y(0)=1\n");
        fprintf(archivo, "# Solución exacta: y(x) = e^(x²/2)\n");
        fprintf(archivo, "# Fórmula: y_{i+1} = y_i + h(2f(x_i,y_i) - f(x_{i-1},y_{i-1}))\n");
        fprintf(archivo, "#\n");
        fprintf(archivo, "# Parámetros:\n");
        fprintf(archivo, "#   Intervalo: [%.1lf, %.1lf]\n", x_0, x_f);
        fprintf(archivo, "#   Paso h: %.6lf\n", h);
        fprintf(archivo, "#   Número de pasos: %d\n", n);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_aprox\ty_exacta\terror_abs\terror_rel(%%)\n");
        
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\t%.10lf\t%.6e\t%.6lf\n",
                    i, x[i], y[i], y_exacta[i], error_abs[i], error_rel[i]);
        }
        
        fprintf(archivo, "\n# Error máximo: %.6e\n", error_max);
        fprintf(archivo, "# Error promedio: %.6e\n", error_prom);
        
        fclose(archivo);
        printf("✓ Resultados guardados en 'metodo_dospasos_resultados.txt'\n\n");
    }
    
    /* Liberar memoria */
    free(x);
    free(y);
    free(y_exacta);
    free(error_abs);
    free(error_rel);
    free(f_actual);
    free(f_anterior);
    
    printf("Presione ENTER para continuar...");
    getchar();
    getchar();
}
