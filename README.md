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

In order to install the library, run `make install`. This will be installed under
`/usr/local/lib`. If you want to install the development files, run `make devinstall`.
The development files (C++ headers) will be installed under `/usr/local/include`.

Please, do not forget to run `ldconfig` after library installation.

## Using the library into your application ##

To use this library into your own applications, you have to include the development
files installation directory in your compiler flags and add an `#include` into your
source files.

If the library is correctly compiled and installed, you can compile your application with this command:

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

