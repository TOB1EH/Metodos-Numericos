/**
 * @file generador_solucion_exacta.c
 * @brief Genera tabla de valores de una función exacta evaluada en un intervalo
 * @author Tobias Funes
 * @version 1.0
 * 
 * Este programa permite:
 * - Evaluar cualquier función matemática en un intervalo [x0, xf]
 * - Especificar el paso mediante h o n
 * - Guardar los resultados en formato tabla (x, y)
 * 
 * FUNCIONES DISPONIBLES:
 * - Puedes modificar la función f(x) directamente en el código
 * - Ejemplos: exp(-x)*sin(2*x), x^2, log(x), etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// FUNCIÓN A EVALUAR - MODIFICA AQUÍ TU FUNCIÓN EXACTA
// ============================================================================

/**
 * @brief Función exacta a evaluar
 * @param x Valor de la variable independiente
 * @return Valor de f(x)
 * 
 * INSTRUCCIONES:
 * Modifica esta función según el problema que estés resolviendo.
 * 
 * EJEMPLOS:
 * - Problema 3: return exp(-x) * sin(2.0 * x);
 * - Parábola:   return x * x;
 * - Exponencial: return exp(x);
 * - Logaritmo:   return log(x);
 * - Trigonométrica: return sin(x) + cos(2*x);
 */
double f(double x) {
    // PROBLEMA 3: y(x) = e^(-x)·sin(2x)
    // return exp(-x) * sin(2.0 * x);
    
    // Descomenta la función que necesites:
    // return x * x;                          // Parábola
    // return exp(x);                         // Exponencial
    // return sin(x);                         // Seno
    // return cos(x);                         // Coseno
    // return log(x);                         // Logaritmo natural
    // return sqrt(x);                        // Raíz cuadrada
    // return pow(x, 3) - 2*x + 1;           // Polinomio cúbico
    // return 1.0 / (1.0 + x*x);             // Función racional
}

/**
 * @brief Descripción de la función (para el encabezado del archivo)
 */
const char* descripcion_funcion() {
    return "y(x) = e^(-x)*sin(2x)";
    
    // Cambia esto según tu función:
    // return "y(x) = x^2";
    // return "y(x) = e^x";
    // return "y(x) = sin(x)";
}

// ============================================================================
// PROGRAMA PRINCIPAL
// ============================================================================

int main(void) {
    double x0, xf, h;
    int n;
    char opcion_entrada;
    char nombre_archivo[256];
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║          GENERADOR DE TABLA DE SOLUCIÓN EXACTA                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Función a evaluar: %s\n\n", descripcion_funcion());
    
    // ========================================================================
    // ENTRADA DE DATOS
    // ========================================================================
    
    printf("PARÁMETROS DEL INTERVALO:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("Ingrese el valor inicial x₀: ");
    scanf("%lf", &x0);
    printf("Ingrese el valor final xₓ: ");
    scanf("%lf", &xf);
    
    // Validar intervalo
    if (xf <= x0) {
        printf("\n❌ Error: xₓ debe ser mayor que x₀\n");
        return 1;
    }
    
    // Selección de método de entrada (h o n)
    printf("\n¿Cómo desea especificar el paso de evaluación?\n");
    printf("  a) Ingresar el número de pasos (n)\n");
    printf("  b) Ingresar el tamaño del paso (h)\n");
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion_entrada);
    
    if (opcion_entrada == 'a' || opcion_entrada == 'A') {
        // Usuario ingresa n, calculamos h
        printf("Ingrese el número de pasos (n): ");
        scanf("%d", &n);
        
        if (n <= 0) {
            printf("\n❌ Error: n debe ser mayor que 0\n");
            return 1;
        }
        
        h = (xf - x0) / n;
        printf("→ Tamaño del paso calculado: h = %.10lf\n", h);
    } else if (opcion_entrada == 'b' || opcion_entrada == 'B') {
        // Usuario ingresa h, calculamos n
        printf("Ingrese el tamaño del paso (h): ");
        scanf("%lf", &h);
        
        if (h <= 0 || h >= (xf - x0)) {
            printf("\n❌ Error: h debe estar entre 0 y %.6lf\n", xf - x0);
            return 1;
        }
        
        n = (int)((xf - x0) / h);
        h = (xf - x0) / n;  // Ajustar h para que sea exacto
        printf("→ Número de pasos calculado: n = %d\n", n);
        printf("→ Tamaño del paso ajustado: h = %.10lf\n", h);
    } else {
        printf("\n❌ Opción no válida\n");
        return 1;
    }
    
    // ========================================================================
    // NOMBRE DEL ARCHIVO DE SALIDA
    // ========================================================================
    
    printf("\nNombre del archivo de salida (sin espacios): ");
    scanf("%s", nombre_archivo);
    
    // Agregar extensión .txt si no la tiene
    if (strstr(nombre_archivo, ".txt") == NULL) {
        strcat(nombre_archivo, ".txt");
    }
    
    // ========================================================================
    // GENERAR TABLA
    // ========================================================================
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    GENERANDO TABLA...                              ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    FILE *fp = fopen(nombre_archivo, "w");
    if (fp == NULL) {
        printf("❌ Error: No se pudo crear el archivo '%s'\n", nombre_archivo);
        return 1;
    }
    
    // Escribir encabezado
    fprintf(fp, "# Tabla de solución exacta\n");
    fprintf(fp, "# Función: %s\n", descripcion_funcion());
    fprintf(fp, "# Intervalo: [%.10lf, %.10lf]\n", x0, xf);
    fprintf(fp, "# Paso h: %.10lf\n", h);
    fprintf(fp, "# Número de puntos: %d\n", n + 1);
    fprintf(fp, "# Formato: x  y\n");
    fprintf(fp, "#\n");
    
    // Generar y escribir datos
    printf("Generando y mostrando %d puntos...\n", n + 1);
    printf("\n");
    printf("  ────────────────────────────────────────\n");
    printf("    x              y\n");
    printf("  ────────────────────────────────────────\n");
    
    for (int i = 0; i <= n; i++) {
        double x = x0 + i * h;
        double y = f(x);
        
        // Escribir al archivo
        fprintf(fp, "%.10lf  %.10lf\n", x, y);
        
        // Mostrar TODOS los puntos
        printf("  %12.6lf  %12.8lf\n", x, y);
    }
    
    fclose(fp);
    
    // ========================================================================
    // RESUMEN
    // ========================================================================
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    ✅ TABLA GENERADA EXITOSAMENTE                  ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("RESUMEN:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("  Función:        %s\n", descripcion_funcion());
    printf("  Intervalo:      [%.6lf, %.6lf]\n", x0, xf);
    printf("  Paso h:         %.10lf\n", h);
    printf("  Puntos:         %d\n", n + 1);
    printf("  Archivo:        %s\n", nombre_archivo);
    printf("────────────────────────────────────────────────────────────────────\n");
    
    printf("\nUSO:\n");
    printf("  Este archivo puede usarse con el graficador genérico para comparar\n");
    printf("  con soluciones numéricas de métodos como Euler, RK4, etc.\n");
    printf("\n");
    
    return 0;
}
