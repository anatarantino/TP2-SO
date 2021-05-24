# TP2-SO

Construcción del núcleo de un Sistema Operativo y estructuras de administración de recursos. 2021-1Q

## Integrantes

* Prado Torres, Macarena
* Rodríguez, Martina
* Tarantino Pedrosa, Ana
  
## Compilación

Para compilar primero hace falta ubicarnos en la carpeta Toolchain:
  ```
    $ cd Toolchain 
  ```
Y luego compilar con:
  ``` 
    $ make all 
  ```
Por defecto, el Memory Manager es memory aunque se puede determinar cuál utilizar compilando alternativamente con alguno de los siguientes comandos:
  ``` 
    $ make MEM_MAN=BUDDY all 
  ```
  ``` 
    $ make MEM_MAN=MEMORY all 
  ```
Por último, resta situarse en la carpeta root nuevamente:
  ``` 
    $ cd .. 
  ```
Y ejecutar:
  ``` 
    $ make all 
  ```
  
## Ejecución

Para ejecutar el programa, desde el directorio TP2-SO ejecutar:  
* En Linux o Mac ``` $ ./run.sh ```    
* En Windows ``` $ ./run.ps1 ``` 

Se abrirá entonces una ventana con la shell.

## Comandos de la Shell

* __help__ &#8594; Comando de ayuda que muestra la lista de comandos y sus usos.
* __inforeg__ &#8594; Imprime el valor de los registros, presionar ctrl + r para actualizar los valores.
* __printmem__ &#8594; Recibe un puntero y hace un memory dump de 32 bytes en la pantalla comenzando en la dirección introducida.
* __time__ &#8594; Imprime la fecha y hora actual en pantalla.
* __chess__ &#8594; Comienza una partida de ajedrez.
* __clear__ &#8594; Limpia la pantalla.
* __divisionByZero__ &#8594; Causa una excepción de división por cero.
* __opCodeException__ &#8594; Causa una excepción de código inválido de operación.
* __ps__ &#8594; Imprime todos los procesos con sus propiedades.
* __loop__ &#8594; Imprime el PID cada cierta cantidad de segundos.
* __kill__ &#8594; Mata un proceso dado su ID.
* __nice__ &#8594; Cambia la prioridad de un proceso dado su ID.
* __block__ &#8594; Bloquea un proceso dado su ID.
* __unblock__ &#8594; Desbloquea un proceso dado su ID.
* __mem__ &#8594; Imprime el estado de la memoria.
* __sem__ &#8594; Imprime información de los semáforos.
* __cat__ &#8594; Imprime en pantalla el input.
* __wc__ &#8594; Cuenta la cantidad de líneas del input.
* __filter__ &#8594; Filtra las vocales en el input.
* __pipe__ &#8594; Imprime todos los pipes con sus propiedades.
* __phylo__ &#8594; Problema de los filósofos comiendo, recibe un número de comienzo (entre 2 y 5). Los filósofos pueden ser agregados usando la tecla 'a' y removidos con 'r'.
* __testMM__ &#8594; Testea el Memory Manager.
* __testSync__ &#8594; Testea la sincronización de los semáforos.
* __testNoSync__ &#8594; Testea la sincronización de los semáforos.
* __testProcesses__ &#8594; Testea la creación de procesos.
* __testPriority__ &#8594; Testea la prioridad del Scheduler.

### Detalles de la Shell

* __ctrl+r__ &#8594; Actualiza los valores de los registros.
* __ctrl+c__ &#8594; Mata el proceso que corre en foreground.
* __ctrl+d__ &#8594; EOF.
* __comando &__ &#8594; Corre el comando en background.
* __comando1 | comando2__ &#8594; Pipea el output del comando1 al input del comando2.

## Testeos

Para realizar los testeos, primero es necesario hacer las siguientes instalaciones:
```
  apt-get install cppcheck
  apt-get install pvs-studio
```
Luego, se puede ejecutar los testos con:
 ```
  make test
 ```
Cuyos resultados serán visibles en los archivos `output.cppOut` y `report.tasks`.  
Para eliminar esos archivos basta con ejecutar:
```
  make cleanTest
```
Por último, para eliminar los ejecutables:
```
  make clean
```
