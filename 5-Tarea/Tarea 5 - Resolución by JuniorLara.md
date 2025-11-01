---
geometry: margin=1in
---
# Resolución de Tarea 5 - Grafos (Fecha: 3 de Noviembre de 2025)

$$
\begin{matrix}
\text{Universidad Simón Bolívar} \\
\text{Departamento de Computación y Tecnología de la Información} \\
\text{CI5651 - Diseño de Algoritmos I} \\
\text{Septiembre - Diciembre 2025} \\
\text{Estudiante: Junior Miguel Lara Torres (17-10303)} \\
\text{ } \\
\Large \text{Tarea 5 (9 puntos)} \\
\end{matrix}
$$

# Indice
- [Resolución de Tarea 5 - Grafos (Fecha: 3 de Noviembre de 2025)](#resolución-de-tarea-5---grafos-fecha-3-de-noviembre-de-2025)
- [Indice](#indice)
- [Pregunta 1](#pregunta-1)
  - [Parte (a)](#parte-a)
  - [Parte (b)](#parte-b)
  - [Parte (c)](#parte-c)
  - [Parte (d)](#parte-d)
- [Pregunta 2](#pregunta-2)
- [Pregunta 3](#pregunta-3)
- [Pregunta 4](#pregunta-4)

# Pregunta 1

Mi nombre y apellido es: **Junior Lara**

* Cadena completa en minúsculas: "juniorlara".
* Eliminación de repeticiones (conservando el orden de aparición): **j, u, n, i, o, r, l, a**
* Cadena de caracteres resultante (S): "juniorla" (n=8 caracteres).

## Parte (a)

El árbol binario de búsqueda se muestra a continuación 

![Árbol Binario de Búsqueda asociado a la cadena: `juniorla`.](./5-Tarea/images/p1a.png)\

## Parte (b)

El recorrido preorder es el siguiente: **j, i, a, u, n, o, l, r**

## Parte (c)

Realizando la enumeración por niveles 

![Árbol Binario de Búsqueda asociado a la cadena: `juniorla` renombrado a niveles](./5-Tarea/images/p1b.png)\

* El recorrido de Euler es el siguiente: **1, 2, 3, 2, 1, 2, 3, 4, 5, 5, 3, 4, 3, 2, 1**

* A nivel de caracteres tenemos: **j, i, a, i, j, u, n, o, l, o, n, r, n, u, j**

## Parte (d)

La cadena es "juniorla". Los nodos a considerar son 'l' y 'a'.

El Ancestro Común Más Bajo (LCA) de dos nodos u y v en un árbol es el nodo con la profundidad mínima (menor nivel) en el sub-arreglo del Recorrido de Euler (L) comprendido entre la primera aparición de u y la primera aparición de v. 

Esto reduce el problema de LCA a un problema de Consulta de Rango Mínimo. (Nota: la justificación es en base 1-indexación)

Tenemos los arreglos

* $EulerLevel = [1, 2, 3, 2, 1, 2, 3, 4, 5, 5, 3, 4, 3, 2, 1]$
* $EulerNodos = [j, i, a, i, j, u, n, o, l, o, n, r, n, u, j]$

1. Identificar las primeras ocurrencias:
    ◦ Posición primera ocurrencia de l: 9.
    ◦ Posición primera ocurrencia de a: 3.
2. Determinar los niveles en el rango $[3..9]$:
    ◦ $EulerLevel[3..9] = [3, 2, 1, 2, 3, 4, 5]$.
    ◦ $EulerNodos[3..9] = [a, i, j, u, n, o, l]$.
3. Encontrar el Mínimo Nivel:
    ◦ El valor mínimo en la secuencia de niveles $[3..9]$ es 1, el cual ocurre en el índice 5.
4. Identificar el LCA:
    ◦ El nodo en la secuencia E en el índice 5 es j.
Por lo tanto, el Ancestro Común Más Bajo entre 'l' y 'a' es j.

# Pregunta 2

# Pregunta 3

# Pregunta 4