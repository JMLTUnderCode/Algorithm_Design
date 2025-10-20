#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

// La restricción del problema (Tarea 4, Pregunta 2) establece que todas las operaciones
// aritméticas, incluyendo el cálculo del Máximo Común Divisor (GCD), se hacen en O(1).
// Incluimos una función GCD implementada de forma iterativa (Euclides) para cumplir con el requisito
// funcional de coprimidad, asumiendo su costo como O(1).
int calculate_gcd(int a, int b)
{
    while (b)
    {
        a %= b;
        swap(a, b);
    }
    return a;
}

/**
 * @brief Encuentra la longitud del par de subarreglos contiguos disjuntos
 *        y familiares más largos.
 *
 * Utiliza Programación Dinámica (Bottom-Up) con optimización de espacio O(n).
 *
 * @param A El arreglo de enteros positivos A[1..n].
 * @param n La longitud del arreglo.
 * @return int La longitud máxima (s+1).
 *
 * Complejidad temporal: O(n^2).
 * Complejidad espacial adicional: O(n).
 */
int longest_familiar_subarrays(const vector<int> &A)
{
    int n = A.size();
    // Caso base: Si n < 2, no pueden formarse pares familiares.
    if (n < 2)
        return 0;

    // Usamos A...A[n]. El vector A indexado a 0 debe tener tamaño n+1.
    // Creamos un vector interno que representa A[1..n].
    // Creamos una copia A_1_based para facilitar la lógica de A[i] donde i >= 1.
    vector<int> A_1_based(n + 1);
    for (int i = 0; i < n; ++i)
        A_1_based[i + 1] = A[i];

    // DP[j] almacenará el valor de DP[i-1][j] (de la fila anterior).
    // DP_row[j] es la longitud del par familiar más largo que termina en (i-1, j).
    // Inicialmente, todos son 0. Usamos tamaño n+1 para indexar 1 a n.
    // Esto cumple con la restricción de memoria adicional O(n).
    vector<int> DP_row(n + 1, 0);

    int max_length = 0;
    int new_length = 0;
    int prev_diag_val = 0;
    int current_diag_val = 0;
    int L_prime = 0;
    int potential_length = 0;

    // Iteramos 'i', el índice de terminación del primer subarreglo. (i = 1 a n)
    for (int i = 1; i - 1 < n; ++i)
    {
        // Variable temporal para almacenar DP[i-1][j-1] (dependencia diagonal).
        // Se inicializa a 0, que es DP[i-1][i-1].
        prev_diag_val = 0;

        // Iteramos 'j', el índice de terminación del segundo subarreglo. (j = i+1 a n)
        // Forzamos j > i para asegurar que los pares (i, j) sean únicos y en orden.
        for (int j = i + 1; j - 1 < n; ++j)
        {
            // 1. Guardamos el valor actual de DP_row[j] (que es DP[i-1][j]).
            //    Este valor se convertirá en la dependencia diagonal (DP[i-1][j]) para la
            //    próxima iteración de j (j+1) en la fila i (i.e., j_new = j+1, i_new=i+1).
            current_diag_val = DP_row[j];

            new_length = 0;

            // 2. Condición de Coprimidad: Chequeamos si A[i] y A[j] son coprimos.
            if (calculate_gcd(A_1_based[i], A_1_based[j]) == 1)
            {
                // L' es la longitud del par familiar que terminaba en (i-1, j-1).
                L_prime = prev_diag_val;
                potential_length = L_prime + 1;

                // 3. Condición de Disyunción: El nuevo par de longitud L debe ser disjunto.
                // El subarreglo termina en i. Si la longitud es L, comienza en i - L + 1.
                // Para que los subarreglos sean disjuntos, L debe ser a lo sumo la distancia entre ellos:
                // L <= j - i
                if (potential_length <= j - i)
                    new_length = potential_length;
            }

            // 4. Actualizamos el máximo global.
            max_length = max(max_length, new_length);

            // 5. Almacenamos el nuevo valor DP[i][j] en el arreglo DP_row[j] para la siguiente fila.
            DP_row[j] = new_length;

            // 6. Actualizamos prev_diag_val para la próxima iteración del bucle j.
            // DP[i-1][j] se convierte en DP[i-1][(j+1)-1].
            prev_diag_val = current_diag_val;
        }
    }

    return max_length;
}

int main()
{
    vector<int> A0 = {3, 10, 1, 8, 4, 5, 3, 6, 9, 2};
    cout << "Ejemplo 0: " << longest_familiar_subarrays(A0) << endl; // Esperado: 4

    vector<int> A1 = {3, 10, 1, 8, 4, 5, 3, 6, 9, 2};
    cout << "Ejemplo 1: " << longest_familiar_subarrays(A1) << endl; // Esperado:

    vector<int> A2 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    cout << "Ejemplo 2: " << longest_familiar_subarrays(A2) << endl; // Esperado:

    vector<int> A3 = {6, 6, 6, 6, 6, 6, 6, 6};
    cout << "Ejemplo 3: " << longest_familiar_subarrays(A3) << endl; // Esperado:

    vector<int> A4 = {5, 7, 9, 11, 13, 15, 17, 19, 21, 23};
    cout << "Ejemplo 4: " << longest_familiar_subarrays(A4) << endl; // Esperado:

    vector<int> A5 = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
    cout << "Ejemplo 5: " << longest_familiar_subarrays(A5) << endl; // Esperado:

    vector<int> A6 = {2, 4, 3, 6, 5, 1, 8};
    cout << "Ejemplo 6: " << longest_familiar_subarrays(A6) << endl; // Esperado:

    vector<int> A7 = {2, 3, 5, 7, 11, 13, 17, 19};
    cout << "Ejemplo 7: " << longest_familiar_subarrays(A7) << endl; // Esperado:

    vector<int> A8 = {1, 3, 5, 7, 9, 11, 13, 15};
    cout << "Ejemplo 8: " << longest_familiar_subarrays(A8) << endl; // Esperado:

    vector<int> A9 = {1, 2, 1, 2, 1, 2, 1, 2, 1};
    cout << "Ejemplo 9: " << longest_familiar_subarrays(A9) << endl; // Esperado:

    vector<int> A10 = {10, 21, 11, 22, 12, 23, 13, 24};
    cout << "Ejemplo 10: " << longest_familiar_subarrays(A10) << endl; // Esperado:

    return 0;
}