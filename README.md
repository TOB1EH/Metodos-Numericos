# 🧮 Métodos Numéricos de Computadoras

Repositorio de implementaciones prácticas desarrolladas durante el cursado de la materia **Métodos Numéricos de Computadoras** de la carrera de **Ingeniería en Informática**.

## 📚 Descripción

Este repositorio contiene el desarrollo completo de los métodos numéricos vistos durante el cursado, incluyendo implementaciones en C/C++, scripts de visualización en Python, y material de exámenes parciales y finales.

## 🗂️ Estructura del Proyecto

| Directorio | Contenido |
|------------|-----------|
| `Localizacion_Raices/` | Métodos para encontrar raíces de ecuaciones |
| `Sistema_Ecuaciones_Lineales/` | Resolución de sistemas lineales |
| `Ajuste_de_curvas/` | Interpolación y regresión |
| `Diferenciacion_numerica/` | Derivadas numéricas |
| `Integracion_numerica/` | Integración numérica |
| `EDOS/` | Ecuaciones Diferenciales Ordinarias |
| `graficador_funciones/` | Herramientas de visualización |


## 🔧 Métodos Implementados

### 1. Localización de Raíces
- **Métodos Cerrados:** Bisección, Falsa Posición
- **Métodos Abiertos:** Punto Fijo, Newton-Raphson, Secante

### 2. Sistemas de Ecuaciones Lineales
- **Método Directo:** Eliminación de Gauss con pivoteo
- **Métodos Iterativos:** Jacobi, Gauss-Seidel con relajación SOR

### 3. Ajuste de Curvas
- **Interpolación:** Lagrange, Splines Lineales, Splines Cúbicos
- **Regresión:** Mínimos Cuadrados (lineal, polinomial, múltiple)

### 4. Diferenciación Numérica
- Diferencias Finitas: Adelante, Atrás, Centradas
- Orden de error: O(h) y O(h²)

### 5. Integración Numérica
- Trapecio Simple y Compuesto
- Simpson 1/3 Simple y Compuesto
- Trapecio Modificado (con derivadas)

### 6. Ecuaciones Diferenciales Ordinarias (EDOs)
- **Método de Euler** (primer orden)
- **Runge-Kutta de 4to Orden (RK4)**
- Sistemas de EDOs y EDOs de orden superior

## ⚙️ Requisitos

- Compilador C/C++ (gcc/g++)
- Python 3.x + matplotlib (para gráficos)
- Gnuplot (opcional, para algunos scripts)

## 🚀 Compilación

Ejemplo para compilar un programa:

```bash
gcc programa.c -o programa -lm
./programa
```

## 📖 Uso

Cada directorio contiene su propio `README.md` con instrucciones específicas de uso, formatos de entrada y ejemplos.

## 👨‍💻 Autor

**Tobías Funes**  
Ingeniería en Informática  
2025

## 📄 Licencia

Proyecto educativo - Uso académico
