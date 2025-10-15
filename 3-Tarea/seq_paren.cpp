#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

// Estructura para almacenar la información de subcadenas bien parentizadas
struct Node
{
    int matched_pairs;   // M: Pares coincidentes totales (longitud = 2*M)
    int unmatched_open;  // O: Paréntesis abiertos sin coincidir (disponibles a la derecha)
    int unmatched_close; // C: Paréntesis cerrados sin coincidir (disponibles a la izquierda)
};

class MaxBPSubsequenceSegmentTree
{
private:
    int n;
    string S;
    vector<Node> tree;

    // Función auxiliar para combinar los resultados de dos nodos hijos (Left y Right)
    Node merge(const Node &L, const Node &R)
    {
        Node result;

        // 1. Calcular nuevas coincidencias formadas por L.O y R.C
        int new_matches = min(L.unmatched_open, R.unmatched_close);

        // 2. Total de pares coincidentes
        result.matched_pairs = L.matched_pairs + R.matched_pairs + new_matches;

        // 3. Unmatched Open: Suma de los hijos menos los que se acaban de emparejar
        result.unmatched_open = L.unmatched_open + R.unmatched_open - new_matches;

        // 4. Unmatched Close: Suma de los hijos menos los que se acaban de emparejar
        result.unmatched_close = L.unmatched_close + R.unmatched_close - new_matches;

        return result;
    }

    // Construcción recursiva del Segment Tree
    void build(int v, int tl, int tr)
    {
        if (tl == tr)
        {
            // Caso Base: Nodo Hoja
            if (S[tl] == '(')
            {
                tree[v] = {0, 1, 0}; // M=0, O=1, C=0
            }
            else if (S[tl] == ')')
            {
                tree[v] = {0, 0, 1}; // M=0, O=0, C=1
            }
            else
            {
                // Si la cadena solo tiene paréntesis, esto es por seguridad
                tree[v] = {0, 0, 0};
            }
        }
        else
        {
            // Caso Recursivo: Nodos Intermedios
            int tm = (tl + tr) / 2;
            build(2 * v, tl, tm);         // Hijo izquierdo
            build(2 * v + 1, tm + 1, tr); // Hijo derecho

            // Combinar los resultados de los hijos
            tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
        }
    }

    // Función de consulta recursiva para obtener la información de un rango [l, r]
    Node query_recursive(int v, int tl, int tr, int l, int r)
    {
        // Inicializar un nodo nulo (cero coincidencias y cero paréntesis pendientes)
        if (l > r || tl > tr)
        {
            return {0, 0, 0};
        }

        if (l == tl && r == tr)
        {
            // El nodo actual cubre exactamente el rango de consulta [l, r]
            return tree[v];
        }

        int tm = (tl + tr) / 2;

        // Consultar y combinar los resultados de las partes que se superponen con [l, r]
        Node L_result = query_recursive(2 * v, tl, tm, l, min(r, tm));
        Node R_result = query_recursive(2 * v + 1, tm + 1, tr, max(l, tm + 1), r);

        // La combinación debe realizarse si ambos lados retornaron datos válidos
        if (L_result.matched_pairs == 0 && L_result.unmatched_open == 0 && L_result.unmatched_close == 0)
        {
            return R_result;
        }
        if (R_result.matched_pairs == 0 && R_result.unmatched_open == 0 && R_result.unmatched_close == 0)
        {
            return L_result;
        }

        return merge(L_result, R_result);
    }

public:
    // S_input es la cadena, asumimos índices base 0 internamente.
    MaxBPSubsequenceSegmentTree(const string &input_S) : S(input_S)
    {
        n = S.length();
        // Redimensionar para 4*n (tamaño estándar para Segment Trees)
        tree.resize(4 * n + 1);
        if (n > 0)
        {
            // Construir desde la raíz (v=1), cubriendo [0, n-1]
            build(1, 0, n - 1);
        }
    }

    // Función para realizar la consulta maxBP(i, j)
    // i y j se asumen como índices base 1 (como en S[1..n]).
    // Internamente usamos base 0.
    int maxBP(int i, int j)
    {
        if (i < 1 || j > n || i > j || n == 0)
            return 0;

        // Convertir a índices base 0: [i-1, j-1]
        Node result = query_recursive(1, 0, n - 1, i - 1, j - 1);

        // La longitud de la subcadena bien parentizada es 2 * M
        return 2 * result.matched_pairs;
    }
};

int main()
{
    string S = "())(())(())(";
    MaxBPSubsequenceSegmentTree segTree(S);

    cout << "S: " << S << endl;
    cout << "maxBP(3, 10): " << segTree.maxBP(3, 10) << " (Expected: 6)" << endl;
    cout << "maxBP(1, 11): " << segTree.maxBP(1, 11) << " (Expected: 10)" << endl;
    cout << "maxBP(1, 4): " << segTree.maxBP(1, 4) << " (Expected: 2)" << endl;

    return 0;
}