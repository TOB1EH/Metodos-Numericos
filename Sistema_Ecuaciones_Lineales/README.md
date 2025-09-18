# Sistema_Ecuaciones_Lineales

Este directorio contiene un programa en C para resolver sistemas de ecuaciones lineales usando métodos numéricos clásicos.

## Métodos implementados

- Eliminación de Gauss (método directo)
- Jacobi (método iterativo)
- Gauss-Seidel con relajación SOR (método iterativo)

## Uso

1. Prepare un archivo `matriz.txt` con el sistema a resolver. El formato esperado es:

   ```
   a11 a12 ... a1n b1
   a21 a22 ... a2n b2
   ...             ...
   an1 an2 ... ann bn
   ```

   Donde `aij` son los coeficientes de la matriz A, y `bi` los términos independientes.

2. Compile el programa con un compilador C estándar, por ejemplo:

   ```bash
   gcc SistemaEcuacionesLineales.c ../libreria_de_aditamentos/aditamentos_ui.c matriz_desde_archivo/matriz_desde_archivo.c -o sistema.o -lm
   ```

3. Ejecute el programa:

   ```bash
   ./sistema.o
   ```

4. Seleccione el método deseado en el menú.

5. Confirme que el archivo `matriz.txt` está correcto cuando se le solicite.

6. Ingrese la tolerancia y, para Gauss-Seidel, el factor de relajación omega (0 < omega < 2).

7. El programa mostrará la solución aproximada, el error y el número de iteraciones.

## Notas

- Los métodos iterativos requieren que la matriz sea diagonalmente dominante para garantizar convergencia.
- El método de Gauss-Seidel implementa relajación sucesiva (SOR) para acelerar la convergencia.
- El método de eliminación de Gauss incluye pivoteo parcial para mejorar la estabilidad numérica.
- Verifique que el determinante de la matriz no sea cero para asegurar solución única.

## Autor

Tobias Funes
Versión 1.0
