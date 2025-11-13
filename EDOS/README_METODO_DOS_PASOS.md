# Método de Dos Pasos (Multipaso) para EDOs

## 📚 Descripción General

El **método de dos pasos** es un método **multipaso** para resolver ecuaciones diferenciales ordinarias (EDOs) de primer orden. A diferencia de los métodos de un solo paso (Euler, Heun, RK4), este método usa información de **dos puntos anteriores** para calcular el siguiente valor.

## 🔬 Formulación Matemática

### Fórmula General

```
y_{i+1} = y_i + h(2f(x_i, y_i) - f(x_{i-1}, y_{i-1}))
```

Donde:
- `y_i`: Valor aproximado en el punto `x_i`
- `h`: Tamaño del paso
- `f(x, y)`: Función derivada de la EDO `dy/dx = f(x, y)`
- `i ≥ 1`: Índice del paso (requiere dos valores previos)

### Características del Método

| Propiedad | Valor |
|-----------|-------|
| **Tipo** | Multipaso |
| **Orden** | O(h²) |
| **Valores previos necesarios** | 2 (y_{i-1}, y_i) |
| **Evaluaciones de f por paso** | 1 |
| **Auto-arrancable** | No (requiere método auxiliar) |

## 🎯 Problema Específico Implementado

### Ecuación Diferencial

```
dy/dx = (x·e^(x²))/y
```

Con condición inicial:
```
y(0) = 1
```

En el intervalo:
```
x ∈ [0, 1]
```

### Solución Analítica

Por **separación de variables**:

```
y dy = x·e^(x²) dx

∫y dy = ∫x·e^(x²) dx

y²/2 = (1/2)e^(x²) + C
```

Aplicando la condición inicial `y(0) = 1`:

```
1²/2 = (1/2)e^0 + C
1/2 = 1/2 + C
C = 0
```

Por lo tanto, la **solución exacta** es:

```
y(x) = e^(x²/2)
```

## 🚀 Algoritmo del Método

### Pseudocódigo

```
ALGORITMO Método_Dos_Pasos
    ENTRADA:
        f(x,y)      // Función derivada
        x_0, y_0    // Condición inicial
        x_f         // Punto final
        n           // Número de pasos
    
    PASO 1: Inicialización
        h ← (x_f - x_0) / n
        x[0] ← x_0
        y[0] ← y_0
    
    PASO 2: Método Arrancador (Euler para y_1)
        x[1] ← x[0] + h
        y[1] ← y[0] + h * f(x[0], y[0])
    
    PASO 3: Aplicar Método de Dos Pasos
        PARA i = 1 HASTA n-1 HACER
            x[i+1] ← x[0] + (i+1) * h
            f_actual ← f(x[i], y[i])
            f_anterior ← f(x[i-1], y[i-1])
            y[i+1] ← y[i] + h * (2*f_actual - f_anterior)
        FIN PARA
    
    PASO 4: Calcular Errores
        PARA i = 0 HASTA n HACER
            y_exacta[i] ← e^(x[i]²/2)
            error_abs[i] ← |y_exacta[i] - y[i]|
            error_rel[i] ← (error_abs[i] / y_exacta[i]) * 100
        FIN PARA
    
    SALIDA:
        Tabla con x[i], y[i], y_exacta[i], errores
        Error máximo, error promedio
```

## 💡 Explicación Paso a Paso

### 1. ¿Por qué necesitamos un método arrancador?

El método de dos pasos necesita **dos valores previos** para calcular el siguiente:

```
y_2 = y_1 + h(2f(x_1,y_1) - f(x_0,y_0))
     ↑           ↑             ↑
   nuevo      actual      anterior
```

Pero solo tenemos `y_0` como condición inicial, por eso:
- **i=0**: Usamos la condición inicial `y_0 = 1`
- **i=1**: Calculamos `y_1` con **Euler** (método de un paso)
- **i≥2**: Usamos el método de dos pasos

### 2. ¿Cómo funciona la fórmula?

La fórmula es una **combinación lineal** de las derivadas en dos puntos:

```
y_{i+1} = y_i + h(2f_i - f_{i-1})
                  ↑      ↑
              peso 2   peso -1
```

Esto da **mayor importancia** a la información más reciente (f_i) que a la anterior (f_{i-1}).

### 3. Ejemplo Numérico

Con `h = 0.1`:

**Paso 0** (Inicial):
```
x_0 = 0.0
y_0 = 1.0
f(0.0, 1.0) = (0·e^0)/1 = 0
```

**Paso 1** (Euler):
```
x_1 = 0.1
y_1 = 1.0 + 0.1 * 0 = 1.0
f(0.1, 1.0) = (0.1·e^0.01)/1.0 ≈ 0.1005
```

**Paso 2** (Dos pasos):
```
x_2 = 0.2
f_actual = f(0.1, 1.0) ≈ 0.1005
f_anterior = f(0.0, 1.0) = 0
y_2 = 1.0 + 0.1(2*0.1005 - 0) = 1.0201
```

## 📊 Ejemplo de Ejecución

### Entrada
```
Número de subintervalos: 10
```

### Salida (Parcial)
```
════════════════════════════════════════════════════════════════════════════
  i       x_i        y_aprox      y_exacta    Error Abs.   Error Rel.(%)
────────────────────────────────────────────────────────────────────────────
  0    0.000000    1.00000000    1.00000000     0.00e+00     0.000000
  1    0.100000    1.00000000    1.00501252     5.01e-03     0.498752
  2    0.200000    1.02020100    1.02020134     3.37e-07     0.000033
  3    0.300000    1.05090857    1.04602786     4.88e-03     0.466595
  ...
 10    1.000000    1.69056022    1.64872127     4.18e-02     2.537661
════════════════════════════════════════════════════════════════════════════

📊 ANÁLISIS DE ERROR:
Error máximo:   4.183895e-02
Error promedio: 1.680183e-02
Error final:    4.183895e-02 (2.537661%)
```

## 🔍 Análisis del Método

### ✅ Ventajas

1. **Eficiencia**: Solo necesita **1 evaluación** de f(x,y) por paso (después del arranque)
2. **Mejor que Euler**: Mayor precisión usando información histórica
3. **Implementación simple**: Fórmula directa, no requiere cálculos intermedios
4. **Memoria de la solución**: Usa el comportamiento previo de la función

### ❌ Desventajas

1. **No es auto-arrancable**: Necesita un método auxiliar (Euler) para y_1
2. **Menor precisión que RK4**: Orden O(h²) vs O(h⁴)
3. **Propagación de errores**: Errores en pasos anteriores afectan los siguientes
4. **Dos valores en memoria**: Debe almacenar y_{i-1} e y_i simultáneamente

### 📈 Comparación con otros métodos

| Método | Orden | Evaluaciones/paso | Auto-arrancable |
|--------|-------|-------------------|-----------------|
| Euler | O(h) | 1 | ✓ |
| Heun | O(h²) | 2 | ✓ |
| Punto Medio | O(h²) | 2 | ✓ |
| RK4 | O(h⁴) | 4 | ✓ |
| **Dos Pasos** | **O(h²)** | **1** | **✗** |

### 🎲 Cuándo usar este método

**Usar cuando:**
- Quieres mejor precisión que Euler con pocas evaluaciones
- Puedes tolerar un método arrancador
- Trabajas con problemas donde la historia es importante

**No usar cuando:**
- Necesitas máxima precisión (usa RK4)
- El problema es muy sensible a errores iniciales
- Necesitas un método auto-arrancable

## 📂 Archivos Generados

### `metodo_dospasos_resultados.txt`

Contiene:
- Tabla completa de resultados
- Valores aproximados y exactos
- Errores absolutos y relativos
- Estadísticas de error

Formato:
```
# MÉTODO DE DOS PASOS - RESULTADOS
# Problema: dy/dx = (x·e^(x²))/y, y(0)=1
# Solución exacta: y(x) = e^(x²/2)
# i	x_i	y_aprox	y_exacta	error_abs	error_rel(%)
0	0.0000000000	1.0000000000	1.0000000000	0.000000e+00	0.000000
1	0.1000000000	1.0000000000	1.0050125208	5.012521e-03	0.498752
...
```

## 🧪 Verificación de la Solución

Para verificar que `y(x) = e^(x²/2)` es correcta:

```
dy/dx = d/dx[e^(x²/2)]
      = e^(x²/2) · d/dx[x²/2]
      = e^(x²/2) · x
      = x·e^(x²/2)
```

Como `y = e^(x²/2)`, entonces `e^(x²/2) = y`, por lo tanto:

```
dy/dx = x·y/y = x·e^(x²)/y  ✓
```

Y verifica la condición inicial:
```
y(0) = e^(0²/2) = e^0 = 1  ✓
```

## 🔧 Uso del Programa

### Compilación
```bash
gcc -Wall -Wextra EDOS.c -o EDOS -lm
```

### Ejecución
```bash
./EDOS
```

Seleccionar opción `e) Método de Dos Pasos (Multipaso)`

### Entrada Requerida
- Número de subintervalos `n` (mínimo 2)

### Salida
- Tabla de resultados en pantalla
- Archivo `metodo_dospasos_resultados.txt`
- Análisis de error completo

## 📖 Referencias

- Burden, R. L., & Faires, J. D. (2011). *Numerical Analysis* (9th ed.). Brooks/Cole.
- Chapra, S. C., & Canale, R. P. (2015). *Numerical Methods for Engineers* (7th ed.). McGraw-Hill.
- Kreyszig, E. (2011). *Advanced Engineering Mathematics* (10th ed.). Wiley.

## 👨‍💻 Autor

Implementación para el curso de Métodos Numéricos  
Fecha: Noviembre 2024

---

**Nota**: Este método es parte de un programa más amplio que incluye otros métodos numéricos para EDOs (Euler, Heun, Punto Medio, RK4).
