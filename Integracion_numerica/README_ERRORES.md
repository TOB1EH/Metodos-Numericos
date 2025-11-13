# 📊 Cálculo de Errores en Integración Numérica

## 🎯 Nueva Funcionalidad Agregada

Todos los métodos de integración numérica ahora incluyen **análisis de error opcional** que permite:

- ✅ Calcular el **error absoluto**
- ✅ Calcular el **error porcentual**
- ✅ Comparar el valor aproximado vs el valor exacto
- ✅ Validar la precisión de cada método

---

## 📐 Fórmulas Utilizadas

### Error Absoluto
```
Error Absoluto = |Valor Exacto - Valor Aproximado|
```

### Error Porcentual
```
Error Porcentual = |Error Absoluto / Valor Exacto| × 100%
```

---

## 🚀 Métodos Actualizados

### 1. **Regla del Trapecio**
   - Trapecio Simple (función)
   - Trapecio Compuesto (función)
   - Trapecio Compuesto (tabla de datos con splines cúbicas)

### 2. **Regla de Simpson 1/3**
   - Simpson Compuesto (función)
   - Simpson Compuesto (tabla de datos con splines cúbicas)

### 3. **Cuadratura de Gauss-Legendre**
   - 2 puntos (exacto para polinomios grado ≤ 3)
   - 3 puntos (exacto para polinomios grado ≤ 5)
   - 4 puntos (exacto para polinomios grado ≤ 7)
   - 5 puntos (exacto para polinomios grado ≤ 9)
   - 6 puntos (exacto para polinomios grado ≤ 11)

---

## 📝 Ejemplo de Uso

### Caso: Trapecio Compuesto con Tabla de Datos

```
========================================
  RESULTADO DE LA INTEGRACIÓN
========================================
Integral aproximada: 0.8484642646
Número de subintervalos: 4
========================================

¿Desea calcular el error? (s/n): s
Ingrese el valor exacto de la integral: 0.8484

--- ANÁLISIS DE ERROR ---
Valor exacto:        0.8484000000
Valor aproximado:    0.8484642646
Error absoluto:      0.0000642646
Error porcentual:    0.007574%
-------------------------
```

### Interpretación

- **Error absoluto pequeño** (0.0000642646) → Muy buena aproximación
- **Error porcentual < 0.01%** → Excelente precisión
- El método es confiable para este problema

---

## 🧮 Cómo Obtener el Valor Exacto

### Opción 1: Cálculo Analítico
Si la función tiene antiderivada conocida:

```
f(x) = 2x + ln(x) - sin(3x)

F(x) = x² + x·ln(x) - x + (1/3)cos(3x) + C

∫[1,2] f(x)dx = F(2) - F(1)
```

### Opción 2: Método de Referencia
- Usar Gauss-Legendre con muchos puntos (6 puntos)
- Usar software matemático (MATLAB, Python, WolframAlpha)
- Calculadoras científicas avanzadas

### Opción 3: Comparación Experimental
- Calcular con varios métodos
- Usar el valor más preciso como referencia
- Comparar errores entre métodos

---

## 📊 Tabla de Comparación de Métodos

| Método | Orden de Error | Mejor Para |
|--------|----------------|------------|
| **Trapecio Simple** | O(h²) | Funciones lineales, pruebas rápidas |
| **Trapecio Compuesto** | O(h²) | Funciones suaves, muchos puntos |
| **Simpson 1/3** | O(h⁴) | Funciones cuadráticas/cúbicas |
| **Gauss-Legendre 2** | O(h⁶) | Pocos puntos, alta precisión |
| **Gauss-Legendre 3** | O(h⁸) | Balance precisión/costo |
| **Gauss-Legendre 4+** | O(h¹⁰⁺) | Máxima precisión requerida |

---

## 💡 Consejos para Análisis de Error

### ✅ Buenas Prácticas

1. **Siempre calcule el error** cuando tenga el valor exacto disponible
2. **Compare múltiples métodos** para el mismo problema
3. **Documente los resultados** para análisis posterior
4. **Verifique convergencia** aumentando puntos de integración

### ⚠️ Advertencias

- Si **error porcentual > 1%** → Considere:
  * Aumentar número de subintervalos
  * Usar método de mayor orden
  * Verificar valor exacto ingresado

- Si **error es 0** → Posible:
  * Función es polinomio exacto para el método
  * Coincidencia numérica fortuita
  * Error de redondeo enmascara diferencias

---

## 🔍 Ejemplo Completo: Problema de Tarea

### Problema
Calcular: ∫[1,2] f'(x)dx donde f'(x) viene de datos tabulados.

### Datos (nodos.txt)
```
5
1.000000  0.561290
1.250000  0.658968
1.500000  0.851628
1.750000  1.037961
2.000000  1.129312
```

### Proceso

1. **Método usado**: Trapecio Compuesto con Splines Cúbicas
2. **Puntos equiespaciados**: 5
3. **Resultado**: 0.8484642646

4. **Valor exacto**: Calculado analíticamente = 0.8484
   
5. **Análisis**:
   ```
   Error absoluto:   0.0000642646
   Error porcentual: 0.007574%
   ```

6. **Conclusión**: ✅ Excelente precisión, error < 0.01%

---

## 📚 Referencias

- **Burden & Faires**: "Numerical Analysis" (Capítulo 4: Integración Numérica)
- **Chapra & Canale**: "Numerical Methods for Engineers" (Capítulos 21-22)
- **Wikipedia**: [Numerical Integration](https://en.wikipedia.org/wiki/Numerical_integration)

---

## 🔧 Compilación y Ejecución

```bash
# Compilar
gcc -Wall -Wextra MetodosIntegracion.c -o MetodosIntegracion -lm

# Ejecutar
./MetodosIntegracion
```

---

## 👨‍💻 Autor

Implementado para el curso de **Métodos Numéricos**

**Fecha**: Noviembre 2025

**Versión**: 2.0 (con análisis de error)
