#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

/* ============================================================================
   PROTOTIPOS DE FUNCIONES
   ============================================================================ */

// Función de prueba
double f(double x);

// Diferenciación hacia ADELANTE
void diferenciacionAdelanteFunc();   // A partir de función
void diferenciacionAdelanteNodos();  // A partir de nodos de archivo

// Diferenciación hacia ATRÁS
void diferenciacionAtrasFunc();      // A partir de función
void diferenciacionAtrasNodos();     // A partir de nodos de archivo

// Diferenciación CENTRADA
void diferenciacionCentradaFunc();   // A partir de función
void diferenciacionCentradaNodos();  // A partir de nodos de archivo

// Funciones auxiliares
void opcionMenu(char *opcion);
void getNodesFromFile(const char *filename, double **x, double **y, int *n);

/* ============================================================================
   FUNCIÓN PRINCIPAL
   ============================================================================ */
int main(void)
{
    char opcion;

    /* Menu de opciones */
    do
    {
        printf("\n╔════════════════════════════════════════════╗\n");
        printf("║   DIFERENCIACIÓN NUMÉRICA                  ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║  DIFERENCIAS HACIA ADELANTE                ║\n");
        printf("║    a) Usando función                       ║\n");
        printf("║    b) Usando nodos de archivo              ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║  DIFERENCIAS HACIA ATRÁS                   ║\n");
        printf("║    c) Usando función                       ║\n");
        printf("║    d) Usando nodos de archivo              ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║  DIFERENCIAS CENTRADAS                     ║\n");
        printf("║    e) Usando función                       ║\n");
        printf("║    f) Usando nodos de archivo              ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║    g) Salir                                ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        printf("Seleccione una opción: ");
        opcionMenu(&opcion);

        switch (opcion)
        {
        case 'a':
            diferenciacionAdelanteFunc();
            break;
        case 'b':
            diferenciacionAdelanteNodos();
            break;
        case 'c':
            diferenciacionAtrasFunc();
            break;
        case 'd':
            diferenciacionAtrasNodos();
            break;
        case 'e':
            diferenciacionCentradaFunc();
            break;
        case 'f':
            diferenciacionCentradaNodos();
            break;
        case 'g':
            printf("\n✓ Saliendo del programa...\n");
            break;
        default:
            printf("\n✗ Opción no válida. Intente de nuevo.\n");
            break;
        }

    } while (opcion != 'g');

    return 0;
}

/**
 * Función de prueba para diferenciación numérica
 * f(x) = 2x + ln(x) - sin(3x)
 * 
 * Derivada analítica: f'(x) = 2 + 1/x - 3·cos(3x)
 * 
 * IMPORTANTE: Esta función debe estar definida en el dominio x > 0 (por ln(x))
 * 
 * Componentes:
 *   - 2x: Término lineal
 *   - ln(x): Logaritmo natural (requiere x > 0)
 *   - sin(3x): Componente trigonométrica con frecuencia 3
 * 
 * @param x Punto donde evaluar la función (debe ser x > 0)
 * @return Valor de f(x)
 */
double f(double x)
{
    // return (2.0*x + log(x) - sin(3.0*x));
    return (log(pow(x, 2) + 1) - sin(x));
}

/**
 * Lee una opción del menú y la convierte a minúscula
 * 
 * @param opcion Puntero donde se almacenará la opción leída
 */
void opcionMenu(char *opcion)
{
    scanf(" %c", opcion);
    *opcion = tolower(*opcion);
}

/**
 * FUNCIÓN AUXILIAR: getNodesFromFile
 * 
 * Lee nodos (x, y) desde un archivo
 * 
 * Formato del archivo:
 *   Primera línea: número de nodos n
 *   Líneas siguientes: pares (x, y)
 * 
 * Ejemplo:
 *   5
 *   1.0  2.5
 *   2.0  4.8
 *   3.0  7.1
 *   ...
 * 
 * @param filename Nombre del archivo
 * @param x Puntero al arreglo de coordenadas x (se asigna memoria)
 * @param y Puntero al arreglo de coordenadas y (se asigna memoria)
 * @param n Puntero donde se almacenará el número de nodos
 * 
 * NOTA: El llamador debe liberar la memoria de x e y
 */
void getNodesFromFile(const char *filename, double **x, double **y, int *n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: No se pudo abrir el archivo '%s'\n", filename);
        *n = 0;
        return;
    }
    
    fscanf(file, "%d", n);
    
    *x = (double *)malloc(*n * sizeof(double));
    *y = (double *)malloc(*n * sizeof(double));
    for (int i = 0; i < *n; i++)
    {
        fscanf(file, "%lf %lf", &(*x)[i], &(*y)[i]);
    }
    
    fclose(file);
    printf("✓ Se leyeron %d nodos del archivo '%s'\n", *n, filename);
}

/**
 *  DIFERENCIACIÓN HACIA ADELANTE - USANDO FUNCIÓN
 *
 *  FÓRMULA: f'(x_i) ≈ [f(x_i + h) - f(x_i)] / h
 *
 *  ERROR: O(h)
 *
 *  CARACTERÍSTICAS:
 *  - Usa el punto actual y el siguiente
 *  - Menos precisa que centrada
 *  - Útil cuando no hay punto anterior disponible
 */
void diferenciacionAdelanteFunc()
{
    /* Intervalos */
    double a = 0.0;
    double b = 0.0;

    double h = 0.0; /* Espaciamiento */
    int n;          /* Número de subintervalos */
    double *x = NULL; /* Puntos x_i */
    double *f_p = NULL; /* Derivadas f'(x_i) */
    FILE *archivo = NULL; /* Archivo de salida */
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  DIFERENCIACIÓN HACIA ADELANTE (Función)   ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    printf("Ingrese los extremos del intervalo [a, b]:\n");
    printf("  a = ");
    scanf("%lf", &a);
    printf("  b = ");
    scanf("%lf", &b);
    printf("Ingrese el número de subintervalos n: ");
    scanf("%d", &n);
    
    h = (b - a) / n;
    
    x = (double *)malloc((n + 1) * sizeof(double));
    f_p = (double *)malloc((n + 1) * sizeof(double));
    
    if (x == NULL || f_p == NULL)
    {
        printf("Error: No se pudo asignar memoria.\n");
        free(x);
        free(f_p);
        return;
    }
    
    /* Calcular derivadas hacia adelante en TODOS los puntos
     * f'(x_i) = [f(x_i + h) - f(x_i)] / h
     */
    for (int i = 0; i <= n; i++)
    {
        x[i] = a + i * h;
        f_p[i] = (f(x[i] + h) - f(x[i])) / h;
    }
    
    /* Mostrar resultados */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS\n");
    printf("════════════════════════════════════════════\n");
    printf("h = %.6lf\n", h);
    printf("Puntos: %d\n\n", n + 1);
    printf("  i      x_i          f'(x_i)\n");
    printf("────────────────────────────────────────────\n");
    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf\n", i, x[i], f_p[i]);
    }
    printf("════════════════════════════════════════════\n");
    
    /* Guardar en archivo */
    archivo = fopen("derivadas_adelante_func.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# Diferenciación hacia ADELANTE (función)\n");
        fprintf(archivo, "# f'(x) ≈ [f(x+h) - f(x)] / h\n");
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", a, b);
        fprintf(archivo, "# h = %.6lf\n", h);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\tf'(x_i)\n");
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\n", i, x[i], f_p[i]);
        }
        fclose(archivo);
        printf("✓ Guardado en 'derivadas_adelante_func.txt'\n");
    }
    
    free(x);
    free(f_p);
    
    printf("\nPresione ENTER...");
    getchar();
    getchar();
}

/**
 * @brief DIFERENCIACIÓN HACIA ADELANTE - USANDO NODOS
 * 
 * @return * void 
 */
void diferenciacionAdelanteNodos()
{
    double *x = NULL; // Array con las coordenadas x de los nodos
    double *y = NULL; // Array con las coordenadas y de los nodos
    double *f_p = NULL; // Array para las derivadas f'(x_i)
    int n;           // Número de nodos 
    FILE *archivo = NULL; // Archivo de salida
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  DIFERENCIACIÓN HACIA ADELANTE (Nodos)     ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    getNodesFromFile("nodos.txt", &x, &y, &n);
    if (n == 0)
    {
        return;
    }
    
    f_p = (double *)malloc(n * sizeof(double));
    
    /* Calcular derivadas hacia adelante
     * f'(x_i) = [f(x_{i+1}) - f(x_i)] / (x_{i+1} - x_i)
     * 
     * NOTA: El último punto usa el penúltimo intervalo
     */
    for (int i = 0; i < n - 1; i++)
    {
        double h_i = x[i + 1] - x[i];
        f_p[i] = (y[i + 1] - y[i]) / h_i;
    }
    
    /* Para el último punto, usar la misma fórmula hacia adelante
     * (extrapolando con el último h) */
    double h_last = x[n - 1] - x[n - 2];
    f_p[n - 1] = (y[n - 1] - y[n - 2]) / h_last;
    
    /* Mostrar resultados */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS\n");
    printf("════════════════════════════════════════════\n");
    printf("Puntos: %d\n\n", n);
    printf("  i      x_i          y_i          f'(x_i)\n");
    printf("────────────────────────────────────────────\n");
    for (int i = 0; i < n; i++)
    {
        printf("%3d | %10.6lf | %10.6lf | %12.8lf\n", i, x[i], y[i], f_p[i]);
    }
    printf("════════════════════════════════════════════\n");
    
    /* Guardar en archivo */
    archivo = fopen("derivadas_adelante_nodos.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# Diferenciación hacia ADELANTE (nodos)\n");
        fprintf(archivo, "# f'(x_i) ≈ [f(x_{i+1}) - f(x_i)] / (x_{i+1} - x_i)\n");
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_i\tf'(x_i)\n");
        for (int i = 0; i < n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\t%.10lf\n", i, x[i], y[i], f_p[i]);
        }
        fclose(archivo);
        printf("✓ Guardado en 'derivadas_adelante_nodos.txt'\n");
    }
    
    free(x);
    free(y);
    free(f_p);
    
    printf("\nPresione ENTER...");
    getchar();
    getchar();
}

/**
 * @brief DIFERENCIACIÓN HACIA ATRÁS - USANDO FUNCIÓN
 * 
 * @return * void 
 */
void diferenciacionAtrasFunc()
{
    /* Intervalos */
    double a = 0.0;
    double b = 0.0;

    double h = 0.0; /* Espaciamiento */
    int n;          /* Número de subintervalos */
    double *x = NULL; /* Puntos x_i */
    double *f_p = NULL; /* Derivadas f'(x_i) */
    FILE *archivo = NULL; /* Archivo de salida */
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  DIFERENCIACIÓN HACIA ATRÁS (Función)      ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    printf("Intervalo [a, b]:\n");
    printf("  a = ");
    scanf("%lf", &a);
    printf("  b = ");
    scanf("%lf", &b);
    printf("Número de subintervalos n: ");
    scanf("%d", &n);
    
    h = (b - a) / n;
    
    x = (double *)malloc((n + 1) * sizeof(double));
    f_p = (double *)malloc((n + 1) * sizeof(double));
    
    if (x == NULL || f_p == NULL)
    {
        printf("Error: No se pudo asignar memoria.\n");
        free(x);
        free(f_p);
        return;
    }
    
    /* Calcular derivadas hacia atrás en TODOS los puntos
     * f'(x_i) = [f(x_i) - f(x_i - h)] / h
     */
    for (int i = 0; i <= n; i++)
    {
        x[i] = a + i * h;
        f_p[i] = (f(x[i]) - f(x[i] - h)) / h;
    }
    
    /* Mostrar resultados */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS\n");
    printf("════════════════════════════════════════════\n");
    printf("h = %.6lf\n", h);
    printf("Puntos: %d\n\n", n + 1);
    printf("  i      x_i          f'(x_i)\n");
    printf("────────────────────────────────────────────\n");
    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf\n", i, x[i], f_p[i]);
    }
    printf("════════════════════════════════════════════\n");
    
    /* Guardar en archivo */
    archivo = fopen("derivadas_atras_func.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# Diferenciación hacia ATRÁS (función)\n");
        fprintf(archivo, "# f'(x) ≈ [f(x) - f(x-h)] / h\n");
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", a, b);
        fprintf(archivo, "# h = %.6lf\n", h);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\tf'(x_i)\n");
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\n", i, x[i], f_p[i]);
        }
        fclose(archivo);
        printf("✓ Guardado en 'derivadas_atras_func.txt'\n");
    }
    
    free(x);
    free(f_p);
    
    printf("\nPresione ENTER...");
    getchar();
    getchar();
}

/**
 * @brief DIFERENCIACIÓN HACIA ATRÁS - USANDO NODOS
 * 
 * @return * void 
 */
void diferenciacionAtrasNodos()
{
    double *x = NULL;
    double *y = NULL;
    double *f_p = NULL;
    int n;
    FILE *archivo = NULL;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  DIFERENCIACIÓN HACIA ATRÁS (Nodos)        ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    getNodesFromFile("nodos.txt", &x, &y, &n);
    
    if (n == 0)
    {
        return;
    }
    
    f_p = (double *)malloc(n * sizeof(double));
    
    /* Calcular derivadas hacia atrás
     * f'(x_i) = [f(x_i) - f(x_{i-1})] / (x_i - x_{i-1})
     * 
     * NOTA: El primer punto usa el primer intervalo
     */
    
    /* Primer punto (extrapolando) */
    double h_first = x[1] - x[0];
    f_p[0] = (y[1] - y[0]) / h_first;
    
    /* Puntos desde i=1 hasta n-1 */
    for (int i = 1; i < n; i++)
    {
        double h_i = x[i] - x[i - 1];
        f_p[i] = (y[i] - y[i - 1]) / h_i;
    }
    
    /* Mostrar resultados */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS\n");
    printf("════════════════════════════════════════════\n");
    printf("Puntos: %d\n\n", n);
    printf("  i      x_i          y_i          f'(x_i)\n");
    printf("────────────────────────────────────────────\n");
    for (int i = 0; i < n; i++)
    {
        printf("%3d | %10.6lf | %10.6lf | %12.8lf\n", i, x[i], y[i], f_p[i]);
    }
    printf("════════════════════════════════════════════\n");
    
    /* Guardar en archivo */
    archivo = fopen("derivadas_atras_nodos.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# Diferenciación hacia ATRÁS (nodos)\n");
        fprintf(archivo, "# f'(x_i) ≈ [f(x_i) - f(x_{i-1})] / (x_i - x_{i-1})\n");
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_i\tf'(x_i)\n");
        for (int i = 0; i < n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\t%.10lf\n", i, x[i], y[i], f_p[i]);
        }
        fclose(archivo);
        printf("✓ Guardado en 'derivadas_atras_nodos.txt'\n");
    }
    
    free(x);
    free(y);
    free(f_p);
    
    printf("\nPresione ENTER...");
    getchar();
    getchar();
}

/* ============================================================================
   5. DIFERENCIACIÓN CENTRADA - USANDO FUNCIÓN
   ============================================================================
   
   FÓRMULA: f'(x_i) ≈ [f(x_i + h) - f(x_i - h)] / (2h)
   
   ERROR: O(h²)  ← MÁS PRECISA que adelante/atrás
   
   CARACTERÍSTICAS:
   - Usa el punto anterior y el siguiente (simétrica)
   - Mayor precisión que adelante/atrás
   - Requiere tener ambos puntos vecinos
   ============================================================================ */

void diferenciacionCentradaFunc()
{
    double a, b, h;
    int n;
    double *x = NULL;
    double *f_p = NULL;
    FILE *archivo = NULL;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  DIFERENCIACIÓN CENTRADA (Función)         ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    printf("Intervalo [a, b]:\n");
    printf("  a = ");
    scanf("%lf", &a);
    printf("  b = ");
    scanf("%lf", &b);
    printf("Número de subintervalos n: ");
    scanf("%d", &n);
    
    h = (b - a) / n;
    
    x = (double *)malloc((n + 1) * sizeof(double));
    f_p = (double *)malloc((n + 1) * sizeof(double));
    
    if (x == NULL || f_p == NULL)
    {
        printf("Error: No se pudo asignar memoria.\n");
        free(x);
        free(f_p);
        return;
    }
    
    /* Calcular derivadas centradas en TODOS los puntos
     * f'(x_i) = [f(x_i + h) - f(x_i - h)] / (2h)
     */
    for (int i = 0; i <= n; i++)
    {
        x[i] = a + i * h;
        f_p[i] = (f(x[i] + h) - f(x[i] - h)) / (2.0 * h);
    }
    
    /* Mostrar resultados */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS\n");
    printf("════════════════════════════════════════════\n");
    printf("h = %.6lf\n", h);
    printf("Puntos: %d\n\n", n + 1);
    printf("  i      x_i          f'(x_i)\n");
    printf("────────────────────────────────────────────\n");
    for (int i = 0; i <= n; i++)
    {
        printf("%3d | %10.6lf | %12.8lf\n", i, x[i], f_p[i]);
    }
    printf("════════════════════════════════════════════\n");
    
    /* Guardar en archivo */
    archivo = fopen("derivadas_centrada_func.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# Diferenciación CENTRADA (función)\n");
        fprintf(archivo, "# f'(x) ≈ [f(x+h) - f(x-h)] / (2h)\n");
        fprintf(archivo, "# Intervalo: [%.6lf, %.6lf]\n", a, b);
        fprintf(archivo, "# h = %.6lf\n", h);
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\tf'(x_i)\n");
        for (int i = 0; i <= n; i++)
        {
            fprintf(archivo, "%d\t%.10lf\t%.10lf\n", i, x[i], f_p[i]);
        }
        fclose(archivo);
        printf("✓ Guardado en 'derivadas_centrada_func.txt'\n");
    }
    
    free(x);
    free(f_p);
    
    printf("\nPresione ENTER...");
    getchar();
    getchar();
}

/**
 * @brief DIFERENCIACIÓN CENTRADA - USANDO NODOS
 * 
 * @return * void 
 */
void diferenciacionCentradaNodos()
{
    double *x = NULL;
    double *y = NULL;
    double *f_p = NULL;
    int n;
    FILE *archivo = NULL;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  DIFERENCIACIÓN CENTRADA (Nodos)           ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    getNodesFromFile("nodos.txt", &x, &y, &n);
    
    if (n == 0)
    {
        return;
    }
    
    if (n < 3)
    {
        printf("Error: Se necesitan al menos 3 nodos para diferenciación centrada.\n");
        free(x);
        free(y);
        return;
    }
    
    f_p = (double *)malloc(n * sizeof(double));
    
    /* Primer punto: usar adelante */
    double h0 = x[1] - x[0];
    f_p[0] = (y[1] - y[0]) / h0;
    
    /* Puntos interiores: usar centrada
     * f'(x_i) = [f(x_{i+1}) - f(x_{i-1})] / (x_{i+1} - x_{i-1})
     */
    for (int i = 1; i < n - 1; i++)
    {
        double h_centrada = x[i + 1] - x[i - 1];
        f_p[i] = (y[i + 1] - y[i - 1]) / h_centrada;
    }
    
    /* Último punto: usar atrás */
    double h_last = x[n - 1] - x[n - 2];
    f_p[n - 1] = (y[n - 1] - y[n - 2]) / h_last;
    
    /* Mostrar resultados */
    printf("\n════════════════════════════════════════════\n");
    printf("  RESULTADOS\n");
    printf("════════════════════════════════════════════\n");
    printf("Puntos: %d\n\n", n);
    printf("  i      x_i          y_i          f'(x_i)       Método\n");
    printf("────────────────────────────────────────────────────────\n");
    for (int i = 0; i < n; i++)
    {
        const char *metodo;
        if (i == 0)
            metodo = "Adelante";
        else if (i == n - 1)
            metodo = "Atrás";
        else
            metodo = "Centrada";
            
        printf("%3d | %10.6lf | %10.6lf | %12.8lf | %s\n", 
               i, x[i], y[i], f_p[i], metodo);
    }
    printf("════════════════════════════════════════════\n");
    
    /* Guardar en archivo */
    archivo = fopen("derivadas_centrada_nodos.txt", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "# Diferenciación CENTRADA (nodos)\n");
        fprintf(archivo, "# f'(x_i) ≈ [f(x_{i+1}) - f(x_{i-1})] / (x_{i+1} - x_{i-1})\n");
        fprintf(archivo, "# Extremos usan diferencias adelante/atrás\n");
        fprintf(archivo, "#\n");
        fprintf(archivo, "# i\tx_i\ty_i\tf'(x_i)\tmetodo\n");
        for (int i = 0; i < n; i++)
        {
            const char *metodo;
            if (i == 0)
                metodo = "adelante";
            else if (i == n - 1)
                metodo = "atras";
            else
                metodo = "centrada";
                
            fprintf(archivo, "%d\t%.10lf\t%.10lf\t%.10lf\t%s\n", 
                   i, x[i], y[i], f_p[i], metodo);
        }
        fclose(archivo);
        printf("✓ Guardado en 'derivadas_centrada_nodos.txt'\n");
    }
    
    free(x);
    free(y);
    free(f_p);
    
    printf("\nPresione ENTER...");
    getchar();
    getchar();
}