# 📐 Diferenciación Numérica - Diferencias Finitas

## 📋 Descripción

Este programa implementa los **3 métodos principales** de diferenciación numérica usando **diferencias finitas**, cada uno con **2 modos de operación** (función o nodos de archivo).

## 🎯 Métodos Implementados

### **1. Diferencias Finitas HACIA ADELANTE (Forward Differences)**

**Fórmula:** `f'(x) ≈ [f(x+h) - f(x)] / h`

**Error:** O(h)

**Características:**
- Usa el punto actual y el punto siguiente
- Menos precisa que centrada
- Útil en extremo inicial de un intervalo

**Funciones:**
- `a)` **diferenciacionAdelanteFunc()** - Trabaja con función f(x)
- `b)` **diferenciacionAdelanteNodos()** - Trabaja con nodos de archivo

---

### **2. Diferencias Finitas HACIA ATRÁS (Backward Differences)**

**Fórmula:** `f'(x) ≈ [f(x) - f(x-h)] / h`

**Error:** O(h)

**Características:**
- Usa el punto actual y el punto anterior
- Menos precisa que centrada
- Útil en extremo final de un intervalo

**Funciones:**
- `c)` **diferenciacionAtrasFunc()** - Trabaja con función f(x)
- `d)` **diferenciacionAtrasNodos()** - Trabaja con nodos de archivo

---

### **3. Diferencias Finitas CENTRADAS (Central Differences)**

**Fórmula:** `f'(x) ≈ [f(x+h) - f(x-h)] / (2h)`

**Error:** O(h²) ← **MÁS PRECISA**

**Características:**
- Usa el punto anterior y el siguiente (simétrica)
- **Mayor precisión** que adelante/atrás
- Requiere tener ambos puntos vecinos disponibles

**Funciones:**
- `e)` **diferenciacionCentradaFunc()** - Trabaja con función f(x)
- `f)` **diferenciacionCentradaNodos()** - Trabaja con nodos de archivo

---

## 📊 Comparación de Métodos

| Método | Fórmula | Error | Puntos Requeridos | Precisión |
|--------|---------|-------|-------------------|-----------|
| **Adelante** | `[f(x+h) - f(x)] / h` | O(h) | x, x+h | ⭐⭐ |
| **Atrás** | `[f(x) - f(x-h)] / h` | O(h) | x-h, x | ⭐⭐ |
| **Centrada** | `[f(x+h) - f(x-h)] / (2h)` | O(h²) | x-h, x, x+h | ⭐⭐⭐⭐ |

---

## 🚀 Uso del Programa

### Compilar:
```bash
gcc -Wall -Wextra -o MetodosDiferencacion MetodosDiferencacion.c -lm
```

### Ejecutar:
```bash
./MetodosDiferencacion
```

### Menú del Programa:
```
╔════════════════════════════════════════════╗
║   DIFERENCIACIÓN NUMÉRICA                  ║
╠════════════════════════════════════════════╣
║  DIFERENCIAS HACIA ADELANTE                ║
║    a) Usando función                       ║
║    b) Usando nodos de archivo              ║
╠════════════════════════════════════════════╣
║  DIFERENCIAS HACIA ATRÁS                   ║
║    c) Usando función                       ║
║    d) Usando nodos de archivo              ║
╠════════════════════════════════════════════╣
║  DIFERENCIAS CENTRADAS                     ║
║    e) Usando función                       ║
║    f) Usando nodos de archivo              ║
╠════════════════════════════════════════════╣
║    g) Salir                                ║
╚════════════════════════════════════════════╝
```

---

## 📁 Formato del Archivo de Nodos

Archivo: `nodos.txt`

```
5
1.0 2.5
2.0 7.389056
3.0 14.154269
4.0 22.765368
5.0 32.744619
```

**Formato:**
- **Primera línea:** Número de nodos (n)
- **Líneas siguientes:** Pares de valores (x_i, y_i)

---

## 📄 Archivos de Salida

Cada método genera su propio archivo con los resultados:

### Usando Función:
- `derivadas_adelante_func.txt` - Diferencias hacia adelante
- `derivadas_atras_func.txt` - Diferencias hacia atrás
- `derivadas_centrada_func.txt` - Diferencias centradas

### Usando Nodos:
- `derivadas_adelante_nodos.txt` - Diferencias hacia adelante
- `derivadas_atras_nodos.txt` - Diferencias hacia atrás
- `derivadas_centrada_nodos.txt` - Diferencias centradas

---

## 🧪 Función de Prueba

**f(x) = 2x + ln(x) - sin(3x)**

**Derivada analítica:** `f'(x) = 2 + 1/x - 3·cos(3x)`

**Dominio:** x > 0 (por el logaritmo natural)

---

## 💡 Ejemplos de Uso

### Ejemplo 1: Diferencias Centradas con Función
```
Seleccione opción: e
Intervalo [a, b]:
  a = 1.0
  b = 5.0
Número de subintervalos n: 10
```

**Resultado:** Calcula f'(x) en 11 puntos usando diferencias centradas

### Ejemplo 2: Diferencias Adelante con Nodos
```
Seleccione opción: b
```

**Resultado:** Lee nodos de `nodos.txt` y calcula derivadas usando diferencias hacia adelante

---

## 📐 Teoría Matemática

### Desarrollo de Taylor

Las fórmulas de diferencias finitas se derivan de las series de Taylor:

**Hacia adelante:**
```
f(x+h) = f(x) + h·f'(x) + O(h²)
→ f'(x) ≈ [f(x+h) - f(x)] / h
```

**Hacia atrás:**
```
f(x-h) = f(x) - h·f'(x) + O(h²)
→ f'(x) ≈ [f(x) - f(x-h)] / h
```

**Centrada:**
```
f(x+h) - f(x-h) = 2h·f'(x) + O(h³)
→ f'(x) ≈ [f(x+h) - f(x-h)] / (2h)
```

### Análisis de Error

- **Adelante/Atrás:** Error proporcional a **h** (primer orden)
- **Centrada:** Error proporcional a **h²** (segundo orden) ← **Mejor**

Por eso, **al dividir h entre 2**:
- Adelante/Atrás: Error se reduce a la **mitad**
- Centrada: Error se reduce a la **cuarta parte**

---

## 🎯 Cuándo Usar Cada Método

| Situación | Método Recomendado |
|-----------|-------------------|
| **Máxima precisión** | Centrada (O(h²)) |
| **Solo punto inicial** | Adelante |
| **Solo punto final** | Atrás |
| **Datos de sensores** | Nodos de archivo |
| **Función analítica** | Función |
| **Datos irregulares** | Nodos (maneja h variable) |

---

## ⚙️ Características Técnicas

### Manejo de Memoria
- ✅ Asignación dinámica con `malloc()`
- ✅ Verificación de memoria (`NULL` check)
- ✅ Liberación automática con `free()`

### Manejo de Archivos
- ✅ Lectura robusta de nodos
- ✅ Escritura con encabezados informativos
- ✅ Formato compatible con Excel/Python

### Formato de Salida
- ✅ Tablas formateadas en pantalla
- ✅ Archivos de texto tabulados
- ✅ Comentarios descriptivos en archivos

---

## 🔍 Validación de Resultados

Para validar, compara con la derivada analítica:

**Para f(x) = 2x + ln(x) - sin(3x):**

**f'(x) = 2 + 1/x - 3·cos(3x)**

**Ejemplo en x = 2:**
- f'(2) analítica ≈ **2.5 + 1/2 - 3·cos(6)** ≈ **3.482...**

Compara este valor con los resultados numéricos para verificar precisión.

---

## 📚 Referencias

- **Burden & Faires**: "Análisis Numérico"
- **Chapra & Canale**: "Métodos Numéricos para Ingenieros"
- **Press et al.**: "Numerical Recipes"

---

## ✅ Checklist de Implementación

- [x] 3 métodos de diferenciación (Adelante, Atrás, Centrada)
- [x] 2 modos por método (Función y Nodos)
- [x] Total: 6 funciones implementadas
- [x] Guardado automático en archivos
- [x] Manejo robusto de memoria
- [x] Interfaz de usuario clara
- [x] Documentación completa en código
- [x] Archivo de prueba `nodos.txt`
- [x] README con teoría y ejemplos

---

**Autor:** [Tu nombre]  
**Fecha:** Noviembre 2024  
**Curso:** Métodos Numéricos
