/*
 * Option.cpp
 *
 *  Created on: 04/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#include "../include/cli++/Option.hpp"
#include "../include/cli++/OptionDefinition.hpp"
#include "../include/cli++/Exceptions.hpp"
#include "../include/cli++/Utils.hpp"

namespace clipp {

static const size_t		MaxParameterCount	= 65536;

Option::Option(const string name, const string value)
	: fId(0),
	  fName(name),
	  fOccurrences(1),
	  fValues(),
	  fOptdef(NULL)
{
	if(fName.empty()) {
		throw length_error("Option: Option name cannot be empty.");
	}
	fValues.reserve(MaxParameterCount);
	if(!value.empty()) {
		fValues.push_back(value);
	}
}

Option::~Option() {
}

void
Option::updateVariable() {
	if(fOptdef->hasVar()) {
		switch(fOptdef->type()) {
			case OptionDefinition::TypeString:
				*(fOptdef->var<string>()) = fValues[0];
				break;
			case OptionDefinition::TypeInteger:
				*(fOptdef->var<int>()) = StringTo<int>(fValues[0], 0);
				break;
			case OptionDefinition::TypeFloat:
				*(fOptdef->var<float>()) = StringTo<float>(fValues[0], 0.0);
				break;
			case OptionDefinition::TypeBoolean:
				*(fOptdef->var<bool>()) = StringTo<bool>(fValues[0], false);
				break;
			case OptionDefinition::TypeNone:
			default:
				throw clipp::error::InvalidArgument("Cannot assign argument to variable: " + fValues[0]);
		}
	}
}

int
Option::id() const {
	return fId;
}

const string
Option::name() const {
	return fName;
}

bool
Option::isNegated() const {
	return fIsNegated;
}

const OptionDefinition*
Option::optionDefinition() const {
	return fOptdef;
}

void
Option::incOccurrenceCount() {
	fOccurrences++;
}

int
Option::occurrences() const {
	return fOccurrences;
}

void
Option::add(const string value) {
	if(!value.empty()) {
		fValues.push_back(value);
	}
}

void
Option::set(const string value, int index) {
	if(!value.empty()) {
		if(index >= 0 && index < (int)fValues.size()) {
			fValues[index] = value;
		} else {
			throw clipp::error::OutOfBounds("Cannot set value " + value + " at index " + StringFrom<int>(index) + " from option '" + fName + "'.", fName);
		}
	}
}

const string
Option::getAt(unsigned short index) const {
	if(index < (int)fValues.size()) {
		return fValues[index];
	} else {
		throw clipp::error::OutOfBounds("Cannot get value at index " + StringFrom<unsigned short>(index) + " from option '" + fName + "'.", fName);
	}
}

const string
Option::get() const {
	return getAt(0);
}

int
Option::countValues() const {
	return fValues.size();
}

void
Option::checkArguments() {
	if(fOptdef == NULL) {
		throw clipp::error::OptionDefinition("FATAL: OptionDefinition pointer from Option (_optdef) is NULL. Contact developer.");
	}
	for(Strings::const_iterator it = fValues.begin(); it != fValues.end(); ++it) {
		fOptdef->checkArgumentType(*it);
		fOptdef->checkArgumentValue(*it);
	}
}

}	// namespace clipp
