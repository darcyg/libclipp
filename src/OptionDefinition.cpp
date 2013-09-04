/*
 * OptionDefinition.cpp
 *
 *  Created on: 04/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#include <cli++/Exceptions.hpp>
#include <cli++/OptionDefinition.hpp>
#include <cli++/Utils.hpp>
#include <cli++/OptionManager.hpp>

namespace clipp {

OptionDefinition::OptionDefinition(const OptionDefinition& od)
	: fOwner(od.fOwner),
	  fId(od.fId),
	  fName(od.fName),
	  fType(od.fType),
	  fDescription(od.fDescription),
	  fHidden(od.fHidden),
	  fRequired(od.fRequired),
	  fHasArgument(od.fHasArgument),
	  fArgumentRequired(od.fArgumentRequired),
	  fMultiple(od.fMultiple),
	  fExclusive(od.fExclusive),
	  fAlias(od.fAlias),
	  fValidStrings(od.fValidStrings),
	  fHasMinValue(false),
	  fMinValue(0),
	  fHasMaxValue(false),
	  fMaxValue(0),
	  fHasDefaultValue(od.fHasDefaultValue),
	  fDefaultValue(od.fDefaultValue),
	  fExecute(od.fExecute)
{
}

OptionDefinition::OptionDefinition(const string name, OptionType type, const string description)
	: fOwner(NULL),
	  fId(0),
	  fName(name),
	  fType(type),
	  fDescription(description),
	  fHidden(false),
	  fRequired(false),
	  fHasArgument(false),
	  fArgumentRequired(false),
	  fMultiple(false),
	  fExclusive(false),
	  fAlias(),
	  fValidStrings(),
	  fHasMinValue(false),
	  fMinValue(0),
	  fHasMaxValue(false),
	  fMaxValue(0),
	  fHasDefaultValue(false),
	  fDefaultValue(),
	  fExecute(NULL)
{
	if(fName.empty()) {
		throw clipp::error::Length("OptionDefinition: Name cannot be empty.");
	}
}

OptionDefinition::OptionDefinition(int id, const string name, OptionType type, const string description)
	: fOwner(NULL),
	  fId(id),
	  fName(name),
	  fType(type),
	  fDescription(description),
	  fHidden(false),
	  fRequired(false),
	  fHasArgument(false),
	  fArgumentRequired(false),
	  fMultiple(false),
	  fExclusive(false),
	  fAlias(),
	  fValidStrings(),
	  fHasMinValue(false),
	  fMinValue(0),
	  fHasMaxValue(false),
	  fMaxValue(0),
	  fHasDefaultValue(false),
	  fDefaultValue(),
	  fExecute(NULL)
{
	if(fName.empty()) {
		throw clipp::error::Length("OptionDefinition: Name cannot be empty.");
	}
}

OptionDefinition::~OptionDefinition() {
}

void
OptionDefinition::checkExclusivity() const {
	if(fExclusive && (fRequired || fMultiple)) {
		// Las opciones exclusivas no pueden ser obligatorias ni ser múltiples)
		throw clipp::error::OptionDefinition("OptionManager: Exclusive options (" + fName + ") cannot be required nor be multiple.");
	}
}

void
OptionDefinition::checkArgumentType(const string argument) const {
	const string indicator = (isLongOption() ? "--" : "-");
	switch(fType) {
		case OptionDefinition::TypeInteger:
			if(!StringIs<int>(argument)) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be an integer: " + argument, fName);
			}
			break;
		case OptionDefinition::TypeFloat:
			if(!StringIs<float>(argument)) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be a float: " + argument, fName);
			}
			break;
		case OptionDefinition::TypeBoolean:
			if(!StringIs<bool>(argument)) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be a boolean: " + argument, fName);
			}
			break;
		case OptionDefinition::TypeString:
		default:
			break;
	}
}

static const string
implode_string_set(const string glue, const StringSet& set, const string wrap = string()) {
	stringstream out;
	StringSet::const_iterator preend = set.end();
	advance(preend, -1);
	for(StringSet::const_iterator it = set.begin(); it != set.end(); ++it) {
		out << (wrap.empty() ? "" : wrap) << *it << (wrap.empty() ? "" : wrap) << (it != preend ? glue : "");
	}
	return out.str();
}

void
OptionDefinition::checkArgumentValue(const string argument) const {
	const string indicator = (isLongOption() ? "--" : "-");

	// Aunque el argumento no sea de tipo cadena, siempre se procesa que contenga cadenas válidas, porque,
	// en realidad, todos los argumentos son cadenas hasta que se realiza la comprobación de tipos.
	if(!fValidStrings.empty() && fValidStrings.find(argument) == fValidStrings.end()) {
		throw clipp::error::InvalidArgument("Invalid argument '" + argument + "' for option '"  + indicator + fName + "'. Argument must be one of " +
				implode_string_set(", ", fValidStrings, (fType == OptionDefinition::TypeString ? "\"" : "")) + ".");
	}

	// Luego se comprueba el argumento ya según su tipo.
	if(fType == OptionDefinition::TypeInteger) {
		bool isint = StringIs<int>(argument);
		if(isint && fHasMaxValue) {
			if(StringTo<int>(argument, 0) > fMaxValue) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be less than " + StringFrom<float>(fMaxValue) + ": " + argument);
			}
		}
		if(isint && fHasMaxValue) {
			if(StringTo<int>(argument, 0) < fMinValue) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be greater than " + StringFrom<float>(fMinValue) + ": " + argument);
			}
		}
	} else if(fType == OptionDefinition::TypeFloat) {
		bool isfloat = StringIs<float>(argument);
		if(isfloat && fHasMaxValue) {
			if(StringTo<float>(argument, 0) > fMaxValue) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be less than " + StringFrom<float>(fMaxValue) + ": " + argument);
			}
		}
		if(isfloat && fHasMaxValue) {
			if(StringTo<float>(argument, 0) < fMinValue) {
				throw clipp::error::InvalidArgument("Argument for option '" + indicator + fName + "' must be greater than " + StringFrom<float>(fMinValue) + ": " + argument);
			}
		}
	}
}

bool
OptionDefinition::operator == (const OptionDefinition& od) const {
	return this->fName == od.fName;
}

bool
OptionDefinition::operator == (const string on) const {
	return this->fName == on;
}

bool
OptionDefinition::operator != (const OptionDefinition& od) const {
	return this->fName != od.fName;
}

bool
OptionDefinition::operator != (const string on) const {
	return this->fName != on;
}

OptionDefinition&
OptionDefinition::id(int id) {
	fId = id;
	return *this;
}

int
OptionDefinition::id() const {
	return fId;
}

OptionDefinition&
OptionDefinition::name(const string name) {
	if(name.empty()) {
		throw clipp::error::Length("OptionDefinition: Name cannot be empty.");
	}
	fName = name;
	return *this;
}

const string
OptionDefinition::name() const {
	return fName;
}

OptionDefinition&
OptionDefinition::type(OptionType ot) {
	fType = ot;
	return *this;
}

OptionDefinition::OptionType
OptionDefinition::type() const {
	return fType;
}

bool
OptionDefinition::isLongOption() const {
	return fName.length() > 1;
}

bool
OptionDefinition::isShortOption() const {
	return fName.length() == 1;
}

OptionDefinition&
OptionDefinition::typeString() {
	fType = OptionDefinition::TypeString;
	return *this;
}

OptionDefinition&
OptionDefinition::typeInteger() {
	fType = OptionDefinition::TypeInteger;
	return *this;
}

OptionDefinition&
OptionDefinition::typeFloat() {
	fType = OptionDefinition::TypeFloat;
	return *this;
}

OptionDefinition&
OptionDefinition::typeBool() {
	fType = OptionDefinition::TypeBoolean;
	return *this;
}

OptionDefinition&
OptionDefinition::alias(const string alias) {
	fAlias = alias;
	if(fOwner != NULL) {
		fOwner->alias(fName,alias,false);	// Importante el tercer parámetro (evita recursividad infinita).
	} else {
		throw clipp::error::OptionDefinition("FATAL: Owner of option definition (" + fName + ") is not set. Contact developer.");
	}
	return *this;
}

bool
OptionDefinition::hasAlias() const {
	return !fAlias.empty();
}

const string
OptionDefinition::alias() const {
	return fAlias;
}

OptionDefinition&
OptionDefinition::description(const string description) {
	fDescription = description;
	return *this;
}

const string
OptionDefinition::description() const {
	return fDescription;
}

void
OptionDefinition::hidden(bool hidden) {
	fHidden = hidden;
}

bool
OptionDefinition::hidden() const {
	return fHidden;
}

OptionDefinition&
OptionDefinition::required(bool required) {
	fRequired = required;
	return *this;
}

bool
OptionDefinition::isRequired() const {
	return fRequired;
}

OptionDefinition&
OptionDefinition::requiredIfOption(const string option) {
	// TODO: Implement this method.
	return *this;
}

bool
OptionDefinition::isRequiredIfOption(const string option) const {
	// TODO: Implement this method.
	return false;
}

OptionDefinition&
OptionDefinition::requiredIfNotOption(const string option) {
	// TODO: Implement this method.
	return *this;
}

bool
OptionDefinition::isRequiredIfNotOption(const string option) const {
	// TODO: Implement this method.
	return false;
}

OptionDefinition&
OptionDefinition::conflictsWith(const string conflicts) {
	fConflictsWith.clear();
	fConflictsWith.insert(conflicts);
	return *this;
}

OptionDefinition&
OptionDefinition::conflictsWith(StringSet conflicts) {
	fConflictsWith.clear();
	fConflictsWith.insert(conflicts.begin(), conflicts.end());
	return *this;
}

StringSet
OptionDefinition::conflictsWith() const {
	return fConflictsWith;
}

OptionDefinition&
OptionDefinition::argument(bool has, bool required) {
	fHasArgument = has;
	fArgumentRequired = required;
	return *this;
}

bool
OptionDefinition::hasArgument() const {
	return fHasArgument;
}

OptionDefinition&
OptionDefinition::argumentRequired(bool required) {
	fArgumentRequired = required;
	if(fArgumentRequired) {
		// Si el argumento es obligatorio, automáticamente se fija que tiene que
		// tener argumento, así no es necesario llamar a la función "argument()".
		fHasArgument = true;
	}
	return *this;
}

bool
OptionDefinition::isArgumentRequired() const {
	return fArgumentRequired;
}

OptionDefinition&
OptionDefinition::multiple(bool multiple) {
	fMultiple = multiple;
	checkExclusivity();
	return *this;
}

bool
OptionDefinition::isMultiple() const {
	return fMultiple;
}

OptionDefinition&
OptionDefinition::exclusive(bool exclusive) {
	fExclusive = exclusive;
	checkExclusivity();
	return *this;
}

bool
OptionDefinition::isExclusive() const {
	return fExclusive;
}

void
OptionDefinition::defaultValue(const string defaultValue) {
	fHasDefaultValue = true;
	fDefaultValue = defaultValue;
}

const string
OptionDefinition::defaultValue() const {
	return fDefaultValue;
}

void
OptionDefinition::removeDefaultValue() {
	fHasDefaultValue = false;
	fDefaultValue = "";
}

bool
OptionDefinition::hasDefaultValue() const {
	return fHasDefaultValue;
}

void
OptionDefinition::execute(OnOptionProcessed executeThis) {
	fExecute = executeThis;
}

OnOptionProcessed
OptionDefinition::execute() const {
	return fExecute;
}

OptionDefinition&
OptionDefinition::validString(const string validString) {
	fValidStrings.insert(validString);
	return *this;
}

OptionDefinition&
OptionDefinition::validStrings(const StringSet validStrings) {
	fValidStrings.insert(validStrings.begin(),validStrings.end());
	return *this;
}

OptionDefinition&
OptionDefinition::validStrings(const string validStrings[], int length) {
	if(length >= 0) {
		for(int i = 0; i < length; i++) {
			fValidStrings.insert(validStrings[i]);
		}
	}
	return *this;
}

StringSet
OptionDefinition::validStrings() const {
	return fValidStrings;
}

bool
OptionDefinition::hasMinValue(float& value) const {
	value = fMinValue;
	return fHasMinValue;
}

OptionDefinition&
OptionDefinition::minValue(float value) {
	fMinValue = value;
	return *this;
}

bool
OptionDefinition::hasMaxValue(float& value) const {
	value = fMaxValue;
	return fHasMaxValue;
}

OptionDefinition&
OptionDefinition::maxValue(float value) {
	fMaxValue = value;
	return *this;
}

}	// namespace clipp
