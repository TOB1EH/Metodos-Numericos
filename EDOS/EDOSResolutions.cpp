/*
 * ============================================================================
 * PROGRAMA: Resolución de EDOs con Múltiples Métodos Numéricos
 * ============================================================================
 * 
 * DESCRIPCIÓN GENERAL:
 *   Este programa implementa varios métodos numéricos para resolver ecuaciones
 *   diferenciales ordinarias (EDOs) de primer orden de la forma:
 *   
 *       dy/dx = f(x, y)
 *   
 *   con una condición inicial y(x₀) = y₀
 * 
 * MÉTODOS IMPLEMENTADOS:
 *   1. Método de Euler (Orden 1)
 *   2. Método de Heun/Euler Mejorado (Orden 2)
 *   3. Método del Punto Medio (Orden 2)
 *   4. Método de Runge-Kutta de 4to Orden (Orden 4)
 *   5. Método de Dos Pasos (Multipaso)
 * 
 * PROBLEMA ESPECÍFICO:
 *   dy/dx = (x·e^(x²))/y
 *   Solución exacta: y(x) = √(e^(x²)) = e^(x²/2)
 * 
 * CARACTERÍSTICAS:
 *   - Calcula errores exactos comparando con solución analítica
 *   - Estima errores de truncamiento local
 *   - Calcula factores de convergencia
 *   - Exporta resultados a archivos de texto
 *   - Genera gráficas mediante scripts de Python
 * 
 * AUTOR: [Tu nombre]
 * FECHA: Noviembre 2025
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Tamaño máximo de subintervalos permitidos
#define MAX_SIZE 200

/* ============================================================================
   PROTOTIPOS DE FUNCIONES
   ============================================================================ */

/**
 * @brief Guarda los valores calculados en un archivo de texto
 * @param X Array con los valores de x
 * @param Y Array con los valores calculados de y
 * @param n Número de subintervalos
 */
void save_in_txt(double X[], double Y[], int n);

/**
 * @brief Guarda la solución exacta en un archivo de texto
 * @param X Array con los valores de x
 * @param Yfunc Puntero a función que calcula la solución exacta y(x)
 * @param n Número de subintervalos
 */ 
void save_in_txt2(double X[], double (*Yfunc)(double), int n);

/**
 * @brief Define la función f(x, y) de la EDO: dy/dx = f(x, y)
 * 
 * PROBLEMA ESPECÍFICO: f(x,y) = (x·e^(x²))/y
 * 
 * @param x Variable independiente
 * @param y Variable dependiente
 * @return Valor de f en el punto (x, y)
 */
double f(double x, double y);

/**
 * @brief Verifica la existencia de archivos de resultados y ejecuta gráficas
 * 
 * Esta función comprueba si existen todos los archivos de resultados necesarios
 * y, en caso afirmativo, ejecuta el script de Python para generar gráficas.
 */ 
void check_and_run_graph();

/**
 * @brief Define la solución exacta y(x) para comparación
 * @param x Variable independiente
 * @return El valor exacto de y en x
 */
double y(double x);

/**
 * @brief Define la derivada total de f con respecto a x
 * @param x Variable independiente
 * @param y Variable dependiente
 * @return El valor de la derivada total en (x, y)
 */
double fprima(double x, double y);  // Nueva función: derivada total de f respecto a x

/**
 * @brief Define la tercera derivada de y con respecto a x
 * @param x Variable independiente
 * @param y Variable dependiente
 * @return El valor de la tercera derivada en (x, y)
 */
double y3prima(double x, double y);

/**
 * @brief Realiza un solo paso del método Runge-Kutta de 4to orden
 * @param x Valor actual de x
 * @param y Valor actual de y
 * @param step Tamaño del paso
 * @return El nuevo valor de y después del paso RK4
 */ 
double rk4_step(double x, double y, double step);

/**
 * @brief Calcula el factor de convergencia para el método de Euler
 * @param n Número de subintervalos
 * @param h Tamaño del paso
 * @param X0 Valor inicial de x
 * @param Xf Valor final de x
 * @param Y0 Valor inicial de y
 */ 
void convergence_factor_euler(int n, double h, double X0, double Xf, double Y0);

/**
 * @brief Calcula el factor de convergencia para el método de Heun
 * @param n Número de subintervalos
 * @param h Tamaño del paso
 * @param X0 Valor inicial de x
 * @param Xf Valor final de x
 * @param Y0 Valor inicial de y
 */
void convergence_factor_heun(int n, double h, double X0, double Xf, double Y0);

/**
 * @brief Calcula el factor de convergencia para el método del Punto Medio
 * @param n Número de subintervalos
 * @param h Tamaño del paso
 * @param X0 Valor inicial de x
 * @param Xf Valor final de x
 * @param Y0 Valor inicial de y
 */
void convergence_factor_midpoint(int n, double h, double X0, double Xf, double Y0);


/**
 * @brief Calcula el factor de convergencia para el método de Runge-Kutta de 4to orden
 * @param n Número de subintervalos
 * @param h Tamaño del paso
 * @param X0 Valor inicial de x
 * @param Xf Valor final de x
 * @param Y0 Valor inicial de y
 *  */ 
void convergence_factor_rk4(int n, double h, double X0, double Xf, double Y0);


/* 
    El método de Heun tiene las siguientes mejoras sobre Euler:
    --> Mayor precisión: Errores consistentemente más pequeños ✓
    --> Comportamiento esperado: Patrón de error consistente con método de segundo orden ✓
    --> Convergencia mejorada: Los errores decrecen más rápidamente ✓

    Método de Euler:
        ✅ Extrema simplicidad: Fácil de implementar y entender
        ✅ Bajo costo computacional: 1 evaluación de f(x,y) por paso
        ✅ Bueno para prototipado: Rápido para pruebas iniciales
        ✅ Análisis de error simple: Comportamiento predecible

        ❌ Baja precisión: Error de truncamiento local O(h²)
        ❌ Inestabilidad: Puede divergir fácilmente con h grande
        ❌ Requiere h pequeño para lograr precisión aceptable
        ❌ Pobre para funciones con alta curvatura

        Casos Ideales:
        --> Problemas simples con comportamiento casi lineal
        --> Cuando la precisión no es crítica
        --> Demostraciones didácticas y conceptuales

    Método de Heun:
        ✅ Buena precisión: Error O(h³) vs O(h²) de Euler
        ✅ Más estable: Menos propenso a divergencia
        ✅ Balanceado: Buen equilibrio precisión-costo
        ✅ Fácil de implementar: Solo dos evaluaciones de f(x,y)

        ❌ Costo moderado: 2 evaluaciones vs 1 de Euler
        ❌ No óptimo: No es el más preciso de los métodos de 2° orden
        ❌ Dependencia del predictor: Si el predictor es malo, afecta el corrector

        Casos Ideales:
        --> Aplicaciones generales que requieren mejor precisión que Euler
        --> Cuando se balancea precisión y simplicidad
        --> Problemas con comportamiento moderadamente no lineal

    Método del Punto Medio:
        ✅ Alta precisión: Generalmente más preciso que Heun para el mismo orden
        ✅ Mejor para simetrías: Excelente para problemas con comportamiento simétrico
        ✅ Estabilidad mejorada: Menos sensible a cambios abruptos
        ✅ Interpretación geométrica clara: Pendiente en el punto medio

        ❌ Costo similar a Heun: 2 evaluaciones de f(x,y)
        ❌ Puede subestimar/sobreestimar: Dependiendo de la concavidad
        ❌ Implementación ligeramente más compleja: Cálculo del punto medio


        Casos Ideales:
        --> Problemas con comportamiento suave y simétrico
        --> Cuando se busca máxima precisión con métodos de segundo orden
        --> Sistemas físicos con comportamiento armónico

*/

int main(int argc, char const *argv[]) {
    // Variables generales
    double X0, Xf, Y0, h;
    // Variables para el método de Neumann
    double Xp, Yp;
    int n;
    double X[MAX_SIZE + 1], Y[MAX_SIZE + 1];

    // Número para seleccionar si hacer cálculo de factor de convergencia
    int conv_choice;

    // Errores a calcular
    double exact_error, local_trunc_error;
    // Variables para cálculo de factor de convergencia
    double X1[MAX_SIZE + 1], X2[MAX_SIZE + 1], X3[MAX_SIZE + 1];
    double Y1[MAX_SIZE + 1], Y2[MAX_SIZE + 1], Y3[MAX_SIZE + 1];
    double Q[MAX_SIZE + 1];

    printf("Ingrese X0 y Xf:\n");
    scanf("%lf %lf", &X0, &Xf);
    printf("Ingrese el dato inicial Y0 = Y(X0):\n");
    scanf("%lf", &Y0);

    printf("¿Desea ingresar el número de subintervalos (n) o el tamaño de paso (h)?\n");
    printf("1. Quiero ingresar n\n");
    printf("2. Quiero ingresar h\n");
    int choice;
    scanf("%d", &choice);
    if(choice == 1) {
        printf("Ingrese el número de subintervalos n (entero):\n");
        scanf("%d", &n);
        // Calcular distancia entre puntos
        h = (Xf - X0) / n;
    } else if(choice == 2) {
        printf("Ingrese el tamaño de paso h:\n");
        scanf("%lf", &h);
        n = (int)((Xf - X0) / h);
        if (n > MAX_SIZE) {
            printf("Error: el número de subintervalos excede el tamaño máximo (%d).\n", MAX_SIZE);
            return 1;
        }
    }

    
    // Calcular la solución
    X[0] = X0;
    Y[0] = Y0;

    printf("Ingrese el método a usar: 4. Runge-Kutta de orden 4 5. Método del Ejercicio\n");
    scanf("%d", &choice);

    switch(choice) {
        case 4:
            printf("¿Desea calcular el factor de convergencia para Runge Kutta de orden 4? (1.Sí 2.No)\n");
            scanf("%d", &conv_choice);

            double k1, k2, k3, k4;
            // Runge-Kutta de orden 4
            for(int i = 0; i <= n-1; i++) {
                k1 = f(X[i], Y[i]);
                k2 = f(X[i] + h/2.0, Y[i] + (h/2.0) * k1);
                k3 = f(X[i] + h/2.0, Y[i] + (h/2.0) * k2);
                k4 = f(X[i] + h, Y[i] + h * k3);
                Y[i+1] = Y[i] + (h/6.0) * (k1 + 2*k2 + 2*k3 + k4);
                X[i+1] = X[i] + h;
            }

            printf("\n%-10s %-15s %-15s %-15s %-15s %-15s\n", 
                   "i", "X[i]", "Y Exacta", "Y RK4", "Error Exacto", "Error Trunc. Local");
            printf("-------------------------------------------------------------------------------------------\n");
            
            for(int i = 0; i <= n; i++) {
                exact_error = fabs(y(X[i]) - Y[i]);

                // Aquí estimamos el error de truncamiento local (ETL) para RK4
                // Estimación práctica del ETL (por paso) comparando h vs h/2
                double Y_full = Y[i];  // valor actual con paso h
                double Y_half;
                if (i == 0) {
                    Y_half = Y[0]; // no aplicable para el primer punto
                    local_trunc_error = 0.0;
                } else {
                    // Recalcular desde X[i-1], Y[i-1] con dos pasos de h/2
                    double mid = rk4_step(X[i-1], Y[i-1], h/2.0);
                    Y_half = rk4_step(X[i-1] + h/2.0, mid, h/2.0);

                    // Estimador del error de truncamiento local: (Y_half - Y_full) / (2^{p+1}-1) = /31
                    local_trunc_error = fabs((Y_half - Y_full) / 15.0);
                }

                printf("%-10d %-15lf %-15lf %-15lf %-15.2e %-15.2e\n", 
                       i, X[i], y(X[i]), Y[i], exact_error, local_trunc_error);

                // Guardar x[i] y Y[i] en runge_kutta_4_results.txt
                save_in_txt(X, Y, n);
                rename("results.txt", "runge_kutta_4_results.txt");
            }
            break;
        case 5:
            X[1] = X0 + h;
            Y[1] = Y[0] + h * f(X[0], Y[0]);

            for(int i = 1; i < n; i++) {
                X[i+1] = X0 + (i+1)*h;
                Y[i+1] = Y[i] + h * (2 * f(X[i], Y[i]) - f(X[i-1], Y[i-1]));
            }
            // Método de Euler
            /* for(int i = 1; i <= n; i++) {
                // Calcular el siguiente X incrementalmente para evitar sorpresas de redondeo
                X[i] = X[i-1] + h; // X[i] = X0 + i*h; equivalentemente
                // Método de Euler
                Y[i] = Y[i-1] + h * f(X[i-1], Y[i-1]); 
            } */
            
            for(int i = 0; i <= n; i++) {
                exact_error = fabs(y(X[i]) - Y[i]);
                printf("En X = %lf, Y Exacta = %lf, Y Euler = %lf, Error Exacto (e%d) = %lf\n", X[i], y(X[i]), Y[i], i, exact_error);
            }
        
            printf("\n%-10s %-15s %-15s %-15s %-15s %-15s\n", 
                   "i", "X[i]", "Y Exacta", "Y Euler", "Error Exacto", "Error Trunc. Local");
            printf("-------------------------------------------------------------------------------------------\n");
        
            // El método de Euler aproxima la curva mediante segmentos de línea recta tangentes a cada punto.
            // Si estos segmentos se encuentran por encima de la curva → Euler sobreestima.
            // Si se encuentran por debajo → Euler subestima.
            // Si local_trunc_error < 0 => El valor calculado por Euler es menor que el exacto. Euler subestima.
            // Si local_trunc_error > 0 => El valor calculado por Euler es mayor que el exacto. Euler sobreestima.
            for(int i = 0; i <= n; i++) {
                exact_error = fabs(y(X[i]) - Y[i]);
                local_trunc_error = (h * h / 2.0) * fprima(X[i], y(X[i])); 
                printf("%-10d %-15lf %-15lf %-15lf %-15lf %-15lf\n", 
                       i, X[i], y(X[i]), Y[i], exact_error, local_trunc_error);
            }
            // Guardar x[i] y Y[i] en method_of_exercise_results.txt
            save_in_txt(X, Y, n);
            // save_in_txt2(X, y, n);
            rename("results.txt", "method_of_exercise_results.txt");
            // rename("results2.txt", "exact_results.txt");
            break;
    }

    
    // Imprimir resultados
    printf("X[i]\t\tY[i]\n");
    // Imprimir todos los puntos calculados incluyendo el último
    for(int i = 0; i <= n; i++) {
        printf("%lf\t%lf\n", X[i], Y[i]);
    }

    // Guardar x[i] y Y[i] en results.txt
    // save_in_txt(X, Y, n);
    
    // Finalmente, imprimimos el archivo results.txt en una gráfica usando Python para visualizar los resultados
    // system("python3 graph_points.py");
    /* if (system("test -f graph_points.py") == 0) {
        system("python3 graph_points.py");
    } else {
        printf("⚠️  Advertencia: 'graph_points.py' no encontrado. Saltando generación de gráfica.\n");
    } */

    check_and_run_graph();

    return 0;
}

double f(double x, double y) {
    return ((x * exp(x*x)) / y);
}

double y(double x) {
    return sqrt(exp(x*x));
}

/**
 * @brief Verifica si existen los cuatro archivos de resultados
 *        y ejecuta graph_points.py si todos están presentes.
 */
void check_and_run_graph() {
    int all_exist = 1;
    const char* filenames[] = {
        "method_of_exercise_results.txt",
        //"exact_results.txt",
        "runge_kutta_4_results.txt"
    };

    printf("\nVerificando archivos de resultados...\n");

    // Verificando existencia de cada archivo
    for (int i = 0; i < 2; i++) {
        FILE *file = fopen(filenames[i], "r");
        if (file == NULL) {
            all_exist = 0;
            printf("Archivo faltante: %s\n", filenames[i]);
        } else {
            fclose(file);
            printf("Archivo encontrado: %s\n", filenames[i]);
        }
    }

    // Si todos los archivos existen, ejecutar el script de Python
    if (all_exist) {
        printf("\n✅ Todos los archivos están presentes. Ejecutando gráfica combinada...\n");
        if (system("test -f graph_points_all.py") == 0) {
            system("python3 graph_points_all.py");
        } else {
            printf("'graph_points_all.py' no encontrado. Saltando generación de gráfica.\n");
        }
    } else {
        printf("\nArchivos de resultados faltantes. Por favor ejecute los métodos faltantes antes de graficar.\n");
    }
}


/**
 * @brief Tercera derivada de y(x) = e^{-x^2}
 * @details Calculada simbólicamente como y''' = 4xy(3 - 2x^2)
 */
double y3prima(double x, double y) {
    return 4 * x * y * (3 - 2 * x * x);
}

double fprima(double x, double y) {
    double fx = -2 * y;
    double fy = -2 * x;
    return fx + fy * f(x, y);
}


double rk4_step(double x, double y, double step) {
    double k1 = f(x, y);
    double k2 = f(x + step/2.0, y + (step/2.0) * k1);
    double k3 = f(x + step/2.0, y + (step/2.0) * k2);
    double k4 = f(x + step,     y + step * k3);
    return y + (step/6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);
}


void save_in_txt(double X[], double Y[], int n) {
    FILE *archivo = fopen("results.txt", "w");
    if (archivo == NULL) {
        printf("Error: No se puede crear el archivo.\n");
        exit(1);
    }

    for (int i = 0; i <= n; i++) {
        fprintf(archivo, "%lf\t%lf\n", X[i], Y[i]);
    }

    fclose(archivo);
}

// y(X[i])
void save_in_txt2(double X[], double (*Yfunc)(double), int n) {
    FILE *archivo = fopen("results2.txt", "w");
    if (archivo == NULL) {
        printf("Error: No se puede crear el archivo.\n");
        exit(1);
    }

    for (int i = 0; i <= n; i++) {
        fprintf(archivo, "%lf\t%lf\n", X[i], Yfunc(X[i]));
    }

    fclose(archivo);
}


void convergence_factor_euler(int n1, double h1, double X0, double Xf, double Y0) {
    double h2 = h1 / 2.0;
    double h3 = h1 / 4.0;

    double Yh[MAX_SIZE + 1], Yh2[MAX_SIZE*2 + 1], Yh4[MAX_SIZE*4 + 1];
    double Xh[MAX_SIZE + 1], Xh2[MAX_SIZE*2 + 1], Xh4[MAX_SIZE*4 + 1];
    double Q[MAX_SIZE + 1];

    // Inicializaciones
    Xh[0] = Xh2[0] = Xh4[0] = X0;
    Yh[0] = Yh2[0] = Yh4[0] = Y0;

    // Euler con paso h
    for (int i = 0; i < n1; i++) {
        Xh[i+1] = Xh[i] + h1;
        Yh[i+1] = Yh[i] + h1 * f(Xh[i], Yh[i]);
    }

    // Euler con paso h/2
    for (int i = 0; i < 2*n1; i++) {
        Xh2[i+1] = Xh2[i] + h2;
        Yh2[i+1] = Yh2[i] + h2 * f(Xh2[i], Yh2[i]);
    }

    // Euler con paso h/4
    for (int i = 0; i < 4*n1; i++) {
        Xh4[i+1] = Xh4[i] + h3;
        Yh4[i+1] = Yh4[i] + h3 * f(Xh4[i], Yh4[i]);
    }

    printf("\n%-10s %-15s %-15s\n", "i", "x_i", "Q_i");
    printf("------------------------------------------\n");

    // Cálculo del factor de convergencia en los mismos puntos X
    // Nota: Q[0] no está definido porque en el punto inicial no hay error
    // (es la condición inicial exacta), por lo que comenzamos desde i=1
    Q[0] = 0.0;  // No definido, se establece a 0 por convención
    
    for (int i = 1; i <= n1; i++) {
        int idx2 = 2*i;   // posición equivalente para h/2
        int idx4 = 4*i;   // posición equivalente para h/4
        double num = fabs(Yh[i] - Yh2[idx2]);
        double den = fabs(Yh2[idx2] - Yh4[idx4]);

        if (den > 1e-12) {
            Q[i] = log(num / den) / log(2.0);
            printf("%-10d %-15lf %-15lf\n", i, Xh[i], Q[i]);
        } else {
            Q[i] = 0.0;
            printf("%-10d %-15lf %-15s\n", i, Xh[i], "N/A (sin error)");
        }
    }

    save_in_txt(Xh, Q, n1);
    rename("results.txt", "convergence_euler.txt");

    if (system("test -f graph_convergence.py") == 0) {
        system("python3 graph_convergence.py");
    } else {
        printf("⚠️  Advertencia: 'graph_convergence.py' no encontrado. Saltando generación de gráfica.\n");
    }
}

void convergence_factor_heun(int n1, double h1, double X0, double Xf, double Y0) {
    double h2 = h1 / 2.0;
    double h3 = h1 / 4.0;

    double Yh[MAX_SIZE + 1], Yh2[MAX_SIZE*2 + 1], Yh4[MAX_SIZE*4 + 1];
    double Xh[MAX_SIZE + 1], Xh2[MAX_SIZE*2 + 1], Xh4[MAX_SIZE*4 + 1];
    double Q[MAX_SIZE + 1];

    Xh[0] = Xh2[0] = Xh4[0] = X0;
    Yh[0] = Yh2[0] = Yh4[0] = Y0;

    // Heun con paso h
    for (int i = 0; i < n1; i++) {
        double predictor = Yh[i] + h1 * f(Xh[i], Yh[i]);
        Yh[i+1] = Yh[i] + (h1/2.0)*(f(Xh[i], Yh[i]) + f(Xh[i]+h1, predictor));
        Xh[i+1] = Xh[i] + h1;
    }

    // Heun con paso h/2
    for (int i = 0; i < 2*n1; i++) {
        double predictor = Yh2[i] + h2 * f(Xh2[i], Yh2[i]);
        Yh2[i+1] = Yh2[i] + (h2/2.0)*(f(Xh2[i], Yh2[i]) + f(Xh2[i]+h2, predictor));
        Xh2[i+1] = Xh2[i] + h2;
    }

    // Heun con paso h/4
    for (int i = 0; i < 4*n1; i++) {
        double predictor = Yh4[i] + h3 * f(Xh4[i], Yh4[i]);
        Yh4[i+1] = Yh4[i] + (h3/2.0)*(f(Xh4[i], Yh4[i]) + f(Xh4[i]+h3, predictor));
        Xh4[i+1] = Xh4[i] + h3;
    }

    printf("\n%-10s %-15s %-15s\n", "i", "x_i", "Q_i");
    printf("------------------------------------------\n");
    Q[0] = 0.0;

    for (int i = 1; i <= n1; i++) {
        int idx2 = 2*i;
        int idx4 = 4*i;
        double num = fabs(Yh[i] - Yh2[idx2]);
        double den = fabs(Yh2[idx2] - Yh4[idx4]);

        if (den > 1e-12) {
            Q[i] = log(num / den) / log(2.0);
            printf("%-10d %-15lf %-15lf\n", i, Xh[i], Q[i]);
        } else {
            Q[i] = 0.0;
            printf("%-10d %-15lf %-15s\n", i, Xh[i], "N/A");
        }
    }

    save_in_txt(Xh, Q, n1);
    rename("results.txt", "convergence_heun.txt");

    if (system("test -f graph_convergence.py") == 0) {
        system("python3 graph_convergence.py");
    }
}

void convergence_factor_midpoint(int n1, double h1, double X0, double Xf, double Y0) {
    double h2 = h1 / 2.0;
    double h3 = h1 / 4.0;

    double Yh[MAX_SIZE + 1], Yh2[MAX_SIZE*2 + 1], Yh4[MAX_SIZE*4 + 1];
    double Xh[MAX_SIZE + 1], Xh2[MAX_SIZE*2 + 1], Xh4[MAX_SIZE*4 + 1];
    double Q[MAX_SIZE + 1];

    Xh[0] = Xh2[0] = Xh4[0] = X0;
    Yh[0] = Yh2[0] = Yh4[0] = Y0;

    // Midpoint paso h
    for (int i = 0; i < n1; i++) {
        double k1 = f(Xh[i], Yh[i]);
        double k2 = f(Xh[i] + h1/2.0, Yh[i] + (h1/2.0)*k1);
        Yh[i+1] = Yh[i] + h1*k2;
        Xh[i+1] = Xh[i] + h1;
    }

    // Midpoint paso h/2
    for (int i = 0; i < 2*n1; i++) {
        double k1 = f(Xh2[i], Yh2[i]);
        double k2 = f(Xh2[i] + h2/2.0, Yh2[i] + (h2/2.0)*k1);
        Yh2[i+1] = Yh2[i] + h2*k2;
        Xh2[i+1] = Xh2[i] + h2;
    }

    // Midpoint paso h/4
    for (int i = 0; i < 4*n1; i++) {
        double k1 = f(Xh4[i], Yh4[i]);
        double k2 = f(Xh4[i] + h3/2.0, Yh4[i] + (h3/2.0)*k1);
        Yh4[i+1] = Yh4[i] + h3*k2;
        Xh4[i+1] = Xh4[i] + h3;
    }

    printf("\n%-10s %-15s %-15s\n", "i", "x_i", "Q_i");
    printf("------------------------------------------\n");
    Q[0] = 0.0;

    for (int i = 1; i <= n1; i++) {
        int idx2 = 2*i;
        int idx4 = 4*i;
        double num = fabs(Yh[i] - Yh2[idx2]);
        double den = fabs(Yh2[idx2] - Yh4[idx4]);

        if (den > 1e-12) {
            Q[i] = log(num / den) / log(2.0);
            printf("%-10d %-15lf %-15lf\n", i, Xh[i], Q[i]);
        } else {
            Q[i] = 0.0;
            printf("%-10d %-15lf %-15s\n", i, Xh[i], "N/A");
        }
    }

    save_in_txt(Xh, Q, n1);
    rename("results.txt", "convergence_midpoint.txt");

    if (system("test -f graph_convergence.py") == 0) {
        system("python3 graph_convergence.py");
    }
}

void convergence_factor_rk4(int n1, double h1, double X0, double Xf, double Y0) {
    double h2 = h1 / 2.0;
    double h3 = h1 / 4.0;

    double Yh[MAX_SIZE + 1], Yh2[MAX_SIZE*2 + 1], Yh4[MAX_SIZE*4 + 1];
    double Xh[MAX_SIZE + 1], Xh2[MAX_SIZE*2 + 1], Xh4[MAX_SIZE*4 + 1];
    double Q[MAX_SIZE + 1];

    Xh[0] = Xh2[0] = Xh4[0] = X0;
    Yh[0] = Yh2[0] = Yh4[0] = Y0;

    // RK4 paso h
    for (int i = 0; i < n1; i++) {
        Yh[i+1] = rk4_step(Xh[i], Yh[i], h1);
        Xh[i+1] = Xh[i] + h1;
    }

    // RK4 paso h/2
    for (int i = 0; i < 2*n1; i++) {
        Yh2[i+1] = rk4_step(Xh2[i], Yh2[i], h2);
        Xh2[i+1] = Xh2[i] + h2;
    }

    // RK4 paso h/4
    for (int i = 0; i < 4*n1; i++) {
        Yh4[i+1] = rk4_step(Xh4[i], Yh4[i], h3);
        Xh4[i+1] = Xh4[i] + h3;
    }

    printf("\n%-10s %-15s %-15s\n", "i", "x_i", "Q_i");
    printf("------------------------------------------\n");
    Q[0] = 0.0;

    for (int i = 1; i <= n1; i++) {
        int idx2 = 2*i;
        int idx4 = 4*i;
        double num = fabs(Yh[i] - Yh2[idx2]);
        double den = fabs(Yh2[idx2] - Yh4[idx4]);

        if (den > 1e-12) {
            Q[i] = log(num / den) / log(2.0);
            printf("%-10d %-15lf %-15lf\n", i, Xh[i], Q[i]);
        } else {
            Q[i] = 0.0;
            printf("%-10d %-15lf %-15s\n", i, Xh[i], "N/A");
        }
    }

    save_in_txt(Xh, Q, n1);
    rename("results.txt", "convergence_rk4.txt");

    if (system("test -f graph_convergence.py") == 0) {
        system("python3 graph_convergence.py");
    }
}


