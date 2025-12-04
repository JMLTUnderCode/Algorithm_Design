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

# Resolución de Tarea 9 - Algoritmos Probabilisticos y Aproximados (Fecha: 8 de Diciembre de 2025)

$$
\begin{matrix}
\text{Universidad Simón Bolívar} \\
\text{Departamento de Computación y Tecnología de la Información} \\
\text{CI5651 - Diseño de Algoritmos I} \\
\text{Septiembre - Diciembre 2025} \\
\text{Estudiante: Junior Miguel Lara Torres (17-10303)} \\
\text{ } \\
\Large \text{Tarea 9 (9 puntos)} \\
\end{matrix}
$$

# Indice
- [Resolución de Tarea 9 - Algoritmos Probabilisticos y Aproximados (Fecha: 8 de Diciembre de 2025)](#resolución-de-tarea-9---algoritmos-probabilisticos-y-aproximados-fecha-8-de-diciembre-de-2025)
- [Indice](#indice)
- [Pregunta 1](#pregunta-1)
- [Pregunta 2](#pregunta-2)
  - [Implementación en C++](#implementación-en-c)

# Pregunta 1

Por resolver

# Pregunta 2

Se solicita diseñar e implementar un algoritmo probabilístico de Monte Carlo para verificar si una matriz $B$ es la inversa de una matriz $A$ (es decir, $A \times B = I$, donde $I$ es la matriz identidad), con una probabilidad de error acotada por $\epsilon$.

Utilizaremos el **Algoritmo de Freivalds** adaptado. El test se basa en el principio de que, si $A \times B \neq I$, es muy probable que una multiplicación matricial-vectorial aleatoria revele la desigualdad.

* Verificar $A \times B = I$ es equivalente a verificar si la matriz de diferencia $D = A \times B - I$ es la matriz nula.
* En lugar de calcular $D$ (lo cual es $O(n^3)$), seleccionamos un vector aleatorio $X$ de tamaño $1 \times n$ (donde $X_i \in \{0, 1\}$). Comprobamos si $X \cdot D = \mathbf{0}$, o lo que es lo mismo: $X \cdot A \cdot B = X \cdot I$. Dado que $X \cdot I = X$, la prueba se reduce a:
* $$X \cdot (A \times B) = X$$
* La multiplicación $X \cdot A$ (vector fila por matriz) toma $O(n^2)$. Luego, $(X \cdot A) \cdot B$ (vector fila por matriz) toma otros $O(n^2)$. El test se ejecuta en $\mathbf{O(n^2)}$ por iteración.
* Si $A \times B \neq I$, la probabilidad de que la prueba falle (produzca $\mathbf{X}$ en la salida) es a lo sumo $1/2$. Para asegurar que la probabilidad de error sea menor que $\epsilon$, debemos repetir el test $k$ veces, donde $k$ se define como $k = \lceil \log_2(1/\epsilon) \rceil$.

Así, el algoritmo implementado (CheckInversion) verifica la relación $A \times B = I$ utilizando la prueba de Freivalds repetida $k$ veces. Sabiendo que multiplicando el costo por iteración por el número de iteraciones la complejidad total queda como: 
$$O(n^2 \log(1/\epsilon))$$

## Implementación en C++

El archivo funcional se encuentra en [check_inversion_matrix.cpp](https://github.com/JMLTUnderCode/Algorithm_Design/blob/main/9-Tarea/check_inversion_matrix.cpp)

```cpp
typedef long long MatElement;
typedef vector<vector<MatElement>> Matrix;
typedef vector<MatElement> Vector;

// Generador de números aleatorios
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

/**
 * Genera un vector aleatorio X (1 x N) con elementos {0, 1}.
 */
Vector GenerateRandomVector(int N)
{
    uniform_int_distribution<MatElement> dist(0, 1);
    Vector X(N);
    for (int i = 0; i < N; ++i)
        X[i] = dist(rng);
    return X;
}

/**
 * Calcula el producto de un vector fila X por una matriz A: R = X * A.
 * R y X son vectores 1D de tamaño N.
 * Complejidad: O(N^2).
 */
Vector MatrixVectorProduct(const Vector &X, const Matrix &A, int N) {
    Vector R(N, 0);
    // Multiplicación X (1xN) * A (NxN) = R (1xN)
    for (int j = 0; j < N; ++j) { // columna de A / elemento de R
        for (int i = 0; i < N; ++i)
            R[j] += X[i] * A[i][j]; // fila de X / fila de A
    }
    return R;
}

/**
 * Algoritmo de Monte Carlo para verificar si B = A^-1 (A * B = I)
 * Retorna true si probablemente es la inversa, false si es definitivamente compuesta.
 */
bool CheckInversion(const Matrix &A, const Matrix &B, int N, double epsilon) {
    if (N == 0)
        return true;

    // Calcular K: número de iteraciones necesarias para error < epsilon
    // K = ceil(log2(1/epsilon))
    int K = max(1, (int)ceil(log2(1.0 / epsilon)));
    cout << "\n--- Ejecucion del Algoritmo de Monte Carlo ---" << endl;
    cout << "Dimension de la matriz N: " << N << endl;
    cout << "Probabilidad de error maximo (epsilon): " << scientific << setprecision(2) << epsilon << endl;
    cout << "Iteraciones requeridas (K): " << K << endl;
    cout << "------------------------------------------" << endl;

    for (int k = 1; k <= K; ++k) {
        // 1. Generar vector aleatorio X
        Vector X = GenerateRandomVector(N);

        // 2. Calcular R_AB = (X * A) * B
        Vector R_A = MatrixVectorProduct(X, A, N);
        Vector R_AB = MatrixVectorProduct(R_A, B, N);

        // 3. Comparar R_AB con X (deberían ser iguales si A*B = I)
        bool match = true;
        for (int i = 0; i < N; ++i) {
            if (R_AB[i] != X[i]) {
                match = false;
                break;
            }
        }

        cout << "Iteracion " << k << "/" << K << ": Resultado: " << (match ? "COINCIDE (Probable)" : "NO COINCIDE (Compuesto)") << endl;

        if (!match) {
            cout << "\nCONCLUSION: B NO es la inversa de A (testigo encontrado)." << endl;
            return false;
        }
    }

    cout << "\nCONCLUSION: B es probablemente la inversa de A." << endl;
    return true;
}
```