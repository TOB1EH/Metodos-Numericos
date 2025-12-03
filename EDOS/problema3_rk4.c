/**
 * @file problema3_rk4.c
 * @brief Problema 3 - Inciso b) Resolver con Runge-Kutta 4to orden
 * @author Tobias Funes
 * 
 * EDO: d²y/dx² + 2(dy/dx) + 5y = 0
 * Condiciones: y(0) = 0, y'(0) = 2
 * Intervalo: [0, 1] con h = 0.01
 * 
 * CONVERSIÓN A SISTEMA DE PRIMER ORDEN:
 *   y₁ = y
 *   y₂ = y' = dy/dx
 * 
 * Sistema equivalente:
 *   dy₁/dx = y₂
 *   dy₂/dx = -2y₂ - 5y₁
 * 
 * Condiciones iniciales:
 *   y₁(0) = 0
 *   y₂(0) = 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ============================================================================
// DEFINICIÓN DEL SISTEMA DE EDOs
// ============================================================================

/**
 * @brief Primera ecuación: dy₁/dx = y₂
 * @param x Variable independiente (no usado en esta ecuación)
 * @param y1 y₁ = y (la función)
 * @param y2 y₂ = y' (la derivada)
 * @return dy₁/dx
 */
double f1(double x, double y1, double y2) {
    (void)x;   // No se usa x
    (void)y1;  // No se usa y1
    return y2;  // dy₁/dx = y₂
}

/**
 * @brief Segunda ecuación: dy₂/dx = -2y₂ - 5y₁
 * @details
 * Esta viene de la EDO original:
 *   y'' + 2y' + 5y = 0
 *   y'' = -2y' - 5y
 *   dy₂/dx = -2y₂ - 5y₁
 * 
 * @param x Variable independiente (no usado)
 * @param y1 y₁ = y
 * @param y2 y₂ = y'
 * @return dy₂/dx
 */
double f2(double x, double y1, double y2) {
    (void)x;  // No se usa x
    return -2.0 * y2 - 5.0 * y1;  // dy₂/dx = -2y₂ - 5y₁
}

// ============================================================================
// SOLUCIÓN EXACTA (para comparación)
// ============================================================================

/**
 * @brief Solución exacta: y(x) = e^(-x)·sin(2x)
 */
double solucionExacta(double x) {
    return exp(-x) * sin(2.0 * x);
}

/**
 * @brief Derivada exacta: y'(x)
 */
double derivadaExacta(double x) {
    return exp(-x) * (2.0 * cos(2.0 * x) - sin(2.0 * x));
}

// ============================================================================
// MÉTODO DE RUNGE-KUTTA 4TO ORDEN
// ============================================================================

/**
 * @brief Resuelve el sistema usando RK4
 */
void rungeKutta4(double x0, double xf, double y1_0, double y2_0, int n,
                 double **x_out, double **y1_out, double **y2_out) {
    
    double h = (xf - x0) / n;
    
    // Reservar memoria
    double *x = (double *)malloc((n + 1) * sizeof(double));
    double *y1 = (double *)malloc((n + 1) * sizeof(double));
    double *y2 = (double *)malloc((n + 1) * sizeof(double));
    
    // Condiciones iniciales
    x[0] = x0;
    y1[0] = y1_0;
    y2[0] = y2_0;
    
    // Iteración de Runge-Kutta 4
    for (int i = 0; i < n; i++) {
        // Coeficientes k₁
        double k1_1 = f1(x[i], y1[i], y2[i]);
        double k1_2 = f2(x[i], y1[i], y2[i]);
        
        // Coeficientes k₂
        double k2_1 = f1(x[i] + h/2.0, y1[i] + (h/2.0)*k1_1, y2[i] + (h/2.0)*k1_2);
        double k2_2 = f2(x[i] + h/2.0, y1[i] + (h/2.0)*k1_1, y2[i] + (h/2.0)*k1_2);
        
        // Coeficientes k₃
        double k3_1 = f1(x[i] + h/2.0, y1[i] + (h/2.0)*k2_1, y2[i] + (h/2.0)*k2_2);
        double k3_2 = f2(x[i] + h/2.0, y1[i] + (h/2.0)*k2_1, y2[i] + (h/2.0)*k2_2);
        
        // Coeficientes k₄
        double k4_1 = f1(x[i] + h, y1[i] + h*k3_1, y2[i] + h*k3_2);
        double k4_2 = f2(x[i] + h, y1[i] + h*k3_1, y2[i] + h*k3_2);
        
        // Actualizar x
        x[i + 1] = x[i] + h;
        
        // Actualizar y₁ y y₂
        y1[i + 1] = y1[i] + (h/6.0) * (k1_1 + 2.0*k2_1 + 2.0*k3_1 + k4_1);
        y2[i + 1] = y2[i] + (h/6.0) * (k1_2 + 2.0*k2_2 + 2.0*k3_2 + k4_2);
    }
    
    // Devolver resultados
    *x_out = x;
    *y1_out = y1;
    *y2_out = y2;
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    // Parámetros del problema
    double x0, xf, h;
    int n;
    
    // Condiciones iniciales
    double y1_0, y2_0;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                INCISO b) - RUNGE-KUTTA 4TO ORDEN                   ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    // Entrada de datos
    printf("PARÁMETROS DEL PROBLEMA:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("Ingrese el valor inicial de x (x_0): ");
    scanf("%lf", &x0);
    printf("Ingrese el valor final de x (x_f): ");
    scanf("%lf", &xf);
    printf("Ingrese el valor inicial de y (y_0): ");
    scanf("%lf", &y1_0);
    printf("Ingrese el valor inicial de y' (y'_0): ");
    scanf("%lf", &y2_0);
    
    // Selección de método de entrada (h o n)
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
        h = (xf - x0) / n;
        printf("→ Tamaño del paso calculado: h = %.6lf\n", h);
    } else if (opcion_entrada == 'b' || opcion_entrada == 'B') {
        /* Usuario ingresa h, calculamos n */
        printf("Ingrese el tamaño del paso (h): ");
        scanf("%lf", &h);
        n = (int)((xf - x0) / h);
        
        /* Ajustar h para que sea exacto */
        h = (xf - x0) / n;
        printf("→ Número de pasos calculado: n = %d\n", n);
        printf("→ Tamaño del paso ajustado: h = %.6lf\n", h);
    } else {
        printf("Opción no válida. Usando valores por defecto del enunciado.\n");
        h = 0.01;
        n = (int)((xf - x0) / h);
        printf("→ Número de pasos: n = %d\n", n);
        printf("→ Tamaño del paso: h = %.4f\n", h);
    }
    
    printf("\n");
    printf("RESUMEN DE PARÁMETROS:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("  Intervalo: [%.1f, %.1f]\n", x0, xf);
    printf("  Paso h: %.6f\n", h);
    printf("  Número de pasos: %d\n", n);
    printf("  Condiciones iniciales: y(0) = %.1f, y'(0) = %.1f\n\n", y1_0, y2_0);
    
    printf("Sistema de EDOs:\n");
    printf("  dy₁/dx = y₂\n");
    printf("  dy₂/dx = -2y₂ - 5y₁\n\n");
    
    // Resolver con RK4
    double *x, *y1, *y2;
    rungeKutta4(x0, xf, y1_0, y2_0, n, &x, &y1, &y2);
    
    // Guardar resultados completos en archivo
    FILE *fp = fopen("problema3_rk4_completo.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "# Problema 3 - Runge-Kutta 4to orden (h = %.4f)\n", h);
        fprintf(fp, "# i x y_numerico y_exacto error_absoluto error_porcentual\n");
        for (int i = 0; i <= n; i++) {
            double y_exacto = solucionExacta(x[i]);
            double error_abs = fabs(y1[i] - y_exacto);
            double error_pct = fabs(y_exacto) > 1e-10 ? (error_abs / fabs(y_exacto)) * 100.0 : 0.0;
            fprintf(fp, "%d %.10f %.10f %.10f %.10e %.10f\n", 
                    i, x[i], y1[i], y_exacto, error_abs, error_pct);
        }
        fclose(fp);
        printf("✅ Resultados completos guardados en: problema3_rk4_completo.txt\n\n");
    }
    
    // Mostrar ÚLTIMOS 6 PUNTOS (como pide el inciso b)
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    ÚLTIMOS 6 PUNTOS (Inciso b)                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("    x        y(numérico)    y(exacto)     y'(numérico)   Error abs    Error %%\n");
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    int inicio_ultimos_6 = n - 5;  // Últimos 6 puntos (incluyendo el final)
    
    for (int i = inicio_ultimos_6; i <= n; i++) {
        double y_exacto = solucionExacta(x[i]);
        double yp_numerico = y2[i];  // y' numérica
        double error_abs = fabs(y1[i] - y_exacto);
        double error_pct = fabs(y_exacto) > 1e-10 ? (error_abs / fabs(y_exacto)) * 100.0 : 0.0;
        
        printf("  %.2f   %12.8f   %12.8f   %12.8f   %.4e   %.6f%%\n",
               x[i], y1[i], y_exacto, yp_numerico, error_abs, error_pct);
    }
    
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    // Estadísticas de error
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     ESTADÍSTICAS DE ERROR                          ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    double error_max = 0.0;
    double error_promedio = 0.0;
    
    for (int i = 0; i <= n; i++) {
        double y_exacto = solucionExacta(x[i]);
        double error_abs = fabs(y1[i] - y_exacto);
        if (error_abs > error_max) error_max = error_abs;
        error_promedio += error_abs;
    }
    error_promedio /= (n + 1);
    
    printf("  Error máximo absoluto: %.10e\n", error_max);
    printf("  Error promedio absoluto: %.10e\n", error_promedio);
    printf("  Número total de puntos: %d\n", n + 1);
    
    // Verificar condiciones iniciales
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                VERIFICACIÓN DE CONDICIONES INICIALES               ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("  y(0) numérico  = %.10f  (debe ser 0.0) %s\n", y1[0], 
           fabs(y1[0]) < 1e-10 ? "✓" : "✗");
    printf("  y'(0) numérico = %.10f  (debe ser 2.0) %s\n", y2[0],
           fabs(y2[0] - 2.0) < 1e-10 ? "✓" : "✗");
    
    // Liberar memoria
    free(x);
    free(y1);
    free(y2);
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     INCISO b) COMPLETADO ✓                         ║\n");
    printf("║                                                                    ║\n");
    printf("║  Próximo paso:                                                     ║\n");
    printf("║    c) Graficar solución exacta vs numérica                         ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    return 0;
}
