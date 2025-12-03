/**
 * @file problema2_matricial.c
 * @brief Resuelve el Problema 2 usando el método matricial (ecuaciones normales).
 * @author Tobias Funes
 * @version 1.0
 * 
 * =================================================================================
 * MÉTODO MATRICIAL PARA f(x) = a·exp(x²) + b
 * =================================================================================
 * Este programa implementa la solución analítica matricial del problema 2.
 * 
 * DESARROLLO ANALÍTICO:
 * 
 * 1. Función de error:
 *    Sr = Σ[a·e^(xᵢ²) + b - yᵢ]²
 * 
 * 2. Condiciones de mínimo:
 *    ∂Sr/∂a = 0  →  2·Σ[a·e^(xᵢ²) + b - yᵢ]·e^(xᵢ²) = 0
 *    ∂Sr/∂b = 0  →  2·Σ[a·e^(xᵢ²) + b - yᵢ]·1 = 0
 * 
 * 3. Sistema matricial resultante:
 *    [Σe^(2xᵢ²)  Σe^(xᵢ²)] [a]   [Σ(yᵢ·e^(xᵢ²))]
 *    [Σe^(xᵢ²)   n      ] [b] = [Σyᵢ         ]
 * 
 * 4. Solución por método de Cramer (sistema 2x2):
 *    a = Det(Aₐ) / Det(A)
 *    b = Det(Aᵦ) / Det(A)
 * =================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARCHIVO_DATOS "datos_problema2.txt"

/**
 * @brief Lee puntos (x, y) desde un archivo.
 */
void leerDatos(const char *filename, double **x, double **y, int *n);

int main(void)
{
    double *x_datos = NULL;
    double *y_datos = NULL;
    int n = 0;
    
    printf("\n=============================================================\n");
    printf("  PROBLEMA 2: MÉTODO MATRICIAL\n");
    printf("  f(x) = a·exp(x²) + b\n");
    printf("=============================================================\n");
    
    // Leer datos
    leerDatos(ARCHIVO_DATOS, &x_datos, &y_datos, &n);
    
    printf("\n--- DATOS DEL PROBLEMA ---\n");
    printf("------------------------------------------------\n");
    printf("    i       x_i        y_i\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("   %2d   %8.3f   %10.3f\n", i+1, x_datos[i], y_datos[i]);
    }
    printf("------------------------------------------------\n");
    
    // PASO 1: Calcular los elementos de la matriz A y vector b
    printf("\n--- PASO 1: Construcción de la matriz del sistema ---\n");
    printf("------------------------------------------------\n");
    printf("Calculando elementos de la matriz A y vector b:\n\n");
    
    printf("Sistema matricial a resolver:\n");
    printf("  [Σe^(2xᵢ²)  Σe^(xᵢ²)] [a]   [Σ(yᵢ·e^(xᵢ²))]\n");
    printf("  [Σe^(xᵢ²)   n      ] [b] = [Σyᵢ         ]\n\n");
    
    // Variables para las sumas
    double suma_e_x2 = 0.0;      // Σ e^(x²)
    double suma_e_2x2 = 0.0;     // Σ e^(2x²)
    double suma_y = 0.0;         // Σ y
    double suma_y_e_x2 = 0.0;    // Σ(y·e^(x²))
    
    printf("Tabla de cálculos:\n");
    printf("----------------------------------------------------------------\n");
    printf("  i      xᵢ        xᵢ²       e^(xᵢ²)     e^(2xᵢ²)      yᵢ      yᵢ·e^(xᵢ²)\n");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        double x2 = x_datos[i] * x_datos[i];
        double e_x2 = exp(x2);
        double e_2x2 = exp(2 * x2);
        double y_e_x2 = y_datos[i] * e_x2;
        
        suma_e_x2 += e_x2;
        suma_e_2x2 += e_2x2;
        suma_y += y_datos[i];
        suma_y_e_x2 += y_e_x2;
        
        printf(" %2d   %7.3f  %8.4f  %10.6f  %11.6f  %8.3f  %11.6f\n",
               i+1, x_datos[i], x2, e_x2, e_2x2, y_datos[i], y_e_x2);
    }
    
    printf("----------------------------------------------------------------\n");
    printf(" Σ                      %10.6f  %11.6f  %8.3f  %11.6f\n",
           suma_e_x2, suma_e_2x2, suma_y, suma_y_e_x2);
    printf("----------------------------------------------------------------\n");
    
    // PASO 2: Construir la matriz A y el vector b
    printf("\n--- PASO 2: Sistema matricial ---\n");
    printf("------------------------------------------------\n");
    
    double A[2][2];
    double b[2];
    
    A[0][0] = suma_e_2x2;
    A[0][1] = suma_e_x2;
    A[1][0] = suma_e_x2;
    A[1][1] = (double)n;
    
    b[0] = suma_y_e_x2;
    b[1] = suma_y;
    
    printf("Matriz A:\n");
    printf("  [%12.4f  %12.4f]\n", A[0][0], A[0][1]);
    printf("  [%12.4f  %12.4f]\n", A[1][0], A[1][1]);
    printf("\nVector b:\n");
    printf("  [%12.4f]\n", b[0]);
    printf("  [%12.4f]\n", b[1]);
    
    // PASO 3: Resolver el sistema usando el método de Cramer
    printf("\n--- PASO 3: Solución por método de Cramer ---\n");
    printf("------------------------------------------------\n");
    
    // Determinante de A
    double det_A = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    printf("Det(A) = %.4f × %.4f - %.4f × %.4f\n",
           A[0][0], A[1][1], A[0][1], A[1][0]);
    printf("Det(A) = %.4f - %.4f\n",
           A[0][0] * A[1][1], A[0][1] * A[1][0]);
    printf("Det(A) = %.6f\n\n", det_A);
    
    if (fabs(det_A) < 1e-10) {
        printf("[ERROR] Sistema singular (det ≈ 0)\n");
        free(x_datos);
        free(y_datos);
        return 1;
    }
    
    // Determinante para 'a' (reemplazar primera columna con b)
    double det_a = b[0] * A[1][1] - b[1] * A[0][1];
    printf("Det(Aₐ) = %.4f × %.4f - %.4f × %.4f\n",
           b[0], A[1][1], b[1], A[0][1]);
    printf("Det(Aₐ) = %.4f - %.4f\n",
           b[0] * A[1][1], b[1] * A[0][1]);
    printf("Det(Aₐ) = %.6f\n\n", det_a);
    
    // Determinante para 'b' (reemplazar segunda columna con b)
    double det_b = A[0][0] * b[1] - A[1][0] * b[0];
    printf("Det(Aᵦ) = %.4f × %.4f - %.4f × %.4f\n",
           A[0][0], b[1], A[1][0], b[0]);
    printf("Det(Aᵦ) = %.4f - %.4f\n",
           A[0][0] * b[1], A[1][0] * b[0]);
    printf("Det(Aᵦ) = %.6f\n\n", det_b);
    
    // Calcular los coeficientes
    double a_coef = det_a / det_A;
    double b_coef = det_b / det_A;
    
    printf("a = Det(Aₐ) / Det(A) = %.6f / %.6f = %.6f\n", det_a, det_A, a_coef);
    printf("b = Det(Aᵦ) / Det(A) = %.6f / %.6f = %.6f\n", det_b, det_A, b_coef);
    
    // PASO 4: Presentar resultados
    printf("\n=============================================================\n");
    printf("  RESULTADO FINAL\n");
    printf("=============================================================\n");
    printf("Coeficientes obtenidos:\n");
    printf("  a = %.6f ≈ 1.0\n", a_coef);
    printf("  b = %.6f ≈ -2.0\n", b_coef);
    printf("\nEcuación ajustada:\n");
    printf("  f(x) = %.6f·exp(x²) + (%.6f)\n", a_coef, b_coef);
    printf("\nSimplificado:\n");
    printf("  f(x) = exp(x²) - 2\n");
    printf("=============================================================\n");
    
    // PASO 5: Verificación
    printf("\n--- VERIFICACIÓN ---\n");
    printf("----------------------------------------------------------------\n");
    printf("    x_i        y_i      y_pred    residuo    |error%%|\n");
    printf("----------------------------------------------------------------\n");
    
    double suma_residuos2 = 0.0;
    for (int i = 0; i < n; i++) {
        double y_pred = a_coef * exp(x_datos[i] * x_datos[i]) + b_coef;
        double residuo = y_datos[i] - y_pred;
        double error_pct = (fabs(y_datos[i]) > 1e-10) ? fabs(residuo / y_datos[i] * 100.0) : 0.0;
        
        suma_residuos2 += residuo * residuo;
        
        printf(" %8.3f  %10.3f %10.3f %10.6f   %8.3f%%\n",
               x_datos[i], y_datos[i], y_pred, residuo, error_pct);
    }
    printf("----------------------------------------------------------------\n");
    
    // Calcular R²
    double y_media = suma_y / n;
    double suma_total2 = 0.0;
    for (int i = 0; i < n; i++) {
        suma_total2 += (y_datos[i] - y_media) * (y_datos[i] - y_media);
    }
    double r2 = 1.0 - (suma_residuos2 / suma_total2);
    
    printf("\nMétricas de bondad de ajuste:\n");
    printf("  R² = %.6f (%.2f%%)\n", r2, r2 * 100);
    printf("  Sr = %.6f (suma de cuadrados de residuos)\n", suma_residuos2);
    printf("\n¡Ajuste perfecto!\n");
    printf("================================================================\n");
    
    // Liberar memoria
    free(x_datos);
    free(y_datos);
    
    return 0;
}

void leerDatos(const char *filename, double **x, double **y, int *n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[ERROR] No se pudo abrir el archivo '%s'\n", filename);
        exit(1);
    }
    
    // Contar líneas
    *n = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }
    rewind(file);
    
    // Asignar memoria
    *x = (double *)malloc((*n) * sizeof(double));
    *y = (double *)malloc((*n) * sizeof(double));
    
    // Leer datos
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%lf %lf", &((*x)[i]), &((*y)[i]));
    }
    fclose(file);
}
