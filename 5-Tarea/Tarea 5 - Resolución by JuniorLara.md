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
  - [Algoritmo](#algoritmo)
    - [3. Implementación en C++](#3-implementación-en-c)
- [Pregunta 3](#pregunta-3)
- [Pregunta 4](#pregunta-4)
  - [Algoritmo](#algoritmo-1)
    - [Fase 1: Construcción del Grafo](#fase-1-construcción-del-grafo)
    - [Fase 2: Cálculo del Emparejamiento Bipartito Máximo (MCBM)](#fase-2-cálculo-del-emparejamiento-bipartito-máximo-mcbm)

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
   
    * Posición primera ocurrencia de l: 9.
    * Posición primera ocurrencia de a: 3.

2. Determinar los niveles en el rango $[3..9]$:
   
    * $EulerLevel[3..9] = [3, 2, 1, 2, 3, 4, 5]$.
    * $EulerNodos[3..9] = [a, i, j, u, n, o, l]$.

3. Encontrar el Mínimo Nivel:

    * El valor mínimo en la secuencia de niveles $[3..9]$ es 1, el cual ocurre en el índice 5.
  
4. Identificar el LCA:
    * El nodo en la secuencia E en el índice 5 es j.
Por lo tanto, el Ancestro Común Más Bajo entre 'l' y 'a' es j.

# Pregunta 2

El problema pide determinar si es posible orientar las aristas de un grafo no dirigido de encuentros $G=(A, E)$ (donde los agentes $A$ son los nodos y los encuentros $E$ son las aristas) de tal forma que toda la información pueda alcanzar a todos los agentes. Esto equivale a encontrar una **orientación fuertemente conexa** ($G'$), es decir, un grafo dirigido donde existe un camino de $u$ a $v$ y de $v$ a $u$ para todo par de nodos $u, v$.

El **Teorema de Robbins** establece que un grafo no dirigido $G$ puede tener una orientación fuertemente conexa si, y solo si, $G$ es **conexo** y **no contiene puentes**.

Si el grafo $G$ es conexo y la cantidad de puentes es nula, el Teorema de Robbins se cumple. En este caso, la orientación fuertemente conexa resultante garantiza un **plan de propagación de información** tal que, al asignar roles de "hablante" y "oyente" (orientación de las aristas), la información generada por cualquier agente puede alcanzar a todos los demás.

## Algoritmo

Para resolver el problema en el tiempo lineal requerido, $O(|A| + |E|)$, utilizaremos una única pasada de Búsqueda en Profundidad (DFS) modificada para cumplir dos objetivos:

1.  **Verificación de Conexidad:** Un DFS iniciado desde un nodo arbitrario explorará el grafo completo si y solo si es conexo. Si no todos los nodos son visitados, el grafo es disconexo.
2.  **Detección de Puentes:** Utilizaremos la extensión del DFS (similar al algoritmo de [Tarjan](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm)) que calcula los tiempos de descubrimiento (`dfs_num`) y el valor más bajo alcanzable (`dfs_low`) para cada nodo. Una arista de árbol $(u, v)$ es un **puente** si, al salir del subárbol de $v$, el punto más alto que se puede alcanzar es el propio $v$ o un nodo que está más abajo, es decir, si $\mathbf{dfs\_low}(v) > \mathbf{dfs\_num}(u)$.

Si el algoritmo verifica que el grafo es conexo y encuentra que el número total de puentes es cero, entonces existe un plan de propagación de información.

### 3. Implementación en C++

La siguiente función en C++ implementa la lógica usando un DFS para verificar la conexidad y contar los puentes en tiempo $O(|A| + |E|)$.

```cpp
// Constantes y estructuras auxiliares para la detección de puentes (Bridges)
typedef vector<vector<int>> AdjList;

// Variables globales para el DFS
int timer_counter;
vector<int> dfs_num;  // Tiempo de descubrimiento (prenum)
vector<int> dfs_low;  // Menor prenum alcanzable desde el subárbol
vector<bool> visited; // Para el chequeo de conectividad
int total_vis;        // Total visitados en DFS.
int num_bridges;

void find_bridges(int u, int p, const AdjList& G) {
    visited[u] = true;
    total_vis++;
    dfs_num[u] = dfs_low[u] = timer_counter++;

    for (int v : G[u]) {
        if (v == p) continue; // Ignorar la arista de vuelta al padre

        if (dfs_num[v] != -1) {
            // Caso 1: Vértice 'v' ya visitado (back edge)
            // Actualizamos dfs_low[u] con el tiempo de descubrimiento (dfs_num) de 'v'
            dfs_low[u] = min(dfs_low[u], dfs_num[v]);
        } else {
            // Caso 2: Vértice 'v' no visitado (tree edge)
            find_bridges(v, u, G);

            // Después de la llamada recursiva, actualizamos dfs_low[u]
            // con el valor más bajo alcanzado por 'v'
            dfs_low[u] = min(dfs_low[u], dfs_low[v]);

            // Detección de Puente: si el subárbol de 'v' no puede alcanzar 
            // a un ancestro estricto de 'u' (o 'u' mismo)
            if (dfs_low[v] > dfs_num[u]) {
                // Si dfs_low[v] > dfs_num[u], la arista (u, v) es un puente
                num_bridges++;
            }
        }
    }
}

bool isStronglyConnectable(int N, const vector<pair<int, int>>& E) {
    if (N <= 1) return true; // Si hay 0 o 1 agente, trivialmente es SC.

    // 1. Construcción del Grafo (Lista de Adyacencia)
    AdjList G(N);
    for (const auto& edge : E) {
        G[edge.first].push_back(edge.second);
        G[edge.second].push_back(edge.first);
    }
    
    // 2. Inicialización de estructuras auxiliares
    timer_counter = 0;
    num_bridges = 0;
    total_vis = 0;
    // -1 indica UNVISITED, usado para inicializar dfs_num y asegurar el chequeo de conectividad
    dfs_num.assign(N, -1);
    dfs_low.assign(N, 0);
    visited.assign(N, false); 

    // 3. Ejecutar DFS modificado (empezamos desde el nodo 0)
    find_bridges(0, -1, G);
    
    // 4. Verificación de Conexidad y Cero Puentes (Biconexidad de Aristas)
    // Por el Teorema de Robbins, si es conexo y no tiene puentes, admite orientación SC.
    if (num_bridges == 0 && total_vis == N) {
        return true;
    } else {
        return false;
    }
}
```

# Pregunta 3

# Pregunta 4

Definimos un **grafo de conflicto** $G=(C, E)$, donde:

* $E$ contiene una arista $(x, y)$ si $x, y \in C$ y su suma $x+y$ es un número primo.

Un grafo es **bipartito** si sus vértices pueden ser divididos en dos conjuntos disjuntos $A$ y $B$ tales que cada arista conecta un vértice de $A$ con uno de $B$.

1. La suma de dos números, $x+y$, es un número primo. Dado que 2 es el único número primo par, y que los números en $C$ son distintos (lo que implica que $x+y \ne 2$ si $x \ne y$ y $x, y > 0$), cualquier suma prima $x+y = P$ debe ser un número primo **impar** ($P > 2$).
2. Para que la suma de dos enteros sea impar, uno debe ser par y el otro debe ser impar.
3. Si definimos $A$ como el conjunto de números impares en $C$, y $B$ como el conjunto de números pares en $C$, toda arista $(x, y) \in E$ necesariamente conecta un nodo en $A$ con un nodo en $B$.

Concluimos que el grafo de conflicto $G$ es **bipartito**.

El objetivo es encontrar el subconjunto máximo de vértices $C' \subseteq C$ tal que no haya aristas entre ningún par de vértices en $C'$. Este subconjunto es conocido como el **Conjunto Independiente Máximo (MIS)**. El número mínimo de elementos a eliminar es, por definición, $|C| - |MIS|$.

El **Teorema de König** establece una equivalencia fundamental en grafos bipartitos: **la cardinalidad del emparejamiento máximo (MCBM) es igual a la cardinalidad del cubrimiento de vértices mínimo (MVC)**.

$$|MCBM| = |MVC|$$

Además, en cualquier grafo, el Conjunto Independiente Máximo ($|MIS|$) y el Cubrimiento de Vértices Mínimo ($|MVC|$) se relacionan por:
$$|MIS| = |V| - |MVC|$$

Sustituyendo el Teorema de König en esta relación, para nuestro grafo bipartito $G$:
$$|MIS| = |C| - |MCBM|$$

Dado que buscamos el **mínimo número de números a eliminar** ($|R|$), y $|R| = |C| - |MIS|$, entonces:
$$|R| = |C| - (|C| - |MCBM|) = |MCBM|$$

Por lo tanto, el problema se reduce a calcular la cardinalidad del **Emparejamiento Bipartito Máximo (MCBM)** en el grafo de conflicto $G$.

## Algoritmo

El algoritmo consiste en dos fases principales: 

1. Construcción del grafo
2. Cálculo del MCBM.

### Fase 1: Construcción del Grafo

1.  **Precomputación de Primos:** Se debe determinar si las sumas $x+y$ son primas. Acá se asume por condición del problema que  podemos consultar si un número es primo en $O(1)$.
2.  **Construcción de Aristas:** Iteramos sobre todos los pares $(x, y)$ donde $x \in A$ (impares) y $y \in B$ (pares). El número total de pares es $n^2$.
    *   Si $x+y$ es primo, agregamos la arista $(x, y)$ a $E$.
    *   El tiempo de construcción es $O(n^2) \times O(1) = O(n^2)$.

### Fase 2: Cálculo del Emparejamiento Bipartito Máximo (MCBM)

La cardinalidad del MCBM puede encontrarse usando algoritmos de flujo máximo (Max Flow) como Edmonds-Karp, o usando el algoritmo de Hopcroft-Karp.

El algoritmo de **Hopcroft-Karp** es uno de los métodos más eficientes para encontrar el MCBM, con una complejidad de tiempo de $O(Edges \cdot \sqrt{Nodos})$.

Sustituyendo $Edges = n^2$ y $Nodos = |C| = n$ tenemos $O(n^2 \cdot \sqrt{n})$