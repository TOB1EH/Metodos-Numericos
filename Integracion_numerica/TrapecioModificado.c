/**
 * ============================================================================
 * PROBLEMA N° 2: MÉTODO DEL TRAPECIO MODIFICADO
 * ============================================================================
 * 
 * Se propone un método para aproximar integrales que consiste de una 
 * modificación del método del trapecio de manera que:
 * 
 * I = ∫[a,b] f(x)dx ≈ Σ(x[i+1] - x[i])f(x[i]) + (1/2)Σ(x[i+1] - x[i])(f(x[i+1]) - f(x[i]))
 * 
 * donde n es el número de sub-intervalos usado.
 * 
 * TAREAS:
 * a) Escribir un pseudo-código para este método, e implementarlo
 * b) Con el método desarrollado calcular ∫[0,1] (3x² + 1)dx, con n = 10, 
 *    n = 100 y n = 1000. Escribir el resultado obtenido y el error absoluto exacto
 * c) Repetir el inciso b) usando el método del trapecio
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
   PROTOTIPOS DE FUNCIONES
   ============================================================================ */

double f(double x);
double trapecioModificado(double a, double b, int n);
double trapecioEstandar(double a, double b, int n);
double integralExacta(double a, double b);

/* ============================================================================
   FUNCIÓN PRINCIPAL
   ============================================================================ */

int main(void)
{
    /* Parámetros del problema */
    double a = 0.0;  // Límite inferior
    double b = 1.0;  // Límite superior
    
    /* Valores de n a probar */
    int valores_n[] = {10, 100, 1000};
    int cantidad_pruebas = 3;
    
    /* Valor exacto de la integral */
    double valor_exacto = integralExacta(a, b);
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║         PROBLEMA N° 2: TRAPECIO MODIFICADO                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\nFunción a integrar: f(x) = 3x² + 1\n");
    printf("Intervalo: [%.1lf, %.1lf]\n", a, b);
    printf("Valor exacto de la integral: %.10lf\n", valor_exacto);
    
    /* ========================================================================
       INCISO B: TRAPECIO MODIFICADO
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  INCISO B: MÉTODO DEL TRAPECIO MODIFICADO                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n┌──────┬───────────────────┬───────────────────┬──────────────┐\n");
    printf("│  n   │   Resultado       │   Error Absoluto  │   Error %%    │\n");
    printf("├──────┼───────────────────┼───────────────────┼──────────────┤\n");
    
    for (int i = 0; i < cantidad_pruebas; i++)
    {
        int n = valores_n[i];
        double resultado = trapecioModificado(a, b, n);
        double error_absoluto = fabs(valor_exacto - resultado);
        double error_porcentual = (error_absoluto / valor_exacto) * 100.0;
        
        printf("│ %-4d │  %.10lf  │   %.10lf  │  %.6lf%%  │\n", 
               n, resultado, error_absoluto, error_porcentual);
    }
    
    printf("└──────┴───────────────────┴───────────────────┴──────────────┘\n");
    
    /* ========================================================================
       INCISO C: TRAPECIO ESTÁNDAR (para comparación)
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  INCISO C: MÉTODO DEL TRAPECIO ESTÁNDAR                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n┌──────┬───────────────────┬───────────────────┬──────────────┐\n");
    printf("│  n   │   Resultado       │   Error Absoluto  │   Error %%    │\n");
    printf("├──────┼───────────────────┼───────────────────┼──────────────┤\n");
    
    for (int i = 0; i < cantidad_pruebas; i++)
    {
        int n = valores_n[i];
        double resultado = trapecioEstandar(a, b, n);
        double error_absoluto = fabs(valor_exacto - resultado);
        double error_porcentual = (error_absoluto / valor_exacto) * 100.0;
        
        printf("│ %-4d │  %.10lf  │   %.10lf  │  %.6lf%%  │\n", 
               n, resultado, error_absoluto, error_porcentual);
    }
    
    printf("└──────┴───────────────────┴───────────────────┴──────────────┘\n");
    
    /* ========================================================================
       COMPARACIÓN
       ======================================================================== */
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  COMPARACIÓN DE MÉTODOS                                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\nNOTA: Ambos métodos son algebraicamente equivalentes.\n");
    printf("Las pequeñas diferencias se deben a errores de redondeo.\n");
    
    printf("\nAnálisis teórico:\n");
    printf("  • Trapecio Modificado: Implementación directa de la fórmula dada\n");
    printf("  • Trapecio Estándar: Fórmula simplificada\n");
    printf("  • Convergencia: O(h²) donde h = (b-a)/n\n");
    printf("  • A medida que n aumenta, ambos convergen al valor exacto\n");
    
    return 0;
}

/* ============================================================================
   FUNCIÓN A INTEGRAR: f(x) = 3x² + 1
   ============================================================================
   
   @param x Punto donde evaluar la función
   @return Valor de f(x)
   ============================================================================ */

double f(double x)
{
    return 3.0 * x * x + 1.0;
}

/* ============================================================================
   MÉTODO DEL TRAPECIO MODIFICADO
   ============================================================================
   
   PSEUDOCÓDIGO:
   ────────────────────────────────────────────────────────────────────────
   
   ALGORITMO trapecioModificado(a, b, n):
       // Entrada: 
       //   a = límite inferior
       //   b = límite superior
       //   n = número de subintervalos
       // Salida:
       //   I = aproximación de la integral
       
       h ← (b - a) / n                    // Calcular tamaño del paso
       
       suma1 ← 0                           // Primer término de la fórmula
       suma2 ← 0                           // Segundo término de la fórmula
       
       PARA i DESDE 0 HASTA n-1 HACER:
           x_i ← a + i * h                // Punto x[i]
           x_i_plus_1 ← a + (i+1) * h     // Punto x[i+1]
           
           h_i ← x_i_plus_1 - x_i         // Ancho del subintervalo i
           
           f_i ← f(x_i)                   // Evaluar f en x[i]
           f_i_plus_1 ← f(x_i_plus_1)     // Evaluar f en x[i+1]
           
           // Primer término: Σ(x[i+1] - x[i]) * f(x[i])
           suma1 ← suma1 + h_i * f_i
           
           // Segundo término: (1/2) * Σ(x[i+1] - x[i]) * (f(x[i+1]) - f(x[i]))
           suma2 ← suma2 + h_i * (f_i_plus_1 - f_i)
       FIN PARA
       
       I ← suma1 + 0.5 * suma2            // Aplicar la fórmula completa
       
       RETORNAR I
   FIN ALGORITMO
   
   ────────────────────────────────────────────────────────────────────────
   
   FÓRMULA IMPLEMENTADA:
   
   I ≈ Σ[i=0 hasta n-1] (x[i+1] - x[i]) * f(x[i]) 
       + (1/2) * Σ[i=0 hasta n-1] (x[i+1] - x[i]) * (f(x[i+1]) - f(x[i]))
   
   NOTA MATEMÁTICA:
   Esta fórmula es algebraicamente equivalente al trapecio estándar:
   I ≈ (h/2) * [f(a) + 2*Σf(x[i]) + f(b)]
   
   Demostración (simplificación algebraica):
   suma1 + 0.5*suma2 = Σh*f[i] + 0.5*Σh*(f[i+1] - f[i])
                     = Σh*f[i] + 0.5*Σh*f[i+1] - 0.5*Σh*f[i]
                     = 0.5*Σh*f[i] + 0.5*Σh*f[i+1]
                     = (h/2) * [f[0] + 2*Σf[i] + f[n]]  ← Trapecio estándar
   
   @param a Límite inferior de integración
   @param b Límite superior de integración
   @param n Número de subintervalos
   @return Aproximación de la integral
   ============================================================================ */

double trapecioModificado(double a, double b, int n)
{
    double h = (b - a) / n;  // Tamaño del paso
    
    double suma1 = 0.0;  // Primer término: Σ(x[i+1] - x[i]) * f(x[i])
    double suma2 = 0.0;  // Segundo término: Σ(x[i+1] - x[i]) * (f(x[i+1]) - f(x[i]))
    
    /* Iterar sobre cada subintervalo [x[i], x[i+1]] */
    for (int i = 0; i < n; i++)
    {
        double x_i = a + i * h;           // Punto x[i]
        double x_i_plus_1 = a + (i + 1) * h;  // Punto x[i+1]
        
        double h_i = x_i_plus_1 - x_i;    // Ancho del subintervalo i
        
        double f_i = f(x_i);               // f(x[i])
        double f_i_plus_1 = f(x_i_plus_1); // f(x[i+1])
        
        /* Acumular el primer término: (x[i+1] - x[i]) * f(x[i]) */
        suma1 += h_i * f_i;
        
        /* Acumular el segundo término: (x[i+1] - x[i]) * (f(x[i+1]) - f(x[i])) */
        suma2 += h_i * (f_i_plus_1 - f_i);
    }
    
    /* Aplicar la fórmula completa: suma1 + (1/2)*suma2 */
    double resultado = suma1 + 0.5 * suma2;
    
    return resultado;
}

/* ============================================================================
   MÉTODO DEL TRAPECIO ESTÁNDAR (para comparación en inciso c)
   ============================================================================
   
   FÓRMULA ESTÁNDAR:
   I ≈ (h/2) * [f(a) + 2*Σf(x[i]) + f(b)]
   
   donde:
   - h = (b-a)/n
   - Σf(x[i]) suma desde i=1 hasta i=n-1 (puntos internos)
   
   @param a Límite inferior de integración
   @param b Límite superior de integración
   @param n Número de subintervalos
   @return Aproximación de la integral
   ============================================================================ */

double trapecioEstandar(double a, double b, int n)
{
    double h = (b - a) / n;  // Tamaño del paso
    
    /* Inicializar suma con los extremos: f(a) + f(b) */
    double suma = f(a) + f(b);
    
    /* Sumar los puntos internos con coeficiente 2 */
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        suma += 2.0 * f(x);
    }
    
    /* Aplicar la fórmula: (h/2) * suma */
    double resultado = (h / 2.0) * suma;
    
    return resultado;
}

/* ============================================================================
   INTEGRAL EXACTA DE f(x) = 3x² + 1
   ============================================================================
   
   CÁLCULO ANALÍTICO:
   
   ∫(3x² + 1)dx = x³ + x + C
   
   ∫[a,b] (3x² + 1)dx = [x³ + x]|[a,b]
                       = (b³ + b) - (a³ + a)
   
   Para [0, 1]:
   = (1³ + 1) - (0³ + 0)
   = 2 - 0
   = 2.0
   
   @param a Límite inferior
   @param b Límite superior
   @return Valor exacto de la integral
   ============================================================================ */

double integralExacta(double a, double b)
{
    return (b*b*b + b) - (a*a*a + a);
}