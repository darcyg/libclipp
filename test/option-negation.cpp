#include <iostream>

using namespace std;

#include <cli++/OptionManager.hpp>

using namespace clipp;

int
main(int argc, char** argv) {

	OptionManager om(argc, argv);

	om.option("commit").allowNoPreffix().required().description("Do commit.");

	try {
		om.process();

		const Option* o = om.getOption("commit");
		if(o != NULL) {
			cout << "Option " << o->name() << " " << (o->isNegated() ? "is negated" : "is not negated") << endl;
		} else {
			cerr << "ERROR: Option not exists." << endl;
		}
	} catch(clipp::error::InvalidOption& e) {
		cerr << "ERROR: " << e.what() << endl;
	} catch(clipp::error::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}

	return 0;
}
