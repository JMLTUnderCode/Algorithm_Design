#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <complex>

using namespace std;

// Definición de tipos para el FFT
using CD = complex<long double>;
const long double PI = acos(-1.0L);

/**
 * Precalcula F[i], el número de divisores de i, para 1 <= i <= N.
 * Esta es la cantidad de formas de escribir i = a * b (a, b > 0).
 * Complejidad: O(N log N)
 */
vector<int> calculate_divisor_counts(int N)
{
    vector<int> F(N + 1, 0);
    // Itera sobre los posibles factores 'a' y marca sus múltiplos 'Y'
    for (int a = 1; a <= N; ++a)
    {
        for (int Y = a; Y <= N; Y += a)
            F[Y]++;
    }
    return F;
}

/**
 * Implementación de la Transformada Rápida de Fourier (FFT).
 * time: O(N log N)
 */
void fft(vector<CD> &a, bool invert)
{
    int n = a.size();
    if (n == 1)
        return;

    // Implementación sin recursión explícita (bit reversal permutation)
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        long double ang = 2 * PI / len * (invert ? -1 : 1);
        CD wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len)
        {
            CD w(1);
            for (int j = 0; j < len / 2; j++)
            {
                CD u = a[i + j];
                CD v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert)
    {
        for (CD &x : a)
            x /= n;
    }
}

/**
 * Realiza la multiplicación de polinomios P * P (convolución).
 * Retorna el vector C, donde C[X] = decomp(X).
 * time: O(N log N)
 */
vector<long long> multiply_polynomial_self(const vector<int> &F, int N)
{
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
pair<int, long long> solve_max_decomposition(int N)
{
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

    for (int X = 1; X <= limit; ++X)
    {
        if (C[X] > max_val)
        {
            max_val = C[X];
            max_X = X;
        }
    }

    return {max_X, max_val};
}

// Interfaz para demostrar la funcionalidad
int main()
{
    vector<int> F_example = {9999, 1000, 100, 50, 25, 10, 5, 2, 1};

    cout << fixed << setprecision(0);
    for (int i = 0; i < F_example.size(); ++i)
    {
        int n = F_example[i];
        auto result = solve_max_decomposition(n);
        cout << "\n--- Solucion para N = " << n << " ---" << endl;
        cout << "Maximo valor de decomp(X): " << result.second << endl;
        cout << "Alcanzado en X = " << result.first << endl;
    }
    cout << endl;

    return 0;
}