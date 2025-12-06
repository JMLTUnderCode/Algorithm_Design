#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>

using namespace std;

// Definición de tipos para la matriz de adyacencia y el conjunto de vértices
typedef vector<vector<int>> AdjacencyList;
typedef pair<int, int> Edge;

/**
 * @brief Implementa el algoritmo de aproximación 2 para el Cubrimiento Mínimo de Vértices.
 *
 * Este algoritmo encuentra un Cubrimiento de Vértices (C) tal que |C| <= 2 * |C*|,
 * donde C* es el cubrimiento mínimo.
 *
 * @param V Número total de vértices (asumidos indexados de 0 a V-1).
 * @param edge_list Lista de todas las aristas del grafo.
 * @return vector<int> Conjunto de vértices que forman el cubrimiento aproximado.
 */
vector<int> approx_vertex_cover(int V, const vector<Edge> &edge_list)
{

    // El conjunto C almacenará los vértices seleccionados para el cubrimiento.
    vector<int> C;

    // 'covered_edge' rastrea si una arista ya ha sido cubierta por los vértices en C.
    // Usamos un set para un acceso rápido y eliminación lógica de aristas cubiertas.
    set<Edge> uncovered_edges(edge_list.begin(), edge_list.end());

    // 'is_in_C' marca qué vértices ya están en el conjunto de cubrimiento C.
    vector<bool> is_in_C(V, false);

    // Iterar mientras queden aristas sin cubrir.
    // Aunque el set 'uncovered_edges' cambia de tamaño, esta sigue siendo una aproximación polinomial.
    while (!uncovered_edges.empty())
    {

        // 1. Seleccionar una arista no cubierta arbitraria {u, v}
        Edge current_edge = *uncovered_edges.begin();
        int u = current_edge.first;
        int v = current_edge.second;

        // 2. Añadir ambos extremos al conjunto de cubrimiento C.

        // Solo agregar si el vértice no está ya en C para evitar duplicados en la salida.
        if (!is_in_C[u])
        {
            C.push_back(u);
            is_in_C[u] = true;
        }
        if (!is_in_C[v])
        {
            C.push_back(v);
            is_in_C[v] = true;
        }

        // 3. Marcar/eliminar las aristas incidentes a u y v como cubiertas.

        // Nota: La forma más eficiente de eliminar aristas incidentes
        // de un set requiere iteradores. Aquí, por simplicidad de implementación,
        // recreamos el set con solo las aristas que no están cubiertas por C.

        // Podríamos usar un iterador para eliminar aristas, pero para el prototipo:
        set<Edge> next_uncovered_edges;
        for (const auto &edge : uncovered_edges)
        {
            int x = edge.first;
            int y = edge.second;

            // Si ninguno de los extremos de la arista está en C, sigue sin cubrir.
            if (!(is_in_C[x] || is_in_C[y]))
            {
                next_uncovered_edges.insert(edge);
            }
        }
        uncovered_edges = next_uncovered_edges;
    }

    return C;
}

// --- Funciones de Utilidad para Mostrar Resultados ---

void print_cover_result(int case_num, const string &name, const vector<int> &cover, int N, int optimal_size)
{
    cout << "--- Caso " << case_num << ": " << name << " (N=" << N << ") ---" << endl;
    cout << "C. Vértices en Cubrimiento (1-base): {";
    for (size_t i = 0; i < cover.size(); ++i)
    {
        cout << cover[i] << (i == cover.size() - 1 ? "" : ", ");
    }
    cout << "}" << endl;
    cout << "Tamaño del Cubrimiento Aproximado |C|: " << cover.size() << endl;
    cout << "Tamaño del Cubrimiento Óptimo (C*): " << optimal_size << endl;
    cout << "Razón de Aproximación |C|/|C*|: " << fixed << setprecision(2)
         << (double)cover.size() / optimal_size << endl;
    cout << "------------------------------------------------" << endl;
}

// --- Casos de Prueba (MAIN FUNCTION) ---

int main()
{
    int N;
    vector<Edge> E;
    vector<int> C;

    cout << "\nEjecución de 6 Casos de Prueba para APPROX-VERTEX-COVER (2-Aproximacion)" << endl;
    cout << "=======================================================================" << endl;

    // --- Caso 1: Grafo de Camino (Path Graph P4) ---
    // Estructura simple. Se espera una solución óptima y exacta.
    N = 4;
    E = {{1, 2}, {2, 3}, {3, 4}};
    // C* = {2, 4} o {1, 3}. |C*|=2
    C = approx_vertex_cover(N, E);
    print_cover_result(1, "Grafo de Camino (P4)", C, N, 2);

    // --- Caso 2: Grafo Estrella (Star Graph K1,5) ---
    // C* = {1}. El algoritmo puede seleccionar {1, v} para la primera arista y terminar.
    // Esto ilustra que el algoritmo puede ser no óptimo para C*=1, alcanzando C=2 (Razón 2.0).
    N = 6;
    E = {{1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}};
    C = approx_vertex_cover(N, E);
    print_cover_result(2, "Grafo Estrella (K1,5)", C, N, 1);

    // --- Caso 3: Ciclo Impar (Odd Cycle C5) ---
    // Un ciclo impar no es bipartito. La aproximación |C|/|C*| aquí es 4/3.
    N = 5;
    E = {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 1}};
    // C* = {1, 3, 5} o {2, 4, 5}. |C*|=3
    C = approx_vertex_cover(N, E);
    print_cover_result(3, "Ciclo Impar (C5)", C, N, 3);

    // --- Caso 4: Grafo Completo (Complete Graph K4) ---
    // Grafo denso. Se necesita N-1 vértices para cubrir N(N-1)/2 aristas.
    N = 4;
    E = {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
    // C* = {1, 2, 3} o {1, 2, 4}, etc. |C*|=3
    C = approx_vertex_cover(N, E);
    print_cover_result(4, "Grafo Completo (K4)", C, N, 3);

    // --- Caso 5: Grafo Bipartito (Worst Case: K3,3) ---
    // Se espera que el algoritmo alcance el peor caso de aproximación: Razón 2.0.
    // OPTIMAL por Teorema de König: |M*| = |C*|. |M*|=3, por lo que |C*|=3.
    N = 6;
    E = {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}};
    C = approx_vertex_cover(N, E);
    print_cover_result(5, "Grafo Bipartito K3,3 (Peor Caso)", C, N, 3);

    // --- Caso 6: Dos Componentes Desconectadas (K3 + K3) ---
    // Prueba que el algoritmo funciona correctamente sobre grafos no conexos,
    // donde la suma de los cubrimientos de sus componentes debe ser la solución.
    N = 6;
    E = {{1, 2}, {2, 3}, {3, 1}, {4, 5}, {5, 6}, {6, 4}};
    // C* = {1, 2, 4, 5} o similar. |C*|=4
    C = approx_vertex_cover(N, E);
    print_cover_result(6, "Grafo Disconexo (K3 + K3)", C, N, 4);

    return 0;
}