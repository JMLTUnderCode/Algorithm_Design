#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void imprimirMatriz(const vector<vector<int>> &matriz)
{
    for (const auto &fila : matriz)
    {
        for (const auto &valor : fila)
        {
            cout << valor << " ";
        }
        cout << endl;
    }
}

int distanciaEdicion(const string &s1, const string &s2)
{
    int n = s1.size();
    int m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 0; i <= n; ++i)
        dp[i][0] = i; // Costo de eliminar todos los caracteres de s1
    for (int j = 0; j <= m; ++j)
        dp[0][j] = j; // Costo de insertar todos los caracteres de

    imprimirMatriz(dp); // Imprimir la matriz en cada paso
    cout << "------------------" << endl;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1]; // Sin costo
            }
            else
            {
                dp[i][j] = 1 + min({
                                   dp[i - 1][j],    // Eliminación
                                   dp[i][j - 1],    // Inserción
                                   dp[i - 1][j - 1] // Sustitución
                               });
            }
        }
        imprimirMatriz(dp); // Imprimir la matriz en cada paso
        cout << "------------------" << endl;
    }
    return dp[n][m];
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Uso: " << argv[0] << " <cadena1> <cadena2>" << endl;
        return 1;
    }

    string str1 = argv[1];
    string str2 = argv[2];

    int distancia = distanciaEdicion(str1, str2);
    cout << "La distancia de edición entre las cadenas es: " << distancia << endl;

    return 0;
}