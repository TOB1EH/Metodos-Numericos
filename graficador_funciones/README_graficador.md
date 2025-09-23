# Graficador de Funciones para Localización de Raíces

Este programa en Python te permite graficar funciones matemáticas para ayudar en la localización visual de raíces y validación de resultados de métodos numéricos.

## Características

- ✅ **Ingreso de funciones usando notación matemática natural**
- ✅ **Gráficas interactivas con zoom y navegación**
- ✅ **Evaluación de funciones en puntos específicos**
- ✅ **Detección automática de intervalos con posibles raíces**
- ✅ **Marcado visual de raíces encontradas**
- ✅ **Soporte para funciones trigonométricas, logarítmicas, exponenciales**
- ✅ **Funciones de ejemplo para probar métodos numéricos**

## Instalación y Configuración

### 1. Instalar Python
Asegúrate de tener Python 3.7 o superior instalado en tu sistema.

### 2. Crear un entorno virtual (Recomendado)
Es una buena práctica usar entornos virtuales para evitar conflictos de dependencias:

```bash
# Crear entorno virtual
python3 -m venv graficador_funciones/venv

# Activar el entorno virtual
# En Windows:
graficador_funciones\venv\Scripts\activate
# En Linux/Mac:
source graficador_funciones/venv/bin/activate
```

### 3. Instalar dependencias
Con el entorno virtual activado, instala las bibliotecas necesarias:

```bash
pip install matplotlib numpy
```

### 4. Verificar instalación
```bash
python -c "import matplotlib, numpy; print('Dependencias instaladas correctamente')"
```

## Uso

### Ejecutar el programa
```bash
# Asegúrate de que el entorno virtual esté activado
python graficador_funciones_simple.py
```

### Funciones de ejemplo para probar

1. **f(x) = x² - 4**
   - Raíz: x = 2
   - Intervalo sugerido: [1, 3]
   - Método: Bisección, Falsa Posición

2. **f(x) = cos(x)**
   - Raíz: x ≈ 0.7391
   - Intervalo sugerido: [0, 1]
   - Método: Punto Fijo (g(x) = cos(x))

3. **f(x) = x³ - x - 2**
   - Raíz: x ≈ 1.5214
   - Intervalo sugerido: [1, 2]
   - Método: Newton-Raphson

4. **f(x) = e^(-x) - x**
   - Raíz: x ≈ 0.5671
   - Intervalo sugerido: [0, 1]
   - Método: Secante

5. **f(x) = log(x) + exp(sin(x)) - x**
   - Raíz: buscar en [1, 5]
   - Método: Cualquiera

### Sintaxis para funciones

El programa soporta la mayoría de las funciones matemáticas estándar:

- **Operadores**: `+`, `-`, `*`, `/`, `**` (potencia)
- **Funciones trigonométricas**: `sin(x)`, `cos(x)`, `tan(x)`
- **Funciones exponenciales y logarítmicas**: `exp(x)`, `log(x)`, `log10(x)`
- **Otras funciones**: `sqrt(x)`, `abs(x)`, `pi`, `e`

#### Ejemplos de funciones válidas:
- `x**2 - 4`
- `sin(x)`
- `cos(x) + 1`
- `exp(-x) - x`
- `x**3 - x - 2`
- `log(x) + exp(sin(x)) - x`

### Sintaxis para funciones

El programa soporta la mayoría de las funciones matemáticas estándar:

- **Operadores**: `+`, `-`, `*`, `/`, `**` (potencia)
- **Funciones trigonométricas**: `sin(x)`, `cos(x)`, `tan(x)`
- **Funciones exponenciales y logarítmicas**: `exp(x)`, `log(x)`, `log10(x)`
- **Otras funciones**: `sqrt(x)`, `abs(x)`, `pi`, `e`

#### Ejemplos de funciones válidas:
- `x**2 - 4`
- `sin(x)`
- `cos(x) + 1`
- `exp(-x) - x`
- `x**3 - x - 2`
- `log(x) + exp(sin(x)) - x`

### Verificación visual
- Las raíces detectadas automáticamente se marcan con puntos rojos
- Los intervalos donde la función cruza el eje x se destacan
- Puedes hacer zoom en la gráfica para ver detalles

## Consejos de uso

1. **Para funciones polinómicas**: Usa un rango amplio para ver todas las raíces
2. **Para funciones trigonométricas**: Limita el rango para evitar demasiadas raíces
3. **Para funciones con asíntotas**: El programa maneja puntos inválidos automáticamente

## Solución de problemas

### Error: "No module named 'matplotlib'"
Solución: Instala las dependencias con `pip install matplotlib numpy`

### Error al evaluar función
- Verifica la sintaxis de la función
- Asegúrate de usar `**` para potencias (no `^`)
- Funciones como `log` requieren argumentos positivos

### Gráfica no se muestra
- En algunos entornos, la gráfica puede no aparecer automáticamente
- Asegúrate de que tu sistema soporte gráficos (X11 en Linux)

## Solución de problemas comunes

### Error: "No module named 'matplotlib'"
**Solución**: Asegúrate de que el entorno virtual esté activado e instala las dependencias:
```bash
source graficador_funciones/venv/bin/activate  # Linux/Mac
pip install matplotlib numpy
```

### Error al evaluar función
- Verifica la sintaxis de la función
- Asegúrate de usar `**` para potencias (no `^`)
- Funciones como `log` requieren argumentos positivos

### Gráfica no se muestra
- En algunos entornos, la gráfica puede no aparecer automáticamente
- Asegúrate de que tu sistema soporte gráficos (X11 en Linux)
- Las gráficas se guardan como archivos PNG en el directorio actual

### KeyboardInterrupt duplicado
- El programa maneja correctamente la interrupción con Ctrl+C
- No hay manejo duplicado de esta excepción

## Integración con métodos numéricos

Este graficador complementa perfectamente los métodos numéricos:

1. **Bisección**: Encuentra intervalos válidos antes de aplicar el método
2. **Newton-Raphson**: Visualiza la función para entender el comportamiento
3. **Secante**: Identifica puntos de inicio apropiados
4. **Punto Fijo**: Verifica la convergencia de las iteraciones

## Licencia

Este programa es de uso libre para fines educativos y de investigación.
