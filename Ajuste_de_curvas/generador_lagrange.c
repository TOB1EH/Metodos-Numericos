/**
 * ============================================================================
 * PROGRAMA: Generador de Polinomios de Lagrange para Graficar
 * ============================================================================
 * 
 * Este programa:
 * 1. Lee puntos desde un archivo (nodos.txt)
 * 2. Calcula cada polinomio de Lagrange L_i(x) individualmente
 * 3. Genera archivos de texto con los valores de cada L_i(x)
 * 4. Genera el polinomio interpolador completo P(x)
 * 5. Crea un script de gnuplot para graficar todo
 * 
 * Uso: ./generador_lagrange
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Lee los puntos de interpolación desde un archivo
 */
void leerPuntosDesdeArchivo(const char *filename, double **x_puntos, double **y_puntos, int *n) {
    FILE *archivo = fopen(filename, "r");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo '%s'\n", filename);
        exit(1);
    }

    // Contar número de líneas
    *n = 0;
    double temp1, temp2;
    while (fscanf(archivo, "%lf %lf", &temp1, &temp2) == 2) {
        (*n)++;
    }

    // Reservar memoria
    *x_puntos = (double *)malloc(*n * sizeof(double));
    *y_puntos = (double *)malloc(*n * sizeof(double));

    if (!(*x_puntos) || !(*y_puntos)) {
        printf("Error: No se pudo reservar memoria\n");
        fclose(archivo);
        exit(1);
    }

    // Leer los datos
    rewind(archivo);
    for (int i = 0; i < *n; i++) {
        fscanf(archivo, "%lf %lf", &(*x_puntos)[i], &(*y_puntos)[i]);
    }

    fclose(archivo);
}

/**
 * Calcula el polinomio de Lagrange L_i(x) para un índice específico
 */
double calcularLi(double *x_puntos, int n, int i, double x) {
    double Li = 1.0;
    
    for (int j = 0; j < n; j++) {
        if (j != i) {
            Li *= (x - x_puntos[j]) / (x_puntos[i] - x_puntos[j]);
        }
    }
    
    return Li;
}

/**
 * Muestra información sobre los puntos leídos
 */
void mostrarPuntos(double *x_puntos, double *y_puntos, int n) {
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║  Puntos de Interpolación Leídos                   ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║   i   │      x_i      │      y_i                  ║\n");
    printf("╠═══════╪═══════════════╪═══════════════════════════╣\n");
    
    for (int i = 0; i < n; i++) {
        printf("║  %2d   │  %10.4f   │  %10.4f              ║\n", i, x_puntos[i], y_puntos[i]);
    }
    
    printf("╚═══════╧═══════════════╧═══════════════════════════╝\n");
}

/**
 * Genera las tablas de cada polinomio de Lagrange
 */
void generarTablasLagrange(double *x_puntos, double *y_puntos, int n) {
    double x_min, x_max;
    int num_puntos;
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║  Generador de Tablas de Polinomios de Lagrange    ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    
    // Encontrar rango
    x_min = x_puntos[0];
    x_max = x_puntos[0];
    for (int i = 1; i < n; i++) {
        if (x_puntos[i] < x_min) x_min = x_puntos[i];
        if (x_puntos[i] > x_max) x_max = x_puntos[i];
    }
    
    printf("\nRango detectado: [%.2f, %.2f]\n", x_min, x_max);
    printf("Ingrese el número de puntos a generar (ej: 100): ");
    scanf("%d", &num_puntos);
    
    if (num_puntos < 10) {
        printf("Advertencia: Número de puntos muy bajo. Usando 100.\n");
        num_puntos = 100;
    }
    
    double h = (x_max - x_min) / (num_puntos - 1);
    
    printf("\n--- Generando archivos ---\n");
    
    // Generar archivo para cada L_i(x)
    for (int i = 0; i < n; i++) {
        char filename[100];
        sprintf(filename, "lagrange_L%d.txt", i);
        
        FILE *archivo = fopen(filename, "w");
        if (!archivo) {
            printf("Error al crear el archivo %s\n", filename);
            continue;
        }
        
        fprintf(archivo, "# x\tL_%d(x)\n", i);
        fprintf(archivo, "# Polinomio de Lagrange L_%d que vale 1 en x=%.2f y 0 en los demás puntos\n", 
                i, x_puntos[i]);
        
        for (int k = 0; k < num_puntos; k++) {
            double x = x_min + k * h;
            double Li = calcularLi(x_puntos, n, i, x);
            fprintf(archivo, "%.6f\t%.6f\n", x, Li);
        }
        
        fclose(archivo);
        printf("  ✓ %s (L_%d(x) - vale 1 en x=%.2f)\n", filename, i, x_puntos[i]);
    }
    
    // Generar archivo con el polinomio interpolador completo P(x)
    FILE *archivo_P = fopen("lagrange_P.txt", "w");
    if (archivo_P) {
        fprintf(archivo_P, "# x\tP(x)\n");
        fprintf(archivo_P, "# Polinomio interpolador completo: P(x) = Suma de y_i * L_i(x)\n");
        
        for (int k = 0; k < num_puntos; k++) {
            double x = x_min + k * h;
            double P = 0.0;
            
            for (int i = 0; i < n; i++) {
                P += y_puntos[i] * calcularLi(x_puntos, n, i, x);
            }
            
            fprintf(archivo_P, "%.6f\t%.6f\n", x, P);
        }
        
        fclose(archivo_P);
        printf("  ✓ lagrange_P.txt (Polinomio interpolador completo)\n");
    }
    
    printf("\n--- Generando script de gnuplot ---\n");
    
    // Generar script de gnuplot
    FILE *gnuplot_script = fopen("plot_lagrange.gp", "w");
    if (gnuplot_script) {
        fprintf(gnuplot_script, "# Script de gnuplot para graficar polinomios de Lagrange\n\n");
        fprintf(gnuplot_script, "set terminal pngcairo size 1400,900 enhanced font 'Arial,12'\n");
        fprintf(gnuplot_script, "set output 'polinomios_lagrange.png'\n\n");
        
        fprintf(gnuplot_script, "set xlabel 'x' font ',14'\n");
        fprintf(gnuplot_script, "set ylabel 'y' font ',14'\n");
        fprintf(gnuplot_script, "set title 'Polinomios de Lagrange Individuales' font ',16'\n");
        fprintf(gnuplot_script, "set grid\n");
        fprintf(gnuplot_script, "set key top left box\n\n");
        
        fprintf(gnuplot_script, "# Estilos de línea\n");
        fprintf(gnuplot_script, "set style line 1 lc rgb '#e41a1c' lt 1 lw 2\n");
        fprintf(gnuplot_script, "set style line 2 lc rgb '#377eb8' lt 1 lw 2\n");
        fprintf(gnuplot_script, "set style line 3 lc rgb '#4daf4a' lt 1 lw 2\n");
        fprintf(gnuplot_script, "set style line 4 lc rgb '#984ea3' lt 1 lw 2\n");
        fprintf(gnuplot_script, "set style line 5 lc rgb '#ff7f00' lt 1 lw 2\n");
        fprintf(gnuplot_script, "set style line 6 lc rgb '#ffff33' lt 1 lw 2\n\n");
        
        fprintf(gnuplot_script, "plot ");
        
        // Graficar cada L_i(x)
        for (int i = 0; i < n; i++) {
            fprintf(gnuplot_script, "'lagrange_L%d.txt' using 1:2 with lines ls %d title 'L_{%d}(x)'", 
                    i, (i % 6) + 1, i);
            fprintf(gnuplot_script, ", \\\n     ");
        }
        
        // Graficar P(x)
        fprintf(gnuplot_script, "'lagrange_P.txt' using 1:2 with lines lw 3 lc rgb 'black' dashtype 2 title 'P(x) = {/Symbol S} y_i L_i(x)'");
        
        // Graficar puntos originales
        fprintf(gnuplot_script, ", \\\n     'nodos.txt' using 1:2 with points pt 7 ps 2 lc rgb 'red' title 'Puntos originales'\n");
        
        fclose(gnuplot_script);
        printf("  ✓ plot_lagrange.gp (Script de gnuplot)\n");
    }
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║  Archivos Generados Exitosamente                  ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║  Para generar la gráfica ejecute:                 ║\n");
    printf("║                                                    ║\n");
    printf("║  C:\\msys64\\ucrt64\\bin\\gnuplot.exe plot_lagrange.gp ║\n");
    printf("║                                                    ║\n");
    printf("║  O simplemente:                                    ║\n");
    printf("║  gnuplot plot_lagrange.gp                          ║\n");
    printf("║                                                    ║\n");
    printf("║  Resultado: polinomios_lagrange.png                ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
}

/**
 * Función principal
 */
int main() {
    double *x_puntos = NULL;
    double *y_puntos = NULL;
    int n = 0;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║  GENERADOR DE POLINOMIOS DE LAGRANGE PARA GRAFICAR        ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // Leer puntos desde archivo
    printf("\nLeyendo puntos desde 'nodos.txt'...\n");
    leerPuntosDesdeArchivo("nodos.txt", &x_puntos, &y_puntos, &n);
    
    printf("✓ Se leyeron %d puntos correctamente\n", n);
    
    // Mostrar los puntos
    mostrarPuntos(x_puntos, y_puntos, n);
    
    // Generar las tablas
    generarTablasLagrange(x_puntos, y_puntos, n);
    
    // Liberar memoria
    free(x_puntos);
    free(y_puntos);
    
    printf("\n✓ Programa finalizado exitosamente\n\n");
    
    return 0;
}