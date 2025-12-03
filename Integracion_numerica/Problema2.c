/**
 * ============================================================================
 * PROBLEMA 2: Cálculo de Integral con Derivada Numérica
 * ============================================================================
 * 
 * Calcular la siguiente integral mediante la regla del Trapecio compuesto:
 * 
 *   I = ∫[0,2] x · (df/dx) dx
 * 
 * donde la función f(x) está dada en forma discreta (tabla de datos).
 * 
 * RESTRICCIÓN: El cálculo de df/dx debe realizarse con un esquema de SEGUNDO ORDEN.
 * 
 * METODOLOGÍA (3 pasos):
 * 
 * PASO 1: Calcular df/dx usando DIFERENCIAS FINITAS DE 2DO ORDEN
 *         - Puntos interiores: f'(xᵢ) = [f(xᵢ₊₁) - f(xᵢ₋₁)] / (2h)  → O(h²)
 *         - Primer punto: f'(x₀) = [f(x₁) - f(x₀)] / h              → O(h)
 *         - Último punto: f'(xₙ) = [f(xₙ) - f(xₙ₋₁)] / h            → O(h)
 * 
 * PASO 2: Calcular la función g(x) = x · f'(x) en cada punto
 * 
 * PASO 3: Integrar g(x) usando REGLA DEL TRAPECIO COMPUESTO
 *         I ≈ (h/2) · [g(x₀) + 2·Σg(xᵢ) + g(xₙ)]
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
   PROTOTIPOS DE FUNCIONES
   ============================================================================ */

void leerDatosTabla(const char *filename, double **x, double **f, int *n);
void calcularDerivadas2doOrden(double *x, double *f, int n, double *f_prima);
void calcularProducto(double *x, double *f_prima, int n, double *g);
double integrarTrapecio(double *x, double *g, int n);
void mostrarTablaResultados(double *x, double *f, double *f_prima, double *g, int n);

/* ============================================================================
   FUNCIÓN PRINCIPAL
   ============================================================================ */

int main(void)
{
    double *x = NULL;       // Array de coordenadas x
    double *f = NULL;       // Array de valores f(x)
    double *f_prima = NULL; // Array de derivadas f'(x)
    double *g = NULL;       // Array de productos g(x) = x · f'(x)
    int n = 0;              // Número de puntos
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  PROBLEMA 2: Integral de x·(df/dx) con Trapecio Compuesto     ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    /* ========================================================================
       PASO 1: LEER DATOS DE LA TABLA
       ======================================================================== */
    
    printf("\n┌────────────────────────────────────────────────────────────────┐\n");
    printf("│ PASO 1: Lectura de Datos                                      │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    leerDatosTabla("datos_tabla.txt", &x, &f, &n);
    
    /* Asignar memoria para derivadas y productos */
    f_prima = (double *)malloc(n * sizeof(double));
    g = (double *)malloc(n * sizeof(double));
    
    if (!f_prima || !g) {
        printf("[ERROR] No se pudo asignar memoria.\n");
        free(x); free(f); free(f_prima); free(g);
        return 1;
    }
    
    /* ========================================================================
       PASO 2: CALCULAR DERIVADAS CON ESQUEMA DE 2DO ORDEN
       ======================================================================== */
    
    printf("\n┌────────────────────────────────────────────────────────────────┐\n");
    printf("│ PASO 2: Cálculo de df/dx (Esquema de Segundo Orden)           │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    calcularDerivadas2doOrden(x, f, n, f_prima);
    
    printf("\nMétodo utilizado:\n");
    printf("  • Punto inicial:  f'(x₀) = [f(x₁)-f(x₀)]/h     (adelante)\n");
    printf("  • Puntos medios:  f'(xᵢ) = [f(xᵢ₊₁)-f(xᵢ₋₁)]/(2h)  (centrada, O(h²))\n");
    printf("  • Punto final:    f'(xₙ) = [f(xₙ)-f(xₙ₋₁)]/h (atrás)\n");
    
    /* ========================================================================
       PASO 3: CALCULAR g(x) = x · f'(x)
       ======================================================================== */
    
    printf("\n┌────────────────────────────────────────────────────────────────┐\n");
    printf("│ PASO 3: Cálculo de g(x) = x · f'(x)                           │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    calcularProducto(x, f_prima, n, g);
    
    /* ========================================================================
       PASO 4: INTEGRAR USANDO TRAPECIO COMPUESTO
       ======================================================================== */
    
    printf("\n┌────────────────────────────────────────────────────────────────┐\n");
    printf("│ PASO 4: Integración por Regla del Trapecio Compuesto          │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    double resultado = integrarTrapecio(x, g, n);
    
    /* ========================================================================
       MOSTRAR TABLA DE RESULTADOS
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  TABLA DE RESULTADOS DETALLADA                                 ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    mostrarTablaResultados(x, f, f_prima, g, n);
    
    /* ========================================================================
       RESULTADO FINAL
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  RESULTADO FINAL                                               ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                ║\n");
    printf("║  I = ∫[0,2] x·(df/dx) dx ≈ %.10lf               ║\n", resultado);
    printf("║                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    /* Información adicional */
    printf("\nDETALLES DEL CÁLCULO:\n");
    printf("  • Número de puntos:        %d\n", n);
    printf("  • Número de subintervalos: %d\n", n-1);
    printf("  • Método de derivación:    Diferencias finitas 2do orden\n");
    printf("  • Método de integración:   Trapecio compuesto\n");
    
    /* ========================================================================
       GUARDAR RESULTADOS EN ARCHIVO
       ======================================================================== */
    
    FILE *archivo = fopen("resultados_problema2.txt", "w");
    if (archivo != NULL) {
        fprintf(archivo, "# PROBLEMA 2: Integral de x·(df/dx)\n");
        fprintf(archivo, "# I = ∫[0,2] x·(df/dx) dx\n");
        fprintf(archivo, "#\n");
        fprintf(archivo, "# Método: Trapecio compuesto con derivadas de 2do orden\n");
        fprintf(archivo, "# Número de puntos: %d\n", n);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# Resultado: %.10lf\n", resultado);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\tf(x_i)\t\tf'(x_i)\t\tg(x_i)=x·f'(x)\n");
        
        for (int i = 0; i < n; i++) {
            fprintf(archivo, "%d\t%.4lf\t%.6lf\t%.6lf\t%.6lf\n", 
                    i, x[i], f[i], f_prima[i], g[i]);
        }
        
        fclose(archivo);
        printf("\n✓ Resultados guardados en 'resultados_problema2.txt'\n");
    }
    
    /* Liberar memoria */
    free(x);
    free(f);
    free(f_prima);
    free(g);
    
    return 0;
}

/* ============================================================================
   IMPLEMENTACIÓN DE FUNCIONES
   ============================================================================ */

/**
 * Lee datos de una tabla desde archivo
 * Formato: cada línea contiene "x f(x)"
 */
void leerDatosTabla(const char *filename, double **x, double **f, int *n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[ERROR] No se pudo abrir el archivo '%s'\n", filename);
        printf("Creando archivo de ejemplo...\n");
        
        // Crear archivo de ejemplo con los datos del problema
        FILE *ejemplo = fopen(filename, "w");
        if (ejemplo) {
            fprintf(ejemplo, "0.00  1.000\n");
            fprintf(ejemplo, "0.25  1.384\n");
            fprintf(ejemplo, "0.50  1.849\n");
            fprintf(ejemplo, "0.75  2.417\n");
            fprintf(ejemplo, "1.00  3.118\n");
            fprintf(ejemplo, "1.25  3.990\n");
            fprintf(ejemplo, "1.50  5.082\n");
            fprintf(ejemplo, "2.00  8.189\n");
            fclose(ejemplo);
            printf("✓ Archivo creado con datos de ejemplo.\n");
            file = fopen(filename, "r");
        }
    }
    
    // Contar número de líneas
    *n = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }
    rewind(file);
    
    // Asignar memoria
    *x = (double *)malloc(*n * sizeof(double));
    *f = (double *)malloc(*n * sizeof(double));
    
    // Leer datos
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%lf %lf", &(*x)[i], &(*f)[i]);
    }
    
    fclose(file);
    
    printf("\n✓ Datos leídos correctamente:\n");
    printf("  Archivo: %s\n", filename);
    printf("  Puntos:  %d\n", *n);
    printf("  Rango:   [%.2lf, %.2lf]\n", (*x)[0], (*x)[*n-1]);
}

/**
 * Calcula derivadas usando esquema de SEGUNDO ORDEN
 * 
 * - Puntos interiores: diferencias centradas O(h²)
 * - Extremos: diferencias adelante/atrás O(h)
 */
void calcularDerivadas2doOrden(double *x, double *f, int n, double *f_prima)
{
    /* Primer punto: diferencia hacia adelante */
    double h0 = x[1] - x[0];
    f_prima[0] = (f[1] - f[0]) / h0;
    
    /* Puntos interiores: diferencias centradas (2do orden) */
    for (int i = 1; i < n-1; i++) {
        double h_centrada = x[i+1] - x[i-1];
        f_prima[i] = (f[i+1] - f[i-1]) / h_centrada;
    }
    
    /* Último punto: diferencia hacia atrás */
    double h_last = x[n-1] - x[n-2];
    f_prima[n-1] = (f[n-1] - f[n-2]) / h_last;
}

/**
 * Calcula g(x) = x · f'(x) para cada punto
 */
void calcularProducto(double *x, double *f_prima, int n, double *g)
{
    for (int i = 0; i < n; i++) {
        g[i] = x[i] * f_prima[i];
    }
}

/**
 * Integra g(x) usando la Regla del Trapecio Compuesto
 * 
 * Fórmula: I ≈ (h/2) · [g₀ + 2·(g₁ + g₂ + ... + gₙ₋₁) + gₙ]
 * 
 * NOTA: Para datos no equiespaciados, se usa la suma de trapecios individuales
 */
double integrarTrapecio(double *x, double *g, int n)
{
    double suma = 0.0;
    
    /* Verificar si los datos son equiespaciados */
    int equiespaciado = 1;
    double h_ref = x[1] - x[0];
    double tolerancia = 1e-6;
    
    for (int i = 1; i < n-1; i++) {
        double h_actual = x[i+1] - x[i];
        if (fabs(h_actual - h_ref) > tolerancia) {
            equiespaciado = 0;
            break;
        }
    }
    
    if (equiespaciado) {
        /* CASO 1: Datos equiespaciados - fórmula estándar */
        printf("\n  → Datos EQUIESPACIADOS detectados (h = %.4lf)\n", h_ref);
        printf("  → Usando fórmula estándar del trapecio\n");
        
        suma = g[0] + g[n-1];
        for (int i = 1; i < n-1; i++) {
            suma += 2.0 * g[i];
        }
        suma = (h_ref / 2.0) * suma;
        
    } else {
        /* CASO 2: Datos NO equiespaciados - suma de trapecios */
        printf("\n  → Datos NO EQUIESPACIADOS detectados\n");
        printf("  → Usando suma de trapecios individuales\n");
        
        for (int i = 0; i < n-1; i++) {
            double h_i = x[i+1] - x[i];
            suma += (h_i / 2.0) * (g[i] + g[i+1]);
        }
    }
    
    return suma;
}

/**
 * Muestra tabla con todos los cálculos intermedios
 */
void mostrarTablaResultados(double *x, double *f, double *f_prima, double *g, int n)
{
    printf("\n┌──────┬──────────┬──────────┬──────────────┬──────────────┐\n");
    printf("│  i   │   x_i    │   f(x)   │   f'(x)      │  g=x·f'(x)   │\n");
    printf("├──────┼──────────┼──────────┼──────────────┼──────────────┤\n");
    
    for (int i = 0; i < n; i++) {
        printf("│ %3d  │ %7.2lf  │ %8.3lf │ %12.6lf │ %12.6lf │\n",
               i, x[i], f[i], f_prima[i], g[i]);
    }
    
    printf("└──────┴──────────┴──────────┴──────────────┴──────────────┘\n");
}