#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * @brief Estructura para representar las coordenadas de un punto.
 */
struct Point
{
    int x, y;
};

// Variables globales para el contexto del problema
int N_Bags;          // Número de maletas (n)
const int INF = 1e9; // Constante para infinito
vector<Point> P;     // Puntos del aeropuerto. P[0] es el avión.
vector<int> memo;    // Tabla de memoización DP[S]

/**
 * @brief Calcula el cuadrado de la distancia cartesiana (tiempo de tránsito).
 *
 * Se asume que esta operación es O(1).
 * @param p1 Primer punto.
 * @param p2 Segundo punto.
 * @return ll El cuadrado de la distancia.
 */
int squared_distance(Point p1, Point p2)
{
    return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
}

/**
 * @brief Calcula el tiempo mínimo para recoger las maletas definidas por la máscara S.
 *
 * Implementa la Programación Dinámica Top-Down con Bitmasking.
 * Complejidad temporal total: O(N * 2^N).
 *
 * @param S Máscara de bits que representa el subconjunto de maletas ya recogidas (1-indexed).
 * @return ll Tiempo mínimo.
 */
int solve_dp(int S)
{
    // Caso Base: Si no quedan maletas por recoger (S=0), el tiempo es 0.
    if (S == 0)
        return 0;

    // Consulta la tabla de memoización (Memoization)
    if (memo[S] != -1)
        return memo[S];

    int min_time = INF;

    // 1. Identificar la maleta de referencia (i).
    // Buscamos el bit menos significativo (LSB) encendido, que corresponde a la
    // maleta de menor índice que aún no ha sido considerada en el subproblema.
    // Esto asegura que cada estado S solo se resuelva una vez con una maleta de inicio fija,
    // manteniendo la complejidad en O(N * 2^N).

    int LSB_mask = S & -S;
    // log2(LSB_mask) nos da el índice del bit (0-based)
    int i_bit_idx = __builtin_ctz(LSB_mask);

    // i_maleta es el índice real de la maleta (1-based index)
    int i_maleta = i_bit_idx + 1;

    // Máscara sin la maleta i
    int S_without_i = S ^ LSB_mask;

    // Costo de ida y vuelta al avión (P) para la maleta i
    int cost_0_i = squared_distance(P[0], P[i_maleta]);

    // 2. Opción 1: Recoger la maleta i sola. (Viaje: P -> P[i] -> P)
    // Costo total = tiempo anterior + costo del viaje 0->i->0
    int cost_single = cost_0_i + cost_0_i;
    min_time = min(min_time, solve_dp(S_without_i) + cost_single);

    // 3. Opción 2: Recoger la maleta i junto con otra maleta j (j > i)
    // Recorremos todas las otras maletas j cuyo bit esté encendido en S_without_i.

    for (int j_bit_idx = 0; j_bit_idx < N_Bags; ++j_bit_idx)
    {
        // Comprobar si el bit j está encendido en la máscara S_without_i
        // y si j_bit_idx es mayor que i_bit_idx (para evitar simetría y redundancia en el bucle)
        if ((S_without_i & (1 << j_bit_idx)))
        {
            int j_maleta = j_bit_idx + 1;

            // Máscara sin la maleta i ni la maleta j
            int S_without_i_j = S_without_i ^ (1 << j_bit_idx);

            int cost_0_j = squared_distance(P[0], P[j_maleta]);
            int cost_i_j = squared_distance(P[i_maleta], P[j_maleta]);

            // Costo del viaje doble: P -> P[i] -> P[j] -> P.
            // Debido a la simetría de la distancia al cuadrado, el orden de recogida (i->j o j->i)
            // no altera el costo total del circuito.
            int cost_double_trip = cost_0_i + cost_i_j + cost_0_j;

            min_time = min(min_time, solve_dp(S_without_i_j) + cost_double_trip);
        }
    }

    // Almacenar el resultado (Memoization) y retornarlo.
    return memo[S] = min_time;
}

// Función principal para resolver el problema (implementación de la Tarea 4, P4)
int solve_problem_4(const vector<Point> &bag_positions)
{
    int n = bag_positions.size();
    if (n == 0)
        return 0;

    N_Bags = n;

    // Punto (0, 0) es el avión.
    P.clear();
    P.push_back({0, 0});

    // P a P[n] son las maletas.
    P.insert(P.end(), bag_positions.begin(), bag_positions.end());

    int num_states = 1 << N_Bags;

    // Inicialización de la tabla de memoización con -1 (INF si usamos Bottom-Up)
    memo.assign(num_states, -1);

    // El estado final es cuando todos los bits están encendidos
    int final_mask = num_states - 1;

    return solve_dp(final_mask);
}

int main()
{
    // n=4 maletas. Avión en (0,0).
    // Maleta 1: (1, 0), Maleta 2: (0, 1), Maleta 3: (10, 0), Maleta 4: (0, 10)
    // Distancias al cuadrado: D(0,1)=1, D(0,2)=1, D(0,3)=100, D(0,4)=100
    // Distancia D(1,2) = 1+1 = 2
    // Distancia D(3,4) = 100+100 = 200

    vector<Point> bags = {{1, 0}, {0, 1}, {10, 0}, {0, 10}};
    int result = solve_problem_4(bags);
    // Análisis manual:
    // Opción 1: (1,2) juntas: Costo = 0->1(1) + 1->2(2) + 2->0(1) = 4. Restan {3,4}
    //           (3,4) juntas: Costo = 0->3(100) + 3->4(200) + 4->0(100) = 400.
    //           Total = 4 + 400 = 404.
    // Opción 2: (1,3) juntas: Costo = 0->1(1) + 1->3(81) + 3->0(100) = 182.
    //           (2,4) juntas: Costo = 0->2(1) + 2->4(81) + 4->0(100) = 182.
    //           Total = 182 + 182 = 364 (Optimal)

    cout << "Tiempo mínimo: " << result << endl; // Esperado: 364
    return 0;
}
