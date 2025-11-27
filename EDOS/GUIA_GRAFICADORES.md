# 📊 GUÍA DE GRAFICADORES UNIVERSALES

**Sistema de Métodos Numéricos - Versión 2.0**

---

## 📋 ÍNDICE

1. [Resumen de Graficadores](#resumen)
2. [Graficador 1: Método Único](#graficador-1)
3. [Graficador 2: Comparación Múltiple](#graficador-2)
4. [Graficador 3: Datos Puros](#graficador-3)
5. [Formatos de Archivo Soportados](#formatos)
6. [Ejemplos de Uso](#ejemplos)
7. [Solución de Problemas](#problemas)

---

## 🎯 RESUMEN DE GRAFICADORES {#resumen}

| Graficador | Archivo | Cuándo Usar |
|------------|---------|-------------|
| **Método Único** | `graficar_metodo_unico.py` | Analizar un método vs solución exacta |
| **Comparación Múltiple** | `graficar_comparacion_multiple.py` | Comparar varios métodos simultáneamente |
| **Datos Puros** | `graficar_datos_puros.py` | Visualizar datos sin solución exacta |

---

## 📈 GRAFICADOR 1: MÉTODO ÚNICO {#graficador-1}

### **Propósito**
Compara UN método numérico contra la solución analítica exacta.

### **Cuándo Usar**
- ✅ Tienes la solución exacta de la EDO
- ✅ Quieres analizar el error de un método específico
- ✅ Necesitas destacar puntos de interés (ej: y(1.5), y(2.0))

### **Cómo Ejecutar**

```bash
python3 graficar_metodo_unico.py
```

### **Flujo Interactivo**

1. **Archivo de datos**: Ingresa el nombre del archivo (ej: `rk4_variante_resultados.txt`)
2. **Nombre del método**: Ej: "RK4 Variante", "Euler", "Heun"
3. **Descripción de la EDO**: Ej: "dy/dx = y - x² + 1, y(0) = 0.5"
4. **Solución exacta**: 
   - Opción `s`: Ingresar fórmula
   - Opción `n`: Solo graficar método numérico
5. **Puntos de interés** (opcional): Ej: `1.5, 2.0`

### **Ejemplo de Fórmulas Exactas**

```python
# Ejemplo 1: Polinomio con exponencial
x**2 + 2*x + 1 - 0.5*np.exp(x)

# Ejemplo 2: Gaussiana
np.exp(-x**2)

# Ejemplo 3: Función trigonométrica
np.sin(x) * np.exp(-x)

# Ejemplo 4: Raíz cuadrada
np.sqrt(1 + x**2)
```

### **Salidas Generadas**

1. **Gráfica PNG**: `grafica_<nombre_metodo>.png`
   - Subplot 1: Comparación método vs exacta
   - Subplot 2: Análisis de error absoluto (si hay solución exacta)

2. **Consola**: Tabla de comparación con errores

---

## 📊 GRAFICADOR 2: COMPARACIÓN MÚLTIPLE {#graficador-2}

### **Propósito**
Grafica varios métodos numéricos en una sola gráfica para comparación visual.

### **Cuándo Usar**
- ✅ Comparar precisión de diferentes métodos
- ✅ Evaluar comportamiento relativo (cuál converge más rápido)
- ✅ Presentaciones donde necesitas ver todos los métodos juntos

### **Cómo Ejecutar**

```bash
python3 graficar_comparacion_multiple.py
```

### **Flujo Interactivo**

1. **Método 1**:
   - Archivo de datos: `euler_resultados.txt`
   - Nombre del método: `Euler`

2. **Método 2**:
   - Archivo de datos: `heun_resultados.txt`
   - Nombre del método: `Heun`

3. **Método N**:
   - *(Presiona Enter vacío para terminar)*

4. **Descripción de la EDO**: Ej: "dy/dx = y - x² + 1, y(0) = 0.5"

5. **Solución exacta** (opcional): `s` o `n`

### **Características**

- Soporta hasta **8 métodos simultáneos** con colores/marcadores diferentes
- Cada método tiene su propio estilo visual
- Leyenda automática con todos los métodos
- Solución exacta en negro (si se proporciona)

### **Salidas Generadas**

1. **Gráfica PNG**: `grafica_comparacion_metodos.png`
2. **Consola**: Resumen de métodos graficados exitosamente

---

## 📉 GRAFICADOR 3: DATOS PUROS {#graficador-3}

### **Propósito**
Grafica datos numéricos cuando NO conoces la solución exacta.

### **Cuándo Usar**
- ✅ No tienes la solución analítica
- ✅ Solo quieres visualizar los resultados
- ✅ Verificar que los datos se ven razonables

### **Cómo Ejecutar**

```bash
python3 graficar_datos_puros.py
```

### **Flujo Interactivo**

1. **Archivo de datos**: Ej: `metodo_dospasos_resultados.txt`
2. **Título de la gráfica**: Ej: "Método de Dos Pasos"
3. **Etiqueta de los datos**: Ej: "Solución Numérica"
4. **¿Mostrar puntos?**: `s` (con marcadores) o `n` (solo línea)
5. **Color**: blue, red, green, orange, purple, cyan, black
6. **Marcador**: o (círculo), s (cuadrado), ^ (triángulo), D (diamante)

### **Salidas Generadas**

1. **Gráfica PNG**: `grafica_datos_puros.png`
2. **Consola**: Tabla con primeros y últimos 5 datos

---

## 📄 FORMATOS DE ARCHIVO SOPORTADOS {#formatos}

Todos los graficadores aceptan estos formatos:

### **Formato A: 3 Columnas (con índice)**

```
0     0.0000       0.5000000000        
1     0.1000       0.6568571528        
2     0.2000       0.8279423727
```

**Estructura**: `índice  x  y`

### **Formato B: 2 Columnas (sin índice)**

```
0.000000	1.000000
0.100000	1.005013
0.200000	1.020201
```

**Estructura**: `x  y`

### **Separadores Aceptados**
- Espacios múltiples
- Tabulaciones
- Combinación de ambos

### **Líneas Ignoradas**
- Líneas vacías
- Líneas que empiezan con `#` (comentarios)
- Encabezados de texto

---

## 💡 EJEMPLOS DE USO {#ejemplos}

### **Ejemplo 1: Analizar RK4 Variante con Error**

```bash
python3 graficar_metodo_unico.py
```

**Entrada**:
```
Archivo de datos: rk4_variante_resultados.txt
Nombre del método: RK4 Variante
Descripción de la EDO: dy/dx = y - x² + 1, y(0) = 0.5
Solución exacta (s/n): s
Ingrese y(x) = x**2 + 2*x + 1 - 0.5*np.exp(x)
Puntos a destacar: 1.5, 2.0
```

**Resultado**: Gráfica con error absoluto y puntos destacados.

---

### **Ejemplo 2: Comparar Euler, Heun y RK4**

```bash
python3 graficar_comparacion_multiple.py
```

**Entrada**:
```
Método 1:
  Archivo: euler_resultados.txt
  Nombre: Euler

Método 2:
  Archivo: heun_resultados.txt
  Nombre: Heun

Método 3:
  Archivo: rk4_variante_resultados.txt
  Nombre: RK4 Variante

Método 4:
  (Enter vacío)

Descripción: dy/dx = y - x² + 1, y(0) = 0.5
Solución exacta: s
y(x) = x**2 + 2*x + 1 - 0.5*np.exp(x)
```

**Resultado**: Gráfica con 3 métodos + solución exacta en negro.

---

### **Ejemplo 3: Visualizar Datos sin Solución Exacta**

```bash
python3 graficar_datos_puros.py
```

**Entrada**:
```
Archivo: metodo_experimental_resultados.txt
Título: Método Experimental
Etiqueta: Datos Numéricos
¿Mostrar puntos?: s
Color: purple
Marcador: D
```

**Resultado**: Gráfica púrpura con diamantes mostrando solo los datos.

---

## ⚠️ SOLUCIÓN DE PROBLEMAS {#problemas}

### **Problema 1: "No se encontraron datos"**

**Causa**: Formato de archivo no reconocido.

**Solución**:
1. Verifica que el archivo tenga números al inicio de cada línea
2. Asegúrate de usar formato de 2 o 3 columnas
3. Revisa que no haya líneas con texto mezclado con números

---

### **Problema 2: "Error en la fórmula"**

**Causa**: Sintaxis incorrecta en la solución exacta.

**Solución**:
- ✅ Usa `np.exp()` en lugar de `exp()`
- ✅ Usa `np.sqrt()` en lugar de `sqrt()`
- ✅ Usa `**` para potencias, no `^`
- ✅ Ejemplo correcto: `x**2 + np.sin(x)`

---

### **Problema 3: "Gráfica no se muestra"**

**Causa**: Matplotlib no configurado correctamente.

**Solución**:
```bash
# Instalar matplotlib si falta
pip install matplotlib numpy

# O con apt (Ubuntu/Debian)
sudo apt install python3-matplotlib python3-numpy
```

---

### **Problema 4: "Archivo no encontrado"**

**Causa**: Ruta incorrecta o archivo no existe.

**Solución**:
1. Verifica que estés en el directorio correcto:
   ```bash
   ls *.txt
   ```
2. Usa rutas relativas o absolutas correctas
3. Ejecuta primero tu programa en C para generar los archivos

---

## 🎓 TIPS Y BUENAS PRÁCTICAS

### **1. Nombres Descriptivos**
- ✅ Usa nombres claros: "Euler", "RK4", "Heun"
- ❌ Evita: "metodo1", "test", "datos"

### **2. Solución Exacta**
- Verifica la fórmula en papel primero
- Prueba con valores conocidos (ej: x=0)
- Si no estás seguro, mejor no incluirla

### **3. Colores para Comparación**
- Usa colores contrastantes
- Rojo para métodos simples (Euler)
- Azul para métodos avanzados (RK4)
- Negro siempre para solución exacta

### **4. Puntos de Interés**
- Elige puntos donde conozcas valores esperados
- Espacíalos uniformemente en el dominio
- No más de 3-4 puntos para no saturar

---

## 📞 SOPORTE

¿Problemas no resueltos? Verifica:

1. ✅ Python 3.6 o superior instalado
2. ✅ Matplotlib y NumPy instalados
3. ✅ Permisos de escritura en el directorio
4. ✅ Formato de archivo correcto
5. ✅ Sintaxis correcta en fórmulas

---

**Última actualización**: Noviembre 2025  
**Versión**: 2.0 - Universal  
**Licencia**: Uso educativo
