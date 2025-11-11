#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>

using namespace std;

// Definiciones de constantes
const int MAX_NODES = 200000 * 20; // Aproximadamente N * log N * 4 para nodos del ST Persistente
const int MAX_N_ARR = 200000 + 5;

// =========================================================
// 1. Segment Tree Persistente (PST)
// =========================================================

// Estructura de un nodo del Árbol de Segmentos Persistente
// Solo necesitamos almacenar el conteo total de elementos en el subárbol
struct Node
{
    int count;                   // Frecuencia de valores en este rango [L, R]
    int left_child, right_child; // Índice del nodo hijo en el array global
};

Node tree[MAX_NODES];
int root[MAX_N_ARR]; // Array de raíces: root[i] almacena la raíz de la versión para A[1..i]
int node_counter;    // Contador global para asignar índices de nodos

// Rango de valores (después de la compresión de coordenadas)
int N_compressed;
// Vector global para la descompresión (índice comprimido -> valor original)
vector<int> decompressed_values;

// Función para construir el Segment Tree inicial (versión 0, vacío)
void build_pst(int p, int L, int R)
{
    if (L == R)
    {
        // Hoja: representa un valor único
        tree[p].count = 0;
        return;
    }
    int mid = L + (R - L) / 2;
    tree[p].left_child = ++node_counter;
    tree[p].right_child = ++node_counter;
    build_pst(tree[p].left_child, L, mid);
    build_pst(tree[p].right_child, mid + 1, R);
    tree[p].count = 0;
}

// Función para actualizar el Segment Tree (añadir una ocurrencia) y hacerlo persistente
// Toma la raíz de la versión anterior (prev_root) y el valor a insertar (val)
int update_pst(int prev_root, int L, int R, int val)
{
    // 1. Creamos un nuevo nodo (copia de camino)
    int new_root = ++node_counter;
    tree[new_root] = tree[prev_root];
    tree[new_root].count++; // Aumentamos la cuenta en el nuevo nodo

    if (L == R)
    {
        return new_root;
    }

    int mid = L + (R - L) / 2;

    // Si el valor está a la izquierda
    if (val <= mid)
    {
        // Copiamos el hijo derecho sin cambios (compartición de nodos)
        tree[new_root].right_child = tree[prev_root].right_child;

        // Actualizamos recursivamente el hijo izquierdo
        tree[new_root].left_child = update_pst(tree[prev_root].left_child, L, mid, val);
    }
    // Si el valor está a la derecha
    else
    {
        // Copiamos el hijo izquierdo sin cambios
        tree[new_root].left_child = tree[prev_root].left_child;

        // Actualizamos recursivamente el hijo derecho
        tree[new_root].right_child = update_pst(tree[prev_root].right_child, mid + 1, R, val);
    }

    return new_root;
}

// Función de consulta: busca el k-ésimo elemento en el rango virtual [root_j] - [root_i_minus_1]
// Devuelve el valor (índice comprimido) del k-ésimo elemento
int query_kth(int root_j, int root_i_minus_1, int L, int R, int k)
{
    if (L == R)
    {
        // Hemos llegado a la hoja, el valor es L
        return L;
    }

    // Calculamos la diferencia de frecuencias en el hijo izquierdo (rango [L, mid] de valores)
    // count_L_j = frecuencia de valores en [L, mid] en el prefijo A[1..j]
    int count_L_j = tree[tree[root_j].left_child].count;

    // count_L_i_minus_1 = frecuencia de valores en [L, mid] en el prefijo A[1..i-1]
    int count_L_i_minus_1 = tree[tree[root_i_minus_1].left_child].count;

    // count_L_range = Frecuencia de valores en [L, mid] para el rango A[i..j]
    int count_L_range = count_L_j - count_L_i_minus_1;

    int mid = L + (R - L) / 2;

    if (k <= count_L_range)
    {
        // El k-ésimo elemento se encuentra en el rango de valores izquierdo [L, mid]
        return query_kth(tree[root_j].left_child, tree[root_i_minus_1].left_child, L, mid, k);
    }
    else
    {
        // El k-ésimo elemento se encuentra en el rango de valores derecho [mid+1, R]
        // Ajustamos k restando los elementos que ya contamos en el lado izquierdo.
        return query_kth(tree[root_j].right_child, tree[root_i_minus_1].right_child, mid + 1, R, k - count_L_range);
    }
}

// =========================================================
// 2. Lógica de Compresión y Preprocesamiento
// =========================================================

// Almacenamiento del arreglo original
int A_original[MAX_N_ARR];
int N_arr;

// Función que maneja la compresión de coordenadas y la construcción del PST
void preprocess_data()
{
    // Los valores de A son la clave para la compresión
    vector<int> sorted_values(A_original + 1, A_original + N_arr + 1);

    // 1. Ordenar y obtener valores únicos
    sort(sorted_values.begin(), sorted_values.end());
    sorted_values.erase(unique(sorted_values.begin(), sorted_values.end()), sorted_values.end());
    N_compressed = sorted_values.size();

    // Guardamos los valores ordenados para la descompresión global
    decompressed_values = sorted_values;

    // 2. Inicialización del PST (versión 0 -> root[0])
    // node_counter empieza en 0; el primer nodo será 1
    node_counter = 0;
    root[0] = ++node_counter;
    build_pst(root[0], 1, N_compressed);

    // 3. Construir N versiones persistentes (O(N log N))
    for (int i = 1; i <= N_arr; ++i)
    {
        // Encontramos el índice comprimido del valor A_original[i]
        int val_compressed = lower_bound(sorted_values.begin(), sorted_values.end(), A_original[i]) - sorted_values.begin() + 1;

        // Creamos la versión R_i a partir de R_{i-1}
        root[i] = update_pst(root[i - 1], 1, N_compressed, val_compressed);
    }
}

// =========================================================
// 3. Función Main: Interfaz Metodológica
// =========================================================

// Función auxiliar para obtener el k-ésimo valor
int get_kth_element(int i, int j, int k)
{
    if (i > j || k <= 0)
        return -1; // Error o rango inválido

    // Los índices i y j en la consulta son 1-based.

    // 1. Obtener los rangos de las raíces persistentes
    int root_j = root[j];
    int root_i_minus_1 = root[i - 1];

    // 2. Realizar la consulta O(log N)
    int compressed_rank = query_kth(root_j, root_i_minus_1, 1, N_compressed, k);

    // 3. Descompresión de Coordenadas
    // Usamos el vector global 'decompressed_values' creado en preprocess_data
    if (compressed_rank < 1 || compressed_rank > (int)decompressed_values.size())
        return -1;
    return decompressed_values[compressed_rank - 1]; // compressed_rank es 1-based
}

int main()
{
    // --- ENTRADA DEL ARREGLO ORIGINAL ---
    cout << "Ingrese el tamaño del arreglo N: ";
    if (!(cin >> N_arr) || N_arr <= 0)
        return 0;

    cout << "Ingrese los " << N_arr << " elementos del arreglo (separados por espacio):\n";
    for (int i = 1; i <= N_arr; ++i)
    {
        cin >> A_original[i];
    }

    // --- FASE DE PREPROCESAMIENTO ---
    preprocess_data();
    cout << "\n--- Preprocesamiento completado. Memoria O(N log N) ---\n";

    // --- CONSULTAS ---
    int Q;
    cout << "Ingrese el número de consultas Q: ";
    cin >> Q;

    cout << "\nIngrese las consultas seleccion(i, j, k) [Índices 1-based]:\n";

    for (int q_count = 1; q_count <= Q; ++q_count)
    {
        int i, j, k;
        cout << "Consulta " << q_count << " (i, j, k): ";
        if (!(cin >> i >> j >> k))
            break;

        cout << "------------------------------------------\n";
        cout << "Consulta: seleccion(" << i << ", " << j << ", " << k << ")\n";

        // 1. Mostrar subarreglo elegido
        cout << "Subarreglo elegido A[" << i << ".." << j << "]: [";
        vector<int> subarray;
        for (int idx = i; idx <= j; ++idx)
        {
            if (idx >= 1 && idx <= N_arr)
            {
                subarray.push_back(A_original[idx]);
                cout << A_original[idx] << (idx < j ? ", " : "");
            }
        }
        cout << "]\n";

        // 2. Mostrar subarreglo ordenado (Simulación, solo para salida metodológica)
        vector<int> sorted_subarray = subarray;
        sort(sorted_subarray.begin(), sorted_subarray.end());

        cout << "Subarreglo ordenado: [";
        for (size_t idx = 0; idx < sorted_subarray.size(); ++idx)
        {
            cout << sorted_subarray[idx] << (idx < sorted_subarray.size() - 1 ? ", " : "");
        }
        cout << "]\n";

        // 3. Encontrar y mostrar el k-ésimo elemento usando PST (O(log N))
        if (i < 1 || j > N_arr || i > j)
        {
            cout << "ERROR: Índices fuera de rango o i > j.\n";
        }
        else if (k < 1 || k > subarray.size())
        {
            cout << "ERROR: k (" << k << ") está fuera del rango de la longitud del subarreglo (" << subarray.size() << ").\n";
        }
        else
        {
            // El resultado se obtiene con la consulta PST
            int result = get_kth_element(i, j, k);
            cout << "El " << k << "-ésimo elemento (solución PST): " << result << "\n";
        }
        cout << "------------------------------------------\n";
    }

    return 0;
}