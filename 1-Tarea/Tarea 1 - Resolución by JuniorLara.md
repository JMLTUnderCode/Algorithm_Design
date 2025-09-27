# Resolución de Tarea 1 - Complejidad Algorítmica y Análisis Amortizado (Fecha: 29 de Septiembre de 2025)

$$
\begin{matrix}
\text{Universidad Simón Bolívar} \\
\text{Departamento de Computación y Tecnología de la Información} \\
\text{CI5651 - Diseño de Algoritmos I} \\
\text{Septiembre - Diciembre 2025} \\
\text{Estudiante: Junior Miguel Lara Torres (17-10303)} \\
\text{ } \\
\Large \text{Tarea 1 (9 puntos)} \\
\end{matrix}
$$

# Indice
- [Resolución de Tarea 1 - Complejidad Algorítmica y Análisis Amortizado (Fecha: 29 de Septiembre de 2025)](#resolución-de-tarea-1---complejidad-algorítmica-y-análisis-amortizado-fecha-29-de-septiembre-de-2025)
- [Indice](#indice)
- [Pregunta 1](#pregunta-1)
  - [Consideraciones](#consideraciones)
  - [Peor caso](#peor-caso)
  - [Mejor caso](#mejor-caso)
- [Pregunta 2](#pregunta-2)
- [Pregunta 3](#pregunta-3)

# Pregunta 1

## Consideraciones
Teniendo en cuenta los siguientes puntos

* $T_{permutaciones} \in \Theta(1)$
* $T_{ordenado} \in \Theta(n)$
* El arreglo **a** puede tener la propiedad de:
  * Un **multiconjunto** teniendo así una cantidad de permutaciones igual a 
    $$\binom{n}{m_1, m_2, ..., m_k} = \frac{n!}{m_1! m_2! ... m_k!}$$
  * Un **conjunto** teniendo asi una cantidad de permutaciones igual a $n!$
* Para el punto 3, se tiene que, en cualquier caso, se cumple que: 
    $$\frac{n!}{m_1! m_2! ... m_k!} \leq n!$$
   Esto porque, cuando no hay repetición de elementos (conjunto), entonces se cumple $(\forall k | 1 \leq k \leq n : m_k = 1)$, por lo tanto: 
    $$\frac{n!}{1! 1! ... 1!} \leq n! \iff n! \leq n!$$
   En caso de haber al menos una repetición de un elemento, entonces: 
    $$\frac{n!}{m_1! m_2! ... m_k!} < n!$$

## Peor caso

Con el arreglo **a** teniendo propiedades de conjunto, tendremos $n!$ permutaciones para revisar. Adicionalmente, el predicado **ordenado** tarda $O(n)$, se tiene entonces $f = n! \cdot n)$, por lo tanto, $bogoMin \in O(n! \cdot n)$.

## Mejor caso

Teniendo un arreglo **a** con propiedades de conjunto y la primera permutación sea la ordenada, y teniendo en cuenta que **ordenado** tarda $O(n)$, entonces al final tendríamos $g = n \cdot 1$, por lo tanto $bogoMin \in O(n)$.

Ya en caso de un arreglo con propiedades de multiconjunto, el mejor caso sería tomar la primera permutación que esté ordenada, así que se cumple el mismo análisis.

# Pregunta 2


# Pregunta 3

