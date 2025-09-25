# Algorithm_Design
¡Bienvenidos a Diseño de Algoritmos (CI5651)! Este curso, dictado por el Prof. Ricardo Monascal en la Universidad Simón Bolívar, explora las técnicas fundamentales para el diseño y análisis de algoritmos eficientes.

Cada tarea tendrá como base tres archivos
- Enunciado de la tarea (PDF)
- Resolución de la tarea (Markdown)
- Resolución de la tarea (PDF)

Para generar un archivo PDF desde un archivo Markdown (.md) en la terminal de Linux, el método más efectivo es usar la herramienta Pandoc, que requiere la instalación de Pandoc y de un motor de LaTeX como TeX Live. Una vez instaladas las dependencias, se utiliza el comando pandoc archivo.md -o archivo.pdf para realizar la conversión y obtener el PDF con un formato adecuado

>[!IMPORTANT]
> Para una correcta visualización de la tarea es recomendable clonar el repositorio y usar una extensión que admita Markdown + Latex complejo (Ej. [Markdown All in One](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one)), dado que durante la resolución de la tarea se usa latex complejo que el motor Markdown de Github no soporta.

## Instalar Pandoc y TeX Live:

Ejecuta el comando de tu distribución para instalar Pandoc y los paquetes necesarios de LaTeX. En sistemas basados en Debian (como Ubuntu): 
```sh
sudo apt-get install pandoc texlive-latex-base texlive-fonts-recommended texlive-extra-utils texlive-xetex
```
En su defecto, si se quiere instalar el paquete completo de Latex (aprox. 4 GB de espacio necesario)
```sh
sudo apt-get install texlive-full
```

## Generación de PDF's automatizado

Se crea un script que permite generar todas las resoluciones version PDF de las tareas modificadas a nivel de .md. 

Estando en la raiz del repositorio basta con ejecutar
```sh
./pdf
```

> [!NOTE]
> Este script solo convertirá los archivos .md a PDF si han sido modificados desde la última generación del PDF, evitando conversiones innecesarias.