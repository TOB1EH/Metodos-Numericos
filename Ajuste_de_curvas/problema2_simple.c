/**
 * @file problema2_simple.c
 * @brief Versión SIMPLE y directa del Problema 2
 * @author Tobias Funes
 * 
 * Resuelve: f(x) = a·exp(x²) + b
 * Método: Ecuaciones normales + Cramer (2x2)
 */
#include <stdio.h>
#include <math.h>

int main(void)
{
    // DATOS DEL PROBLEMA
    double x[] = {0.5, 0.8, 1.3, 2.0};
    double y[] = {-0.716, -0.103, 3.419, 52.598};
    int n = 4;
    
    printf("\n========================================\n");
    printf("  PROBLEMA 2 - VERSIÓN SIMPLE\n");
    printf("  f(x) = a·exp(x²) + b\n");
    printf("========================================\n\n");
    
    // PASO 1: Calcular las sumas necesarias
    double suma_e_x2 = 0.0;      // Σ e^(x²)
    double suma_e_2x2 = 0.0;     // Σ e^(2x²)
    double suma_y = 0.0;         // Σ y
    double suma_y_e_x2 = 0.0;    // Σ(y·e^(x²))
    
    for (int i = 0; i < n; i++) {
        double e_x2 = exp(x[i] * x[i]);
        suma_e_x2 += e_x2;
        suma_e_2x2 += e_x2 * e_x2;
        suma_y += y[i];
        suma_y_e_x2 += y[i] * e_x2;
    }
    
    // PASO 2: Armar el sistema matricial
    // [A00  A01] [a]   [b0]
    // [A10  A11] [b] = [b1]
    
    double A00 = suma_e_2x2;
    double A01 = suma_e_x2;
    double A10 = suma_e_x2;
    double A11 = n;
    double b0 = suma_y_e_x2;
    double b1 = suma_y;
    
    // PASO 3: Resolver con Cramer (fórmulas directas)
    double det_A = A00 * A11 - A01 * A10;
    double det_a = b0 * A11 - b1 * A01;  // Reemplazar col 1 con b
    double det_b = A00 * b1 - A10 * b0;  // Reemplazar col 2 con b
    
    double a = det_a / det_A;
    double b = det_b / det_A;
    
    // RESULTADOS
    printf("RESULTADO:\n");
    printf("  a = %.6f ≈ 1.0\n", a);
    printf("  b = %.6f ≈ -2.0\n\n", b);
    printf("  f(x) = exp(x²) - 2\n");
    printf("========================================\n\n");
    
    // Verificación rápida
    printf("Verificación:\n");
    for (int i = 0; i < n; i++) {
        double y_pred = a * exp(x[i] * x[i]) + b;
        printf("  x=%.1f: y_real=%.3f, y_pred=%.3f\n", 
               x[i], y[i], y_pred);
    }
    printf("========================================\n");
    
    return 0;
}
