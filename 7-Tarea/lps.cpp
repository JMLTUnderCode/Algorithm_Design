#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Función para calcular la longitud del Longest Proper Prefix (LPS) array para KMP.
// La complejidad es O(n).
string compute_lps_array(const string &S)
{
    int N = S.length();
    // La subcadena más grande debe ser propia (T != S), así que si N <= 1,
    // solo podemos retornar la cadena vacía.
    if (N <= 1)
        return "";

    // lps[i] almacenará la longitud del prefijo propio más largo de S[0...i]
    // que también es un sufijo de S[0...i].
    vector<int> lps(N, 0);

    int len = 0; // Longitud del prefijo más largo anterior que es también sufijo
    int i = 1;

    // Lazo que calcula lps[i] para i = 1 a N-1
    while (i < N)
    {
        if (S[i] == S[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            // Desigualdad de caracteres
            // Retrocedemos a la longitud LPS anterior (len - 1)
            if (len != 0)
                len = lps[len - 1];
            else
            {
                // Si len es 0, no hay un prefijo común, lps[i] = 0
                lps[i] = 0;
                i++;
            }
        }
    }
    // La longitud L de la subcadena T deseada es el último elemento del arreglo LPS.
    // Esto se debe a que lps[N-1] almacena la longitud del prefijo más largo
    // de S[0...N-1] que también es un sufijo de S[0...N-1].
    int L = lps[N - 1];

    // Extraemos la subcadena de longitud L.
    return S.substr(0, L);
}

// Interfaz para probar la solución
int main()
{
    cout << "--- Solucion al Problema 2 de la Tarea 7 (KMP LPS) ---\n";

    vector<string> test_cases = {
        "ABRACADABRA", // Esperado: ABRA
        "AREPERA",     // Esperado: A
        "ALGORITMO",   // Esperado: ""
        "AABAACAA",    // Esperado: AA
        "AAAA",        // Esperado: AAA
        "AAAAA",       // Esperado: AAAA
        "X",           // Esperado: ""
        "",            // Esperado: ""
    };

    for (const string &S : test_cases)
    {
        string T = compute_lps_array(S);
        cout << "\nCadena S: \"" << S << "\"\n";
        if (T.empty())
            cout << "Subcadena T mas grande: \"\" (Cadena vacia)\n";
        else
            cout << "Subcadena T mas grande: \"" << T << "\" (Longitud " << T.length() << ")\n";
    }

    return 0;
}