#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <map>

using namespace std;

// Utilizamos long long para las coordenadas para asegurar precisión en el producto cruz.
struct Point
{
    long long x, y;
    int id; // Identificador único para rastrear el punto original.
};

// Función de orientación (producto cruz). >0: CCW, <0: CW, =0: colineal
long long ccw(Point p, Point q, Point r)
{
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
}

// Distancia al cuadrado (auxiliar, opcional)
long long distSq(Point p1, Point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Implementación robusta del Convex Hull.
// Conserva los puntos colineales en el borde (útil para "onion peeling" / capas).
vector<Point> graham_scan(vector<Point> &P)
{
    int n = P.size();
    if (n <= 2)
        return P;

    // Ordenar por x, luego por y
    sort(P.begin(), P.end(), [](const Point &a, const Point &b)
         {
        if (a.x != b.x)
            return a.x < b.x;
        return a.y < b.y; });

    vector<Point> lower, upper;

    // Construir la mitad inferior (permitimos colinealidad en el borde: pop solo si giro < 0)
    for (int i = 0; i < n; ++i)
    {
        while (lower.size() >= 2 && ccw(lower[lower.size() - 2], lower.back(), P[i]) < 0)
            lower.pop_back();
        lower.push_back(P[i]);
    }

    // Construir la mitad superior
    for (int i = n - 1; i >= 0; --i)
    {
        while (upper.size() >= 2 && ccw(upper[upper.size() - 2], upper.back(), P[i]) < 0)
            upper.pop_back();
        upper.push_back(P[i]);
    }

    // Concatenar lower + upper (excluyendo los últimos elementos porque se repiten)
    // Si todos los puntos son colineales, this will produce duplicates; we handle that gracefully.
    lower.pop_back();
    upper.pop_back();

    vector<Point> ch = lower;
    ch.insert(ch.end(), upper.begin(), upper.end());

    return ch;
}

// 4. Función principal: Contar capas de Convex Hull
int count_layers(const vector<Point> &original_P)
{
    if (original_P.size() < 3)
        return 0;

    vector<Point> P = original_P;
    int layer_count = 0;

    // Repetimos mientras queden suficientes puntos para formar una capa
    while (P.size() >= 3)
    {

        // 1. Calcular el Convex Hull (se modifica el orden de P)
        vector<Point> hull_points = graham_scan(P);

        // Si el CH no es un polígono, el proceso termina
        if (hull_points.size() < 3)
            break;

        layer_count++;

        // 2. Identificar y eliminar los puntos de la capa actual

        // Usamos un mapa para marcar eficientemente los IDs de los puntos del hull
        map<int, bool> is_on_hull;
        for (const auto &hp : hull_points)
            is_on_hull[hp.id] = true;

        vector<Point> next_P;
        next_P.reserve(P.size() - hull_points.size());

        // 3. Construir el nuevo conjunto de puntos P' (los puntos no eliminados)
        for (const auto &p : P)
        {
            if (is_on_hull.find(p.id) == is_on_hull.end())
                next_P.push_back(p);
        }

        P = next_P;
    }

    return layer_count;
}

// Interfaz para probar la solución
int main()
{
    // Ejemplo ilustrativo de 3 capas
    cout << "--- Solucion al Problema 3 de la Tarea 7 (Capas de Convexidad) ---" << endl;

    vector<Point> P_test = {
        {0, 0},
        {6, 0},
        {6, 6},
        {3, 8},
        {0, 3}, // Capa 1: Vertices (0,0), (6,0), (6,6), (3,8), (0,3)
        {1, 1},
        {5, 1},
        {5, 5},
        {3, 7},
        {1, 3}, // Capa 2
        {2, 2},
        {4, 2},
        {4, 4},
        {3, 5},
        {2, 3}, // Capa 3
        {3, 3}, // Punto interior (no formara una capa por sí solo)
    };

    // Asignar IDs únicos
    for (int i = 0; i < P_test.size(); ++i)
    {
        P_test[i].id = i;
    }

    int layers = count_layers(P_test);
    cout << "\nConjunto de puntos analizado: " << P_test.size() << " puntos." << endl;
    cout << "El numero total de capas de convexidad es: " << layers << endl;

    // Ejemplo 2: Un solo cuadrado (1 capa)
    vector<Point> P2 = {{0, 0, 0}, {1, 0, 1}, {1, 1, 2}, {0, 1, 3}};
    int layers2 = count_layers(P2);
    cout << "\nConjunto de puntos P2 (cuadrado): " << P2.size() << " puntos." << endl;
    cout << "El numero total de capas de convexidad es: " << layers2 << endl; // Esperado: 1

    return 0;
}