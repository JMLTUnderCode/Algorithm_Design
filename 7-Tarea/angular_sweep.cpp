#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Constante de tolerancia para comparaciones con punto flotante
const double EPS = 1e-9;
const double PI = acos(-1.0);

// Estructura para representar un punto
struct Point
{
    double x, y;
    int id; // Para seguimiento
};

// Estructura para representar un evento en el barrido angular
struct Event
{
    double angle; // Angulo en radianes (0 a 2*PI)
    int type;     // 1 para inicio de arco (+1 cobertura), -1 para fin de arco (-1 cobertura)
};

// Estructura para el resultado completo
struct OptimalResult
{
    int max_count = 0;
    Point optimal_center = {0.0, 0.0};
    vector<Point> covered_points;
};

// Función de comparación para ordenar los eventos.
bool compare_events(const Event &a, const Event &b)
{
    if (fabs(a.angle - b.angle) > EPS)
        return a.angle < b.angle;

    return a.type > b.type;
}

// Calcula la distancia euclidiana
double dist(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// Funcion para calcular el centro C basado en un punto Pi y un ángulo polar theta
Point calculate_center(Point pi, double R, double theta)
{
    // El centro C está a distancia R de Pi, en la dirección angular theta
    return {
        pi.x + R * cos(theta),
        pi.y + R * sin(theta)};
}

/**
 * Funcion principal que implementa el algoritmo O(N^2 log N) y encuentra el centro óptimo.
 */
OptimalResult max_points_covered(vector<Point> &P, double R)
{
    int N = P.size();
    if (N == 0)
        return {};

    // Inicializar IDs para depuración y unicidad
    for (int i = 0; i < N; ++i)
        P[i].id = i;

    OptimalResult result;

    // Variables para rastrear la mejor solución encontrada
    // Inicializamos con 1 (al menos el propio pivote)
    result.max_count = 1;
    int best_pivot_idx = 0;
    double best_angle = 0.0;

    // O(N) iteraciones principales.
    for (int i = 0; i < N; ++i)
    {
        vector<Event> events;

        // O(N) para generar eventos.
        for (int j = 0; j < N; ++j)
        {
            if (i == j)
                continue;

            double d = dist(P[i], P[j]);

            // Si la distancia es mayor que 2R, P[j] no puede ser cubierta por C(P[i], R).
            if (d > 2.0 * R + EPS)
                continue;

            // Calculamos el ángulo alpha del vector P[i] -> P[j]
            double alpha = atan2(P[j].y - P[i].y, P[j].x - P[i].x);

            // Calculamos el ángulo beta de apertura del arco: beta = acos(d / (2*R))
            double ratio = min(1.0, d / (2.0 * R)); // Asegura que el argumento de acos no exceda 1
            double beta = acos(ratio);

            double angle_start = alpha - beta;
            double angle_end = alpha + beta;

            // Normalización de ángulos a [0, 2*PI) para el barrido.
            // Para la aritmética de eventos, es más sencillo trabajar con [-PI, PI) o similar
            // y luego mapear a [0, 2PI) si es necesario. Aquí usamos la detección de cruce de 0.

            // Convertir a [0, 2*PI)
            auto normalize = [](double angle)
            {
                while (angle < 0)
                    angle += 2.0 * PI;
                while (angle >= 2.0 * PI)
                    angle -= 2.0 * PI;
                return angle;
            };

            angle_start = normalize(angle_start);
            angle_end = normalize(angle_end);

            // Generamos los eventos de inicio y fin.
            if (angle_start <= angle_end + EPS)
            {
                // El arco no cruza el eje 0 (2*PI).
                events.push_back({angle_start, 1});
                events.push_back({angle_end, -1});
            }
            else
            {
                // El arco cruza 2*PI (wrap around). Segmento 1: [angle_start, 2*PI). Segmento 2: [0, angle_end].
                events.push_back({angle_start, 1});
                events.push_back({2.0 * PI - EPS, -1}); // Fin cerca de 2*PI
                events.push_back({0.0, 1});             // Inicio en 0
                events.push_back({angle_end, -1});
            }
        }

        // 3. Barrido Angular

        // O(N log N) para ordenar.
        sort(events.begin(), events.end(), compare_events);

        int current_coverage = 1; // P[i] siempre está cubierto

        // Iteración O(N)
        for (const auto &event : events)
        {
            current_coverage += event.type;

            // Si encontramos una cobertura superior, actualizamos el máximo.
            // Utilizamos el ángulo del evento (si es de inicio o inmediatamente después de un inicio)
            // como el centro óptimo candidato.
            if (current_coverage > result.max_count)
            {
                result.max_count = current_coverage;
                best_pivot_idx = i;
                best_angle = event.angle;

                // Si el evento era de fin (-1), significa que la cobertura máxima fue justo
                // antes. Usamos este ángulo como una aproximación del inicio del plateau de cobertura.
                // Sin embargo, para simplicidad y robustez, usamos el ángulo del evento que
                // nos llevó a este nuevo máximo.
            }
        }
    }

    // 4. Calcular el Centro Óptimo Final
    result.optimal_center = calculate_center(P[best_pivot_idx], R, best_angle);

    // 5. Generar la lista de puntos cubiertos por el centro óptimo.
    for (const auto &p : P)
    {
        // dist devuelve la distancia euclidiana; comparar con R (no R^2)
        if (dist(p, result.optimal_center) <= R + EPS)
            result.covered_points.push_back(p);
    }

    return result;
}

// Estructura para definir casos de prueba
struct TestCase
{
    string description;
    vector<Point> P;
    double R;
    int expected;
};

// Función auxiliar que ejecuta un caso de prueba y realiza la impresión común
void run_test_case(const TestCase &tc)
{
    OptimalResult res = max_points_covered(const_cast<vector<Point> &>(tc.P), tc.R);

    cout << "\n"
         << tc.description << ", R = " << tc.R << endl;
    cout << "Maxima cantidad de puntos cubiertos: " << res.max_count;
    if (tc.expected >= 0)
        cout << " (Esperado: " << tc.expected << ")";
    cout << endl;
    cout << "Centro Optimo (x, y): (" << res.optimal_center.x << ", " << res.optimal_center.y << ")" << endl;
    cout << "Puntos cubiertos: ";
    for (const auto &p : res.covered_points)
        cout << "(" << p.x << ", " << p.y << ") ";
    cout << endl;
}

// Interfaz para probar la solución
int main()
{
    cout << fixed << setprecision(2);

    // Definición de casos en una lista para evitar repetir couts.
    vector<TestCase> cases = {
        {"Caso 1: 5 puntos (simetricos)", {{1.0, 1.0}, {2.0, 1.0}, {1.0, 3.0}, {2.0, 3.0}, {1.5, 2.0}}, 1.0, 3},
        {"Caso 2: 4 puntos alineados", {{0.0, 0.0}, {2.0, 0.0}, {4.0, 0.0}, {6.0, 0.0}}, 1.0, 2},
        {"Caso 3: 4 puntos formando cuadrado", {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}}, 0.5, 2},
        {"Caso 4: 4 puntos formando cuadrado", {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}}, 0.8, 4},
        {"Caso 5: 4 puntos formando cuadrado", {{0.0, 0.0}, {0.0, 10.0}, {10.0, 0.0}, {10.0, 10.0}}, 5.0, 2},
    };

    // Ejecutar cada caso
    for (const auto &tc : cases)
        run_test_case(tc);

    return 0;
}