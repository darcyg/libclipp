/*
 * short-options.cpp
 *
 *  Created on: 10/04/2013
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>

int
main(int argc, char** argv) {

	clipp::OptionManager om(argc, argv);

	clipp::StringList validstrings;
	validstrings.push_back("warning");
	validstrings.push_back("error");
	validstrings.push_back("w");
	validstrings.push_back("e");

	om.option("help", 'h').alias("h").description("Muestra la ayuda.");
	om.option("level", 'l').alias("l").validStrings(validstrings).argumentRequired().description("Fija el nivel");

	try {

		om.process();

		const clipp::Option* op;
		while((op = om.next()) != NULL) {
			switch(op->id()) {
				case 'h':
					cout << om.description() << endl;
					break;
				case 'l':
					cout << "Level is: " << op->get() << endl;
					break;
				default:
					break;
			}
		}

	} catch(clipp::error::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}

	return 0;
}
