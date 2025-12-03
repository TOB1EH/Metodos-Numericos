#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
   CONFIGURACIÓN DE LA FUNCIÓN
   ============================================================================ */
#define FUNCION_DESCRIPCION "f(x) = e^(2·sin(x)) · [1 + ln(x)]"

/* ============================================================================
   PROTOTIPOS
   ============================================================================ */
double f_exacta(double x);
double simpsonCompuesto(double a, double b, int n_sub, double *x_tabla, double *y_tabla, int usar_tabla);
int leerDatosDesdeArchivo(const char *filename, double **x, double **y);

int main(int argc, char const *argv[])
{
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║      INTEGRACIÓN CON SIMPSON COMPUESTO 1/3               ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    /* Leer datos desde nodos.txt */
    double *x_tabla = NULL;
    double *y_tabla = NULL;
    int n_puntos = 0;
    
    printf("\nLeyendo datos desde 'nodos.txt'...\n");
    n_puntos = leerDatosDesdeArchivo("nodos.txt", &x_tabla, &y_tabla);
    
    if (n_puntos == 0) {
        printf("[ERROR] No se pudieron leer los datos del archivo.\n");
        return 1;
    }
    
    printf("✓ Se leyeron %d puntos correctamente\n", n_puntos);
    printf("\nDatos leídos:\n");
    printf("  Intervalo: [%.4lf, %.4lf]\n", x_tabla[0], x_tabla[n_puntos-1]);
    printf("  Primer punto: (%.4lf, %.4lf)\n", x_tabla[0], y_tabla[0]);
    printf("  Último punto: (%.4lf, %.4lf)\n", x_tabla[n_puntos-1], y_tabla[n_puntos-1]);
    
    /* Determinar número de subintervalos */
    int n_subintervalos = n_puntos - 1;
    
    printf("\nFunción: %s\n", FUNCION_DESCRIPCION);
    printf("Número de subintervalos: %d\n", n_subintervalos);
    
    /* Verificar si es par para Simpson 1/3 */
    if (n_subintervalos % 2 != 0) {
        printf("\nAJUSTE: Simpson 1/3 requiere n subintervalos PAR\n");
        n_puntos--;
        n_subintervalos = n_puntos - 1;
        printf("   → Usando %d puntos (%d subintervalos)\n", n_puntos, n_subintervalos);
    }
    
    /* ========================================================================
       OPCIÓN 1: INTEGRAR CON SIMPSON USANDO DATOS DE LA TABLA (SPLINE)
       ======================================================================== */
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  INTEGRACIÓN CON DATOS DE LA TABLA (spline)              ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    double integral_spline = simpsonCompuesto(x_tabla[0], x_tabla[n_puntos-1], 
                                             n_subintervalos, x_tabla, y_tabla, 1);
    
    printf("\nResultado:\n");
    printf("  ∫[%.2lf, %.2lf] f(x)dx ≈ %.10lf  (usando datos de tabla)\n", 
           x_tabla[0], x_tabla[n_puntos-1], integral_spline);
    
    /* ========================================================================
       OPCIÓN 2: INTEGRAR CON SIMPSON USANDO FUNCIÓN EXACTA
       ======================================================================== */
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  INTEGRACIÓN CON FUNCIÓN EXACTA                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    double integral_exacta = simpsonCompuesto(x_tabla[0], x_tabla[n_puntos-1], 
                                             n_subintervalos, NULL, NULL, 0);
    
    printf("\nResultado:\n");
    printf("  ∫[%.2lf, %.2lf] f(x)dx ≈ %.10lf  (usando función exacta)\n", 
           x_tabla[0], x_tabla[n_puntos-1], integral_exacta);
    
    /* ========================================================================
       COMPARACIÓN FINAL
       ======================================================================== */
    
    double diferencia = fabs(integral_spline - integral_exacta);
    double error_rel = (diferencia / fabs(integral_exacta)) * 100.0;
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  COMPARACIÓN DE RESULTADOS                                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n┌──────────────────────────┬──────────────────┐\n");
    printf("│ Método                   │    Resultado     │\n");
    printf("├──────────────────────────┼──────────────────┤\n");
    printf("│ Simpson con tabla        │  %14.10lf  │\n", integral_spline);
    printf("│ Simpson con f exacta     │  %14.10lf  │\n", integral_exacta);
    printf("├──────────────────────────┼──────────────────┤\n");
    printf("│ Diferencia absoluta      │  %14.10lf  │\n", diferencia);
    printf("│ Error relativo           │  %12.6lf%%  │\n", error_rel);
    printf("└──────────────────────────┴──────────────────┘\n");
    
    /* Liberar memoria */
    free(x_tabla);
    free(y_tabla);
    
    return 0;
}

/**
 * Función exacta: f(x) = e^(2·sin(x)) · [1 + ln(x)]
 */
double f_exacta(double x)
{
    return exp(2.0 * sin(x)) * (1.0 + log(x));
}

/**
 * Integración por Simpson 1/3 compuesto
 * Si usar_tabla = 1: usa x_tabla, y_tabla
 * Si usar_tabla = 0: usa f_exacta
 */
double simpsonCompuesto(double a, double b, int n_sub, double *x_tabla, double *y_tabla, int usar_tabla)
{
    if (n_sub % 2 != 0) {
        printf("[ERROR] Simpson requiere n_sub PAR\n");
        return 0.0;
    }
    
    double h = (b - a) / n_sub;
    double suma = 0.0;
    
    if (usar_tabla) {
        /* Usar datos de la tabla */
        suma = y_tabla[0] + y_tabla[n_sub];
        
        for (int i = 1; i < n_sub; i += 2) {
            suma += 4.0 * y_tabla[i];
        }
        
        for (int i = 2; i < n_sub; i += 2) {
            suma += 2.0 * y_tabla[i];
        }
    } else {
        /* Usar función exacta */
        suma = f_exacta(a) + f_exacta(b);
        
        for (int i = 1; i < n_sub; i += 2) {
            double x = a + i * h;
            suma += 4.0 * f_exacta(x);
        }
        
        for (int i = 2; i < n_sub; i += 2) {
            double x = a + i * h;
            suma += 2.0 * f_exacta(x);
        }
    }
    
    return (h / 3.0) * suma;
}

/**
 * Lee datos desde archivo de texto
 * Formato: cada línea contiene "x y"
 * Retorna el número de puntos leídos
 */
int leerDatosDesdeArchivo(const char *filename, double **x, double **y)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[ERROR] No se pudo abrir el archivo '%s'\n", filename);
        return 0;
    }
    
    /* Contar líneas válidas */
    int n = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        /* Ignorar líneas vacías o que comienzan con # */
        if (line[0] != '\n' && line[0] != '#' && line[0] != ' ') {
            n++;
        }
    }
    rewind(file);
    
    if (n == 0) {
        printf("[ERROR] El archivo está vacío o no tiene datos válidos\n");
        fclose(file);
        return 0;
    }
    
    /* Asignar memoria */
    *x = (double *)malloc(n * sizeof(double));
    *y = (double *)malloc(n * sizeof(double));
    
    if (!(*x) || !(*y)) {
        printf("[ERROR] Error de asignación de memoria\n");
        free(*x);
        free(*y);
        fclose(file);
        return 0;
    }
    
    /* Leer datos */
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < n) {
        if (line[0] != '\n' && line[0] != '#' && line[0] != ' ') {
            if (sscanf(line, "%lf %lf", &(*x)[i], &(*y)[i]) == 2) {
                i++;
            }
        }
    }
    
    fclose(file);
    
    if (i != n) {
        printf("[ADVERTENCIA] Se esperaban %d puntos pero se leyeron %d\n", n, i);
        return i;
    }
    
    return n;
}
