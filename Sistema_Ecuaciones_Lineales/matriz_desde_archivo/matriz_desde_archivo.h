#ifndef MATRIZ_DESDE_ARCHIVO_H
#define MATRIZ_DESDE_ARCHIVO_H

/**
 * @brief Lee un sistema de ecuaciones lineales desde un archivo.
 * @param filename Nombre del archivo.
 * @param A Puntero a la matriz A (salida).
 * @param b Puntero al vector b (salida).
 * @param n Puntero al tamaño del sistema (salida).
 * @return 0 si todo salió bien, 1 si hubo error.
 *
 * El archivo debe tener n filas, cada una con n coeficientes y el término independiente.
 */
int leerSistemaDesdeArchivo(const char *filename, double ***A, double **b, int *n);

/**
 * @brief Libera la memoria reservada para la matriz y el vector.
 * @param A Matriz de coeficientes.
 * @param b Vector de términos independientes.
 * @param n Tamaño del sistema.
 */
void liberarMemoria(double **A, double *b, int n);

#endif // MATRIZ_DESDE_ARCHIVO_H