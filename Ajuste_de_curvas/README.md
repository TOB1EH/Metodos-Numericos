# Ajuste de Curvas: Interpolación y Regresión

Este directorio contiene programas en C para realizar ajuste de curvas utilizando dos enfoques principales: **interpolación** y **regresión**.

## Descripción de los Métodos

### 1. Interpolación (`interpolacion.c`)

La **interpolación** consiste en encontrar una función (generalmente un polinomio) que pasa **exactamente** por un conjunto de puntos de datos. Es útil cuando se confía plenamente en la precisión de los datos.

**Métodos implementados:**
- **Polinomio de Lagrange:** Construye un único polinomio de alto grado que pasa por todos los puntos.
- **Splines Lineales:** Conecta puntos consecutivos con segmentos de recta. Es simple y rápido, pero la curva resultante no es suave.
- **Splines Cúbicos:** Utiliza polinomios de tercer grado en cada subintervalo, asegurando que la curva resultante sea continua y suave (continuidad en la primera y segunda derivada). Evita las oscilaciones de los polinomios de alto grado (Fenómeno de Runge).

### 2. Regresión (`regresion.c`)

La **regresión** se utiliza cuando los datos tienen "ruido" o incertidumbre. En lugar de pasar exactamente por los puntos, se busca una función que se **aproxime** a la tendencia general de los datos, minimizando el error global.

**Método implementado:**
- **Regresión Polinomial por Mínimos Cuadrados:** Encuentra un polinomio de un grado especificado que minimiza la suma de los cuadrados de las distancias verticales entre los puntos de datos y la curva del polinomio.

## Requisitos

- Un compilador de C (como `gcc`).
- La librería `gauss_con_pivot.c` y `aditamentos_ui.c` (y sus cabeceras) deben estar en las rutas correctas como se especifica en los `#include`.

## Preparación de Datos

Ambos programas leen los puntos de datos de un archivo de texto llamado `nodos.txt`. El formato del archivo debe ser el siguiente, con cada punto `(x, y)` en una nueva línea:

```
x0 y0
x1 y1
x2 y2
...
xn yn
```

**Ejemplo de `nodos.txt`:**
```
-1.0 5.0
0.0 1.0
1.0 2.0
2.0 11.0
```

## Compilación

Abre una terminal en el directorio `Ajuste_de_curvas` y compila los programas.

**Para compilar `interpolacion.c`:**
```bash
gcc interpolacion.c ../libreria_de_aditamentos/aditamentos_ui.c gauss_con_pivot.c -o interpolacion.o -lm
```

**Para compilar `regresion.c`:**
```bash
gcc regresion.c ../libreria_de_aditamentos/aditamentos_ui.c gauss_con_pivot.c -o regresion.o -lm
```

## Ejecución

**Para ejecutar el programa de interpolación:**
```bash
./interpolacion.o
```

**Para ejecutar el programa de regresión:**
```bash
./regresion.o
```

Al ejecutar, se presentará un menú para que selecciones el método que deseas utilizar. Sigue las instrucciones en pantalla para ingresar los parámetros necesarios (como el grado del polinomio en la regresión o el punto a evaluar en la interpolación).

## Notas

- **Interpolación vs. Regresión:** Usa interpolación si tus datos son exactos y necesitas una curva que pase por ellos. Usa regresión si tus datos son experimentales o ruidosos y buscas la mejor tendencia.
- **Fenómeno de Runge:** Ten cuidado al usar interpolación con polinomios de alto grado y puntos equiespaciados, ya que pueden ocurrir grandes oscilaciones. En estos casos, los splines son una mejor alternativa.
- **Grado del Polinomio en Regresión:** El grado del polinomio debe ser menor que el número de puntos de datos. Un grado muy alto puede llevar a un sobreajuste (overfitting) del modelo.