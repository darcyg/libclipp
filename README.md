libclipp (libcli++)
===================

`libcli++` es una librería para sistemas Linux para gesionar la los parámetros
de la línea de comandos. Es una librería similar a 'getopt' pero con más
características.

## Compilación ##

Para compilar la librería se puede usar el archivo `Makefile` a través del
comando `make` con los parámetros indicados. Si se ejecuta `make` sin
parámetros, mostrará la ayuda con los mismos.

Es necesario tener `g++` versión 4.6 o superior y es compatible con C++03 (no
ha sido probada a compilar con C++11).

```bash
diego@linux:~/projects/libcli++/src:$ make debug
```

## Instalación ##

Una vez compilada la librería, se puede ejecutar el comando `make install` o
`make devinstall` para instalar dicha librería. Si se decide instalarla de
forma manual, la librería debería ir en `/usr/local/lib` y los archivos de
cabecera en `/usr/local/include`.

Una vez instalada no hay que olvidar ejectuar `ldconfig`.

## Compilación de una aplicación haciendo uso de la librería ##

Para utilizar esta librería en una aplicación, basta con incluir el directorio
de los archivos de cabecera en la línea de compilación y añadir el parámetro
para que se enlace con la misma.

Si la librería está correctamente instalada, basta con compilar la aplicación
con el siguiente comando:

```bash
diego@linux:~/projects/libcli++/test:$ g++ -o mi_aplicacion -L/usr/local/include -I/usr/local/lib *.cpp
```

## Ejemplos de uso ##

Para usar la librería hay que incluir en el código fuente el archivo
`<cli++/OptionManager.hpp>` y hacer uso del _namespace_ `clipp` (opcional).

En este ejemplo se pueden ver las características básicas de la librería.

```cpp
#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>

using namespace clipp;

int
main(int argc, char** argv) {

        OptionManager om(argc,argv);

        om.credits("libcli++ TEST");
        om.usage("test [options]");
        om.option("help")
        	.exclusive()
        	.description("Muestra esta pantalla de ayuda.");
        om.option("input-file")
        	.alias("i")
        	.multiple()
        	.required()
        	.argumentRequired()
        	.description("Archivo de entrada.");
        om.option("print-name")
        	.alias("n")
        	.argument()
        	.description("Imprime el nombre (se puede especificar el argumento pero no es requerido).");
        om.option("a", 'a')
        	.description("Hace la cosa A.")
        	.argument()
        	.typeInteger();
        om.option("b", 'b')
        	.description("Hace la cosa B.");
        om.option("c", 'c')
        	.description("Hace la cosa C.");
        om.option("verbose")
        	.alias("v")
        	.multiple()
        	.description("Muestra más información en pantalla (se puede especificar varias veces).");
        
        try {
        
			om.process();
			
			// Se procesan las opciones. Se pueden procesar antes las opciones exclusivas.
			// Para el proceso automático en bucle, las opciones tienen que tener ID.
			const clipp::Option* op = NULL;
			while((op = fOptionManager->next()) != NULL) {
				switch(op->id()) {
					case 'a':
						// acción A
						break;
					case 'b':
						// acción B
						break;
					case 'c':
						// acción C
						break;
					default:
						break;
				}	// switch
			}	// while
		
		} catch(clipp::error::Exception& e) {
			cerr << "ERROR: " << e.what() << endl;
		}

        return 0;
}

```

## Carencias ##

* Carencia asignación automática a variables según la opción.
* No probada en entornos Windows.

