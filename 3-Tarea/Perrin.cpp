#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Definimos la matriz 3x3
typedef long long ll;
typedef vector<vector<ll>> Matrix;

// Tamaño de la matriz para la recurrencia de Perrin (3x3)
const int K = 3;

/**
 * @brief Multiplica dos matrices KxK (3x3).
 * La complejidad de esta operación es O(K^3) = O(1).
 * @param A Matriz A.
 * @param B Matriz B.
 * @return Matrix Resultado de A * B.
 */
Matrix multiply(const Matrix &A, const Matrix &B)
{
    Matrix C(K, vector<ll>(K, 0));
    for (int i = 0; i < K; ++i)
    {
        for (int j = 0; j < K; ++j)
        {
            for (int k = 0; k < K; ++k)
            {
                // Asumimos que las operaciones son O(1)
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

/**
 * @brief Calcula la potencia M^n usando exponenciación binaria (divide y vencerás).
 * La complejidad es O(K^3 * log n) = O(log n).
 * @param M Matriz base.
 * @param n Exponente.
 * @return Matrix Resultado de M^n.
 */
Matrix power(Matrix M, ll n)
{
    // Matriz identidad para inicializar el resultado
    Matrix R(K, vector<ll>(K, 0));
    for (int i = 0; i < K; ++i)
    {
        R[i][i] = 1;
    }

    while (n > 0)
    {
        if (n & 1)
        { // Si n es impar
            R = multiply(R, M);
        }
        M = multiply(M, M);
        n >>= 1; // n = n / 2
    }
    return R;
}

/**
 * @brief Calcula el n-ésimo número de Perrin, P(n), en tiempo O(log n).
 *
 * P(n) está definido por P(0)=3, P(1)=0, P(2)=2, P(n)=P(n-2)+P(n-3).
 * El cálculo usa la exponenciación de matrices: S_n = M^(n-2) * S_2.
 * @param n El índice del número de Perrin a calcular.
 * @return ll El valor de P(n).
 */
ll perrin(ll n)
{
    // Casos base (n=0, 1, 2)
    if (n == 0)
        return 3;
    if (n == 1)
        return 0;
    if (n == 2)
        return 2;

    // 1. Definir la matriz de transición M
    // M = {{0, 1, 1}, {1, 0, 0}, {0, 1, 0}}
    Matrix M = {
        {0, 1, 1},
        {1, 0, 0},
        {0, 1, 0}};

    // 2. Calcular M^(n-2)
    Matrix M_power = power(M, n - 2);

    // 3. Vector base S_2 = {P(2), P(1), P(0)} = {2, 0, 3}
    vector<ll> S_base = {2, 0, 3};

    // 4. Calcular S_n = M_power * S_2. P(n) es el primer elemento.
    // Multiplicación de matriz por vector: S_n = M_power * S_base
    ll P_n = 0;
    for (int i = 0; i < K; ++i)
    {
        P_n += M_power[0][i] * S_base[i];
    }

    return P_n;
}

// Función principal para prueba y validación
int main()
{
    int N;
    cout << "Ingrese el valor de N para calcular P(N): ";
    cin >> N;

    ll result = perrin(N);
    cout << "P(" << N << ") = " << result << endl;

    return 0;
}