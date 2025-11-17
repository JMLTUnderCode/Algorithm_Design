---
header-includes:
  - \usepackage{xcolor}
  - \usepackage{listings}
  - \definecolor{codebg}{RGB}{0,0,0}
  - \definecolor{textcolor}{RGB}{255,255,255}
  - \lstset{
      backgroundcolor=\color{codebg},
      basicstyle=\ttfamily\small\color{textcolor},
      frame=leftline,
      framerule=2pt,
      rulecolor=gray!60,
      numbers=left,
      numberstyle=\tiny\color{gray},
      keywordstyle=\color{cyan!70},
      commentstyle=\color{green!60},
      stringstyle=\color{orange},
      showspaces=false,
      showstringspaces=false,
      tabsize=2,
    }
geometry: margin=1in
---

# Resolución de Tarea 7 - Cadenas de Caracteres y Geometría Computacional (Fecha: 17 de Noviembre de 2025)

$$
\begin{matrix}
\text{Universidad Simón Bolívar} \\
\text{Departamento de Computación y Tecnología de la Información} \\
\text{CI5651 - Diseño de Algoritmos I} \\
\text{Septiembre - Diciembre 2025} \\
\text{Estudiante: Junior Miguel Lara Torres (17-10303)} \\
\text{ } \\
\Large \text{Tarea 7 (9 puntos)} \\
\end{matrix}
$$

# Indice
- [Resolución de Tarea 7 - Cadenas de Caracteres y Geometría Computacional (Fecha: 17 de Noviembre de 2025)](#resolución-de-tarea-7---cadenas-de-caracteres-y-geometría-computacional-fecha-17-de-noviembre-de-2025)
- [Indice](#indice)
- [Pregunta 1](#pregunta-1)
  - [(a) Construcción del Árbol de Sufijos para $w$](#a-construcción-del-árbol-de-sufijos-para-w)
  - [(b) Construcción del Arreglo de Sufijos (SA)](#b-construcción-del-arreglo-de-sufijos-sa)
  - [(c) Cálculo de PLCP\[k\] y LCP\[k\]](#c-cálculo-de-plcpk-y-lcpk)
    - [1. Cálculo de Phi](#1-cálculo-de-phi)
    - [2. Cálculo de PLCP\[k\] (Prefijo Común Permutado más Largo)](#2-cálculo-de-plcpk-prefijo-común-permutado-más-largo)
    - [3. Cálculo de LCP\[i\] (Prefijo Común más Largo)](#3-cálculo-de-lcpi-prefijo-común-más-largo)
- [Pregunta 2](#pregunta-2)
  - [Implementación en C++](#implementación-en-c)

# Pregunta 1

El carné a utilizar es "17-10303". La cadena $n$ a considerar, sin el guión, es **$w = "1710303"$**. La longitud de la cadena es $N=7$.

## (a) Construcción del Árbol de Sufijos para $w$

Un árbol de sufijos se construye a partir de la cadena $w$ al organizar todos sus sufijos en una estructura de árbol que aprovecha los prefijos compartidos, similar a un Trie. En este caso, la estructura contendría todos los sufijos de $w=$ "1710303":

1. 1710303
2. 710303
3. 10303
4. 0303
5. 303
6. 03
7. 3

![Árbol de sufijos para 1710303](./7-Tarea/images/p1a.png)\

## (b) Construcción del Arreglo de Sufijos (SA)

El arreglo de sufijos ($SA$) almacena los índices de inicio de los sufijos de $w$ en el orden en que aparecerían si estuvieran ordenados lexicográficamente.

Los sufijos ordenados de $w=$ "1710303" son:

| Índice |   Sufijo  |
| :----: | :------- |
|    7   | $\lambda$ |
|    3   |    0303   |
|    5   |     03    |
|    2   |   10303   |
|    0   |  1710303  |
|    4   |    303    |
|    6   |     3     |
|    1   |  710303   |

El Arreglo de Sufijos (SA) es:
$$SA =[7, 3, 5, 2, 0, 4, 6, 1]$$

## (c) Cálculo de PLCP\[k\] y LCP\[k\]

Para calcular los arreglos de Longitud de Prefijo Común (LCP) y Prefijo Común Permutado más Largo (PLCP), primero se define el arreglo auxiliar $\Phi$ (Phi).

### 1. Cálculo de Phi

El arreglo $\Phi$ almacena, para el sufijo que empieza en la posición $k$, el índice de inicio del sufijo que lo precede inmediatamente en el arreglo de sufijos ordenado ($SA$).

$$\Phi[SA[i]] = SA[i-1]$$

El caso base es $SA=7$ (el sufijo $\lambda$), para el cual $\Phi$ se define como $\lambda$ o $-1$.

| Rank (i) | $SA[i]$ (Índice) | Sufijo $w[SA[i]..]$ | $SA[i-1]$ (Predecesor) | $\mathbf{\Phi[SA[i]]}$ |
| :---: | :---: | :--- | :---: | :---: |
| 0 | 7 | $\lambda$ | N/A | -1 |
| 1 | 3 | 0303 | 7 | 7 |
| 2 | 5 | 03 | 3 | 3 |
| 3 | 2 | 10303 | 5 | 5 |
| 4 | 0 | 1710303 | 2 | 2 |
| 5 | 4 | 303 | 0 | 0 |
| 6 | 6 | 3 | 4 | 4 |
| 7 | 1 | 710303 | 6 | 6 |

**Arreglo $\Phi$ (Indexado por posición $k$ de 0 a 7):**
$$\Phi = [2, 6, 5, 7, 0, 3, 4, -1]$$

### 2. Cálculo de PLCP[k] (Prefijo Común Permutado más Largo)

$PLCP[k]$ es la longitud del prefijo común más largo entre el sufijo que comienza en $k$ ($w[k..]$) y el sufijo que comienza en $\Phi[k]$ ($w[\Phi[k]..]$).

| k | $\Phi[k]$ | Sufijo $w[k..]$ | Sufijo $w[\Phi[k]..]$ | $\mathbf{PLCP[k]}$ |
| :---: | :---: | :--- | :--- | :---: |
| 0 | 2 | **1**710303 | **1**0303 | 1 |
| 1 | 6 | 710303 | 3 | 0 |
| 2 | 5 | 10303 | 03 | 0 |
| 3 | 7 | 0303 | $\lambda$ | 0 |
| 4 | 0 | 303 | 1710303 | 0 |
| 5 | 3 | **03** | **03**03 | 2 |
| 6 | 4 | **3** | **3**03 | 1 |
| 7 | -1 | $\lambda$ | N/A | 0 |

**Arreglo PLCP** (Indexado por posición $k$ de 0 a 7):
$$PLCP =[1, 0, 0, 0, 0 , 2, 1, 0]$$

### 3. Cálculo de LCP[i] (Prefijo Común más Largo)

$LCP[i]$ es la longitud del prefijo común más largo entre el sufijo de rango $i$ ($SA[i]$) y el sufijo de rango $i-1$ ($SA[i-1]$). Se calcula utilizando el arreglo PLCP mediante la fórmula $LCP[i] = PLCP[SA[i]]$.

$LCP$ se define como 0.

| Rank (i) | $SA[i]$ | Sufijo $w[SA[i]..]$ | Sufijo $w[SA[i-1]..]$ | $PLCP[SA[i]]$ | $\mathbf{LCP[i]}$ |
| :---: | :---: | :--- | :--- | :---: | :---: |
| 0 | 7 | $\lambda$ | N/A | N/A | **0** |
| 1 | 3 | 0303 | $\lambda$ | $PLCP=0$ | **0** |
| 2 | 5 | **03** | **03**03 | $PLCP=2$ | **2** |
| 3 | 2 | 10303 | 03 | $PLCP=0$ | **0** |
| 4 | 0 | **1**710303 | **1**0303 | $PLCP=1$ | **1** |
| 5 | 4 | 303 | 1710303 | $PLCP=0$ | **0** |
| 6 | 6 | **3** | **3**03 | $PLCP=1$ | **1** |
| 7 | 1 | 710303 | 3 | $PLCP=0$ | **0** |

**Arreglo LCP** (Indexado por rango $i$ de 0 a 7):
$$LCP = [0, 0, 2, 0, 1, 0, 1, 0]$$

# Pregunta 2

Este problema es una aplicación directa del algoritmo de pre-procesamiento utilizado en Knuth-Morris-Pratt (KMP), el cual calcula la longitud del Prefijo Propio más Largo (LPS, por sus siglas en inglés) que también es un sufijo para cada prefijo de la cadena.

El algoritmo KMP utiliza una tabla de saltos ($b$ o $\pi$) que, para una cadena $x$, almacena en la posición $i$ la longitud del prefijo propio más largo de $x[0..i]$ que también es un sufijo de $x[0..i]$.

Si aplicamos este algoritmo de pre-procesamiento a la cadena $S$, el último elemento de la tabla LPS calculará precisamente la longitud $L$ del prefijo propio más largo de $S$ que es también un sufijo de $S$.

La complejidad temporal de este pre-procesamiento es lineal, $O(N)$, lo que satisface el requerimiento de eficiencia del problema.

## Implementación en C++

El archivo funcional se encuentra en [lps.cpp](https://github.com/JMLTUnderCode/Algorithm_Design/blob/main/7-Tarea/lps.cpp)

```cpp
// Función para calcular la longitud del Longest Proper Prefix (LPS) array para KMP.
// La complejidad es O(n).
string compute_lps_array(const std::string &S) {
    int N = S.length();
    // La subcadena más grande debe ser propia (T != S), así que si N <= 1,
    // solo podemos retornar la cadena vacía.
    if (N <= 1)
        return "";

    // lps[i] almacenará la longitud del prefijo propio más largo de S[0...i]
    // que también es un sufijo de S[0...i].
    vector<int> lps(N, 0);

    int len = 0; // Longitud del prefijo más largo anterior que es también sufijo
    int i = 1;

    // Lazo que calcula lps[i] para i = 1 a N-1
    while (i < N) {
        if (S[i] == S[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            // Desigualdad de caracteres
            // Retrocedemos a la longitud LPS anterior (len - 1)
            if (len != 0)
                len = lps[len - 1];
            else {
                // Si len es 0, no hay un prefijo común, lps[i] = 0
                lps[i] = 0;
                i++;
            }
        }
    }
    // La longitud L de la subcadena T deseada es el último elemento del arreglo LPS.
    // Esto se debe a que lps[N-1] almacena la longitud del prefijo más largo
    // de S[0...N-1] que también es un sufijo de S[0...N-1].
    int L = lps[N - 1];
    
    // Extraemos la subcadena de longitud L.
    return S.substr(0, L);
}
```



