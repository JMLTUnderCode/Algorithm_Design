# Resolución de Tarea 4 - Programación Dinámica (Fecha: 20 de Octubre de 2025)

$$
\begin{matrix}
\text{Universidad Simón Bolívar} \\
\text{Departamento de Computación y Tecnología de la Información} \\
\text{CI5651 - Diseño de Algoritmos I} \\
\text{Septiembre - Diciembre 2025} \\
\text{Estudiante: Junior Miguel Lara Torres (17-10303)} \\
\text{ } \\
\Large \text{Tarea 4 (9 puntos)} \\
\end{matrix}
$$

# Indice
- [Resolución de Tarea 4 - Programación Dinámica (Fecha: 20 de Octubre de 2025)](#resolución-de-tarea-4---programación-dinámica-fecha-20-de-octubre-de-2025)
- [Indice](#indice)
- [Pregunta 1](#pregunta-1)

# Pregunta 1

Mi fecha de naimiento es: 8 de **Junio** de 1999 y mi dia de nacimiento es **Martes**. Verificar [acá](https://www.gigacalculator.com/calculators/weekday-calculator.php).

La tabla de la distancia de edición entre **Martes** $\to$ **Junio** es

$$\begin{array}{|c|c|c|c|c|c|c|} \hline
    \text{Index} & 1 & 2 & 3 & 4 & 5 & 6 \\ \hline
    \text{Día} & M & A & R & T & E & S \\
    \text{Mes} & J & U & N & I & O & \\ \hline
\end{array}
$$

* En la inicialización de fila 0 y columna 0 tenemos:
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 0 & 0 & 0 & 0 & 0 \\
     2 & 0 & 0 & 0 & 0 & 0 \\
     3 & 0 & 0 & 0 & 0 & 0 \\
     4 & 0 & 0 & 0 & 0 & 0 \\
     5 & 0 & 0 & 0 & 0 & 0 \\
     6 & 0 & 0 & 0 & 0 & 0 \\ \hline
\end{array}
$$

* Tabla para $i = 1$
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 1 & 2 & 3 & 4 & 5 \\
     2 & 0 & 0 & 0 & 0 & 0 \\
     3 & 0 & 0 & 0 & 0 & 0 \\
     4 & 0 & 0 & 0 & 0 & 0 \\
     5 & 0 & 0 & 0 & 0 & 0 \\
     6 & 0 & 0 & 0 & 0 & 0 \\ \hline
\end{array}
$$

* Tabla para $i = 2$
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 1 & 2 & 3 & 4 & 5 \\
     2 & 2 & 2 & 3 & 4 & 5 \\
     3 & 0 & 0 & 0 & 0 & 0 \\
     4 & 0 & 0 & 0 & 0 & 0 \\
     5 & 0 & 0 & 0 & 0 & 0 \\
     6 & 0 & 0 & 0 & 0 & 0 \\ \hline
\end{array}
$$

* Tabla para $i = 3$
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 1 & 2 & 3 & 4 & 5 \\
     2 & 2 & 2 & 3 & 4 & 5 \\
     3 & 3 & 3 & 3 & 4 & 5 \\
     4 & 0 & 0 & 0 & 0 & 0 \\
     5 & 0 & 0 & 0 & 0 & 0 \\
     6 & 0 & 0 & 0 & 0 & 0 \\ \hline
\end{array}
$$

* Tabla para $i = 4$
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 1 & 2 & 3 & 4 & 5 \\
     2 & 2 & 2 & 3 & 4 & 5 \\
     3 & 3 & 3 & 3 & 4 & 5 \\
     4 & 4 & 4 & 4 & 4 & 5 \\
     5 & 0 & 0 & 0 & 0 & 0 \\
     6 & 0 & 0 & 0 & 0 & 0 \\ \hline
\end{array}
$$

* Tabla para $i = 5$
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 1 & 2 & 3 & 4 & 5 \\
     2 & 2 & 2 & 3 & 4 & 5 \\
     3 & 3 & 3 & 3 & 4 & 5 \\
     4 & 4 & 4 & 4 & 4 & 5 \\
     5 & 5 & 5 & 5 & 5 & 5 \\
     6 & 0 & 0 & 0 & 0 & 0 \\ \hline
\end{array}
$$

* Tabla para $i = 6$
$$
\begin{array}{|c|ccccc|} \hline
     0 & 1 & 2 & 3 & 4 & 5 \\ \hline
     1 & 1 & 2 & 3 & 4 & 5 \\
     2 & 2 & 2 & 3 & 4 & 5 \\
     3 & 3 & 3 & 3 & 4 & 5 \\
     4 & 4 & 4 & 4 & 4 & 5 \\
     5 & 5 & 5 & 5 & 5 & 5 \\
     6 & 6 & 6 & 6 & 6 & \begin{array}{|c|} \hline 6 \\ \hline \end{array} \\ \hline
\end{array}
$$

Por lo tanto, a distancia de edición entre las cadenas es 6.

>[!NOTE]
> Dado que no existe al menos una letra en común entre los estados iniciales de las palabras notamos que la distancia es de 6 igual a la longitud de la palabra mas larga (martes). Esto básicamente simula que se debe cambiar todos los caracteres para conseguir el objetivo. Acá el código en C++ con el algoritmo respectivo para realizar pruebas: [distancia_edicion.cpp](https://github.com/JMLTUnderCode/Algorithm_Design/blob/main/4-Tarea/distancia_edicion.cpp).


