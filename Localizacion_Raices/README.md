# Localizacion_Raices

Este directorio contiene un programa en C para localizar raíces de funciones usando métodos numéricos clásicos.

## Métodos implementados

- Bisección
- Falsa Posición
- Punto Fijo
- Newton-Raphson
- Secante

## Uso

1. Compile el programa con un compilador C estándar, por ejemplo:

   ```bash
   gcc Raices.c -o raices.o -lm
   ```

2. Ejecute el programa:

   ```bash
   ./raices.o
   ```

3. Seleccione el tipo de método (cerrados o abiertos) y luego el método específico.

4. Ingrese los parámetros solicitados (intervalos, valores iniciales, tolerancia).

5. El programa mostrará la raíz aproximada, el error y el número de iteraciones.

## Formato de entrada

No requiere archivos externos. Los datos se ingresan por consola.

## Notas

- Asegúrese de que la función y sus derivadas sean adecuadas para el método seleccionado.
- Para métodos cerrados, el intervalo debe contener un cambio de signo.
- Para métodos abiertos, seleccione valores iniciales adecuados para garantizar convergencia.

## Autor

Tobias Funes  
Versión 1.0
