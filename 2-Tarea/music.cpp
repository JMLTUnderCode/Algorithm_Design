
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

struct Music
{
    int id;     // Identificador
    int t;      // Inicio en segundos
    int d;      // Duración en segundos
    int finish; // Tiempo de finalización
};

vector<Music> schedulingMusic(vector<Music> &songs)
{
    vector<Music> result;
    if (songs.empty())
        return result;

    // Calcular tiempo de finalización
    for (auto &song : songs)
    {
        song.finish = song.t + song.d;
    }

    // Ordenar por tiempo de finalización
    sort(songs.begin(), songs.end(), [](const Music &a, const Music &b)
         { return a.finish <= b.finish; });

    // Selección de canciones
    result.push_back(songs[0]);
    int lastFinish = songs[0].finish;
    for (int i = 1; i < songs.size(); ++i)
    {
        if (lastFinish <= songs[i].t)
        {
            result.push_back(songs[i]);
            lastFinish = songs[i].finish;
        }
    }
    return result;
}

/**
 * Función principal del programa.
 *
 * Formato del archivo de entrada:
 * --------------------------------
 * La primera línea contiene un entero S, que indica la cantidad de problemas a resolver.
 * Por cada problema:
 *   - Una línea con un entero N, la cantidad de canciones para ese problema.
 *   - Luego N líneas, cada una con dos enteros t y d:
 *       t: tiempo de inicio de la canción (en segundos desde el inicio del día)
 *       d: duración de la canción (en segundos)
 *
 * Ejemplo de archivo de entrada:
 * 3
 * 3
 * 3 2
 * 2 1
 * 1 2
 * 4
 * 1 1
 * 3 4
 * 5 1
 * 2 3
 * 2
 * 2 1
 * 2 3
 *
 * Esto representa 3 problemas:
 *  - El primero con 3 canciones (3 2), (2 1), (1 2)
 *  - El segundo con 4 canciones
 *  - El tercero con 2 canciones
 *
 * El programa procesa cada problema y muestra la solución correspondiente.
 */
int main(int argc, char *argv[])
{
    // Verifica que se haya pasado el nombre del archivo de entrada como argumento
    if (argc < 2)
    {
        cerr << "Uso: " << argv[0] << " <archivo_entrada>\n";
        return 1;
    }

    // Abre el archivo de entrada para lectura
    ifstream fin(argv[1]);
    if (!fin)
    {
        cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    // Lee la cantidad de problemas a resolver
    int S;
    fin >> S;
    for (int s = 0; s < S; ++s)
    {
        // Lee la cantidad de canciones para el problema actual
        int N;
        fin >> N;
        vector<Music> songs(N);
        // Lee los datos de cada canción: tiempo de inicio y duración
        for (int i = 0; i < N; ++i)
        {
            songs[i].id = i + 1; // Asigna un identificador único a cada canción (1-indexado)
            fin >> songs[i].t >> songs[i].d;
        }
        // Llama a la función que selecciona el máximo conjunto de canciones no solapadas
        vector<Music> scheduledSongs = schedulingMusic(songs);
        // Imprime la solución para el problema actual
        cout << "Solución problema " << s + 1 << ": " << scheduledSongs.size() << " Canciones --> { ";
        for (int i = 0; i < scheduledSongs.size(); ++i)
        {
            if (i > 0)
                cout << ", ";
            cout << scheduledSongs[i].id;
        }
        cout << " }\n";
    }
    return 0;
}