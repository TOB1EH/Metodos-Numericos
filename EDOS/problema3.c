/**
 * ============================================================================
 * PROBLEMA 3: Resolución de EDO con Euler y RK4
 * ============================================================================
 * 
 * Ecuación diferencial:
 *   dy/dt / y - (4y - t²) = 0  con y(0) = 1
 * 
 * Simplificando:
 *   dy/dt = y · (4y - t²)
 * 
 * TAREAS:
 * 1. Resolver con EULER usando h = 0.1
 * 2. Resolver con RK4 usando h = 0.1
 * 3. Calcular error relativo en t = 1, 2, 3
 * 4. Fórmula del error: e_rel = |y^E - y^RK4| / y^RK4
 * 5. Mostrar valores de y(1), y(2), y(3) con ambos métodos
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
   PROTOTIPOS DE FUNCIONES
   ============================================================================ */

double f(double t, double y);
void euler(double t0, double y0, double tf, double h, double **t_arr, double **y_arr, int *n);
void rk4(double t0, double y0, double tf, double h, double **t_arr, double **y_arr, int *n);
double obtenerValorEn(double *t_arr, double *y_arr, int n, double t_objetivo);
void mostrarTablaComparativa(double *t_euler, double *y_euler, int n_euler,
                             double *t_rk4, double *y_rk4, int n_rk4);

/* ============================================================================
   FUNCIÓN PRINCIPAL
   ============================================================================ */

int main(void)
{
    /* Parámetros del problema */
    double t0 = 0.0;   // Condición inicial: t
    double y0 = 1.0;   // Condición inicial: y(0) = 1
    double tf = 3.0;   // Tiempo final
    double h = 0.1;    // Paso de integración
    
    /* Arrays para almacenar resultados */
    double *t_euler = NULL, *y_euler = NULL;
    double *t_rk4 = NULL, *y_rk4 = NULL;
    int n_euler = 0, n_rk4 = 0;
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  PROBLEMA 3: Resolución de EDO con Euler y RK4                ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\nEcuación diferencial:\n");
    printf("  dy/dt = y · (4y - t²)\n");
    printf("  Condición inicial: y(0) = 1\n");
    printf("  Intervalo: [0, 3]\n");
    printf("  Paso: h = %.1lf\n", h);
    
    /* ========================================================================
       PASO 1: RESOLVER CON EULER
       ======================================================================== */
    
    printf("\n┌────────────────────────────────────────────────────────────────┐\n");
    printf("│ MÉTODO DE EULER                                                │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    printf("\nResolviendo con Euler...\n");
    euler(t0, y0, tf, h, &t_euler, &y_euler, &n_euler);
    printf("✓ Euler completado (%d puntos)\n", n_euler);
    
    /* ========================================================================
       PASO 2: RESOLVER CON RK4
       ======================================================================== */
    
    printf("\n┌────────────────────────────────────────────────────────────────┐\n");
    printf("│ MÉTODO DE RUNGE-KUTTA 4                                        │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    printf("\nResolviendo con RK4...\n");
    rk4(t0, y0, tf, h, &t_rk4, &y_rk4, &n_rk4);
    printf("✓ RK4 completado (%d puntos)\n", n_rk4);
    
    /* ========================================================================
       PASO 3: EXTRAER VALORES EN t = 1, 2, 3
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  RESULTADOS EN PUNTOS ESPECÍFICOS                              ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    double t_objetivos[] = {1.0, 2.0, 3.0};
    int num_objetivos = 3;
    
    printf("\n┌──────────┬──────────────────┬──────────────────┬──────────────────┐\n");
    printf("│    t     │   y (Euler)      │    y (RK4)       │  Error Relativo  │\n");
    printf("├──────────┼──────────────────┼──────────────────┼──────────────────┤\n");
    
    for (int i = 0; i < num_objetivos; i++) {
        double t = t_objetivos[i];
        double y_e = obtenerValorEn(t_euler, y_euler, n_euler, t);
        double y_r = obtenerValorEn(t_rk4, y_rk4, n_rk4, t);
        
        // Calcular error relativo: |y^E - y^RK4| / y^RK4
        double error_rel = fabs(y_e - y_r) / fabs(y_r);
        double error_rel_porcentaje = error_rel * 100.0;
        
        printf("│  %.1lf     │  %14.10lf  │  %14.10lf  │  %10.6lf%%      │\n",
               t, y_e, y_r, error_rel_porcentaje);
    }
    
    printf("└──────────┴──────────────────┴──────────────────┴──────────────────┘\n");
    
    /* ========================================================================
       PASO 4: TABLA COMPARATIVA COMPLETA
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  TABLA COMPARATIVA COMPLETA (cada 10 pasos)                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    mostrarTablaComparativa(t_euler, y_euler, n_euler, t_rk4, y_rk4, n_rk4);
    
    /* ========================================================================
       PASO 5: ANÁLISIS FINAL
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ANÁLISIS DE RESULTADOS                                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    double y3_euler = obtenerValorEn(t_euler, y_euler, n_euler, 3.0);
    double y3_rk4 = obtenerValorEn(t_rk4, y_rk4, n_rk4, 3.0);
    double error_final = fabs(y3_euler - y3_rk4) / fabs(y3_rk4) * 100.0;
    
    printf("\nVALOR FINAL EN t = 3:\n");
    printf("  y(3) con Euler: %.10lf\n", y3_euler);
    printf("  y(3) con RK4:   %.10lf\n", y3_rk4);
    printf("  Error relativo: %.6lf%%\n", error_final);
    
    printf("\nCONCLUSIONES:\n");
    printf("  • Euler es un método de orden 1: error O(h)\n");
    printf("  • RK4 es un método de orden 4: error O(h⁴)\n");
    printf("  • RK4 es más preciso para el mismo paso h\n");
    printf("  • El error acumulado aumenta con el tiempo\n");
    
    /* ========================================================================
       PASO 6: GUARDAR RESULTADOS
       ======================================================================== */
    
    FILE *archivo = fopen("resultados_problema3.txt", "w");
    if (archivo != NULL) {
        fprintf(archivo, "# PROBLEMA 3: EDO con Euler y RK4\n");
        fprintf(archivo, "# dy/dt = y · (4y - t²), y(0) = 1\n");
        fprintf(archivo, "# h = %.1lf\n\n", h);
        
        fprintf(archivo, "# RESULTADOS EN PUNTOS ESPECÍFICOS:\n");
        fprintf(archivo, "# t\ty_Euler\t\ty_RK4\t\tError_Rel(%%)\n");
        
        for (int i = 0; i < num_objetivos; i++) {
            double t = t_objetivos[i];
            double y_e = obtenerValorEn(t_euler, y_euler, n_euler, t);
            double y_r = obtenerValorEn(t_rk4, y_rk4, n_rk4, t);
            double error_rel = fabs(y_e - y_r) / fabs(y_r) * 100.0;
            
            fprintf(archivo, "%.1lf\t%.10lf\t%.10lf\t%.6lf\n",
                    t, y_e, y_r, error_rel);
        }
        
        fprintf(archivo, "\n# TABLA COMPLETA:\n");
        fprintf(archivo, "# i\tt\ty_Euler\t\ty_RK4\n");
        for (int i = 0; i < n_euler; i++) {
            fprintf(archivo, "%d\t%.2lf\t%.10lf\t%.10lf\n",
                    i, t_euler[i], y_euler[i], y_rk4[i]);
        }
        
        fclose(archivo);
        printf("\n✓ Resultados guardados en 'resultados_problema3.txt'\n");
    }
    
    /* Liberar memoria */
    free(t_euler);
    free(y_euler);
    free(t_rk4);
    free(y_rk4);
    
    return 0;
}

/* ============================================================================
   IMPLEMENTACIÓN DE FUNCIONES
   ============================================================================ */

/**
 * Función f(t, y) que define la EDO
 * 
 * dy/dt = f(t, y) = y · (4y - t²)
 */
double f(double t, double y)
{
    return y * (4.0 * y - t * t);
}

/**
 * Método de Euler
 * 
 * Fórmula: y[i+1] = y[i] + h · f(t[i], y[i])
 */
void euler(double t0, double y0, double tf, double h, 
           double **t_arr, double **y_arr, int *n)
{
    *n = (int)((tf - t0) / h) + 1;
    
    *t_arr = (double *)malloc(*n * sizeof(double));
    *y_arr = (double *)malloc(*n * sizeof(double));
    
    (*t_arr)[0] = t0;
    (*y_arr)[0] = y0;
    
    for (int i = 0; i < *n - 1; i++) {
        (*t_arr)[i + 1] = (*t_arr)[i] + h;
        (*y_arr)[i + 1] = (*y_arr)[i] + h * f((*t_arr)[i], (*y_arr)[i]);
    }
}

/**
 * Método de Runge-Kutta 4to Orden
 * 
 * Fórmula:
 *   k1 = f(t, y)
 *   k2 = f(t + h/2, y + h/2·k1)
 *   k3 = f(t + h/2, y + h/2·k2)
 *   k4 = f(t + h, y + h·k3)
 *   y[i+1] = y[i] + (h/6)·(k1 + 2k2 + 2k3 + k4)
 */
void rk4(double t0, double y0, double tf, double h,
         double **t_arr, double **y_arr, int *n)
{
    *n = (int)((tf - t0) / h) + 1;
    
    *t_arr = (double *)malloc(*n * sizeof(double));
    *y_arr = (double *)malloc(*n * sizeof(double));
    
    (*t_arr)[0] = t0;
    (*y_arr)[0] = y0;
    
    for (int i = 0; i < *n - 1; i++) {
        double t = (*t_arr)[i];
        double y = (*y_arr)[i];
        
        double k1 = f(t, y);
        double k2 = f(t + h/2.0, y + (h/2.0)*k1);
        double k3 = f(t + h/2.0, y + (h/2.0)*k2);
        double k4 = f(t + h, y + h*k3);
        
        (*t_arr)[i + 1] = t + h;
        (*y_arr)[i + 1] = y + (h/6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
    }
}

/**
 * Obtiene el valor de y en un tiempo objetivo específico
 */
double obtenerValorEn(double *t_arr, double *y_arr, int n, double t_objetivo)
{
    for (int i = 0; i < n; i++) {
        if (fabs(t_arr[i] - t_objetivo) < 1e-6) {
            return y_arr[i];
        }
    }
    return 0.0; // No encontrado
}

/**
 * Muestra tabla comparativa entre Euler y RK4
 */
void mostrarTablaComparativa(double *t_euler, double *y_euler, int n_euler,
                             double *t_rk4, double *y_rk4, int n_rk4)
{
    printf("\n┌──────┬──────────┬──────────────────┬──────────────────┬──────────────┐\n");
    printf("│  i   │    t     │   y (Euler)      │    y (RK4)       │  Error Rel   │\n");
    printf("├──────┼──────────┼──────────────────┼──────────────────┼──────────────┤\n");
    
    // Mostrar cada 10 pasos para no saturar la pantalla
    int paso = 10;
    for (int i = 0; i < n_euler; i += paso) {
        double error_rel = fabs(y_euler[i] - y_rk4[i]) / fabs(y_rk4[i]) * 100.0;
        
        printf("│ %4d │  %.2lf    │  %14.10lf  │  %14.10lf  │  %10.6lf%% │\n",
               i, t_euler[i], y_euler[i], y_rk4[i], error_rel);
    }
    
    // Asegurar que se muestre el último punto
    int ultimo = n_euler - 1;
    if (ultimo % paso != 0) {
        double error_rel = fabs(y_euler[ultimo] - y_rk4[ultimo]) / fabs(y_rk4[ultimo]) * 100.0;
        printf("│ %4d │  %.2lf    │  %14.10lf  │  %14.10lf  │  %10.6lf%% │\n",
               ultimo, t_euler[ultimo], y_euler[ultimo], y_rk4[ultimo], error_rel);
    }
    
    printf("└──────┴──────────┴──────────────────┴──────────────────┴──────────────┘\n");
}