#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <deque>
#include <limits>

using namespace std;

// Usamos long long para manejar costos grandes (Area = H * W)
using ll = long long;

struct Rectangle
{
    ll h; // alto
    ll w; // ancho
};

/**
 * Filtra el conjunto R para obtener solo rectángulos maximales R'.
 * Ordena por ancho descendente y luego filtra por altura.
 * Complejidad: O(N log N)
 */
vector<Rectangle> filter_maximal_rects(vector<Rectangle> &R)
{
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
    for (const auto &r : R)
    {
        // Si ya encontramos un rectángulo con mayor o igual altura Y mayor ancho,
        // este rectángulo actual es dominado.
        // Como estamos ordenados por ancho decreciente, solo necesitamos verificar la altura.
        if (r.h > max_h_so_far)
        {
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
bool is_redundant(int j1, int j2, int j3, const vector<ll> &DP, const vector<Rectangle> &R_prime)
{
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

    for (int i = 1; i <= N_prime; ++i)
    {
        ll H_i = R_prime[i - 1].h;
        ll best = INF;
        for (int j = 0; j < i; ++j)
        {
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

int main()
{
    vector<pair<int, vector<Rectangle>>> diccionary_example = {
        {1, {{1, 10}, {10, 1}, {5, 5}}},                        // 45
        {2, {{10, 10}, {1, 1}, {5, 5}, {20, 2}, {2, 12}}},      // 160
        {3, {{10, 5}, {2, 20}, {5, 10}}},                       // 140
        {4, {{2, 2}, {1, 1}, {3, 3}, {4, 4}}},                  // 16
        {5, {{1, 100}, {100, 1}}},                              // 200
        {6, {{10, 12}, {12, 10}}},                              // 144
        {7, {{5, 8}, {4, 9}, {10, 3}, {2, 20}, {2, 20}}},       // 115
        {8, {{4, 9}, {5, 8}, {10, 3}}},                         // 75
        {9, {{1, 10}, {2, 9}, {3, 8}, {4, 7}, {5, 6}}},         // 50
        {10, {{3, 4}, {2, 6}, {5, 1}, {4, 5}, {6, 2}, {1, 7}}}, // 40
    };

    for (auto example : diccionary_example)
    {
        cout << "\nCaso " << example.first << ": N=" << example.second.size() << " rectangulos." << endl;
        ll res = solve_optimal_partition(example.second);
        cout << "Costo total minimo de particion: " << res << endl;
    }

    return 0;
}