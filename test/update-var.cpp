#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>

using namespace clipp;

int
main(int argc, char** argv) {

	OptionManager om(argc, argv);

	string str_var = "";
	int int_var = 0;
	float float_var = 0.0;
	bool bool_var = false;

	om.option("string").required().var(&str_var).description("Update a string variable.");
	om.option("int").required().var(&int_var).description("Update a integer variable.");
	om.option("float").required().var(&float_var).description("Update a float variable.");
	om.option("bool").required().var(&bool_var).description("Update a boolean variable.");

	try {
		cout << "str_var  =" << str_var << endl;
		cout << "int_var  =" << int_var << endl;
		cout << "float_var=" << float_var << endl;
		cout << "bool_var =" << bool_var << endl;
		
		om.process();

		cout << "str_var  =" << str_var << endl;
		cout << "int_var  =" << int_var << endl;
		cout << "float_var=" << float_var << endl;
		cout << "bool_var =" << bool_var << endl;
		
	} catch(clipp::error::InvalidOption& e) {
		cerr << "ERROR: " << e.what() << endl;
	} catch(clipp::error::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}

	return 0;
}