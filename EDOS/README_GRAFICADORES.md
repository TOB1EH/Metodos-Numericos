# 📊 Graficadores Universales - Guía Rápida

## 🚀 Inicio Rápido

### **1️⃣ Graficador de Método Único**
Analiza un método con análisis de error.

```bash
python3 graficar_metodo_unico.py
```

**Usa cuando**: Tienes la solución exacta y quieres ver el error de un método.

---

### **2️⃣ Graficador de Comparación Múltiple**
Compara varios métodos en una gráfica.

```bash
python3 graficar_comparacion_multiple.py
```

**Usa cuando**: Quieres comparar Euler vs Heun vs RK4, etc.

---

### **3️⃣ Graficador de Datos Puros**
Visualiza datos sin solución exacta.

```bash
python3 graficar_datos_puros.py
```

**Usa cuando**: No conoces la solución exacta, solo quieres ver los datos.

---

## 📝 Ejemplo Completo: RK4 Variante

```bash
python3 graficar_metodo_unico.py
```

**Ingresa**:
```
Archivo de datos: rk4_variante_resultados.txt
Nombre del método: RK4 Variante
Descripción de la EDO: dy/dx = y - x² + 1, y(0) = 0.5
¿Solución exacta? (s/n): s
Ingrese y(x) = x**2 + 2*x + 1 - 0.5*np.exp(x)
Puntos a destacar: 1.5, 2.0
```

**Resultado**: Gráfica con error y puntos destacados.

---

## 📚 Formatos de Archivo Aceptados

### Formato 1 (3 columnas):
```
0     0.0000       0.5000000000        
1     0.1000       0.6568571528
```

### Formato 2 (2 columnas):
```
0.000000	1.000000
0.100000	1.005013
```

**Ambos funcionan con todos los graficadores.**

---

## 🎨 Fórmulas Matemáticas (Sintaxis Python)

```python
# Polinomios
x**2 + 3*x + 1

# Exponenciales
np.exp(x)
np.exp(-x**2)

# Trigonométricas
np.sin(x)
np.cos(2*x)

# Raíces
np.sqrt(x)
np.sqrt(1 + x**2)

# Combinaciones
x**2 + 2*x + 1 - 0.5*np.exp(x)
```

---

## ⚡ Comandos Útiles

```bash
# Ver archivos disponibles
ls *resultados*.txt

# Ejecutar directamente
./graficar_metodo_unico.py

# Ver la guía completa
cat GUIA_GRAFICADORES.md

# Ver este README
cat README_GRAFICADORES.md
```

---

## 📄 Documentación Completa

Lee **`GUIA_GRAFICADORES.md`** para:
- Explicación detallada de cada graficador
- Más ejemplos de uso
- Solución de problemas
- Tips y buenas prácticas

---

## ✅ Checklist Antes de Graficar

- [ ] Ejecutaste tu programa en C y generaste archivos `.txt`
- [ ] Los archivos tienen formato de 2 o 3 columnas
- [ ] Si vas a usar solución exacta, tienes la fórmula correcta
- [ ] Matplotlib y NumPy están instalados

---

## 🎯 Comparación de Graficadores

| Característica | Método Único | Comparación Múltiple | Datos Puros |
|----------------|--------------|----------------------|-------------|
| Solución exacta | ✅ Opcional | ✅ Opcional | ❌ No |
| Análisis de error | ✅ Sí | ❌ No | ❌ No |
| Múltiples métodos | ❌ No | ✅ Sí | ❌ No |
| Puntos destacados | ✅ Sí | ❌ No | ❌ No |
| **Mejor para** | Análisis detallado | Comparaciones | Visualización simple |

---

**¿Dudas?** Consulta `GUIA_GRAFICADORES.md`
