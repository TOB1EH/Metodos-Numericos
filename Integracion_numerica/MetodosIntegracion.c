#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

/**
 * Lee una opción del menú del usuario y la convierte a minúscula
 * @param opcion Puntero donde se almacenará la opción ingresada
 */
void opcionMenu (char *opcion);

/**
 * Función a integrar: f(x) = 2x + ln(x) - sin(3x)
 * @param x Punto donde evaluar la función
 * @return Valor de f(x)
 */
double f(double x);

/**
 * Lee nodos (x, y) desde un archivo de texto
 * Formato del archivo: primera línea n (cantidad de nodos), luego n líneas con "x y"
 * @param filename Nombre del archivo a leer
 * @param x_values Puntero donde se almacenarán las coordenadas x (memoria dinámica)
 * @param y_values Puntero donde se almacenarán las coordenadas y (memoria dinámica)
 * @param n Puntero donde se almacenará la cantidad de nodos leídos
 */
void getNodesFromFile (const char* filename, double** x_values, double** y_values, int* n);

/**
 * Muestra los nodos en formato de tabla con bordes UTF-8
 * También verifica si los puntos están equiespaciados y muestra información del dominio
 * @param x_values Array con las coordenadas x de los nodos
 * @param y_values Array con las coordenadas y de los nodos
 * @param n Cantidad de nodos
 */
void mostrarNodosEnTabla (double *x_values, double *y_values, int n);

/**
 * Implementa la Regla del Trapecio (Simple y Compuesto)
 * Menú con 3 opciones:
 *   a) Trapecio Simple: I ≈ (b-a)/2 · [f(a) + f(b)]
 *   b) Trapecio Compuesto con función: I ≈ (h/2) · [f(a) + 2·Σf(xᵢ) + f(b)]
 *   c) Trapecio Compuesto con tabla de datos usando splines cúbicas
 */
void trapecioCompuesto ();

/**
 * Implementa la Regla de Simpson 1/3 Compuesta
 * Menú con 2 opciones:
 *   a) Simpson con función: I ≈ (h/3) · [f₀ + 4·Σf_impar + 2·Σf_par + fₙ]
 *   b) Simpson con tabla de datos usando splines cúbicas (requiere n PAR)
 * NOTA: n debe ser PAR (número de subintervalos)
 */
void simpsonCompuesto ();

// ==================== FUNCIONES AUXILIARES PARA SPLINES CÚBICAS ====================

/**
 * Construye el sistema tridiagonal A·M = b para splines cúbicas naturales
 * Condiciones de frontera: M₀ = 0 y Mₙ₋₁ = 0 (segunda derivada nula en extremos)
 * Sistema tridiagonal:
 *   - Diagonal inferior: hᵢ₋₁
 *   - Diagonal principal: 2(hᵢ₋₁ + hᵢ)
 *   - Diagonal superior: hᵢ
 *   - Lado derecho: 6·[(yᵢ₊₁-yᵢ)/hᵢ - (yᵢ-yᵢ₋₁)/hᵢ₋₁]
 * 
 * @param x Array con las coordenadas x de los nodos (n elementos)
 * @param y Array con las coordenadas y de los nodos (n elementos)
 * @param n Cantidad de nodos
 * @param A Matriz de coeficientes (n×n, almacenada por filas)
 * @param b_vec Vector de términos independientes (n elementos)
 */
void construirSistemaSplinesCubicas(double *x, double *y, int n, double *A, double *b_vec);

/**
 * Resuelve el sistema lineal A·x = b mediante Eliminación Gaussiana con pivoteo parcial
 * Pasos:
 *   1. Fase de eliminación hacia adelante con pivoteo
 *   2. Sustitución hacia atrás
 * 
 * @param A Matriz de coeficientes (n×n, almacenada por filas)
 * @param b_vec Vector de términos independientes (n elementos)
 * @param solution Vector donde se almacenará la solución (n elementos)
 * @param n Dimensión del sistema
 */
void eliminacionGaussiana(double *A, double *b_vec, double *solution, int n);

/**
 * Evalúa la spline cúbica en un punto x_eval
 * Usa la fórmula de Hermite para splines cúbicas:
 *   S_j(x) = (M_j/6h_j)·(x_{j+1}-x)³ + (M_{j+1}/6h_j)·(x-x_j)³
 *          + [y_j - M_j·h_j²/6]·(x_{j+1}-x)/h_j
 *          + [y_{j+1} - M_{j+1}·h_j²/6]·(x-x_j)/h_j
 * 
 * @param x_eval Punto donde evaluar la spline
 * @param x Array con las coordenadas x de los nodos
 * @param y Array con las coordenadas y de los nodos
 * @param M Array con las segundas derivadas M_i (solución del sistema)
 * @param n Cantidad de nodos
 * @return Valor de la spline en x_eval
 */
double evaluarSpline(double x_eval, double *x, double *y, double *M, int n);

/**
 * Implementa la Cuadratura de Gauss-Legendre con 2 a 6 puntos
 * Transforma el intervalo [a,b] a [-1,1] mediante:
 *   x = (b-a)/2 · ξ + (a+b)/2
 * Fórmula: I ≈ (b-a)/2 · Σ wᵢ·f(xᵢ)
 * 
 * Puntos disponibles: 2, 3, 4, 5 o 6
 * Pesos y nodos están predefinidos según tablas de Gauss-Legendre
 */
void gaussLagrange ();

/**
 * ============================================================================
 * FUNCIÓN PRINCIPAL - MENÚ DE MÉTODOS DE INTEGRACIÓN NUMÉRICA
 * ============================================================================
 * Presenta un menú interactivo con las siguientes opciones:
 *   a) Regla del Trapecio (Simple y Compuesto)
 *   b) Regla de Simpson 1/3 Compuesto
 *   c) Cuadratura de Gauss-Legendre (2-6 puntos)
 *   e) Salir
 * 
 * @param argc Cantidad de argumentos de línea de comandos (no usado)
 * @param argv Array de argumentos de línea de comandos (no usado)
 * @return 0 si la ejecución fue exitosa
 */
int main(int argc, char const *argv[])
{
    char opcion;
    int stopDoWhile = 0;

    /* Menu de opciones */
    do
    {
        system("clear"); // Limpiar la pantalla (Linux/Mac). En Windows usar "cls"
        printf("\n==================================================\n");
        printf("   MÉTODOS NUMÉRICOS - MÉTODOS DE INTEGRACIÓN NUMÉRICA\n");
        printf("==================================================\n");
        printf("Seleccione una opción:\n");
        printf("  a) Regla del Trapecio\n");
        printf("  b) Regla de Simpson Compuesto\n");
        printf("  c) Coordenadas de Gauss\n");
        printf("  e) Salir\n");
        printf("----------------------------------------\n");
        opcionMenu(&opcion);
        switch (opcion)
        {
        case 'a':
            trapecioCompuesto();
            break;
        case 'b':
            simpsonCompuesto();
            break;
        case 'c':
            gaussLagrange();
            break;
        case 'e':
            /* Salir del programa */
            printf("Saliendo del programa...\n");
            stopDoWhile = 1;
            break;
        default:
            printf("Opción no válida. Intente de nuevo.\n");
            break;
        }
    } while (stopDoWhile == 0);
    return 0;
}

/**
 * Lee una opción del menú ingresada por el usuario
 * Convierte automáticamente la entrada a minúscula para facilitar comparaciones
 * 
 * @param opcion Puntero a char donde se almacenará la opción (a-z)
 */
void opcionMenu (char *opcion)
{
    printf("Ingrese una opcion: ");
    scanf(" %c", opcion);
    *opcion = tolower(*opcion); // coonvierte los caracteres a minusculas
}

/**
 * Función de prueba para integración numérica
 * f(x) = 2x + ln(x) - sin(3x)
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
    // return sqrt(sin(sqrt(x)));
    // return (x * x - sin(sqrt(x)));
    //return log(x) + exp(sin(x)) - x;
    return (2*x + log(x) - sin(3*x));
}

/**
 * Lee nodos (pares x, y) desde un archivo de texto
 * 
 * Formato esperado del archivo:
 *   Línea 1: n (cantidad de nodos)
 *   Líneas 2 a n+1: x_i y_i (coordenadas separadas por espacio)
 * 
 * Ejemplo de archivo:
 *   3
 *   0.0 1.0
 *   1.0 2.5
 *   2.0 4.0
 * 
 * NOTA: Esta función aloca memoria dinámicamente que debe ser liberada con free()
 * 
 * @param filename Nombre del archivo a leer (ej: "nodos.txt")
 * @param x_values Puntero doble donde se almacenará el array de coordenadas x
 * @param y_values Puntero doble donde se almacenará el array de coordenadas y
 * @param n Puntero donde se almacenará la cantidad de nodos leídos
 */
void getNodesFromFile (const char* filename, double** x_values, double** y_values, int* n)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error al abrir el archivo %s\n", filename);
        exit(1);
    }
    fscanf(file, "%d", n); // Leer el número de nodos

    *x_values = (double*)malloc(*n * sizeof(double));
    *y_values = (double*)malloc(*n * sizeof(double));

    for (int i = 0; i < *n; i++)
    {
        fscanf(file, "%lf %lf", &(*x_values)[i], &(*y_values)[i]);
    }

    fclose(file);
    printf("Datos leídos correctamente desde %s\n", filename);
}

/**
 * ============================================================================
 * FUNCIÓN: mostrarNodosEnTabla
 * ============================================================================
 * Muestra los nodos leídos en formato de tabla con bordes UTF-8
 * 
 * Características:
 *   - Tabla formateada con caracteres de caja UTF-8 (╔ ═ ╗ ║ ╠ ╣ ╚ ╝)
 *   - Muestra índice, coordenadas x e y con 6 decimales
 *   - Información adicional del dominio (límites, rango)
 *   - Detección automática de espaciamiento uniforme vs no uniforme
 * 
 * Información mostrada:
 *   - Tabla de nodos (i, x_i, y_i)
 *   - Total de nodos
 *   - Límite inferior (a) y superior (b)
 *   - Rango total (b - a)
 *   - Tipo de espaciamiento (equiespaciado o no equiespaciado)
 * 
 * @param x_values Array con las coordenadas x de los nodos
 * @param y_values Array con las coordenadas y de los nodos
 * @param n Cantidad de nodos
 */
void mostrarNodosEnTabla (double *x_values, double *y_values, int n)
{
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║        TABLA DE NODOS LEÍDOS              ║\n");
    printf("╠═══════╦═════════════════╦═════════════════╣\n");
    printf("║   i   ║       x_i       ║       y_i       ║\n");
    printf("╠═══════╬═════════════════╬═════════════════╣\n");

    for (int i = 0; i < n; i++) {
        printf("║  %3d  ║  %13.6lf  ║  %13.6lf  ║\n", i, x_values[i], y_values[i]);
    }

    printf("╚═══════╩═════════════════╩═════════════════╝\n");
    printf("Total de nodos: %d\n", n);

    // Mostrar información adicional
    if (n >= 2) {
        printf("\nInformación del dominio:\n");
        printf("  • Límite inferior (a): %.6lf\n", x_values[0]);
        printf("  • Límite superior (b): %.6lf\n", x_values[n-1]);
        printf("  • Rango total: %.6lf\n", x_values[n-1] - x_values[0]);

        // Verificar si los puntos son equiespaciados
        if (n > 2) {
            int equiespaciado = 1;
            double h_ref = x_values[1] - x_values[0];
            double tolerancia = 1e-6;

            for (int i = 1; i < n-1; i++) {
                double h_actual = x_values[i+1] - x_values[i];
                if (fabs(h_actual - h_ref) > tolerancia) {
                    equiespaciado = 0;
                    break;
                }
            }

            if (equiespaciado) {
                printf("  • Espaciamiento: EQUIESPACIADO (h = %.6lf)\n", h_ref);
            } else {
                printf("  • Espaciamiento: NO EQUIESPACIADO\n");
                printf("    → Se requiere interpolación para trapecio compuesto\n");
            }
        }
    }
    printf("\n");
}

/**
 * ============================================================================
 * FUNCIÓN: trapecioCompuesto
 * ============================================================================
 * Implementa la Regla del Trapecio en tres modalidades:
 * 
 * A) TRAPECIO SIMPLE:
 *    Fórmula: I ≈ (b-a)/2 · [f(a) + f(b)]
 *    - Usa solo los extremos del intervalo
 *    - Rápido pero poco preciso
 *    - Error: O((b-a)³)
 * 
 * B) TRAPECIO COMPUESTO (con función):
 *    Fórmula: I ≈ (h/2) · [f(a) + 2·Σf(x_i) + f(b)]
 *    donde h = (b-a)/n y x_i = a + i·h para i = 1,2,...,n-1
 *    - Divide el intervalo en n subintervalos iguales
 *    - Mayor precisión que el simple
 *    - Error: O(h²) donde h = (b-a)/n
 * 
 * C) TRAPECIO COMPUESTO (con tabla de datos):
 *    Para datos NO equiespaciados, usa el proceso de 3 pasos:
 *    1. Construir splines cúbicas naturales → función continua S(x)
 *    2. Generar tabla equiespaciada evaluando S(x)
 *    3. Aplicar trapecio compuesto a la tabla generada
 * 
 * MENÚ INTERACTIVO:
 *   a) Trapecio SIMPLE
 *   b) Trapecio COMPUESTO (función)
 *   c) Trapecio COMPUESTO (tabla de datos)
 *   d) Volver al menú principal
 */
void trapecioCompuesto ()
{
    /* Opcion para el menu */
    char opcion;

    /* Limites de integracion */
    double a = 0.0; // Limite inferior
    double b = 0.0; // Limite superior

    int n = 0; // Numero de Subintervalos
    double h = 0.0; // Ancho de los subintervalos
    double x = 0.0; // Variable para los puntos de muestreo

    /* Sumador */
    double suma = 0.0;

    do
    {
        printf("\n----------------------------------------\n");
        printf("   REGLA DEL TRAPECIO\n");
        printf("----------------------------------------\n");
        printf("¿Implementar la Regla usando una funcion o una tabla de datos?");
        printf(" Seleccione una opción:\n");
        printf("  a) Trapecio SIMPLE (usando una función)\n");
        printf("  b) Trapecio COMPUESTO (usando una función)\n");
        printf("  c) Trapecio COMPUESTO (usando tabla de datos)\n");
        printf("  d) Volver Atras...\n");
        opcionMenu(&opcion);

        switch (opcion)
        {
        case 'a':
            /* ==========================================
               OPCIÓN A: TRAPECIO SIMPLE
               ==========================================
               Fórmula: I ≈ (b-a)/2 · [f(a) + f(b)]
               
               Solo usa los dos extremos del intervalo [a,b]
               Es la aproximación más básica:
               - 1 trapecio para todo el intervalo
               - 2 evaluaciones de la función
               - Error: O((b-a)³)
               ========================================== */
            printf("\n>>> TRAPECIO SIMPLE <<<\n");
            printf("Usa solo los extremos: f(a) y f(b)\n");
            printf("Fórmula: I ≈ (b-a)/2 · [f(a) + f(b)]\n\n");

            printf("Inserte los limites de integracion:\n");
            printf("Inserte el limite inferior a: ");
            scanf("%lf", &a);
            printf("Inserte el limite superior b: ");
            scanf("%lf", &b);

            /* Aplicar fórmula del trapecio simple */
            suma = (b - a) / 2.0 * (f(a) + f(b));

            printf("\n========================================\n");
            printf("  RESULTADO - TRAPECIO SIMPLE\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", suma);
            printf("Intervalo: [%.6lf, %.6lf]\n", a, b);
            printf("Ancho del intervalo: %.6lf\n", b - a);
            printf("========================================\n");
            printf("\nNOTA: Esta es una aproximación básica.\n");
            printf("      Use trapecio compuesto para mayor precisión.\n");

            printf("\nPresione ENTER para continuar...");
            getchar();
            getchar();
            break;
        case 'b':
            /* ==========================================
               OPCIÓN B: SIMPSON COMPUESTO (con tabla de datos)
               ==========================================
               Similar al Trapecio con tabla, pero con Simpson:
               
               MISMO PROBLEMA: Datos pueden ser NO UNIFORMES
               MISMA SOLUCIÓN: Proceso de 3 pasos
               
               PASO 1: INTERPOLACIÓN con Splines Cúbicas
                       - Construir función continua S(x)
                       - Garantiza suavidad C²
               
               PASO 2: RE-MUESTREO UNIFORME
                       - Crear m puntos equiespaciados
                       - REQUISITO: m-1 debe ser PAR
                       - Evaluar S(xᵢ) en cada punto
               
               PASO 3: INTEGRACIÓN con Simpson Compuesto
                       - Aplicar fórmula: I ≈ (h/3)·[f₀+4·Σf_impar+2·Σf_par+fₙ]
               
               VENTAJA sobre Trapecio:
               - Mayor precisión: O(h⁴) vs O(h²)
               - Exacto para polinomios de grado ≤ 3
               ========================================== */
            /* ==========================================
               OPCIÓN B: TRAPECIO COMPUESTO (con función)
               ==========================================
               Fórmula: I ≈ (h/2) · [f(a) + 2·Σf(xᵢ) + f(b)]
               
               Divide [a,b] en n subintervalos iguales:
               - h = (b-a)/n  (tamaño de cada subintervalo)
               - xᵢ = a + i·h  (nodos intermedios)
               
               Mejora la precisión al usar más trapecios:
               - n trapecios pequeños en lugar de 1 grande
               - (n+1) evaluaciones de la función
               - Error: O(h²) = O((b-a)²/n²)
               
               NOTA: Los nodos intermedios se multiplican por 2
               porque son compartidos entre trapecios adyacentes
               ========================================== */
            printf("\n>>> TRAPECIO COMPUESTO <<<\n");
            printf("Divide el intervalo en n subintervalos\n\n");

            printf("Inserte los limites de integracion:\n");
            printf("Inserte el limite inferior a: ");
            scanf("%lf", &a);
            printf("Inserte el limite superior b: ");
            scanf("%lf", &b);
            printf("Ingrese el numero de subintervalos: ");
            scanf("%d", &n);

            /* Calcular h */
            h = (b - a) / n;

            /* Regla del Trapecio Compuesta:
             * I ≈ (h/2) * [f(a) + 2·Σf(xᵢ) + f(b)]
             * donde i = 1, 2, ..., n-1 (puntos internos)
             *
             * ALGORITMO:
             * 1. Calcular extremos: f(a) y f(b)
             * 2. Calcular puntos internos con coeficiente 2
             *    Loop: i=1 hasta n-1
             *    - Calcular x = a + i*h
             *    - Acumular 2*f(x) en suma
             * 3. Aplicar fórmula: resultado = (h/2) * [f(a) + suma + f(b)]
             */
            suma = f(a) + f(b);  // Extremos: f(a) + f(b)

            // Sumar puntos internos con coeficiente 2
            for (int i = 1; i < n; i++)  // i va de 1 a n-1 (puntos internos)
            {
                x = a + i * h;  // x₁ = a+h, x₂ = a+2h, ..., x_{n-1} = a+(n-1)h
                suma += 2.0 * f(x);
            }

            suma = (h / 2.0) * suma;

            printf("\n========================================\n");
            printf("  RESULTADO - TRAPECIO COMPUESTO\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", suma);
            printf("Número de subintervalos: %d\n", n);
            printf("Espaciamiento h: %.6lf\n", h);
            printf("========================================\n");

            printf("\nPresione ENTER para continuar...");
            getchar(); // Captura el ENTER pendiente del scanf anterior
            getchar(); // Espera que el usuario presione ENTER
            break;
        case 'c':
            /* ==========================================
               OPCIÓN C: TRAPECIO COMPUESTO (con tabla de datos)
               ==========================================
               Cuando los datos son PUNTOS DISCRETOS (no una función):
               
               PROBLEMA: Los datos pueden ser NO UNIFORMES
               - Nodos con espaciamiento irregular: x₀, x₁, ..., xₙ
               - No podemos aplicar directamente la fórmula del trapecio
               
               SOLUCIÓN: Proceso de 3 pasos
               
               PASO 1: INTERPOLACIÓN con Splines Cúbicas
                       - Construir splines entre cada par de nodos
                       - Garantiza continuidad C² (suave)
                       - Resultado: función continua S(x)
               
               PASO 2: RE-MUESTREO UNIFORME
                       - Crear m puntos equiespaciados en [a,b]
                       - h = (b-a)/m
                       - xᵢ = a + i·h
                       - Evaluar S(xᵢ) en cada punto
               
               PASO 3: INTEGRACIÓN con Trapecio Compuesto
                       - Aplicar fórmula estándar a datos uniformes
                       - I ≈ (h/2)·[S(a) + 2·ΣS(xᵢ) + S(b)]
               
               VENTAJAS:
               - Maneja datos no uniformes correctamente
               - Preserva suavidad entre puntos
               - Usa método de integración estándar
               ========================================== */
            /* Puntos que se obtienen del archivo */
            double *x_values = NULL;
            double *y_values = NULL;

            double *A = NULL; // Matriz de coeficientes
            double *b_vec = NULL; // Vector de términos independientes
            double *solution = NULL; // Vector solución

            printf("Implementacion usando una tabla de datos...\n");
            getNodesFromFile("nodos.txt", &x_values, &y_values, &n);

            /* Mostrar los nodos en formato de tabla */
            mostrarNodosEnTabla(x_values, y_values, n);

            /* Inicializar A y b */
            A = (double*)malloc(n * n * sizeof(double));
            b_vec = (double*)malloc(n * sizeof(double));
            solution = (double*)malloc(n * sizeof(double));

            printf("\n--- PASO 1: Construyendo sistema de splines cúbicas ---\n");
            /* Construir el sistema de ecuaciones para los splines
             * 
             * TEORÍA DE SPLINES CÚBICAS:
             * Para n puntos, tenemos n-1 segmentos
             * Cada segmento i tiene un polinomio cúbico:
             *   Sᵢ(x) = aᵢ + bᵢ(x-xᵢ) + cᵢ(x-xᵢ)² + dᵢ(x-xᵢ)³
             * 
             * CONDICIONES:
             * 1. Interpolación: Sᵢ(xᵢ) = yᵢ y Sᵢ(xᵢ₊₁) = yᵢ₊₁
             * 2. Continuidad C¹: S'ᵢ(xᵢ₊₁) = S'ᵢ₊₁(xᵢ₊₁)
             * 3. Continuidad C²: S''ᵢ(xᵢ₊₁) = S''ᵢ₊₁(xᵢ₊₁)
             * 4. Frontera natural: S''(x₀) = S''(xₙ) = 0
             * 
             * RESULTADO: Sistema tridiagonal A·M = b
             * donde M = [M₀, M₁, ..., Mₙ₋₁] son las segundas derivadas
             */

            // Construir el sistema tridiagonal para encontrar las segundas derivadas M_i
            construirSistemaSplinesCubicas(x_values, y_values, n, A, b_vec);

            // Resolver el sistema A * M = b para obtener las segundas derivadas
            eliminacionGaussiana(A, b_vec, solution, n);

            printf("Splines cúbicas calculadas exitosamente.\n");
            printf("Segundas derivadas (M_i) en los nodos:\n");
            for (int i = 0; i < n; i++) {
                printf("  M[%d] = %.6lf\n", i, solution[i]);
            }

            printf("\n--- PASO 2: Generando tabla equiespaciada ---\n");
            /* RE-MUESTREO UNIFORME
             * 
             * OBJETIVO: Convertir datos no uniformes en uniformes
             * 
             * PROCESO:
             * 1. Determinar límites: a = x_min, b = x_max
             * 2. Crear n_equi puntos uniformes:
             *    h = (b-a)/(n_equi-1)
             *    x_equi[i] = a + i*h
             * 3. Evaluar spline en cada punto:
             *    y_equi[i] = S(x_equi[i])
             * 
             * RESULTADO: Tabla con espaciamiento constante h
             * que preserva la forma de los datos originales
             */

            int n_equi = 0; // Número de puntos equiespaciados
            printf("Ingrese el número de puntos equiespaciados para integración: ");
            scanf("%d", &n_equi);

            double *x_equi = (double*)malloc(n_equi * sizeof(double));
            double *y_equi = (double*)malloc(n_equi * sizeof(double));

            a = x_values[0];     // Límite inferior
            b = x_values[n-1];   // Límite superior
            h = (b - a) / (n_equi - 1);

            printf("Límites: a = %.4lf, b = %.4lf\n", a, b);
            printf("Espaciamiento h = %.6lf\n", h);

            // Evaluar la spline en los puntos equiespaciados
            for (int i = 0; i < n_equi; i++) {
                x_equi[i] = a + i * h;
                y_equi[i] = evaluarSpline(x_equi[i], x_values, y_values, solution, n);
            }
            printf("Tabla equiespaciada generada con %d puntos.\n", n_equi);

            printf("\n--- PASO 3: Aplicando Regla del Trapecio Compuesto ---\n");
            /* INTEGRACIÓN NUMÉRICA
             * 
             * Ahora que tenemos datos UNIFORMES (x_equi, y_equi),
             * podemos aplicar la fórmula estándar del Trapecio Compuesto:
             * 
             * I ≈ (h/2) · [y₀ + 2·(y₁ + y₂ + ... + yₙ₋₁) + yₙ]
             * 
             * VENTAJA: Los datos ahora están equiespaciados (h constante)
             * por lo que la fórmula es válida directamente.
             */
            suma = 0.0;
            suma = y_equi[0] + y_equi[n_equi - 1]; // f(a) + f(b)

            for (int i = 1; i < n_equi - 1; i++) {
                suma += 2 * y_equi[i];
            }

            suma = (h / 2.0) * suma;

            printf("\n========================================\n");
            printf("  RESULTADO DE LA INTEGRACIÓN\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", suma);
            printf("Número de subintervalos: %d\n", n_equi - 1);
            printf("========================================\n");

            /* Liberar memoria */
            free(x_values);
            free(y_values);
            free(A);
            free(b_vec);
            free(solution);
            free(x_equi);
            free(y_equi);

            printf("\nPresione ENTER para continuar...");
            getchar();
            getchar();
            break;
        case 'd':
            /* Volver Atras */
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opción no válida. Intente de nuevo.\n");
            break;
        }
    } while (opcion != 'd');
}

/**
 * ============================================================================
 * FUNCIÓN AUXILIAR: construirSistemaSplinesCubicas
 * ============================================================================
 * Construye el sistema lineal tridiagonal A·M = b para splines cúbicas naturales
 * 
 * SPLINES CÚBICAS NATURALES:
 *   En cada intervalo [x_j, x_{j+1}], la spline es un polinomio cúbico:
 *   S_j(x) = a_j + b_j·(x-x_j) + c_j·(x-x_j)² + d_j·(x-x_j)³
 * 
 * CONDICIONES DE FRONTERA NATURALES:
 *   - S''(x_0) = 0  →  M_0 = 0
 *   - S''(x_{n-1}) = 0  →  M_{n-1} = 0
 * 
 * SISTEMA TRIDIAGONAL (n ecuaciones para n incógnitas M_i):
 *   Primera ecuación: M_0 = 0
 *   Ecuaciones intermedias (i = 1,...,n-2):
 *     h_{i-1}·M_{i-1} + 2(h_{i-1}+h_i)·M_i + h_i·M_{i+1} = 
 *       6·[(y_{i+1}-y_i)/h_i - (y_i-y_{i-1})/h_{i-1}]
 *   Última ecuación: M_{n-1} = 0
 * 
 * donde h_i = x_{i+1} - x_i
 * 
 * @param x Array con coordenadas x de los nodos (n elementos, ordenados)
 * @param y Array con coordenadas y de los nodos (n elementos)
 * @param n Cantidad de nodos
 * @param A Matriz tridiagonal n×n (salida, almacenada por filas)
 * @param b_vec Vector de términos independientes n (salida)
 */
void construirSistemaSplinesCubicas(double *x, double *y, int n, double *A, double *b_vec)
{
    // Inicializar matriz A con ceros
    for (int i = 0; i < n * n; i++) {
        A[i] = 0.0;
    }

    // Calcular diferencias h_i = x_{i+1} - x_i
    double *h = (double*)malloc((n-1) * sizeof(double));
    for (int i = 0; i < n-1; i++) {
        h[i] = x[i+1] - x[i];
    }

    /* Primera ecuación: M_0 = 0 (condición natural) */
    A[0 * n + 0] = 1.0;
    b_vec[0] = 0.0;

    /* Ecuaciones intermedias (i = 1, 2, ..., n-2) */
    for (int i = 1; i < n-1; i++) {
        // Coeficiente de M_{i-1}
        A[i * n + (i-1)] = h[i-1];

        // Coeficiente de M_i
        A[i * n + i] = 2.0 * (h[i-1] + h[i]);

        // Coeficiente de M_{i+1}
        A[i * n + (i+1)] = h[i];

        // Lado derecho
        b_vec[i] = 6.0 * ((y[i+1] - y[i]) / h[i] - (y[i] - y[i-1]) / h[i-1]);
    }

    /* Última ecuación: M_{n-1} = 0 (condición natural) */
    A[(n-1) * n + (n-1)] = 1.0;
    b_vec[n-1] = 0.0;

    free(h);
}

/**
 * ============================================================================
 * FUNCIÓN AUXILIAR: eliminacionGaussiana
 * ============================================================================
 * Resuelve el sistema lineal A·x = b mediante Eliminación Gaussiana con pivoteo parcial
 * 
 * ALGORITMO:
 *   1. FASE DE ELIMINACIÓN (triangularización):
 *      - Para cada columna k:
 *        a) Pivoteo parcial: buscar el elemento de mayor valor absoluto
 *        b) Intercambiar filas si es necesario
 *        c) Eliminación: hacer ceros debajo del pivote
 * 
 *   2. FASE DE SUSTITUCIÓN HACIA ATRÁS:
 *      - Resolver desde la última ecuación hacia la primera
 *      - x_i = (b_i - Σ a_{ij}·x_j) / a_{ii}
 * 
 * PIVOTEO PARCIAL:
 *   - Mejora la estabilidad numérica del algoritmo
 *   - Evita divisiones por números muy pequeños
 *   - Reduce errores de redondeo
 * 
 * COMPLEJIDAD: O(n³) operaciones
 * 
 * NOTA: Trabaja con copias de A y b para preservar los valores originales
 * 
 * @param A Matriz de coeficientes n×n (almacenada por filas, no se modifica)
 * @param b_vec Vector de términos independientes n (no se modifica)
 * @param solution Vector donde se almacena la solución x (salida, n elementos)
 * @param n Dimensión del sistema
 */
void eliminacionGaussiana(double *A, double *b_vec, double *solution, int n)
{
    // Crear copia de A y b para no modificar los originales
    double *A_copy = (double*)malloc(n * n * sizeof(double));
    double *b_copy = (double*)malloc(n * sizeof(double));

    for (int i = 0; i < n * n; i++) {
        A_copy[i] = A[i];
    }
    for (int i = 0; i < n; i++) {
        b_copy[i] = b_vec[i];
    }

    /* Fase de eliminación */
    for (int k = 0; k < n-1; k++) {
        // Pivoteo parcial
        int max_row = k;
        double max_val = fabs(A_copy[k * n + k]);

        for (int i = k+1; i < n; i++) {
            if (fabs(A_copy[i * n + k]) > max_val) {
                max_val = fabs(A_copy[i * n + k]);
                max_row = i;
            }
        }

        // Intercambiar filas si es necesario
        if (max_row != k) {
            for (int j = 0; j < n; j++) {
                double temp = A_copy[k * n + j];
                A_copy[k * n + j] = A_copy[max_row * n + j];
                A_copy[max_row * n + j] = temp;
            }
            double temp = b_copy[k];
            b_copy[k] = b_copy[max_row];
            b_copy[max_row] = temp;
        }

        // Eliminación
        for (int i = k+1; i < n; i++) {
            double factor = A_copy[i * n + k] / A_copy[k * n + k];
            for (int j = k; j < n; j++) {
                A_copy[i * n + j] -= factor * A_copy[k * n + j];
            }
            b_copy[i] -= factor * b_copy[k];
        }
    }

    /* Sustitución hacia atrás */
    for (int i = n-1; i >= 0; i--) {
        solution[i] = b_copy[i];
        for (int j = i+1; j < n; j++) {
            solution[i] -= A_copy[i * n + j] * solution[j];
        }
        solution[i] /= A_copy[i * n + i];
    }

    free(A_copy);
    free(b_copy);
}

/**
 * ============================================================================
 * FUNCIÓN AUXILIAR: evaluarSpline
 * ============================================================================
 * Evalúa la spline cúbica S(x) en un punto x_eval dado
 * 
 * FÓRMULA DE HERMITE PARA SPLINES CÚBICAS:
 *   En el intervalo [x_j, x_{j+1}], la spline cúbica es:
 * 
 *   S_j(x) = M_j/(6h_j) · (x_{j+1} - x)³ + M_{j+1}/(6h_j) · (x - x_j)³
 *          + [y_j - M_j·h_j²/6] · (x_{j+1} - x)/h_j
 *          + [y_{j+1} - M_{j+1}·h_j²/6] · (x - x_j)/h_j
 * 
 * donde:
 *   - h_j = x_{j+1} - x_j  (ancho del intervalo)
 *   - M_j, M_{j+1} son las segundas derivadas en los extremos
 *   - y_j, y_{j+1} son los valores de la función en los extremos
 * 
 * PROPIEDADES DE LA SPLINE:
 *   - S_j(x_j) = y_j  (interpolación)
 *   - S_j(x_{j+1}) = y_{j+1}  (interpolación)
 *   - Primera derivada continua en x_j
 *   - Segunda derivada continua en x_j
 * 
 * MANEJO DE PUNTOS FUERA DEL RANGO:
 *   - Si x < x_0: usa el primer intervalo (extrapolación)
 *   - Si x > x_{n-1}: usa el último intervalo (extrapolación)
 * 
 * @param x_eval Punto donde evaluar la spline
 * @param x Array con coordenadas x de los nodos (n elementos, ordenados)
 * @param y Array con coordenadas y de los nodos (n elementos)
 * @param M Array con segundas derivadas M_i (n elementos, de eliminacionGaussiana)
 * @param n Cantidad de nodos
 * @return Valor de S(x_eval)
 */
double evaluarSpline(double x_eval, double *x, double *y, double *M, int n)
{
    // Encontrar el intervalo [x_j, x_{j+1}] que contiene x_eval
    int j = 0;
    for (int i = 0; i < n-1; i++) {
        if (x_eval >= x[i] && x_eval <= x[i+1]) {
            j = i;
            break;
        }
    }

    // Si x_eval está fuera del rango, usar el primer o último intervalo
    if (x_eval < x[0]) j = 0;
    if (x_eval > x[n-1]) j = n-2;

    double h_j = x[j+1] - x[j];

    // Calcular la spline cúbica S_j(x)
    double t1 = (M[j] / 6.0) * pow(x[j+1] - x_eval, 3) / h_j;
    double t2 = (M[j+1] / 6.0) * pow(x_eval - x[j], 3) / h_j;
    double t3 = (y[j] - M[j] * h_j * h_j / 6.0) * (x[j+1] - x_eval) / h_j;
    double t4 = (y[j+1] - M[j+1] * h_j * h_j / 6.0) * (x_eval - x[j]) / h_j;

    return t1 + t2 + t3 + t4;
}

/**
 * ============================================================================
 * FUNCIÓN: simpsonCompuesto
 * ============================================================================
 * Implementa la Regla de Simpson 1/3 Compuesta para integración numérica
 * 
 * FÓRMULA DE SIMPSON 1/3 COMPUESTA:
 *   I ≈ (h/3) · [f(x₀) + 4·Σf(x_impar) + 2·Σf(x_par) + f(xₙ)]
 * 
 * donde:
 *   - h = (b-a)/n  (ancho de cada subintervalo)
 *   - n debe ser PAR (número de subintervalos)
 *   - Puntos impares: x₁, x₃, x₅, ..., xₙ₋₁ (coeficiente 4)
 *   - Puntos pares: x₂, x₄, x₆, ..., xₙ₋₂ (coeficiente 2)
 *   - Extremos: x₀ y xₙ (coeficiente 1)
 * 
 * PATRÓN DE COEFICIENTES: 1 - 4 - 2 - 4 - 2 - ... - 4 - 1
 * 
 * PRECISIÓN:
 *   - Error: O(h⁴) donde h = (b-a)/n
 *   - Más preciso que Trapecio (O(h²))
 *   - EXACTO para polinomios de grado ≤ 3
 * 
 * DOS MODALIDADES:
 * 
 * A) CON FUNCIÓN:
 *    - Evaluación directa de f(x) en los puntos equiespaciados
 *    - Usuario ingresa límites a, b y número de subintervalos n (debe ser PAR)
 * 
 * B) CON TABLA DE DATOS (proceso de 3 pasos):
 *    1. Construir splines cúbicas naturales → función continua S(x)
 *    2. Generar tabla equiespaciada evaluando S(x) (n PAR)
 *    3. Aplicar Simpson 1/3 a la tabla generada
 * 
 * VALIDACIÓN: El programa verifica que n sea PAR y solicita reingreso si no lo es
 * 
 * MENÚ INTERACTIVO:
 *   a) Simpson con función
 *   b) Simpson con tabla de datos
 *   c) Volver al menú principal
 */
void simpsonCompuesto ()
{
    /* Opcion para el menu */
    char opcion;

    /* Limites de integracion */
    double a = 0.0; // Limite inferior
    double b = 0.0; // Limite superior

    int n = 0; // Numero de Subintervalos
    double h = 0.0; // Ancho de los subintervalos

    /* Sumador */
    double suma = 0.0;

    do
    {
        printf("\n----------------------------------------\n");
        printf("   REGLA DE SIMPSON COMPUESTO\n");
        printf("----------------------------------------\n");
        printf("¿Implementar la Regla usando una funcion o una tabla de datos?");
        printf(" Seleccione una opción:\n");
        printf("  a) Usando una Funcion.\n");
        printf("  b) Usando una tabla de datos.\n");
        printf("  c) Volver Atras...\n");
        opcionMenu(&opcion);
        switch (opcion)
        {
        case 'a':
            /* ==========================================
               OPCIÓN A: SIMPSON COMPUESTO (con función)
               ==========================================
               Fórmula: I ≈ (h/3) · [f₀ + 4·Σf_impar + 2·Σf_par + fₙ]
               
               REQUISITO: n debe ser PAR (número PAR de subintervalos)
               
               Basado en parábolas de segundo grado:
               - Aproxima la función con segmentos parabólicos
               - Cada parábola pasa por 3 puntos consecutivos
               - Requiere número impar de puntos (n+1 puntos)
               
               COEFICIENTES:
               - Extremos (x₀, xₙ): coeficiente 1
               - Índices impares (x₁, x₃, x₅, ...): coeficiente 4
               - Índices pares internos (x₂, x₄, x₆, ...): coeficiente 2
               
               PRECISIÓN:
               - Error: O(h⁴) = O((b-a)⁴/n⁴)
               - Mucho mejor que trapecio O(h²)
               - Exacto para polinomios de grado ≤ 3
               ========================================== */
            printf("Implementacion usando una funcion...\n");
            printf("Inserte los limites de integracion:\n");
            printf("Inserte el limite inferior a: ");
            scanf("%lf", &a);
            printf("Inserte el limite superior b: ");
            scanf("%lf", &b);

            do
            {
                printf("Ingrese el numero de subintervalos (debe ser PAR): ");
                scanf("%d", &n);
                if (n % 2 != 0) {
                    printf("Error: El número debe ser par. Intente de nuevo.\n");
                }
            } while (n % 2 != 0);

            /* Calcular h */
            h = (b - a) / n;

            /* Regla de Simpson 1/3 compuesta:
             * I ≈ (h/3) * [f(x₀) + 4·f(x₁) + 2·f(x₂) + 4·f(x₃) + 2·f(x₄) + ... + 4·f(xₙ₋₁) + f(xₙ)]
             *
             * Patrón: f₀ + 4·(f₁+f₃+f₅+...) + 2·(f₂+f₄+f₆+...) + fₙ
             * 
             * ALGORITMO DE IMPLEMENTACIÓN:
             * 1. Inicializar suma con extremos: f(a) + f(b)
             * 2. Loop 1: índices impares (1,3,5,...) → coef 4
             *    - i = 1, 3, 5, ..., n-1
             *    - suma += 4·f(a + i·h)
             * 3. Loop 2: índices pares internos (2,4,6,...) → coef 2
             *    - i = 2, 4, 6, ..., n-2
             *    - suma += 2·f(a + i·h)
             * 4. Multiplicar por h/3
             * 
             * NOTA: Separar en dos loops evita condiciones dentro del loop
             * y hace el código más claro y eficiente
             */

            suma = f(a) + f(b);  // Extremos: f(x₀) + f(xₙ)

            // Sumar términos impares (índices 1, 3, 5, ..., n-1) con coeficiente 4
            for (int i = 1; i < n; i += 2) {
                double x = a + i * h;
                suma += 4.0 * f(x);
            }

            // Sumar términos pares (índices 2, 4, 6, ..., n-2) con coeficiente 2
            for (int i = 2; i < n; i += 2) {
                double x = a + i * h;
                suma += 2.0 * f(x);
            }

            suma = (h / 3.0) * suma;

            printf("\n========================================\n");
            printf("  RESULTADO DE LA INTEGRACIÓN\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", suma);
            printf("Número de subintervalos: %d\n", n);
            printf("Espaciamiento h: %.6lf\n", h);
            printf("========================================\n");

            printf("\nPresione ENTER para continuar...");
            getchar(); // Captura el ENTER pendiente del scanf anterior
            getchar(); // Espera que el usuario presione ENTER
            break;
        case 'b':
            /* Puntos que se obtienen del archivo */
            double *x_values = NULL;
            double *y_values = NULL;

            double *A = NULL; // Matriz de coeficientes
            double *b_vec = NULL; // Vector de términos independientes
            double *solution = NULL; // Vector solución

            printf("Implementacion usando una tabla de datos...\n");
            getNodesFromFile("nodos.txt", &x_values, &y_values, &n);

            /* Mostrar los nodos en formato de tabla */
            mostrarNodosEnTabla(x_values, y_values, n);

            /* Inicializar A y b */
            A = (double*)malloc(n * n * sizeof(double));
            b_vec = (double*)malloc(n * sizeof(double));
            solution = (double*)malloc(n * sizeof(double));

            /* ========================================
               PASO 1: Construir Splines Cúbicas Naturales
               ======================================== */
            printf("\n--- PASO 1: Construyendo sistema de splines cúbicas ---\n");

            // Construir el sistema tridiagonal para encontrar las segundas derivadas M_i
            construirSistemaSplinesCubicas(x_values, y_values, n, A, b_vec);

            // Resolver el sistema A * M = b para obtener las segundas derivadas
            eliminacionGaussiana(A, b_vec, solution, n);

            printf("Splines cúbicas calculadas exitosamente.\n");
            printf("Segundas derivadas (M_i) en los nodos:\n");
            for (int i = 0; i < n; i++) {
                printf("  M[%d] = %.6lf\n", i, solution[i]);
            }

            /* ========================================
               PASO 2: Generar tabla equiespaciada (DEBE SER PAR)
               ======================================== */
            printf("\n--- PASO 2: Generando tabla equiespaciada ---\n");

            int n_equi = 0; // Número de puntos equiespaciados
            int n_subintervalos = 0; // Número de subintervalos (debe ser par)

            do {
                printf("Ingrese el número de subintervalos para Simpson (debe ser PAR): ");
                scanf("%d", &n_subintervalos);
                if (n_subintervalos % 2 != 0) {
                    printf("Error: El número debe ser par. Intente de nuevo.\n");
                }
            } while (n_subintervalos % 2 != 0);

            n_equi = n_subintervalos + 1; // n puntos = n subintervalos + 1

            double *x_equi = (double*)malloc(n_equi * sizeof(double));
            double *y_equi = (double*)malloc(n_equi * sizeof(double));

            a = x_values[0];     // Límite inferior
            b = x_values[n-1];   // Límite superior
            h = (b - a) / n_subintervalos;

            printf("Límites: a = %.4lf, b = %.4lf\n", a, b);
            printf("Número de subintervalos: %d\n", n_subintervalos);
            printf("Espaciamiento h = %.6lf\n", h);

            // Evaluar la spline en los puntos equiespaciados
            for (int i = 0; i < n_equi; i++) {
                x_equi[i] = a + i * h;
                y_equi[i] = evaluarSpline(x_equi[i], x_values, y_values, solution, n);
            }

            printf("Tabla equiespaciada generada con %d puntos.\n", n_equi);

            /* Calculo de I */
            printf("\n--- PASO 3: Aplicando Regla de Simpson 1/3 Compuesta ---\n");

            suma = 0.0;

            // Extremos: f(x₀) + f(xₙ)
            suma = y_equi[0] + y_equi[n_equi - 1];

            // Términos impares (índices 1, 3, 5, ..., n_equi-2) con coeficiente 4
            for (int i = 1; i < n_equi - 1; i += 2) {
                suma += 4.0 * y_equi[i];
            }

            // Términos pares (índices 2, 4, 6, ..., n_equi-3) con coeficiente 2
            for (int i = 2; i < n_equi - 1; i += 2) {
                suma += 2.0 * y_equi[i];
            }

            suma = (h / 3.0) * suma;

            printf("\n========================================\n");
            printf("  RESULTADO DE LA INTEGRACIÓN\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", suma);
            printf("Número de subintervalos: %d\n", n_subintervalos);
            printf("Espaciamiento h: %.6lf\n", h);
            printf("========================================\n");

            /* Liberar memoria */
            free(x_values);
            free(y_values);
            free(A);
            free(b_vec);
            free(solution);
            free(x_equi);
            free(y_equi);

            printf("\nPresione ENTER para continuar...");
            getchar();
            getchar();
            break;
        case 'c':
            /* Volver Atras */
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opción no válida. Intente de nuevo.\n");
            break;
        }
    } while (opcion != 'c');
}

/**
 * ============================================================================
 * FUNCIÓN: gaussLagrange
 * ============================================================================
 * Implementa la Cuadratura de Gauss-Legendre para integración numérica
 * 
 * FÓRMULA GENERAL:
 *   I = ∫[a,b] f(x)dx ≈ (b-a)/2 · Σ wᵢ·f(xᵢ)
 * 
 * TRANSFORMACIÓN DE INTERVALO:
 *   Transforma [a,b] → [-1,1] mediante:
 *   x = (b-a)/2 · ξ + (a+b)/2
 * 
 * donde ξ son los nodos de Gauss-Legendre en [-1,1]
 * 
 * CARACTERÍSTICAS:
 *   - Método de alta precisión con pocas evaluaciones
 *   - n puntos → exacto para polinomios de grado ≤ 2n-1
 *   - Ejemplo: 2 puntos → exacto hasta grado 3
 *              3 puntos → exacto hasta grado 5
 * 
 * NODOS Y PESOS DISPONIBLES:
 *   - 2 puntos: Exacto para polinomios grado ≤ 3
 *   - 3 puntos: Exacto para polinomios grado ≤ 5
 *   - 4 puntos: Exacto para polinomios grado ≤ 7
 *   - 5 puntos: Exacto para polinomios grado ≤ 9
 *   - 6 puntos: Exacto para polinomios grado ≤ 11
 * 
 * ESTRUCTURA DE ARRAYS:
 *   - Cada array contiene: [pesos... , nodos...]
 *   - Ejemplo para n=2: [w₀, w₁, ξ₀, ξ₁]
 * 
 * VENTAJAS:
 *   ✓ Muy eficiente: pocas evaluaciones, alta precisión
 *   ✓ Ideal para funciones suaves
 *   ✓ No requiere puntos equiespaciados
 * 
 * DESVENTAJAS:
 *   ✗ Menos preciso para funciones con singularidades
 *   ✗ Nodos y pesos son fijos (no ajustables)
 * 
 * MODO INTERACTIVO:
 *   - Usuario ingresa límites [a,b]
 *   - Puede probar diferentes cantidades de puntos (2-6)
 *   - Muestra resultado después de cada cálculo
 *   - Ingresa 0 para salir
 */
void gaussLegendre ()
{
    /* Limites de integracion */
    double a = 0.0;
    double b = 0.0;

    double I = 0.0; // Resultado de la integral

    int puntos = 0; // Numero de puntos de Gauss

    /* Pesos y nodos predefinidos para hasta 6 puntos */
    /* ESTRUCTURA DE LOS ARREGLOS:
     * Para n puntos, el arreglo tiene 2n elementos:
     *   - Primeros n elementos: PESOS (w₀, w₁, ..., wₙ₋₁)
     *   - Siguientes n elementos: NODOS (t₀, t₁, ..., tₙ₋₁)
     * 
     * NOTA: Los nodos están en el intervalo [-1, 1]
     *       Se transformarán al intervalo [a, b] con:
     *       x = ((b-a)/2)·t + (a+b)/2
     */
    double two[] = {1, 1, -0.577350269, 0.577350269};

    double three[] = {0.5555556, 0.8888889, 0.5555556, -0.774596669, 0.0, 0.774596669};

    double four[] = {0.3478548, 0.6521452, 0.6521452, 0.3478548, -0.861136312, -0.339981044, 0.339981044, 0.861136312};

    double five[] = {0.2369269, 0.4786287, 0.5688889, 0.4786287, 0.2369269, -0.906179846, -0.538469310, 0.0, 0.538469310, 0.906179846};

    double six[] = {0.1713245, 0.3607616, 0.4679139, 0.4679139, 0.3607616, 0.1713245, -0.932469514, -0.661209386, -0.238619186, 0.238619186, 0.661209386, 0.932469514};

    printf("\n-----------------------------------------------\n");
    printf("   FORMULA DE GAUSS-LAGRANGE CON MAS PUNTOS\n");
    printf("-----------------------------------------------\n");
    printf("Inserte los limites de integracion:\n");
    printf("Inserte el limite inferior a: ");
    scanf("%lf", &a);
    printf("Inserte el limite superior b: ");
    scanf("%lf", &b);

    do
    {
        /* Calcular I */
        printf("\nIngresar el numero de puntos de Gauss (entre 2 y 6), o 0 para salir: ");
        scanf("%d", &puntos);
        
        if (puntos == 0) {
            printf("Saliendo de Gauss-Lagrange...\n");
            break;
        }
        
        switch (puntos)
        {
        case 2:
            /* GAUSS-LEGENDRE con 2 PUNTOS
             * 
             * FÓRMULA: I ≈ (b-a)/2 · [w₀·f(x₀) + w₁·f(x₁)]
             * 
             * TRANSFORMACIÓN de nodos t ∈ [-1,1] → x ∈ [a,b]:
             *   x = ((b-a)/2)·t + (a+b)/2
             * 
             * PRECISIÓN: Exacto para polinomios de grado ≤ 3
             */
            I = ((b - a) / 2.0) * (two[0] * f((b - a) / 2.0 * two[2] + (a + b) / 2.0) +
                             two[1] * f((b - a) / 2.0 * two[3] + (a + b) / 2.0));
            
            printf("\n========================================\n");
            printf("  RESULTADO - GAUSS-LEGENDRE (2 puntos)\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", I);
            printf("Intervalo: [%.6lf, %.6lf]\n", a, b);
            printf("Número de evaluaciones: 2\n");
            printf("========================================\n");
            break;
        case 3:
            /* GAUSS-LEGENDRE con 3 PUNTOS
             * 
             * FÓRMULA: I ≈ (b-a)/2 · [w₀·f(x₀) + w₁·f(x₁) + w₂·f(x₂)]
             * 
             * NOTA: El punto central (x₁) está en t=0, que se mapea a (a+b)/2
             * 
             * PRECISIÓN: Exacto para polinomios de grado ≤ 5
             */
            I = ((b - a) / 2.0) * (three[0] * f((b - a) / 2.0 * three[3] + (a + b) / 2.0) +
                             three[1] * f((b - a) / 2.0 * three[4] + (a + b) / 2.0) +
                             three[2] * f((b - a) / 2.0 * three[5] + (a + b) / 2.0));
            
            printf("\n========================================\n");
            printf("  RESULTADO - GAUSS-LEGENDRE (3 puntos)\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", I);
            printf("Intervalo: [%.6lf, %.6lf]\n", a, b);
            printf("Número de evaluaciones: 3\n");
            printf("========================================\n");
            break;
        case 4:
            I = ((b - a) / 2.0) * (four[0] * f((b - a) / 2.0 * four[4] + (a + b) / 2.0) +
                             four[1] * f((b - a) / 2.0 * four[5] + (a + b) / 2.0) +
                             four[2] * f((b - a) / 2.0 * four[6] + (a + b) / 2.0) +
                             four[3] * f((b - a) / 2.0 * four[7] + (a + b) / 2.0));
            
            printf("\n========================================\n");
            printf("  RESULTADO - GAUSS-LEGENDRE (4 puntos)\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", I);
            printf("Intervalo: [%.6lf, %.6lf]\n", a, b);
            printf("Número de evaluaciones: 4\n");
            printf("========================================\n");
            break;
        case 5:
            I = ((b - a) / 2.0) * (five[0] * f((b - a) / 2.0 * five[5] + (a + b) / 2.0) +
                             five[1] * f((b - a) / 2.0 * five[6] + (a + b) / 2.0) +
                             five[2] * f((b - a) / 2.0 * five[7] + (a + b) / 2.0) +
                             five[3] * f((b - a) / 2.0 * five[8] + (a + b) / 2.0) +
                             five[4] * f((b - a) / 2.0 * five[9] + (a + b) / 2.0));
            
            printf("\n========================================\n");
            printf("  RESULTADO - GAUSS-LEGENDRE (5 puntos)\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", I);
            printf("Intervalo: [%.6lf, %.6lf]\n", a, b);
            printf("Número de evaluaciones: 5\n");
            printf("========================================\n");
            break;
        case 6:
            I = ((b - a) / 2.0) * (six[0] * f((b - a) / 2.0 * six[6] + (a + b) / 2.0) +
                             six[1] * f((b - a) / 2.0 * six[7] + (a + b) / 2.0) +
                             six[2] * f((b - a) / 2.0 * six[8] + (a + b) / 2.0) +
                             six[3] * f((b - a) / 2.0 * six[9] + (a + b) / 2.0) +
                             six[4] * f((b - a) / 2.0 * six[10] + (a + b) / 2.0) +
                             six[5] * f((b - a) / 2.0 * six[11] + (a + b) / 2.0));
            
            printf("\n========================================\n");
            printf("  RESULTADO - GAUSS-LEGENDRE (6 puntos)\n");
            printf("========================================\n");
            printf("Integral aproximada: %.10lf\n", I);
            printf("Intervalo: [%.6lf, %.6lf]\n", a, b);
            printf("Número de evaluaciones: 6\n");
            printf("========================================\n");
            break;
        default:
            printf("Número de puntos no soportado. Intente de nuevo.\n");
            break;
        }
    } while (puntos != 0);

    printf("\nPresione ENTER para continuar...");
    getchar();
    getchar();
}