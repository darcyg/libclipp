libclipp (libcli++)
===================

`libcli++` is a Linux library to manage command line interface. This is a
library similar to `getopt` but with more features. This is licensed under [LGPL v2.1](https://github.com/diegolagoglez/libclipp/blob/master/LICENSE).

## Compilation ##

In order to compile this library you can use the `Makefile` file which have
its own parameters to generate different library flavours. You can type `make`
without options to show help.

`g++` 4.6 is needed to compile with C++03 support.

```bash
diego@linux:~/projects/libcli++/src:$ make debug
```

## Instalation ##

__TO BE TRANSLATED__

Una vez compilada la librería, se puede ejecutar el comando `make install` o
`make devinstall` para instalar dicha librería. Si se decide instalarla de
forma manual, la librería debería ir en `/usr/local/lib` y los archivos de
cabecera en `/usr/local/include`.

Please, do not forget to run `ldconfig` after library installation.

## Using the library into your application ##

Para utilizar esta librería en una aplicación, basta con incluir el directorio
de los archivos de cabecera en la línea de compilación y añadir el parámetro
para que se enlace con la misma.

Si la librería está correctamente instalada, basta con compilar la aplicación
con el siguiente comando:

```bash
diego@linux:~/projects/libcli++/test:$ g++ -o my_aplication -I/usr/local/include -L/usr/local/lib *.cpp
```

## Examples ##

In order to use the library, you have to include the header file `<cli++/OptionManager.hpp>`
and the `clipp` namespace (optional).

In this example, you can view the basics of this library:

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
        	.description("Shows this help screen.");
        om.option("input-file")
        	.alias("i")
        	.multiple()
        	.required()
        	.argumentRequired()
        	.description("Input file.");
        om.option("print-name")
        	.alias("n")
        	.argument()
        	.description("Print the name (argument is not required).");
        om.option("a", 'a')
        	.description("Do A.")
        	.argument()
        	.typeInteger();
        om.option("b", 'b')
        	.description("Do B.");
        om.option("c", 'c')
        	.description("Do C.");
        om.option("verbose")
        	.alias("v")
        	.multiple()
        	.description("More verbose (it can be specified many times).");
        
        try {
        
			om.process();
			
			// Options processing. Exclusive options can be processed before.
			// For the automatic loop, options must have ID.
			const clipp::Option* op = NULL;
			while((op = fOptionManager->next()) != NULL) {
				switch(op->id()) {
					case 'a':
						// action A
						break;
					case 'b':
						// action B
						break;
					case 'c':
						// action C
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

## Lacks ##

Please, [refer to enhancements into the issues](https://github.com/diegolagoglez/libclipp/issues?labels=enhancement&page=1&state=open).

