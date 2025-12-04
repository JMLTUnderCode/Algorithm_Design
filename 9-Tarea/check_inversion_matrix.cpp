#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <numeric>
#include <iomanip>

using namespace std;

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
Vector MatrixVectorProduct(const Vector &X, const Matrix &A, int N)
{
    Vector R(N, 0);
    // Multiplicación X (1xN) * A (NxN) = R (1xN)
    for (int j = 0; j < N; ++j)
    { // columna de A / elemento de R
        for (int i = 0; i < N; ++i)
            R[j] += X[i] * A[i][j]; // fila de X / fila de A
    }
    return R;
}

/**
 * Algoritmo de Monte Carlo para verificar si B = A^-1 (A * B = I)
 * Retorna true si probablemente es la inversa, false si es definitivamente compuesta.
 */
bool CheckInversion(const Matrix &A, const Matrix &B, int N, double epsilon)
{

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

    for (int k = 1; k <= K; ++k)
    {
        // 1. Generar vector aleatorio X
        Vector X = GenerateRandomVector(N);

        // 2. Calcular R_AB = (X * A) * B
        Vector R_A = MatrixVectorProduct(X, A, N);
        Vector R_AB = MatrixVectorProduct(R_A, B, N);

        // 3. Comparar R_AB con X (deberían ser iguales si A*B = I)
        bool match = true;
        for (int i = 0; i < N; ++i)
        {
            if (R_AB[i] != X[i])
            {
                match = false;
                break;
            }
        }

        cout << "Iteracion " << k << "/" << K << ": Resultado: " << (match ? "COINCIDE (Probable)" : "NO COINCIDE (Compuesto)") << endl;

        if (!match)
        {
            cout << "\nCONCLUSION: B NO es la inversa de A (testigo encontrado)." << endl;
            return false;
        }
    }

    cout << "\nCONCLUSION: B es probablemente la inversa de A." << endl;
    return true;
}

void RunTest(int case_num, const string &description, const Matrix &A, const Matrix &B, int N, double epsilon, bool expected_result)
{
    cout << "\n==========================================" << endl;
    cout << "CASO DE PRUEBA " << case_num << ": " << description << endl;
    cout << "Dimension N: " << N << ", Error Epsilon: " << epsilon << endl;

    // Ejecutar el test
    bool result = CheckInversion(A, B, N, epsilon);

    cout << "\n--- Resumen del Caso de Prueba " << case_num << " ---" << endl;
    cout << "Conclusion del Test de Freivalds: "
         << (result ? "Probablemente Inversa (TRUE)" : "Definitivamente NO Inversa (FALSE)") << endl;
    cout << "Resultado Esperado (Teorico): "
         << (expected_result ? "TRUE (A*B = I)" : "FALSE (A*B != I)") << endl;

    if (result == expected_result)
    {
        cout << "Resultado Lógico: Éxito en la Detección." << endl;
    }
    else
    {
        // En un caso real (TRUE), el algoritmo solo fallaría si es un error Monte Carlo (muy raro).
        // En un caso NO INVERSA (FALSE), solo fallaría si falla al encontrar testigo (imposible si A*B!=I y N es grande).
        cout << "Resultado Lógico: Inconsistente con el resultado teórico esperado." << endl;
    }
}

// ===================================
// FUNCIÓN PRINCIPAL CON EJEMPLOS
// ===================================

int main()
{
    int N = 3;
    double EPSILON = 0.001; // Probabilidad de error máxima < 1/2^K. K >= 7.

    // ----------------------------------------------------------------
    // CASOS DONDE B ES INVERSA DE A (Resultado esperado: TRUE)
    // ----------------------------------------------------------------

    // Caso 1: Identidad x Identidad = Identidad
    Matrix A1 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix B1 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    RunTest(1, "B es inversa (A=I, B=I)", A1, B1, N, EPSILON, true);

    // Caso 2: Matriz triangular superior x su inversa
    Matrix A2 = {{1, 2, 3}, {0, 1, 4}, {0, 0, 1}};
    Matrix B2 = {{1, -2, 5}, {0, 1, -4}, {0, 0, 1}}; // Inversa de A2
    RunTest(2, "B es inversa (Matrices complejas)", A2, B2, N, EPSILON, true);

    // Caso 3: Matriz simple x su inversa
    Matrix A3 = {{2, 1, 0}, {-1, 0, 0}, {0, 0, 1}};
    Matrix B3 = {{0, -1, 0}, {1, 2, 0}, {0, 0, 1}}; // Inversa de A3
    RunTest(3, "B es inversa (Rotación simple)", A3, B3, N, EPSILON, true);

    // ----------------------------------------------------------------
    // CASOS DONDE B NO ES INVERSA DE A (Resultado esperado: FALSE)
    // ----------------------------------------------------------------

    // Caso 4: Una entrada incorrecta (A=I, B casi I)
    Matrix A4 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix B4 = {{1, 0, 0}, {0, 1, 0}, {0, 1, 1}}; // B[1][2] es 1, debería ser 0
    RunTest(4, "B NO es inversa (Diferencia de 1 entrada)", A4, B4, N, EPSILON, false);

    // Caso 5: A es singular (rango < N) y B=I. A*B != I.
    Matrix A5 = {{1, 2, 3}, {2, 4, 6}, {1, 1, 1}}; // Fila 2 = 2 * Fila 1
    Matrix B5 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    RunTest(5, "B NO es inversa (A es singular)", A5, B5, N, EPSILON, false);

    // Caso 6: Cerca, pero no. Pequeña diferencia con el Caso 3.
    Matrix A6 = {{2, 1, 0}, {-1, 0, 0}, {0, 0, 1}};
    Matrix B6 = {{0, -1, 0}, {1, 1, 0}, {0, 0, 1}}; // B[2] es 1, debería ser 2.
    RunTest(6, "B NO es inversa (Pequeña desviación)", A6, B6, N, EPSILON, false);

    cout << "\n==========================================" << endl;
    return 0;
}