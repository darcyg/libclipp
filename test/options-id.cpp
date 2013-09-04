/*
 * options-id.cpp
 *
 *  Created on: 25/02/2013
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>
#include <cli++/Exceptions.hpp>

int
main(int argc, char** argv) {

	clipp::OptionManager om(argc, argv);

	om.option("a", 1);
	om.option("b", 2);
	om.option("c", 3);

	try {
		om.process();
	} catch(clipp::error::Exception& e) {
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	const clipp::Option* op;
	while((op = om.next()) != NULL) {
		switch(op->id()) {
			case 1:
				cout << "Option 'a'." << endl;
				break;
			case 2:
				cout << "Option 'b'." << endl;
				break;
			case 3:
				cout << "Option 'c'." << endl;
				break;
			default:
				cout << "No option id processed: " << op->id() << endl;
				break;
		}
	}

	return 0;
}
