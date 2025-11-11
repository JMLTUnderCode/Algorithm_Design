#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Constante para el tamaño máximo de nodos. Asumimos N <= 100000.
const int MAX_N = 100000 + 5;
const int INF = 1e9;

// =========================================================
// 1. Segment Tree con Lazy Propagation (para operaciones de rango)
// =========================================================

// Estructura que representa un nodo en el Segment Tree
struct Node
{
    int count_true; // Cantidad de aristas 'true' en el rango [L, R]
    int length;     // Longitud total del segmento (R - L + 1)
    bool lazy;      // Bandera de propagación perezosa: true si necesita toggle
};

Node st[4 * MAX_N]; // Arreglo para almacenar el Segment Tree (4*N de espacio es común)

// Función auxiliar para calcular la cantidad de nodos por nivel
void combine_nodes(int p)
{
    // La cuenta total es la suma de las cuentas de los hijos
    st[p].count_true = st[2 * p].count_true + st[2 * p + 1].count_true;
}

// Aplica la operación 'toggle' (inversión de valor) a un nodo
void apply_lazy(int p)
{
    if (st[p].lazy)
    {
        // La inversión significa que count_true se convierte en (total - count_true)
        st[p].count_true = st[p].length - st[p].count_true;

        // Si no es una hoja, pasa la promesa a los hijos
        if (st[p].length > 1)
        {
            st[2 * p].lazy = !st[2 * p].lazy;
            st[2 * p + 1].lazy = !st[2 * p + 1].lazy;
        }
        st[p].lazy = false; // La promesa ya fue resuelta en este nivel
    }
}

// Construye el Segment Tree
void build_st(int p, int L, int R, const vector<int> &initial_values)
{
    st[p].length = R - L + 1;
    st[p].lazy = false;

    if (L == R)
    {
        // Las hojas representan las aristas (indexadas de 0 a |N|-2)
        // El valor inicial del predicado es 1 si es true, 0 si es false
        st[p].count_true = initial_values[L];
        return;
    }

    int mid = L + (R - L) / 2;
    build_st(2 * p, L, mid, initial_values);
    build_st(2 * p + 1, mid + 1, R, initial_values);
    combine_nodes(p); // Combina los resultados de los hijos
}

// Actualización de Rango (Toggle) con Lazy Propagation (O(log N))
void update_range_st(int p, int L, int R, int i, int j)
{
    apply_lazy(p); // Resuelve promesas pendientes antes de operar

    // Si el rango del nodo está completamente fuera del rango de consulta
    if (L > j || R < i)
        return;

    // Si el rango del nodo está completamente contenido en el rango de consulta
    if (L >= i && R <= j)
    {
        // Aplicamos la promesa 'toggle' al nodo actual
        st[p].lazy = !st[p].lazy;
        apply_lazy(p); // Aplicar inmediatamente para actualizar count_true
        return;
    }

    // El rango intersecta parcialmente, propagamos y recursamos
    int mid = L + (R - L) / 2;
    update_range_st(2 * p, L, mid, i, j);
    update_range_st(2 * p + 1, mid + 1, R, i, j);

    // Recalculamos el valor después de las llamadas recursivas
    combine_nodes(p);
}

// Consulta de Rango (Agregación: devuelve el conteo de 'true's) (O(log N))
Node query_st(int p, int L, int R, int i, int j)
{
    apply_lazy(p); // Resuelve promesas pendientes

    // Si el rango está completamente fuera
    if (L > j || R < i)
    {
        return {0, 0, false};
    }

    // Si el rango está completamente dentro
    if (L >= i && R <= j)
    {
        return st[p]; // Devolvemos el nodo con su valor actualizado
    }

    // Intersección parcial
    int mid = L + (R - L) / 2;
    Node left_res = query_st(2 * p, L, mid, i, j);
    Node right_res = query_st(2 * p + 1, mid + 1, R, i, j);

    // Combinación de resultados
    Node result;
    result.count_true = left_res.count_true + right_res.count_true;
    result.length = left_res.length + right_res.length;
    result.lazy = false;
    return result;
}

// =========================================================
// 2. Heavy-Light Decomposition (HLD)
// =========================================================

// Estructuras para manejar la HLD y el mapeo de aristas

// Arreglos de preprocesamiento DFS (O(|N|))
vector<pair<int, int>> adj[MAX_N]; // {vecino, índice_arista}
int parent[MAX_N];                 // Padre del nodo (para subir)
int depth[MAX_N];                  // Profundidad
int subtree_size[MAX_N];           // Tamaño del subárbol
int heavy_child[MAX_N];            // Hijo pesado (con el subárbol más grande)

// Arreglos de linealización y mapeo HLD
int chain_head[MAX_N];   // Nodo inicial (raíz) de cada cadena pesada
int pos_in_base[MAX_N];  // Posición en el array lineal del Segment Tree
int edge_to_node[MAX_N]; // Mapeo: índice lineal -> nodo 'inferior' de la arista

int time_counter; // Contador global para asignar posiciones lineales
int N_nodes;      // Número de nodos

// Arreglo temporal para inicializar el Segment Tree
vector<int> edge_initial_values;

// Primera pasada DFS: calcula profundidad, tamaño de subárbol e identifica hijos pesados
void dfs_preprocess(int u, int p, int d)
{
    parent[u] = p;
    depth[u] = d;
    subtree_size[u] = 1;
    heavy_child[u] = -1;
    int max_size = -1;

    for (auto &edge : adj[u])
    {
        int v = edge.first;
        if (v != p)
        {
            // Guardar el valor inicial del predicado para la arista (u, v)
            edge_initial_values[v] = edge.second; // Usamos el índice de 'v' como índice lineal temporal
            dfs_preprocess(v, u, d + 1);
            subtree_size[u] += subtree_size[v];

            if (subtree_size[v] > max_size)
            {
                max_size = subtree_size[v];
                heavy_child[u] = v; // La conexión a v es pesada
            }
        }
    }
}

// Segunda pasada DFS: realiza la descomposición y linealiza
void hld_decompose(int u, int head_node)
{
    chain_head[u] = head_node;
    pos_in_base[u] = time_counter++;
    edge_to_node[pos_in_base[u]] = u;

    // Si la arista lineal representa la conexión (parent[u], u),
    // su valor inicial es el del predicado en esa arista (edge_initial_values[u]).

    // 1. Visita primero al hijo pesado (Heavy Child)
    if (heavy_child[u] != -1)
    {
        hld_decompose(heavy_child[u], head_node);
    }

    // 2. Visita a los hijos livianos (Light Children), que inician nuevas cadenas
    for (auto &edge : adj[u])
    {
        int v = edge.first;
        if (v != parent[u] && v != heavy_child[u])
        {
            hld_decompose(v, v); // v es la nueva cabeza de cadena
        }
    }
}

// Inicialización completa de HLD y Segment Tree (O(|N|))
void initialize_hld(int N, int root = 1)
{
    N_nodes = N;
    time_counter = 0;
    // El Segment Tree operará sobre N-1 aristas (aunque la indexación va de 1 a N)
    edge_initial_values.assign(N + 1, 0);

    // Asumimos 0 como el padre de la raíz si usamos 1-based indexing
    dfs_preprocess(root, 0, 0);
    hld_decompose(root, root);

    // Reajustar edge_initial_values para que coincida con la posición lineal
    vector<int> linearized_edge_values(N_nodes, 0);
    for (int u = 1; u <= N_nodes; ++u)
    {
        // La arista linealizada en pos_in_base[u] corresponde al valor del predicado
        // en la arista que conecta parent[u] con u.
        // Ignoramos la arista de la raíz (u=root), ya que no tiene parent.
        if (u != root)
        {
            linearized_edge_values[pos_in_base[u]] = edge_initial_values[u];
        }
    }

    // El Segment Tree opera sobre las N-1 aristas, indexadas desde 1 (o 0 si usamos 0-based).
    // Usaremos las N-1 aristas linealizadas, ocupando de 1 a N-1 en el arreglo ST.
    // El Segment Tree se construye sobre las posiciones lineales 1 a N_nodes-1.
    build_st(1, 1, N_nodes - 1, linearized_edge_values); // Construir sobre las aristas (N-1)
}

// Función de utilidad: realiza la operación (toggle o query) en el camino (O(log^2 N))
Node perform_path_op(int u, int v, bool is_query)
{
    Node total_result = {0, 0, false}; // Contendrá el resultado agregado del camino

    while (chain_head[u] != chain_head[v])
    {
        // Mueve el nodo más profundo (con cabeza de cadena más profunda) hacia arriba
        if (depth[chain_head[u]] < depth[chain_head[v]])
        {
            swap(u, v);
        }

        int head = chain_head[u];
        int start_pos = pos_in_base[head];
        int end_pos = pos_in_base[u];

        // La operación es sobre el rango lineal de aristas [start_pos, end_pos]
        // Las aristas están indexadas por la posición del nodo 'hijo' en la cadena.

        if (start_pos > end_pos)
            swap(start_pos, end_pos);

        if (is_query)
        {
            Node current_res = query_st(1, 1, N_nodes - 1, start_pos, end_pos);
            // Agregación de resultados (suma de 'true's y longitud)
            total_result.count_true += current_res.count_true;
            total_result.length += current_res.length;
        }
        else
        {
            // Acción toggle
            update_range_st(1, 1, N_nodes - 1, start_pos, end_pos);
        }

        // Sube al padre de la cabeza de la cadena actual
        u = parent[head];
    }

    // Procesar el segmento final (u y v están ahora en la misma cadena)
    if (u == v)
        return total_result; // Si uno es LCA, no hay aristas entre ellos en esta pasada

    // Asegura que u es el nodo más profundo para definir el rango
    if (depth[u] < depth[v])
    {
        swap(u, v);
    }

    // El rango final es: (pos_in_base[v] + 1) hasta pos_in_base[u]
    int start_pos = pos_in_base[v] + 1; // Arista inmediatamente después de v
    int end_pos = pos_in_base[u];

    if (start_pos > end_pos)
        swap(start_pos, end_pos);

    if (is_query)
    {
        Node current_res = query_st(1, 1, N_nodes - 1, start_pos, end_pos);
        total_result.count_true += current_res.count_true;
        total_result.length += current_res.length;
    }
    else
    {
        update_range_st(1, 1, N_nodes - 1, start_pos, end_pos);
    }

    return total_result;
}

// =========================================================
// 3. Operaciones del Problema
// =========================================================

void toggle_action(int x, int y)
{
    perform_path_op(x, y, false);
}

bool exists_query(int x, int y)
{
    Node result = perform_path_op(x, y, true);
    // Si el conteo de aristas 'true' es > 0, al menos una existe
    return result.count_true > 0;
}

bool forall_query(int x, int y)
{
    Node result = perform_path_op(x, y, true);
    // Si el conteo de aristas 'true' es igual a la longitud total, todas cumplen
    return result.count_true == result.length && result.length > 0;
}

// =========================================================
// 4. Función Main (Interfaz)
// =========================================================

int main()
{
    int N, Q; // N nodos, Q operaciones
    cout << "Ingrese el número de nodos N y de operaciones Q: ";
    if (!(cin >> N >> Q) || N <= 1)
    {
        cout << "N debe ser al menos 2." << endl;
        return 0;
    }

    if (N_nodes > MAX_N)
    {
        cout << "N excede el límite MAX_N." << endl;
        return 0;
    }

    cout << "Ingrese las " << N - 1 << " aristas (u, v, predicado_0_o_1):\n";
    // Usamos índices 1-based para los nodos
    for (int i = 0; i < N - 1; ++i)
    {
        int u, v, val;
        cin >> u >> v >> val;
        // La HLD funciona mejor en árboles con aristas dirigidas de padre a hijo.
        // Al construir la lista de adyacencia (grafo no dirigido), asumimos que
        // la dirección padre-hijo se define en dfs_preprocess.
        adj[u].push_back({v, val});
        adj[v].push_back({u, val});
    }

    // Fase de Precondicionamiento HLD y Segment Tree (O(|N|))
    initialize_hld(N);
    cout << "\n--- Precondicionamiento HLD y Segment Tree completado. ---\n";

    cout << "\nIngrese operaciones (forall x y, exists x y, toggle x y):\n";
    cout << "Escriba 'exit' o ingrese EOF para terminar.\n";

    string op;
    int x, y;

    for (int q_count = 1; q_count <= Q; ++q_count)
    {
        if (!(cin >> op))
            break; // Terminar si hay EOF
        if (op == "exit")
            break;
        if (!(cin >> x >> y))
            break;

        cout << "Operación " << q_count << ": " << op << "(" << x << ", " << y << ") -> ";

        if (op == "toggle")
        {
            toggle_action(x, y);
            cout << "Ejecutada la inversión de predicados." << endl;
        }
        else if (op == "forall")
        {
            bool result = forall_query(x, y);
            cout << (result ? "TRUE (Todas cumplen)" : "FALSE") << endl;
        }
        else if (op == "exists")
        {
            bool result = exists_query(x, y);
            cout << (result ? "TRUE (Al menos una cumple)" : "FALSE") << endl;
        }
        else
        {
            cout << "Operación no reconocida." << endl;
        }
    }

    cout << "\n--- Programa finalizado. ---\n";

    return 0;
}