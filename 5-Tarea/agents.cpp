#include <iostream>
#include <vector>

using namespace std;

// Constantes y estructuras auxiliares para la detección de puentes (Bridges)
typedef vector<vector<int>> AdjList;

// Variables globales para el DFS
int timer_counter;
vector<int> dfs_num;  // Tiempo de descubrimiento (prenum)
vector<int> dfs_low;  // Menor prenum alcanzable desde el subárbol
vector<bool> visited; // Para el chequeo de conectividad
int total_vis;        // Total visitados en DFS.
int num_bridges;

void find_bridges(int u, int p, const AdjList &G)
{
    visited[u] = true;
    total_vis++;
    dfs_num[u] = dfs_low[u] = timer_counter++;

    for (int v : G[u])
    {
        if (v == p)
            continue; // Ignorar la arista de vuelta al padre

        // Caso 1: Vértice 'v' ya visitado (back edge)
        // Actualizamos dfs_low[u] con el tiempo de descubrimiento (dfs_num) de 'v'
        if (dfs_num[v] != -1)
            dfs_low[u] = min(dfs_low[u], dfs_num[v]);
        else
        {
            // Caso 2: Vértice 'v' no visitado (tree edge)
            find_bridges(v, u, G);

            // Después de la llamada recursiva, actualizamos dfs_low[u]
            // con el valor más bajo alcanzado por 'v'
            dfs_low[u] = min(dfs_low[u], dfs_low[v]);

            // Detección de Puente: si el subárbol de 'v' no puede alcanzar
            // a un ancestro estricto de 'u' (o 'u' mismo)
            if (dfs_low[v] > dfs_num[u])
                num_bridges++; // Si dfs_low[v] > dfs_num[u], la arista (u, v) es un puente
        }
    }
}

bool isStronglyConnectable(int N, const vector<pair<int, int>> &E)
{
    if (N <= 1)
        return true; // Si hay 0 o 1 agente, trivialmente es SC.

    // 1. Construcción del Grafo (Lista de Adyacencia)
    AdjList G(N);
    for (const auto &edge : E)
    {
        G[edge.first].push_back(edge.second);
        G[edge.second].push_back(edge.first);
    }

    // 2. Inicialización de estructuras auxiliares
    timer_counter = 0;
    num_bridges = 0;
    total_vis = 0;
    // -1 indica UNVISITED, usado para inicializar dfs_num y asegurar el chequeo de conectividad
    dfs_num.assign(N, -1);
    dfs_low.assign(N, 0);
    visited.assign(N, false);

    // 3. Ejecutar DFS modificado (empezamos desde el nodo 0)
    find_bridges(0, -1, G);

    // 4. Verificación de Conexidad y Cero Puentes (Biconexidad de Aristas)
    // Por el Teorema de Robbins, si es conexo y no tiene puentes, admite orientación SC.
    if (num_bridges == 0 && total_vis == N)
        return true;
    else
        return false;
}

int main()
{
    int N = 5; // Número de agentes
    vector<pair<int, int>> E = {
        {0, 1},
        {1, 2},
        {2, 0},
        {1, 3},
        {3, 4},
        {4, 1}};
    bool result = isStronglyConnectable(N, E);
    if (result)
        cout << "El grafo puede ser orientado para ser fuertemente conexo.\n";
    else
        cout << "El grafo NO puede ser orientado para ser fuertemente conexo.\n";

    N = 5;
    E = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4}}; // Este grafo tiene puentes
    result = isStronglyConnectable(N, E);
    if (result)
        cout << "El grafo puede ser orientado para ser fuertemente conexo.\n";
    else
        cout << "El grafo NO puede ser orientado para ser fuertemente conexo.\n";

    return 0;
}