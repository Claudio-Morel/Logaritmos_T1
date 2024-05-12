
# Tarea 1 CC4102 
## M-tree 
Integrantes: 
- Matías Godoy C.
- Damián González G.
- Claudio Morel M.

### Para ejecutar el código:

Usando un compilador de c++ como g++ se necesita compilar el archivo experiment.cpp que contiene el main junto con las importaciones de todos los otros archivos cpp necesarios. Para esto se puede ejecutar el comando:

```
g++ experiment.cpp -o experiment
```
Para ejecutar ahora el archivo se necesita usar:

```
./experiment
```
Este programa solicitará ingresar los límites de los exponentes de potencia de 2, para los cuales se quiere ejecutar el programa.
Esto es, si quiero que el programa ejecute para $2^{10}$ hasta $2^{15}$, debo ingresar los valores 10 y 15 por terminar cuando estos sean solicitados como "Inicio:" y "Fin:" respectivamente.

Se recomienda en caso de querer realizar una prueba no superar $2^{16}$, puesto que de este valor en adelante el algoritmo de SextonSwinank comienza a demorarse del orden de horas en ejecutarse. ($2^{15}$ toma aproximadamente 10 minutos)

### Resultados

Una vez ejecutado el programa se creara en el mismo directorio donde esté el proyecto una carpeta llamada "results" donde se crearan archivos txt por cada iteración con:

- La cantidad de puntos
- El tiempo que demoro en crearse el árbol
- Por cada una de las 100 consultas los resultados de:
  - Cantidad de puntos encontrados. 
  - Tiempo de ejecución en microsegundos.
  - Cantidad de I/Os realizadas en la consulta.
  
Los nombres de estos archivos indican el método usado para la creación del árbol y el número de puntos de la manera:

\<cp: CiacciaPatella, ss: SextonSwinbank>Experiment<N° de puntos>
