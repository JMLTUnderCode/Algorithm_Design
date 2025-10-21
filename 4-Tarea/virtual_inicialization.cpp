#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include <cctype>
#include <algorithm>

using namespace std;

// Códigos ANSI para colores y atributos de texto en terminales compatibles.
// Nota: El efecto de parpadeo (blink) no está soportado en todos los emuladores de terminal;
// muchos terminales modernos ignoran la secuencia de "blink" por razones de accesibilidad.
// Si el parpadeo no funciona en tu terminal, puedes eliminar el código de BLINK y usar solo color.
static const string ANSI_RESET = "\x1B[0m";
static const string ANSI_BOLD = "\x1B[1m";
static const string ANSI_BLINK = "\x1B[5m"; // Parpadeo (puede ser ignorado)
static const string ANSI_GREEN = "\x1B[32m";
static const string ANSI_RED = "\x1B[31m";

// Prefijos formateados
static const string OK_PREFIX = ANSI_GREEN + ANSI_BLINK + "[OK]" + ANSI_RESET + " ";
static const string ERROR_PREFIX = ANSI_RED + ANSI_BLINK + "[ERROR]" + ANSI_RESET + " ";

/**
 * @brief Implementa el concepto de Arreglo con Inicialización Virtual.
 *
 * Utiliza tres estructuras principales:
 * T: El arreglo principal que almacena los valores.
 * A: Arreglo auxiliar que almacena los índices (0-based) de T que han sido inicializados,
 *    ordenados por el momento en que se inicializaron (rank).
 * B: Arreglo auxiliar que almacena el "rank" (momento de inicialización) de cada posición
 *    en T.
 * ctr: Contador que indica cuántos elementos válidos (inicializados) hay en T.
 *
 * Las operaciones son O(1) ya que solo implican accesos directos a memoria
 * y operaciones aritméticas de costo constante.
 */
class VirtualArray
{
private:
    int N;         // Tamaño del arreglo (indexado de 0 a N-1)
    int ctr;       // Contador de elementos inicializados (equivale al tamaño lógico)
    vector<int> T; // Arreglo de valores principal
    vector<int> A; // Auxiliar 1 (Almacena índices de T)
    vector<int> B; // Auxiliar 2 (Almacena el rank)

    /**
     * @brief Verifica si una posición es válida dentro del rango [0, N-1].
     * @param pos La posición a verificar.
     * @return true si la posición es válida.
     */
    bool isValidPos(int pos) const
    {
        return pos >= 0 && pos < N;
    }

    /**
     * @brief Implementa la lógica de la Inicialización Virtual para chequear si T[pos]
     *        ha sido inicializado.
     *
     * Se verifica que 1 <= B[pos] <= ctr y que A[B[pos] - 1] == pos.
     * El -1 en A[B[pos] - 1] se debe a que B almacena el rank (1-based), pero
     * A y los vectores en C++ son 0-based.
     * @param pos La posición a consultar.
     * @return true si la posición está inicializada.
     */
    bool isInitialized(int pos) const
    {
        // Paso 1: Verificar que el rank B[pos] esté dentro de los límites de ctr.
        if (B[pos] < 1 || B[pos] > ctr)
            return false;

        // Paso 2: Verificar la consistencia: T[pos] es el elemento A[rank - 1].
        // Si no se cumple, B[pos] contiene "basura" de una inicialización anterior.
        return A[B[pos] - 1] == pos;
    }

public:
    /**
     * @brief Constructor. Inicializa las estructuras con el tamaño dado.
     * @param size El tamaño del arreglo (N).
     */
    VirtualArray(int size) : N(size), ctr(0)
    {
        if (N < 1)
            throw invalid_argument("El tamaño del arreglo debe ser positivo.");

        // Los vectores se redimensionan al tamaño N. El contenido inicial (basura)
        // no es relevante, como requiere el método de inicialización virtual.
        T.resize(N);
        A.resize(N);
        B.resize(N);
    }

    /**
     * @brief Asigna un valor en una posición, garantizando O(1).
     * @param pos La posición (0-based) a asignar.
     * @param val El valor a almacenar.
     */
    void assign(int pos, int val)
    {
        if (!isValidPos(pos))
            throw invalid_argument("Posición POS(" + to_string(pos) + ") inválida.");

        // Chequear si es la primera asignación (o si ha sido limpiado y reusado)
        if (!isInitialized(pos))
        {
            // Asignación por primera vez: Actualizamos los punteros y el contador [4, 7].
            ctr++;            // 1. Incrementamos ctr [4]
            A[ctr - 1] = pos; // 2. A[ctr] = pos (índice 0-based) [4]
            B[pos] = ctr;     // 3. B[pos] = ctr (rank 1-based) [4]
        }

        // Asignamos el valor en T[pos]
        T[pos] = val;
        cout << OK_PREFIX << "Asignado valor " << val << " en posición " << pos << "." << endl;
    }

    /**
     * @brief Consulta si una posición está inicializada y devuelve su valor.
     * @param pos La posición (0-based) a consultar.
     * @return El valor en la posición, si está inicializada.
     */
    void consultation(int pos) const
    {
        if (!isValidPos(pos))
            throw invalid_argument("Posición POS(" + to_string(pos) + ") inválida.");

        // Si está inicializada, devolvemos el valor. O(1)
        if (isInitialized(pos))
            cout << OK_PREFIX << "La posición (" << pos << ") está inicializada con valor: " << T[pos] << endl;
        else
            cout << OK_PREFIX << "La posición (" << pos << ") NO está inicializada." << endl;
    }

    /**
     * @brief "Limpia" la tabla restaurando el contador a cero. O(1).
     *
     * Las operaciones de limpieza no tocan los arreglos T, A o B [2, 5],
     * por lo que el tiempo es O(1) [8].
     */
    void clear()
    {
        ctr = 0;
        cout << OK_PREFIX << "Tabla limpiada. Todas las posiciones están sin inicializar." << endl;
    }
};

/**
 * @brief Numeración de comandos disponibles.
 */
enum Commands
{
    DESCONOCIDO,
    ASIGNAR,
    CONSULTAR,
    LIMPIAR,
    SALIR
};

/**
 * @brief Mapa de cadenas en minúsculas a comandos.
 */
map<string, Commands> command_map = {
    {"asignar", ASIGNAR},
    {"consultar", CONSULTAR},
    {"limpiar", LIMPIAR},
    {"salir", SALIR}};

/**
 * @brief Muestra la interfaz de comandos disponibles.
 */
void interface()
{
    cout << "****************************************************************\n";
    cout << "                 Comandos disponibles:\n";
    cout << " - ASIGNAR POS VAL   -> Asigna el valor VAL en la posición POS.\n";
    cout << " - CONSULTAR POS     -> Consulta el valor en la posición POS.\n";
    cout << " - LIMPIAR           -> Limpia el arreglo virtual.\n";
    cout << " - SALIR             -> Termina el programa.\n";
}

/**
 * @brief Convierte una cadena a minúsculas.
 * @param str La cadena de entrada.
 * @return La cadena en minúsculas.
 */
string lowercase(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c)
              { return tolower(c); });
    return result;
}

/**
 * @brief Maneja los comandos del usuario para el Arreglo Virtual.
 * @param N Tamaño del arreglo virtual.
 */
void command_manager(int N)
{
    // Inicialización del arreglo virtual
    VirtualArray va(N);
    cout << "\nArreglo virtual de tamaño " << N << " inicializado (posiciones 0 a " << N - 1 << ")." << endl;

    // Mostrar la interfaz de comandos
    interface();

    string line;
    string command;

    while (true)
    {
        cout << "\nCliente (" << N << ") > ";
        if (!getline(cin, line))
            break;

        stringstream ss(line);
        ss >> command;

        try
        {
            if (command_map[lowercase(command)] == ASIGNAR)
            {
                int pos, val;
                if (!(ss >> pos >> val))
                    throw runtime_error("Uso: ASIGNAR POS VAL");
                va.assign(pos, val);
            }
            else if (command_map[lowercase(command)] == CONSULTAR)
            {
                int pos;
                if (!(ss >> pos))
                    throw runtime_error("Uso: CONSULTAR POS");
                va.consultation(pos);
            }
            else if (command_map[lowercase(command)] == LIMPIAR)
            {
                va.clear();
            }
            else if (command_map[lowercase(command)] == SALIR)
            {
                cout << "Saliendo del programa." << endl;
                break;
            }
            else if (command_map[lowercase(command)] == DESCONOCIDO)
            {
                cout << "Comando desconocido. Opciones: ASIGNAR, CONSULTAR, LIMPIAR, SALIR." << endl;
            }
        }
        catch (const exception &e)
        {
            // Mostrar el prefijo de error en rojo parpadeante seguido del mensaje
            cerr << ERROR_PREFIX << e.what() << endl;
        }
        command.clear();
    }
}

/**
 * @brief Función principal que maneja el cliente de consola.
 * @param argc Número de argumentos.
 * @param argv Vector de argumentos.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << ERROR_PREFIX << "Uso: " << argv[0] << " <tamaño_del_arreglo>" << ANSI_RESET << endl;
        return 1;
    }

    int N;
    try
    {
        N = stoi(argv[1]);
        command_manager(N);
    }
    catch (const exception &e)
    {
        cerr << ERROR_PREFIX << e.what() << ANSI_RESET << endl;
    }

    return 0;
}