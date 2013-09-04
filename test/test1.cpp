/*
 * test1.cpp
 *
 *  Created on: 27/02/2012
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>

using namespace clipp;

int
main(int argc, char** argv) {

	OptionManager om(argc,argv);

	om.credits("libcli++ TEST");
	om.usage("test1 [options]");
	om.option("help").exclusive().description("Muestra esta pantalla de ayuda.");
	om.option("input-file").alias("i").multiple().required().argumentRequired().description("Archivo de entrada.");
	om.option("print-name").alias("n").argument().description("Imprime el nombre (se puede especificar).");
	om.option("a").description("Hace la cosa A.").argument().typeInteger();
	om.option("b").description("Hace la cosa B.");
	om.option("c").description("Hace la cosa C.");
	om.option("verbose").alias("v").multiple().description("Muestra más información en pantalla (se puedes especificar varias veces).");
	//om.argumentCountRange().min = 2;
	//om.argumentCountRange().max = 3;

	cout << om << endl;

	om.process();

	cout << "\n-----------------------------------------------\n" << om.dump() << "\n----------------------------------------------" << endl;

	return 0;
}
