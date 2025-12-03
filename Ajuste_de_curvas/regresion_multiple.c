/**
 * @file regresion_multiple.c
 * @brief Regresión lineal múltiple generalizada con funciones base definidas por el usuario.
 * @author Tobias Funes
 * @version 2.0
 * 
 * =================================================================================
 * TEORÍA: REGRESIÓN LINEAL MÚLTIPLE GENERALIZADA
 * =================================================================================
 * Este programa resuelve el problema de ajustar datos a una combinación lineal de
 * funciones base arbitrarias:
 * 
 *   y = a₁·φ₁(x) + a₂·φ₂(x) + ... + aₘ·φₘ(x)
 * 
 * donde φᵢ(x) son funciones base definidas por el usuario, y aᵢ son los coeficientes
 * a determinar.
 * 
 * EJEMPLOS DE FUNCIONES BASE:
 * - Polinomiales: φ₁(x)=1, φ₂(x)=x, φ₃(x)=x², ...
 * - Exponenciales: φ₁(x)=exp(x²), φ₂(x)=1
 * - Trigonométricas: φ₁(x)=sin(x), φ₂(x)=cos(x), φ₃(x)=1
 * - Mixtas: φ₁(x)=exp(x), φ₂(x)=x², φ₃(x)=x, φ₄(x)=1
 * 
 * SISTEMA DE ECUACIONES NORMALES:
 * Para m funciones base, el sistema matricial es:
 * 
 *   [Σ(φ₁·φ₁)  Σ(φ₁·φ₂)  ...  Σ(φ₁·φₘ)] [a₁]   [Σ(φ₁·y)]
 *   [Σ(φ₂·φ₁)  Σ(φ₂·φ₂)  ...  Σ(φ₂·φₘ)] [a₂] = [Σ(φ₂·y)]
 *   [   ...       ...     ...     ...  ] [...] = [  ...  ]
 *   [Σ(φₘ·φ₁)  Σ(φₘ·φ₂)  ...  Σ(φₘ·φₘ)] [aₘ]   [Σ(φₘ·y)]
 * 
 * Este sistema se resuelve usando eliminación Gaussiana con pivoteo parcial.
 * =================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_FUNCIONES 10
#define MAX_NOMBRE 100
#define ARCHIVO_DATOS "nodos.txt"

// Tipo de función: puntero a función que toma double y retorna double
typedef double (*FuncionBase)(double);

// Estructura para almacenar información de cada función base
typedef struct {
    FuncionBase funcion;
    char nombre[MAX_NOMBRE];
    char descripcion[MAX_NOMBRE];
} InfoFuncion;

/**
 * @brief Lee puntos (x, y) desde un archivo.
 */
void leerDatos(const char *filename, double **x, double **y, int *n);

/**
 * @brief Resuelve un sistema de ecuaciones lineales usando Gauss con pivoteo parcial.
 */
void resolverSistema(double **A, double *b, int n, double *solucion);

/**
 * @brief Construye el sistema de ecuaciones normales para regresión múltiple.
 */
void construirSistemaNormal(double *x_datos, double *y_datos, int n_datos,
                           InfoFuncion *funciones, int n_func,
                           double **A, double *b);

/**
 * @brief Muestra el menú de funciones base predefinidas.
 */
int menuFuncionesPredefinidas(InfoFuncion *funciones);

// ============================================================================
// FUNCIONES BASE PREDEFINIDAS (el usuario puede agregar más)
// ============================================================================

// Funciones constantes y polinomiales
double func_constante(double x) { return 1.0; }
double func_x(double x) { return x; }
double func_x2(double x) { return x * x; }
double func_x3(double x) { return x * x * x; }

// Funciones exponenciales
double func_exp_x(double x) { return exp(x); }
double func_exp_x2(double x) { return exp(x * x); }
double func_exp_menos_x(double x) { return exp(-x); }

// Funciones trigonométricas
double func_sin_x(double x) { return sin(x); }
double func_cos_x(double x) { return cos(x); }
double func_tan_x(double x) { return tan(x); }

// Funciones logarítmicas
double func_ln_x(double x) { return (x > 0) ? log(x) : 0.0; }
double func_log10_x(double x) { return (x > 0) ? log10(x) : 0.0; }

// Funciones raíces
double func_sqrt_x(double x) { return (x >= 0) ? sqrt(x) : 0.0; }

// Funciones inversas
double func_1_x(double x) { return (fabs(x) > 1e-10) ? 1.0/x : 0.0; }

int main(void)
{
    double *x_datos = NULL;
    double *y_datos = NULL;
    int n_datos = 0;
    InfoFuncion funciones[MAX_FUNCIONES];
    int n_func = 0;
    
    system("clear");
    printf("\n=============================================================\n");
    printf("  REGRESIÓN LINEAL MÚLTIPLE GENERALIZADA\n");
    printf("=============================================================\n");
    printf("Este programa ajusta datos a una combinación lineal de\n");
    printf("funciones base: y = a₁·φ₁(x) + a₂·φ₂(x) + ... + aₘ·φₘ(x)\n");
    printf("=============================================================\n");
    
    // PASO 1: Leer datos del archivo
    printf("\nLeyendo datos desde '%s'...\n", ARCHIVO_DATOS);
    leerDatos(ARCHIVO_DATOS, &x_datos, &y_datos, &n_datos);
    
    printf("\n--- PASO 1: Datos originales ---\n");
    printf("------------------------------------------------\n");
    printf("    i       x_i        y_i\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n_datos; i++) {
        printf("   %2d   %8.4f   %10.4f\n", i+1, x_datos[i], y_datos[i]);
    }
    printf("------------------------------------------------\n");
    
    // PASO 2: Seleccionar funciones base
    n_func = menuFuncionesPredefinidas(funciones);
    
    if (n_func == 0) {
        printf("[ERROR] Debe seleccionar al menos una función base.\n");
        free(x_datos);
        free(y_datos);
        return 1;
    }
    
    printf("\n--- PASO 2: Funciones base seleccionadas ---\n");
    printf("------------------------------------------------\n");
    printf("f(x) = ");
    for (int i = 0; i < n_func; i++) {
        if (i > 0) printf(" + ");
        printf("a%d·%s", i+1, funciones[i].nombre);
    }
    printf("\n------------------------------------------------\n");
    
    // PASO 3: Calcular valores de las funciones base para cada punto
    printf("\n--- PASO 3: Evaluación de funciones base ---\n");
    printf("------------------------------------------------\n");
    printf("    i       x_i    ");
    for (int j = 0; j < n_func; j++) {
        printf("%10s ", funciones[j].nombre);
    }
    printf("\n------------------------------------------------\n");
    
    double **phi = (double **)malloc(n_datos * sizeof(double *));
    for (int i = 0; i < n_datos; i++) {
        phi[i] = (double *)malloc(n_func * sizeof(double));
        printf("   %2d   %8.4f  ", i+1, x_datos[i]);
        for (int j = 0; j < n_func; j++) {
            phi[i][j] = funciones[j].funcion(x_datos[i]);
            printf("%10.4f ", phi[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------\n");
    
    // PASO 4: Construir y resolver el sistema de ecuaciones normales
    printf("\n--- PASO 4: Sistema de ecuaciones normales ---\n");
    
    double **A = (double **)malloc(n_func * sizeof(double *));
    for (int i = 0; i < n_func; i++) {
        A[i] = (double *)malloc(n_func * sizeof(double));
    }
    double *b_vec = (double *)malloc(n_func * sizeof(double));
    double *coeficientes = (double *)malloc(n_func * sizeof(double));
    
    construirSistemaNormal(x_datos, y_datos, n_datos, funciones, n_func, A, b_vec);
    
    printf("\nMatriz A (%dx%d):\n", n_func, n_func);
    for (int i = 0; i < n_func; i++) {
        printf("  [");
        for (int j = 0; j < n_func; j++) {
            printf("%12.4f ", A[i][j]);
        }
        printf("]\n");
    }
    
    printf("\nVector b:\n  [");
    for (int i = 0; i < n_func; i++) {
        printf("%12.4f ", b_vec[i]);
    }
    printf("]\n");
    
    // Resolver el sistema
    printf("\nResolviendo sistema con Gauss-Jordan...\n");
    resolverSistema(A, b_vec, n_func, coeficientes);
    
    // PASO 5: Mostrar resultados
    printf("\n=============================================================\n");
    printf("  RESULTADO FINAL\n");
    printf("=============================================================\n");
    printf("Coeficientes obtenidos:\n");
    for (int i = 0; i < n_func; i++) {
        printf("  a%d = %12.6f  (para %s)\n", i+1, coeficientes[i], funciones[i].descripcion);
    }
    
    printf("\nEcuación ajustada:\n  f(x) = ");
    for (int i = 0; i < n_func; i++) {
        if (i > 0 && coeficientes[i] >= 0) printf(" + ");
        else if (i > 0) printf(" ");
        printf("%.6f·%s", coeficientes[i], funciones[i].nombre);
    }
    printf("\n=============================================================\n");
    
    // PASO 6: Verificación del ajuste
    printf("\n--- PASO 5: Verificación del ajuste ---\n");
    printf("----------------------------------------------------------------\n");
    printf("    x_i        y_i      y_pred    residuo    |error%%|\n");
    printf("----------------------------------------------------------------\n");
    
    double suma_residuos2 = 0.0;
    double suma_total2 = 0.0;
    double suma_y = 0.0;
    for (int i = 0; i < n_datos; i++) {
        suma_y += y_datos[i];
    }
    double y_media = suma_y / n_datos;
    
    for (int i = 0; i < n_datos; i++) {
        // Calcular y predicho
        double y_pred = 0.0;
        for (int j = 0; j < n_func; j++) {
            y_pred += coeficientes[j] * phi[i][j];
        }
        
        double residuo = y_datos[i] - y_pred;
        double error_pct = (fabs(y_datos[i]) > 1e-10) ? fabs(residuo / y_datos[i] * 100.0) : 0.0;
        
        suma_residuos2 += residuo * residuo;
        suma_total2 += (y_datos[i] - y_media) * (y_datos[i] - y_media);
        
        printf(" %8.4f  %10.4f %10.4f %10.4f   %8.3f%%\n", 
               x_datos[i], y_datos[i], y_pred, residuo, error_pct);
    }
    printf("----------------------------------------------------------------\n");
    
    // Calcular métricas
    double r2 = (suma_total2 > 1e-10) ? 1.0 - (suma_residuos2 / suma_total2) : 0.0;
    double syx = sqrt(suma_residuos2 / (n_datos - n_func));
    
    printf("\nMétricas de bondad de ajuste:\n");
    printf("  R² = %.6f (%.2f%%)\n", r2, r2 * 100);
    printf("  Sy/x = %.6f\n", syx);
    printf("  Sr = %.6f (suma de cuadrados de residuos)\n", suma_residuos2);
    printf("\n* R² cercano a 1 indica un excelente ajuste\n");
    printf("----------------------------------------------------------------\n");
    
    // Liberar memoria
    for (int i = 0; i < n_datos; i++) {
        free(phi[i]);
    }
    free(phi);
    for (int i = 0; i < n_func; i++) {
        free(A[i]);
    }
    free(A);
    free(b_vec);
    free(coeficientes);
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

void construirSistemaNormal(double *x_datos, double *y_datos, int n_datos,
                           InfoFuncion *funciones, int n_func,
                           double **A, double *b)
{
    // Inicializar matrices
    for (int i = 0; i < n_func; i++) {
        b[i] = 0.0;
        for (int j = 0; j < n_func; j++) {
            A[i][j] = 0.0;
        }
    }
    
    // Construir el sistema: A[i][j] = Σ(φᵢ(xₖ)·φⱼ(xₖ)), b[i] = Σ(φᵢ(xₖ)·yₖ)
    for (int k = 0; k < n_datos; k++) {
        double x = x_datos[k];
        double y = y_datos[k];
        
        for (int i = 0; i < n_func; i++) {
            double phi_i = funciones[i].funcion(x);
            b[i] += phi_i * y;
            
            for (int j = 0; j < n_func; j++) {
                double phi_j = funciones[j].funcion(x);
                A[i][j] += phi_i * phi_j;
            }
        }
    }
}

void resolverSistema(double **A, double *b, int n, double *solucion)
{
    // Gauss-Jordan con pivoteo parcial
    for (int i = 0; i < n; i++) {
        // Encontrar el pivote
        int max_fila = i;
        for (int k = i + 1; k < n; k++) {
            if (fabs(A[k][i]) > fabs(A[max_fila][i])) {
                max_fila = k;
            }
        }
        
        // Intercambiar filas si es necesario
        if (max_fila != i) {
            double *temp = A[i];
            A[i] = A[max_fila];
            A[max_fila] = temp;
            
            double temp_b = b[i];
            b[i] = b[max_fila];
            b[max_fila] = temp_b;
        }
        
        // Verificar que el pivote no sea cero
        if (fabs(A[i][i]) < 1e-12) {
            printf("[ADVERTENCIA] Pivote muy pequeño en fila %d: %.2e\n", i, A[i][i]);
        }
        
        // Normalizar la fila del pivote
        double pivote = A[i][i];
        for (int j = i; j < n; j++) {
            A[i][j] /= pivote;
        }
        b[i] /= pivote;
        
        // Eliminar en las demás filas
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = A[k][i];
                for (int j = i; j < n; j++) {
                    A[k][j] -= factor * A[i][j];
                }
                b[k] -= factor * b[i];
            }
        }
    }
    
    // Copiar solución
    for (int i = 0; i < n; i++) {
        solucion[i] = b[i];
    }
}

int menuFuncionesPredefinidas(InfoFuncion *funciones)
{
    int opcion = 0;
    int n_func = 0;
    int indices_seleccionados[MAX_FUNCIONES];
    
    // Definir las funciones disponibles
    InfoFuncion disponibles[] = {
        {func_constante, "1", "1 (constante)"},
        {func_x, "x", "x"},
        {func_x2, "x²", "x²"},
        {func_x3, "x³", "x³"},
        {func_exp_x, "exp(x)", "exp(x)"},
        {func_exp_x2, "exp(x²)", "exp(x²)"},
        {func_exp_menos_x, "exp(-x)", "exp(-x)"},
        {func_sin_x, "sin(x)", "sin(x)"},
        {func_cos_x, "cos(x)", "cos(x)"},
        {func_ln_x, "ln(x)", "ln(x)"},
        {func_sqrt_x, "√x", "√x"},
        {func_1_x, "1/x", "1/x"}
    };
    int n_disponibles = 12;
    
    printf("\n=============================================================\n");
    printf("  MENÚ: SELECCIÓN DE FUNCIONES BASE\n");
    printf("=============================================================\n");
    printf("Seleccione las funciones base φᵢ(x) que desea usar:\n\n");
    
    for (int i = 0; i < n_disponibles; i++) {
        printf("  %2d. %s\n", i+1, disponibles[i].descripcion);
    }
    printf("\n  0. Terminar selección\n");
    printf("=============================================================\n");
    
    while (n_func < MAX_FUNCIONES) {
        printf("\nFunciones seleccionadas hasta ahora: %d\n", n_func);
        if (n_func > 0) {
            printf("  → ");
            for (int i = 0; i < n_func; i++) {
                if (i > 0) printf(", ");
                printf("%s", funciones[i].nombre);
            }
            printf("\n");
        }
        
        printf("Seleccione función (0 para terminar): ");
        scanf("%d", &opcion);
        while (getchar() != '\n');
        
        if (opcion == 0) break;
        
        if (opcion < 1 || opcion > n_disponibles) {
            printf("[ERROR] Opción inválida. Seleccione entre 1 y %d.\n", n_disponibles);
            continue;
        }
        
        // Verificar si ya fue seleccionada
        int ya_seleccionada = 0;
        for (int i = 0; i < n_func; i++) {
            if (indices_seleccionados[i] == opcion - 1) {
                ya_seleccionada = 1;
                break;
            }
        }
        
        if (ya_seleccionada) {
            printf("[ADVERTENCIA] Esta función ya fue seleccionada.\n");
            continue;
        }
        
        // Agregar la función
        indices_seleccionados[n_func] = opcion - 1;
        funciones[n_func] = disponibles[opcion - 1];
        n_func++;
        printf("✓ Agregada: %s\n", disponibles[opcion - 1].descripcion);
    }
    
    return n_func;
}
