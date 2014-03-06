#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>

using namespace clipp;

int
main(int argc, char** argv) {

	OptionManager om(argc, argv);

	int my_var = 0;
	om.option("update").required().var(&my_var).description("Update a variable.");

	try {
		cout << "my_var=" << my_var << endl;
		
		om.process();

		cout << "my_var=" << my_var << endl;
		
	} catch(clipp::error::InvalidOption& e) {
		cerr << "ERROR: " << e.what() << endl;
	} catch(clipp::error::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}

	return 0;
}