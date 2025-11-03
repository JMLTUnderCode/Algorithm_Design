#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <map>

using namespace std;

// Criba (vector dinámico). Se redimensiona en runtime según el mayor valor
// que necesitemos consultar (2 * c_max en el problema).
vector<int> sieve_array; // Array 0/1 para la criba (1 = Primo)

// =========================================================
// 1. Criba de Eratóstenes.
// =========================================================

/**
 * Implementación de la Criba de Eratóstenes.
 * Pre-calcula los primos hasta max_val. O(N log log N) donde N=max_val.
 * La consulta de primalidad resultante es O(1).
 */
void sieve(int max_val)
{
    if (max_val < 2)
        max_val = 2;

    // Reservar espacio para 0..max_val
    sieve_array.assign(max_val + 1, 1);
    sieve_array[0] = sieve_array[1] = 0; // 0 y 1 no son primos

    for (int p = 2; p * p <= max_val; p++)
    {
        if (sieve_array[p] == 1)
        {
            for (int i = p * p; i <= max_val; i += p)
                sieve_array[i] = 0; // Marca los múltiplos como no primos
        }
    }
}

/**
 * Función auxiliar para consultar si un número es primo. O(1) tiempo.
 */
bool is_prime(int v)
{
    if (v < 0 || v >= (int)sieve_array.size())
        return false;
    return sieve_array[v] == 1;
}

// =========================================================
// 2. Hopcroft-Karp para MCBM
// =========================================================

/**
 * Implementación del algoritmo Hopcroft-Karp para Apareamiento Bipartito Máximo (MCBM).
 * Complejidad: O(E * sqrt(V)) = O(n^2.5).
 */
namespace HopcroftKarp
{
    int V_L, V_R;            // Tamaños de los conjuntos L y R
    vector<vector<int>> Adj; // Lista de adyacencia del grafo bipartito
    vector<int> match_R;     // match_R[v] = el nodo en L emparejado con v en R
    vector<int> match_L;     // match_L[u] = el nodo en R emparejado con u en L
    vector<int> dist;        // Distancias usadas en BFS para caminos de aumento más cortos

    // Constantes para distancias
    const int NIL = 0;
    const int INF = 1000000000;

    // BFS: Fase 1 de Hopcroft-Karp. Encuentra caminos de aumento más cortos.
    bool bfs()
    {
        vector<int> Q;
        for (int u = 1; u <= V_L; u++)
        {
            if (match_L[u] == NIL)
            {
                dist[u] = 0;
                Q.push_back(u);
            }
            else
                dist[u] = INF;
        }
        dist[NIL] = INF;

        int head = 0;
        while (head < Q.size())
        {
            int u = Q[head++];
            if (dist[u] < dist[NIL])
            {
                for (int v : Adj[u])
                {
                    if (dist[match_R[v]] == INF)
                    {
                        dist[match_R[v]] = dist[u] + 1;
                        Q.push_back(match_R[v]);
                    }
                }
            }
        }
        // Retorna true si se encontró un camino de aumento (dist[NIL] != INF)
        return dist[NIL] != INF;
    }

    // DFS: Fase 2 de Hopcroft-Karp. Maximiza el número de caminos de aumento disjuntos.
    bool dfs(int u)
    {
        if (u != NIL)
        {
            for (int v : Adj[u])
            {
                // Buscamos un camino de aumento más corto
                if (dist[match_R[v]] == dist[u] + 1)
                {
                    if (dfs(match_R[v]))
                    {
                        // Invertimos el camino (toggle)
                        match_R[v] = u;
                        match_L[u] = v;
                        return true;
                    }
                }
            }
            dist[u] = INF;
            return false;
        }
        return true;
    }

    // Función principal que calcula el MCBM
    int max_matching(int n_L, int n_R, const vector<vector<int>> &graph_adj)
    {
        V_L = n_L;
        V_R = n_R;
        Adj = graph_adj;

        // Inicialización de estructuras
        match_L.assign(V_L + 1, NIL);
        match_R.assign(V_R + 1, NIL);
        dist.assign(V_L + 1, INF);

        int result = 0;
        // Repetir mientras existan caminos de aumento
        while (bfs())
        {
            for (int u = 1; u <= V_L; u++)
            {
                // Si u es libre e inicializa un camino de aumento en la capa más corta
                if (match_L[u] == NIL && dfs(u))
                    result++;
            }
        }
        return result;
    }
}

// =========================================================
// 3. Función Principal para resolver P4/T5
// =========================================================

// Usaremos esta función para encapsular el requerimiento de la tarea 5.
// El conjunto C está representado por un vector de enteros.
int min_removals(const vector<int> &C)
{
    int minimum_removals = 0;

    if (C.empty())
        return minimum_removals;

    // Paso 1a: Encontrar el máximo valor
    int c_max = *max_element(C.begin(), C.end());

    // Paso 1b: Criba de Eratóstenes hasta 2 * Cmax
    // No usamos un límite fijo: creamos la criba hasta 2 * c_max (como mínimo 2)
    int limit = max(2, 2 * c_max);
    sieve(limit);

    // Paso 2a: Separar C en conjuntos L (impares) y R (pares)
    vector<int> L_vals, R_vals;
    vector<int> map_to_index(2 * c_max + 1, 0); // Mapea valor a índice en L/R
    for (int x : C)
    {
        if (x % 2 != 0)
        {
            if (x == 1)             // Excluimos al 1 si está presente
                minimum_removals++; // Asegurarse de contar el 1 si está presente
            else
                L_vals.push_back(x);
        }
        else
            R_vals.push_back(x);
    }

    // Si la suma es prima (mayor que 2), debe ser impar, por lo tanto (Impar + Par).
    // El único primo par es 2. Si x+y=2, x=1, y=1, pero los elementos en C son distintos.
    // Si C contiene 1, 1 es impar. Si C contiene 2, 2 es par. 1+2=3 (Primo).
    // La bipartición en par/impar es fundamental.

    int n_L = L_vals.size();
    int n_R = R_vals.size();

    // Mapeamos los valores de L y R a índices 1-basados para Hopcroft-Karp
    // L: 1..n_L, R: 1..n_R. NIL (0) es para nodos no emparejados.

    vector<vector<int>> adj_hk(n_L + 1); // Lista de adyacencia solo para el conjunto L

    // Paso 2b: Construir aristas
    for (int i = 0; i < n_L; ++i)
    {
        for (int j = 0; j < n_R; ++j)
        {
            int sum = L_vals[i] + R_vals[j];
            // Si la suma es prima, añadimos una arista entre el nodo i+1 (en L)
            // y el nodo j+1 (en R)
            if (sum <= limit && is_prime(sum))
                adj_hk[i + 1].push_back(j + 1);
        }
    }

    // Paso 3: Calcular MCBM usando Hopcroft-Karp
    int mcbm_size = HopcroftKarp::max_matching(n_L, n_R, adj_hk);

    // Resultado: Mínimo de remociones = MVC = MCBM
    // NOTA: El problema pide la cantidad de números a *eliminar*.
    // El tamaño del MVC es igual al MCBM.
    // Si interpretamos el problema como encontrar el Máximo Conjunto Independiente (MIS)
    // y luego calcular n - MIS, en un grafo bipartito, MVC = MCBM, y |MIS| = |V| - |MVC|.
    // Aquí, V = |L| + |R| = n.
    // El número de elementos a ELIMINAR para romper todas las aristas "malas" es el MVC.

    // Resultado: Eliminaciones totales = 1 (si se eliminó el 1) + MCBM(C')
    minimum_removals += mcbm_size;

    return minimum_removals;
}

int main()
{
    // Crear un diccionario (map) de arreglos C (arreglos C dinámicos) de tamaños variables.
    // Usamos map<string, pair<int*, int>> donde el pair contiene (puntero_al_arreglo, tamaño)
    map<string, pair<int *, int>> dict;

    // Ejemplo 0: arreglo de tamaño 10
    int n0 = 10;
    int *arr0 = new int[n0];
    int vals0[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < n0; ++i)
        arr0[i] = vals0[i];
    dict["C0"] = make_pair(arr0, n0);

    // Ejemplo 1: arreglo de tamaño 4
    int n1 = 4;
    int *arr1 = new int[n1];
    int vals1[] = {1, 3, 2, 4};
    for (int i = 0; i < n1; ++i)
        arr1[i] = vals1[i];
    dict["C1"] = make_pair(arr1, n1);

    // Ejemplo 2: arreglo de tamaño 4
    int n2 = 4;
    int *arr2 = new int[n2];
    int vals2[] = {1, 9, 20, 30};
    for (int i = 0; i < n2; ++i)
        arr2[i] = vals2[i];
    dict["C2"] = make_pair(arr2, n2);

    // Ejemplo 3: arreglo de tamaño 3
    int n3 = 3;
    int *arr3 = new int[n3];
    int vals3[] = {1, 3, 2};
    for (int i = 0; i < n3; ++i)
        arr3[i] = vals3[i];
    dict["C3"] = make_pair(arr3, n3);

    // Ejemplo 4: arreglo de tamaño 2
    int n4 = 2;
    int *arr4 = new int[n4];
    int vals4[] = {1, 2};
    for (int i = 0; i < n4; ++i)
        arr4[i] = vals4[i];
    dict["C4"] = make_pair(arr4, n4);

    // Ejemplo 5: arreglo de tamaño 6
    int n5 = 6;
    int *arr5 = new int[n5];
    int vals5[] = {1, 3, 5, 7, 9, 11};
    for (int i = 0; i < n5; ++i)
        arr5[i] = vals5[i];
    dict["C5"] = make_pair(arr5, n5);

    // Ejemplo 6: arreglo de tamaño 9
    int n6 = 9;
    int *arr6 = new int[n6];
    int vals6[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < n6; ++i)
        arr6[i] = vals6[i];
    dict["C6"] = make_pair(arr6, n6);

    // Iterar por todos los arreglos C en el diccionario, aplicar min_removals y mostrar resultado
    for (auto &kv : dict)
    {
        const string &name = kv.first;
        int *arr = kv.second.first;
        int sz = kv.second.second;

        // Convertir arreglo C a vector<int> para pasar a la función existente
        vector<int> Cvec(arr, arr + sz);
        int result = min_removals(Cvec);

        cout << "Conjunto " << name << ": ";
        for (int i = 0; i < sz; ++i)
            cout << arr[i] << " ";
        cout << " -> Eliminaciones minimas (MVC): " << result << endl;

        // Liberar memoria del arreglo dinámico
        delete[] arr;
    }

    return 0;
}