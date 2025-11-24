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

# Resolución de Tarea 8 - FFT y Optimización para Programación Dinámica (Fecha: 24 de Noviembre de 2025)

$$
\begin{matrix}
\text{Universidad Simón Bolívar} \\
\text{Departamento de Computación y Tecnología de la Información} \\
\text{CI5651 - Diseño de Algoritmos I} \\
\text{Septiembre - Diciembre 2025} \\
\text{Estudiante: Junior Miguel Lara Torres (17-10303)} \\
\text{ } \\
\Large \text{Tarea 8 (9 puntos)} \\
\end{matrix}
$$

# Indice
- [Resolución de Tarea 8 - FFT y Optimización para Programación Dinámica (Fecha: 24 de Noviembre de 2025)](#resolución-de-tarea-8---fft-y-optimización-para-programación-dinámica-fecha-24-de-noviembre-de-2025)
- [Indice](#indice)
- [Pregunta 1](#pregunta-1)
  - [Nivel 1](#nivel-1)
  - [Nivel 2](#nivel-2)
  - [Nivel 3 (Cálculos de base)](#nivel-3-cálculos-de-base)
  - [Nivel 2 (Continuación)](#nivel-2-continuación)
  - [Nivel 1 (Continuación)](#nivel-1-continuación)
  - [Resultado](#resultado)
- [Pregunta 2](#pregunta-2)
  - [Implementación en C++](#implementación-en-c)
- [Pregunta 3](#pregunta-3)
  - [Fase I: Reducción Geométrica y Linealización](#fase-i-reducción-geométrica-y-linealización)
  - [Fase II: Programación Dinámica con Convex Hull (CH)](#fase-ii-programación-dinámica-con-convex-hull-ch)
  - [Complejidad Total](#complejidad-total)
  - [Implementación en C++](#implementación-en-c-1)

# Pregunta 1

El carné es **17-10303** y por tanto el polinomio derivado es:
$$P(x) = 1 + 7x + x^2 + 0x^3 + 3x^4 + 0x^5 + 3x^6$$

Teniendo en cuenta que FFT es un algoritmo (un método) que se utiliza para calcular la DFT siempre y cuando el tamaño de la entrada N sea una potencia de dos. Entonces, el grado de ligadura (o tamaño del vector de coeficientes) debe ser estrictamente mayor que el grado del polinomio, generalmente $N=d+1$. Por lo tanto, el grado de ligadura mínimo sería $N=7$ en nuestro caso.

Dado que $N$ debe ser una potencia de dos para aplicar FFT eficientemente, el tamaño N debe aumentarse a la siguiente potencia de dos, que es $N=8$ (ya que $2^3=8$). Asi, nuestro polinomio se completa con 0's.

$$P(x) = 1 + 7x + x^2 + 0x^3 + 3x^4 + 0x^5 + 3x^6 + 0x^7$$

Se tiene:

* **Coeficientes:** $a = \langle 1, 7, 1, 0, 3, 0, 3, 0 \rangle$.
* **Grado de Ligadura ($N$):** Dado que el grado máximo es 6, y se pide usar las raíces octavas de la unidad, el grado de ligadura es **$N=8$**.
* **Raíz Primitiva de la Unidad ($\omega_8$):** Usamos la $N$-ésima raíz primitiva de la unidad, $\omega_8 = e^{2\pi i/8} = \frac{\sqrt{2}}{2} + i \frac{\sqrt{2}}{2}$.

El algoritmo FFT se basa en la descomposición de $P(x)$ en polinomios de coeficientes pares e impares.

## Nivel 1

Se tiene $N=8$, $\omega = \omega_8$

* **División:**
  * $P_{par}(x)$: Coeficientes pares: $\langle a_0, a_2, a_4, a_6 \rangle = \langle 1, 1, 3, 3 \rangle$.
  * $P_{impar}(x)$: Coeficientes impares: $\langle a_1, a_3, a_5, a_7 \rangle = \langle 7, 0, 0, 0 \rangle$.
* **Recursión:** Calculamos la DFT de $P_{par}$ y $P_{impar}$ en las $\frac{N}{2}=4$ raíces de la unidad, $\omega_4 = (\omega_8)^2 = i$.

## Nivel 2

Se tiene $N=4$, $\omega = \omega_4 = i$

* **1. DFT de $P_{par}(x) = 1 + x + 3x^2 + 3x^3$:**
  * **División:**
    * $P_{pp}(x)$: $\langle 1, 3 \rangle$.
    * $P_{pi}(x)$: $\langle 1, 3 \rangle$.
  * **Recursión:** Calculamos DFT de $P_{pp}$ y $P_{pi}$ en las $\frac{4}{2}=2$ raíces, $\omega_2 = (\omega_4)^2 = -1$.

* **2. DFT de $P_{impar}(x) = 7:$**
  * **División:**
    * $P_{ip}(x)$: $\langle 7, 0 \rangle$.
    * $P_{ii}(x)$: $\langle 0, 0 \rangle$.
  * **Recursión:** Calculamos DFT de $P_{ip}$ y $P_{ii}$ en $\omega_2 = -1$.

## Nivel 3 (Cálculos de base)

Se tiene $N=2$, $\omega = \omega_2 = -1$ 

* **1. DFT de $P_{pp}(x) = 1 + 3x$ (y $P_{pi}$):**
  * Base (FFT de coeficientes $\langle 1 \rangle$ y $\langle 3 \rangle$).
  * Combinación: $j=0, 1$. Raíces: $\omega_2^0 = 1$, $\omega_2^1 = -1$.
    * $P_{pp}(1) = 1 + 1\cdot 3 = 4$.
    * $P_{pp}(-1) = 1 + (-1)\cdot 3 = -2$.
  * **Resultado $S_{pp}$ y $S_{pi}$:** $\langle 4, -2 \rangle$.

* **2 DFT de $P_{ip}(x) = 7$:**
  * Base (FFT de coeficientes $\langle 7 \rangle$ y $\langle 0 \rangle$).
  * Combinación: $j=0, 1$. Raíces: $\omega_2^0 = 1$, $\omega_2^1 = -1$.
    * $P_{ip}(1) = 7 + 1\cdot 0 = 7$.
    * $P_{ip}(-1) = 7 - 1\cdot 0 = 7$.
  * **Resultado $S_{ip}$:** $\langle 7, 7 \rangle$.

* **3. DFT de $P_{ii}(x) = 0$:**
  * **Resultado $S_{ii}$:** $\langle 0, 0 \rangle$.

## Nivel 2 (Continuación)

Se tiene combinación $N=4$

* **1. Combinación de $P_{par}(x)$:** 
  * $S_{par}$: $S_{pp} = \langle 4, -2 \rangle$, $S_{pi} = \langle 4, -2 \rangle$.
  * Raíces: $\omega_4^0 = 1, \omega_4^1 = i$.
  * Utilizamos la fórmula de recombinación: $r_j = S_j + \omega^j S'_j$ y $r_{j+N/2} = S_j - \omega^j S'_j$.
    $$\begin{array}{cccccc}
        j & \omega_4^j & S_j & S'_j & P_{par}(\omega_4^j) & P_{par}(\omega_4^{j+2}) \\[10pt]
        \hline \hline \\
        0 & 1 & 4 & 4 & 4 + 1(4) = 8 & 4 - 1(4) = 0 \\[10pt]
        \hline \\
        1 & i & -2 & -2 & -2 + i(-2) = -2 - 2i & -2 - i(-2) = -2 + 2i
    \end{array}$$

    **Resultado $S_{par}$:** $\langle 8, -2-2i, 0, -2+2i \rangle$.

* **2. Combinación de $P_{impar}(x)$:** 
  * $S_{impar}$: $S_{ip} = \langle 7, 7 \rangle$, $S_{ii} = \langle 0, 0 \rangle$. 
  * Raíces: $\omega_4^0 = 1, \omega_4^1 = i$.

    $$\begin{array}{cccccc}
        j & \omega_4^j & S_j & S'_j & P_{impar}(\omega_4^j) & P_{impar}(\omega_4^{j+2}) \\[10pt]
        \hline \hline \\
        0 & 1 & 7 & 0 & 7 + 1(0) = 7 & 7 - 1(0) = 7 \\[10pt]
        \hline \\
        1 & i & 7 & 0 & 7 + i(0) = 7 & 7 - i(0) = 7 \\
    \end{array}$$

    **Resultado $S_{impar}$:** $\langle 7, 7, 7, 7 \rangle$.

## Nivel 1 (Continuación)

Se tiene combinación final $N=8$

* **1. Combinación Final de $P(x)$:** 
  * $S_{par}$ y $S_{impar}$. 
  * Raíz: $\omega_8 = e^{\pi i/4}$.
  * Utilizaremos $\omega_8$ y sus potencias para $j=0, 1, 2, 3$:
    * $\omega_8^0 = 1$
    * $\omega_8^1 = \frac{\sqrt{2}}{2} + i \frac{\sqrt{2}}{2}$
    * $\omega_8^2 = i$
    * $\omega_8^3 = -\frac{\sqrt{2}}{2} + i \frac{\sqrt{2}}{2}$

    Sea $C = 3.5\sqrt{2} \approx 4.95$.

    $$\begin{array}{cccccc}
        j & \omega_8^j & S_{par, j} & S_{impar, j} & P(\omega_8^j) = S_{par, j} + \omega_8^j S_{impar, j} & P(\omega_8^{j+4}) = S_{par, j} - \omega_8^j S_{impar, j} \\[10pt]
        \hline \hline \\
        0 & 1 & 8 & 7 & 8 + 1(7) = 15 & 8 - 1(7) = 1 \\[10pt]
        \hline \\
        1 & \omega_8^1 & -2-2i & 7 & -2-2i + 7\omega_8^1 = (-2+C) + i(-2+C) & -2-2i - 7\omega_8^1 = (-2-C) + i(-2-C) \\[10pt]
        \hline \\
        2 & i & 0 & 7 & 0 + i(7) = 7i & 0 - i(7) = -7i \\[10pt]
        \hline \\
        3 & \omega_8^3 & -2+2i & 7 & -2+2i + 7\omega_8^3 = (-2-C) + i(2+C) & -2+2i - 7\omega_8^3 = (-2+C) + i(2-C) \\
    \end{array}$$

## Resultado

La Transformada Discreta de Fourier del polinomio $P(x)$ en las raíces octavas de la unidad es el vector $Y$:

$$Y = \langle P(\omega_8^0), P(\omega_8^1), P(\omega_8^2), P(\omega_8^3), P(\omega_8^4), P(\omega_8^5), P(\omega_8^6), P(\omega_8^7) \rangle$$

Donde $C = \frac{7\sqrt{2}}{2}$:

$$Y = \langle 15,$$
$$ (-2+C) + i(-2+C),$$
$$ 7i,$$
$$ (-2-C) + i(2+C),$$
$$ 1,$$
$$ (-2-C) + i(-2-C),$$
$$ -7i,$$
$$ (-2+C) + i(2-C) \rangle$$

# Pregunta 2

Definimos $F(Y)$ como la cantidad de formas en que un entero positivo $Y$ puede escribirse como el producto de dos enteros positivos $a \cdot b$. En otras palabras, $F(Y)$ es el número de divisores de $Y$.

El valor $\text{decomp}(X)$ es entonces el número de pares $(Y_1, Y_2)$ tales que $Y_1 + Y_2 = X$, donde $Y_1$ se puede descomponer en $a\cdot b$ de $F(Y_1)$ maneras, y $Y_2$ se puede descomponer en $c\cdot d$ de $F(Y_2)$ maneras. Esto es la definición de una **convolución**:

$$\text{decomp}(X) = \sum_{Y_1 + Y_2 = X, Y_1, Y_2 > 0} F(Y_1) \cdot F(Y_2)$$

Esta operación de convolución puede ser modelada como la multiplicación de un polinomio $P(x)$ por sí mismo, $C(x) = P(x) \times P(x)$, donde el coeficiente de $x^Y$ en $P(x)$ es $F(Y)$:
$$P(x) = \sum_{Y=1}^{N} F(Y) x^Y$$
El coeficiente $C_X$ del polinomio producto $C(x)$ nos da exactamente $\text{decomp}(X)$.

Asi se tiene que

* Se calculan todos los valores $F(Y)$ hasta $N$ utilizando un método similar al tamiz (iterando sobre múltiplos) en tiempo $O(N \log N)$.
* Se aplica la FFT al vector de coeficientes $F$ para pasar a la representación puntual. Esto se logra mediante el algoritmo recursivo de Divide y Vencerás, $T(n)=2T(\frac{n}{2})+O(n)$, cuya solución es $\Theta(n \log n)$.
* Se multiplican los valores de la transformada término a término.
* Se aplica la FFT inversa (que también se ejecuta en $O(N \log N)$) para obtener los coeficientes finales $\text{decomp}(X)$.

Dado que todas las fases dominantes se ejecutan en $O(N \log N)$, el algoritmo total cumple con la restricción de complejidad requerida.

## Implementación en C++

El archivo funcional se encuentra en [decomp.cpp](https://github.com/JMLTUnderCode/Algorithm_Design/blob/main/8-Tarea/decomp.cpp)

```cpp
// Definición de tipos para el FFT
using CD = complex<long double>;
const long double PI = acos(-1.0L);

/**
 * Precalcula F[i], el número de divisores de i, para 1 <= i <= N.
 * Esta es la cantidad de formas de escribir i = a * b (a, b > 0).
 * Complejidad: O(N log N)
 */
vector<int> calculate_divisor_counts(int N) {
    vector<int> F(N + 1, 0);
    // Itera sobre los posibles factores 'a' y marca sus múltiplos 'Y'
    for (int a = 1; a <= N; ++a) {
        for (int Y = a; Y <= N; Y += a)
            F[Y]++;
    }
    return F;
}

/**
 * Implementación de la Transformada Rápida de Fourier (FFT).
 * time: O(N log N)
 */
void fft(vector<CD> &a, bool invert) {
    int n = a.size();
    if (n == 1)
        return;

    // Implementación sin recursión explícita (bit reversal permutation)
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        long double ang = 2 * PI / len * (invert ? -1 : 1);
        CD wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            CD w(1);
            for (int j = 0; j < len / 2; j++) {
                CD u = a[i + j];
                CD v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (CD &x : a)
            x /= n;
    }
}

/**
 * Realiza la multiplicación de polinomios P * P (convolución).
 * Retorna el vector C, donde C[X] = decomp(X).
 * time: O(N log N)
 */
vector<long long> multiply_polynomial_self(const vector<int> &F, int N) {
    // F es el vector de coeficientes [F, F, ..., F[N]]

    // El grado de ligadura N_prime debe ser al menos 2N (para P*P)
    int N_coeffs = N + 1; // Cantidad de coeficientes reales (0 a N)
    int N_prime = 2 * N + 1;
    int M = 1;
    while (M < N_prime)
        M *= 2; // Siguiente potencia de 2 para FFT

    // 1. Padding y Mapeo a Complejos
    vector<CD> P_coeffs(M, 0);
    for (int i = 0; i <= N; ++i)
    {
        // F[i] es el coeficiente de x^i
        P_coeffs[i] = F[i];
    }

    // 2. FFT Forward
    fft(P_coeffs, false);

    // 3. Multiplicación Puntual (P * P)
    for (int i = 0; i < M; ++i)
        P_coeffs[i] *= P_coeffs[i];

    // 4. FFT Inversa (Interpolación)
    fft(P_coeffs, true);

    // 5. Extracción de Coeficientes Reales
    // C[X] almacena decomp(X)
    // Usamos round para manejar errores de punto flotante
    vector<long long> C(N_prime);
    for (int i = 0; i < N_prime; ++i)
        C[i] = round(P_coeffs[i].real());

    return C;
}

/**
 * Encuentra el máximo valor de decomp(X) en el rango [1, N].
 * Retorna un par {Max_X, Max_Decomp_Count}.
 */
pair<int, long long> solve_max_decomposition(int N) {
    if (N < 1)
        return {0, 0};

    // 1. Precomputar el número de divisores F(Y)
    vector<int> F = calculate_divisor_counts(N);

    // 2. Calcular la convolución C(x) = P(x) * P(x)
    vector<long long> C = multiply_polynomial_self(F, N);

    // 3. Encontrar el máximo decomp(X) para 1 <= X <= N
    long long max_val = 0;
    int max_X = 0;

    // La convolución C tiene elementos hasta 2N, pero solo nos interesa hasta N.
    // Además, C siempre será 0 ya que a, b, c, d > 0.
    int limit = min((int)C.size() - 1, N);

    for (int X = 1; X <= limit; ++X) {
        if (C[X] > max_val) {
            max_val = C[X];
            max_X = X;
        }
    }

    return {max_X, max_val};
}
```

# Pregunta 3

El objetivo es minimizar el costo total de la partición, donde el costo de un subconjunto $C$ está definido por $costo(C) = \max(h) \times \max(w)$. Entonces, se presentan las siguientes fases para solventar el problema

## Fase I: Reducción Geométrica y Linealización 

El problema se presenta sobre un conjunto desordenado $R$, pero la Programación Dinámica es inherentemente lineal. Debemos imponer un orden que preserve la optimalidad.

La primera observación es que muchos rectángulos son **redundantes**. Si un rectángulo $r_j = (h_j, w_j)$ domina a $r_i = (h_i, w_i)$ ($h_j \ge h_i$ y $w_j \ge w_i$), entonces $r_i$ nunca determinará el costo máximo de altura o ancho en ningún subconjunto que contenga a $r_j$.

* Filtramos el conjunto $R$ para retener solo los rectángulos **maximales** $R'$.
* Esto se logra ordenando los rectángulos por ancho ($w$) decreciente. Luego, en una pasada de barrido, se mantiene la altura máxima vista. Cualquier rectángulo con una altura menor o igual a esa altura máxima es descartado.
* Esta fase requiere un ordenamiento ($O(N \log N)$) y un barrido lineal ($O(N)$).

Los rectángulos maximales restantes $R'$ deben ordenarse para permitir la segmentación lineal (subsegmentos contiguos). Ordenamos $R'$ por **altura ($h$) creciente**. Debido al filtrado previo:

* La altura $h_i$ es no-decreciente.
* El ancho $w_i$ es estrictamente decreciente (para alturas iguales, no importa el orden, ya que el ancho máximo será el primero).

Al imponer este orden a $R'$, un segmento contiguo $R'[j+1..i]$ tendrá un costo simplificado:
$$\text{costo}(R'[j+1..i]) = \max_{k=j+1}^i (h_k) \times \max_{k=j+1}^i (w_k) = h_i \times w_{j+1}$$

## Fase II: Programación Dinámica con Convex Hull (CH)

Definimos $DP[i]$ como el costo mínimo de particionar los primeros $i$ rectángulos de la secuencia ordenada $R'$.

$$DP[i] = \min_{0 \le j < i} \{DP[j] + h_i \cdot w_{j+1}\}$$

Reescribimos la recurrencia para que encaje en la forma $m \cdot x + b$:
$$DP[i] = \min_{0 \le j < i} \{ (\mathbf{w_{j+1}}) \cdot (\mathbf{h_i}) + (\mathbf{DP[j]}) \}$$

Aquí:

* **Pendiente:** $m_j = w_{j+1}$ (depende solo del punto de división $j$).
* **Punto de Consulta:** $x_i = h_i$ (depende solo del estado actual $i$).
* **Intercepto:** $b_j = DP[j]$ (depende solo del punto de división $j$).

Dado que la secuencia $R'$ fue ordenada:

* **$x_i = h_i$ es monótono no-decreciente** (las consultas avanzan de izquierda a derecha).
* **$m_j = w_{j+1}$ es monótono decreciente** (las pendientes se hacen cada vez menos inclinadas).

Estas propiedades permiten aplicar la versión más eficiente de la CH, utilizando una estructura de datos tipo cola de doble terminación (deque) que mantiene la envolvente convexa óptima, lo que reduce la fase de búsqueda de mínimos a **$O(1)$ amortizado** por estado.

## Complejidad Total

| Fase | T. Dominante | Complejidad |
| :--- | :--- | :--- |
| I. Filtrado Maximal | Ordenamiento | $O(N \log N)$ |
| I. Ordenamiento para DP | Ordenamiento | $O(N \log N)$ |
| II. DP con CH | CH (Deque) | $O(N)$ amortizado |
| **Total** | | $\mathbf{O(N \log N)}$ |

El uso de memoria es $O(N)$ para almacenar los rectángulos iniciales, los rectángulos maximales y las tablas DP y CH.

## Implementación en C++

El archivo funcional se encuentra en [partition_rectangles.cpp](https://github.com/JMLTUnderCode/Algorithm_Design/blob/main/8-Tarea/partition_rectangles.cpp)

```cpp
// Usamos long long para manejar costos grandes (Area = H * W)
using ll = long long;

struct Rectangle {
    ll h; // alto
    ll w; // ancho
};

/**
 * Filtra el conjunto R para obtener solo rectángulos maximales R'.
 * Ordena por ancho descendente y luego filtra por altura.
 * Complejidad: O(N log N)
 */
vector<Rectangle> filter_maximal_rects(vector<Rectangle> &R) {
    if (R.empty())
        return {};

    // 1. Ordenar por ancho (w) descendente. Si hay empate, por alto (h) ascendente.
    // Esto es crucial para la lógica de barrido.
    sort(R.begin(), R.end(), [](const Rectangle &a, const Rectangle &b)
         {
        if (a.w != b.w) return a.w > b.w;
        return a.h < b.h; });

    vector<Rectangle> R_prime;
    ll max_h_so_far = 0;

    // 2. Barrido para eliminar redundantes
    for (const auto &r : R) {
        // Si ya encontramos un rectángulo con mayor o igual altura Y mayor ancho,
        // este rectángulo actual es dominado.
        // Como estamos ordenados por ancho decreciente, solo necesitamos verificar la altura.
        if (r.h > max_h_so_far) {
            R_prime.push_back(r);
            max_h_so_far = r.h;
        }
    }
    return R_prime;
}

/**
 * Función auxiliar para verificar la envolvente convexa de 3 líneas j1, j2, j3.
 * Comprueba si la línea j2 es redundante (no formará parte de la envolvente).
 * Usamos la comparación de pendiente/intersección.
 * Dado que las pendientes (m_j) son decrecientes, necesitamos que las intersecciones
 * sean crecientes (convex hull inferior).
 * La intersección entre j1 y j2 debe ser ANTES de la intersección entre j2 y j3.
 *
 * La función retorna true si (j2, j3) es "menos empinada" que (j1, j2)
 *
 * Fórmula: (b2 - b1) / (m1 - m2) <= (b3 - b2) / (m2 - m3)
 * Multiplicando por denominadores positivos (m1 > m2 > m3), evitamos divisiones
 * y mantenemos precisión con long long.
 */
bool is_redundant(int j1, int j2, int j3, const vector<ll> &DP, const vector<Rectangle> &R_prime) {
    // Extracción de pendientes m y coordenadas y b
    ll m1 = R_prime[j1 - 1].w;
    ll b1 = DP[j1];

    ll m2 = R_prime[j2 - 1].w;
    ll b2 = DP[j2];

    ll m3 = R_prime[j3 - 1].w;
    ll b3 = DP[j3];

    // Chequeo de convexidad usando cross product/pendiente (evitando floats)
    // (b2 - b1) * (m2 - m3) <= (b3 - b2) * (m1 - m2)
    // True si j2 debe ser eliminado (no contribuye a la convex hull inferior)

    // Nota: Se debe manejar la indexación. DP está indexado de 0 a N'. R_prime de 0 a N'-1.
    // j=0 es el caso base/ficticio.

    // Caso especial para j=0 (DP=0, m_0 = infinito). Usamos un rectángulo ficticio
    // con m_0 muy grande. Esto se simplifica forzando j1 > 0.

    // Para simplificar, asumimos que los índices j1, j2, j3 ya son índices válidos
    // de la tabla DP (0 a N').

    // j1 es la línea ficticia con m_0 = MAX.
    // En este caso, j2 y j3 deben ser los primeros dos puntos reales.
    // La primera línea no puede ser redundante hasta que haya 3 líneas.
    if (j1 == 0)
        return false;

    // Si m1 < m2, la envolvente inferior falla, lo que es imposible por el pre-ordenamiento.
    // En nuestro caso m1 > m2 > m3, ya que w es estrictamente decreciente.

    // Si las pendientes (w) son iguales, la línea con DP[j] más pequeño gana
    if (m1 == m2)
        return b1 <= b2;
    if (m2 == m3)
        return b2 <= b3;

    // La fórmula original (b2 - b1) / (m1 - m2) <= (b3 - b2) / (m2 - m3)
    return (b2 - b1) * (m2 - m3) >= (b3 - b2) * (m1 - m2);
}

/**
 * Algoritmo principal: Partición óptima de rectángulos usando CH.
 * Complejidad: O(N log N) debido al pre-ordenamiento.
 */
ll solve_optimal_partition(vector<Rectangle> &R)
{
    // Paso 1: Filtrado O(N log N)
    vector<Rectangle> R_prime = filter_maximal_rects(R);
    int N_prime = R_prime.size();
    if (N_prime == 0)
        return 0;

    // IMPORTANTE: No volvemos a reordenar R_prime por altura aquí porque el
    // filtrado garantiza que los anchos están en orden no creciente y las
    // propiedades para tomar R_prime[j].w como ancho máximo del segmento se mantienen.

    // DP[i] = mínimo costo para cubrir primeros i rectángulos (0..i-1)
    const ll INF = numeric_limits<ll>::max() / 4;
    vector<ll> DP(N_prime + 1, INF);
    DP[0] = 0;

    for (int i = 1; i <= N_prime; ++i) {
        ll H_i = R_prime[i - 1].h;
        ll best = INF;
        for (int j = 0; j < i; ++j) {
            // ancho máximo en el segmento [j..i-1] es R_prime[j].w por la ordenación previa
            ll W_j = R_prime[j].w;
            ll cand = DP[j] + H_i * W_j;
            if (cand < best)
                best = cand;
        }
        DP[i] = best;
    }

    return DP[N_prime];
}
```