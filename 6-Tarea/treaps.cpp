#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <limits>
#include <functional>

using namespace std;

struct Node
{
    int value;
    int priority;
    int size;
    Node *left, *right;

    // Asignamos una prioridad aleatoria (crucial para el balanceo O(log n) promedio)
    Node(int val) : value(val), size(1), left(nullptr), right(nullptr)
    {
        priority = rand();
    }
};

int get_size(Node *t)
{
    return t ? t->size : 0;
}

void update_size(Node *t)
{
    if (t)
        t->size = get_size(t->left) + get_size(t->right) + 1;
}

/**
 * Operación SPLIT (División)
 * Divide el Treap 't' en dos ('l' con los primeros 'k' elementos y 'r' con el resto).
 * Tiempo promedio O(log n).
 */
void split(Node *t, Node *&l, Node *&r, int k)
{
    if (!t)
    {
        l = r = nullptr;
        return;
    }

    int current_rank = get_size(t->left) + 1;

    if (k < current_rank)
    {
        split(t->left, l, t->left, k);
        r = t;
    }
    else
    {
        split(t->right, t->right, r, k - current_rank);
        l = t;
    }
    update_size(t);
}

/**
 * Operación MERGE (Unión)
 * Combina dos Treaps 'l' y 'r' en 't', manteniendo la propiedad de heap.
 * Tiempo promedio O(log n).
 */
void merge(Node *&t, Node *l, Node *r)
{
    if (!l)
    {
        t = r;
        return;
    }
    if (!r)
    {
        t = l;
        return;
    }

    // Se elige la raíz basada en la prioridad (la de menor prioridad va arriba)
    if (l->priority < r->priority)
    {
        merge(l->right, l->right, r);
        t = l;
    }
    else
    {
        merge(r->left, l, r->left);
        t = r;
    }
    update_size(t);
}

/**
 * Implementa la reordenación de subarreglos simulando multiswap(a, b).
 * Intercambia el segmento T1 = A[a..b-1] por T2 = A[b..2b-a-1].
 */
void multiswap_treap(Node *&t, int a, int b, int N)
{
    int L = b - a;
    int end_T2 = b + L - 1;

    // a y b están basados en 1. Las operaciones split esperan un número de elementos.

    if (end_T2 > N || L <= 0)
        return;

    Node *T_prefix = nullptr, *T_rest = nullptr;
    Node *T1 = nullptr, *T_T2_suffix = nullptr;
    Node *T2 = nullptr, *T_suffix = nullptr;

    // 1. Separar T_prefix (A[1..a-1])
    split(t, T_prefix, T_rest, a - 1);

    // 2. Separar T1 (A[a..b-1]). Longitud L.
    split(T_rest, T1, T_T2_suffix, L);

    // 3. Separar T2 (A[b..b+L-1]). Longitud L.
    split(T_T2_suffix, T2, T_suffix, L);

    // 4. Re-unir en el orden: T_prefix, T2, T1, T_suffix
    Node *T_new_1 = nullptr, *T_new_2 = nullptr;

    merge(T_new_1, T_prefix, T2);
    merge(T_new_2, T_new_1, T1);
    merge(t, T_new_2, T_suffix);
}

// Recorrido In-order para obtener la secuencia resultante
vector<int> inorder_traversal(Node *t)
{
    vector<int> result;
    function<void(Node *)> traverse = [&](Node *node)
    {
        if (!node)
            return;
        traverse(node->left);
        result.push_back(node->value);
        traverse(node->right);
    };
    traverse(t);
    return result;
}

// --- LÓGICA DE INTERFAZ Y MAIN ---

// Función para imprimir el arreglo
void print_array(const vector<int> &arr)
{
    cout << "[";
    for (size_t i = 0; i < arr.size(); ++i)
        cout << arr[i] << (i == arr.size() - 1 ? "" : " ");
    cout << "]" << endl;
}

// Función principal de la consola
int main()
{
    // Inicializar semilla para prioridades aleatorias
    srand(time(0));

    cout << "--- Problema 1 Tarea 6: Multiswap con Treap Implícito ---" << endl;

    // 1. Lectura del arreglo inicial
    string line;
    vector<int> initial_values;

    cout << "Ingrese los elementos del arreglo separados por espacios (ej: 9 1 8 0 12):" << endl;

    // Leer la línea completa de entrada
    if (!getline(cin, line))
    {
        cerr << "Error de lectura." << endl;
        return 1;
    }

    stringstream ss(line);
    int val;
    while (ss >> val)
    {
        initial_values.push_back(val);
    }

    if (initial_values.empty())
    {
        cerr << "El arreglo inicial no puede estar vacío." << endl;
        return 1;
    }

    int N = initial_values.size();
    Node *root = nullptr;

    // 2. Construcción del Treap Implícito inicial
    for (int value : initial_values)
    {
        Node *new_node = new Node(value);
        merge(root, root, new_node);
    }

    cout << "\nArreglo Inicial (N=" << N << "): ";
    print_array(inorder_traversal(root));

    // 3. Solicitud y ejecución de operaciones multiswap
    int num_operations;
    cout << "\n¿Cuántas operaciones multiswap desea realizar? (size(arreglo) solicitudes): ";
    if (!(cin >> num_operations) || num_operations <= 0)
    {
        cerr << "Número de operaciones inválido." << endl;
        return 1;
    }

    // Consumir el resto de la línea después de leer el entero
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int k = 1; k <= num_operations; ++k)
    {
        int a, b;
        cout << "\n--- Operación " << k << " de " << num_operations << " ---" << endl;
        cout << "Ingrese los parámetros a y b (separados por espacio, 1-based, 1 <= a < b <= N): ";

        if (!(cin >> a >> b))
        {
            cerr << "Error de lectura o formato de entrada inválido." << endl;
            break;
        }

        // Validación básica de los parámetros (índices 1-based)
        if (a <= 0 || b > N || a >= b)
        {
            cout << "Advertencia: Parámetros inválidos (1 <= a < b <= N). Se salta esta operación." << endl;
            // Consumir el resto de la línea
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --k; // Para reintentar la operación k
            continue;
        }

        // Ejecutar multiswap
        // La implementación `multiswap_treap` maneja internamente la validación de rango
        // para el segundo segmento (T2)
        multiswap_treap(root, a, b, N);

        // Mostrar el arreglo resultante
        cout << "Arreglo resultante tras multiswap(" << a << ", " << b << "): ";
        print_array(inorder_traversal(root));

        // Consumir el resto de la línea
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\n--- Ejecución finalizada ---" << endl;
    return 0;
}