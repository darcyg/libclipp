/*
 * OptionManager.cpp
 *
 *  Created on: 04/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#include <sstream>
#include <iomanip>

#include <iostream>
using namespace std;

using std::stringstream;

#include "../include/cli++/Exceptions.hpp"
#include "../include/cli++/OptionManager.hpp"
#include "../include/cli++/Utils.hpp"

namespace clipp {

namespace consts {

const char		OptionIdentifier			= '-';

const string	ShortOptionMarker			= "-";
const string	LongOptionMarker			= "--";
const char		OptionArgumentSeparator		= '=';
const string	AliasSeparator				= "|";
const string	NegationPreffix				= "no-";

}	// namespace consts

OptionManager::OptionManager(int argc, char** argv)
	: fRawOptions(),
	  fDefinitions(),
	  fOrderedDefinitions(),
	  fOptions(),
	  fOptionsIterator(),
	  fArguments(),
	  fArgumentsCount(-1,-1),
	  fExecute(NULL),
	  fCredits(),
	  fUsage(),
	  fArgc(argc),
	  fArgv(argv)
{
	// Metemos los argumentos en una cola
	for(int i = 0; i < argc; i++) {
		fRawOptions.push_back(argv[i]);
	}
}

OptionManager::~OptionManager() {
	// Limpiamos la lista de definiciones de opciones ordenada, pero no
	// las borramos aquí, las borramos en el siguiente bucle.
	fOrderedDefinitions.clear();

	// Borramos las definiciones de las opciones (ahora sí).
	for(OptionDefinitionMap::iterator it = fDefinitions.begin(); it != fDefinitions.end(); ++it) {
		if(it->first == it->second->name()) {
			delete it->second;
		} else {
			it->second->fAlias = "";
		}
	}
	fDefinitions.clear();

	// Borramos la lista de opciones (en esta lista están repetidas, pero como son punteros pues
	// no ocupa más memoria; eso sí, aquí no las borramos, las borramos en el siguiente bucle).
	fOptionsList.clear();

	// Borramos las opciones, esta vez sí, liberando la memoria.
	for(OptionMap::iterator it = fOptions.begin(); it != fOptions.end(); ++it) {
		if(it->second != NULL) {
			delete it->second;
			it->second = NULL;
		}
	}
	fOptions.clear();
}

const string
OptionManager::argumentHelpDescription(const OptionDefinition* optdef) const {
	string result = "";
	switch(optdef->type()) {
		case OptionDefinition::TypeBoolean:
			result = "%b";
			break;
		case OptionDefinition::TypeFloat:
			result = "%f";
			break;
		case OptionDefinition::TypeInteger:
			result = "%i";
			break;
		case OptionDefinition::TypeString:
			result = "%s";
			break;
		case OptionDefinition::TypeNone:
		default:
			result = "%a";
			break;
	}
	return (optdef->isArgumentRequired() ? result : "[" + result + "]");
}

bool
OptionManager::hasOptionDefinition(const string name) const {
	return fDefinitions.find(name) != fDefinitions.end();
}

OptionDefinition*
OptionManager::getOptionDefinition(const string nameOrAlias) const {
	OptionDefinitionMap::const_iterator finder = fDefinitions.find(nameOrAlias);
	if(finder != fDefinitions.end()) {
		return finder->second;
	} else {
		return NULL;
	}
}

const string
OptionManager::popRawOption() {
	string result = fRawOptions.front();
	fRawOptions.pop_front();
	return result;
}

const string
OptionManager::peekRawOption() const {
	return fRawOptions.front();
}

unsigned int
OptionManager::remainRawOptions() const {
	return fRawOptions.size();
}

bool
OptionManager::isOption(const string option) const {
	return isShortOption(option) || isLongOption(option);
}

bool
OptionManager::isShortOption(const string option) const {
	return option.length() > 1 && option[0] == consts::OptionIdentifier && option[1] != consts::OptionIdentifier;
}

bool
OptionManager::isLongOption(const string option) const {
	return option.length() > 2 && option[0] == consts::OptionIdentifier && option[1] == consts::OptionIdentifier;
}

const string
OptionManager::normalizeOption(const string option, bool isShort) const {
	return option.substr(isShort ? 1 : 2);
}

void
OptionManager::decodeShortOption(const string option, bool normalize) {
	string op = (normalize ? normalizeOption(option,true) : option);
	if(op.length() == 1) {
		// La opción es de un sólo caracter. Se procesa normalmente (option: -a -> op: a).
		OptionDefinition* optdef = getOptionDefinition(op);
		if(optdef != NULL) {
			postProcessOption(optdef, op);
		} else {
			throw clipp::error::InvalidOption("Invalid option: " + option, option);
		}
	} else {
		// La opción tiene más de un carácter. Pueden pasar dos cosas: o que la opción lleve el parámetro junto a ella,
		// o que la opción lleve junto a ella otra y otras opciones de un sólo caracter (option: -av -> op: -a -v || op: -a v).
		string singleoption = op.substr(0, 1);
		const OptionDefinition* optdef = getOptionDefinition(singleoption);
		if(optdef != NULL) {
			// La opción existe. Se procesa.
			if(optdef->hasArgument()) {
				// La opción tiene un argumento, de momento opcional.
				if(optdef->isArgumentRequired()) {
					// La opción tiene argumento obligatorio. Se coge y se añade dicha opción.
					addOption(optdef, op.substr(1));
				} else {
					// La opción tiene un argument opcional. Miramos a ver si el primer elemento de este argumento es una opción.
					// Si lo es, es que es ambigua. Si no lo es, es que es el argumento.
					string secondSortOption = op.substr(1, 2);
					if(hasOptionDefinition(secondSortOption)) {
						// La opción es ambigua.
						throw clipp::error::AmbiguousOption("Ambiguous option construction: " + option, option);
					} else {
						addOption(optdef, op.substr(1));
					}
				}
			} else {
				// La opción no tiene argumento por lo que se añade.
				addOption(optdef);
				// El resto de la parte de esta opción se procesa sólo como opciones cortas. Como esto es
				// recursivo, si hay más de dos opciones seguidas, se procesará sin problemas.
				decodeShortOption(op.substr(1),false);
			}
		} else {
			// La opción no existe. Se lanza excepción.
			throw clipp::error::InvalidOption("Invalid option: " + option, option);
		}
	}
}

void
OptionManager::decodeLongOption(const string option, bool normalize) {
	string op = (normalize ? normalizeOption(option, false) : option);
	OptionDefinition* optdef = getOptionDefinition(op);
	if(optdef != NULL) {
		// La opción existe.
		postProcessOption(optdef, op);
	} else {
		// La opción no existe, pero puede que sea de la forma --option=argument o una negada: --no-option
		size_t position = option.find_first_of(consts::OptionArgumentSeparator);
		if(position != string::npos) {
			// La opción tiene el argumento de la forma --option=argument
			string processedOption = normalizeOption(option.substr(0, position));
			string argument = option.substr(position + 1);
			optdef = getOptionDefinition(processedOption);
			if(optdef != NULL) {
				if(optdef->hasArgument()) {
					addOption(optdef, argument);
				} else {
					// La opción tiene argumento pero en realidad no lo lleva.
					throw clipp::error::InvalidArgument("Invalid argument for option: --" + processedOption, processedOption);
				}
			} else {
				throw clipp::error::InvalidOption("Invalid option: -" + processedOption, processedOption);
			}
		} else if(op.substr(0, consts::NegationPreffix.size()) == consts::NegationPreffix) {	// Comprobamos si la opción es la negada y si existe.
			// TODO: No se soportan negadas con argumento.
			// Puede que sea una opción negada. Veamos si existe una normal...
			op = op.substr(consts::NegationPreffix.size(), -1);
			optdef = getOptionDefinition(op);
			if(optdef != NULL) {
				postProcessOption(optdef, op, true);
			} else {
				// La opción es una opción negada pero no hay ninguna normal válida para esa negación.
				throw clipp::error::InvalidOption("Invalid option: --" + consts::NegationPreffix + op, op);
			}
		} else {
			// La opción no existe ni tiene el argumento de la forma --option=argument ni es una negada como --no-option así que...
			throw clipp::error::InvalidOption("Invalid option: " + option, option);
		}
	}
}

void
OptionManager::postProcessOption(const OptionDefinition* optdef, const string realOption, bool isNegated) {
	if(optdef->hasArgument()) {
		// Si tiene argumentos...
		if(optdef->isArgumentRequired()) {
			if(remainRawOptions() > 0) {
				// Pueden ser obligatorios...
				if(!isOption(peekRawOption())) {
					addOption(optdef, popRawOption(), isNegated);
				} else {
					//throw clipp::error::RequiredArgument("Argument required for option: " + string(optdef->isShortOption() ? "-" : "--") + optdef->name() + " (" + realOption + ")", optdef->name());
					throw clipp::error::RequiredArgument("Argument required for option: " + string(realOption.length() == 1 ? "-" : "--") + realOption, realOption);
				}
			} else {
				//throw clipp::error::RequiredArgument("Argument required for option: " + string(optdef->isShortOption() ? "-" : "--") + optdef->name() + " (" + realOption + ")", optdef->name());
				throw clipp::error::RequiredArgument("Argument required for option: " + string(realOption.length() == 1 ? "-" : "--") + realOption, realOption);
			}
		} else {
			if(remainRawOptions() > 0) {
				// ...o pueden ser opcionales.
				// Se añade la opción y el argumento en caso de que este no sea otra opción, ya que este argumento es opcional.
				addOption(optdef, (isOption(peekRawOption()) ? "" : popRawOption()), isNegated);
			} else {
				// No hay más argumentos en la línea de comandos, así que se añade opción normal.
				addOption(optdef, "", isNegated);
			}
		}
	} else {
		// Si no tiene argumentos ni opcionales ni obligatorios, se añade directamente a las opciones.
		addOption(optdef, "", isNegated);
	}
}

void
OptionManager::addOption(const OptionDefinition* optdef, const string argument, bool isNegated) {
	string name = optdef->name();
	if(hasOption(name)) {
		if(optdef->isMultiple()) {
			fOptions[name]->incOccurrenceCount();
			if(!argument.empty()) {
				fOptions[name]->add(argument);
			}
			fOptionsList.push_back(fOptions[name]);	// En esta lista están ordenadas según la línea de comandos y duplicadas (son punteros).
		} else {
			throw clipp::error::MultipleOption("Multiple option not allowed: " + string(optdef->isShortOption() ? "-" : "--") + name, name);
		}
	} else {
		Option* option = new Option(name, argument);
		option->fOptdef = const_cast<OptionDefinition*>(optdef);
		option->fIsNegated = isNegated;
		fOptions[name] = option;
		fOptionsList.push_back(option);	// en esta lista están ordenadas según la línea de comandos y duplicadas (son punteros).
	}
	fOptions[name]->fId = optdef->id();
}

void
OptionManager::checkProcessedOptions() const {
	bool exclusivity = false;
	string exclusiveOption = "";
	bool isShortOption = false;
	// Comprobamos si la opción es exclusiva y hay otras diferentes.
	for(OptionDefinitionMap::const_iterator it = fDefinitions.begin(); it != fDefinitions.end(); ++it) {
		if(hasOption(it->first) && it->second->isExclusive()) {
			exclusivity = true;
			exclusiveOption = it->first;
			isShortOption = it->second->isShortOption();
			break;
		}
	}

	if(exclusivity && fOptions.size() > 1) {
		throw clipp::error::ExclusiveOption("Option is exclusive: " + string(isShortOption ? "-" : "--") + exclusiveOption, exclusiveOption);
	}

	// Comprobamos si una opción es requerida y no está entre las opciones.
	if(!exclusivity) {
		for(OptionDefinitionMap::const_iterator it = fDefinitions.begin(); it != fDefinitions.end(); ++it) {
			if(it->second->isRequired() && !hasOption(it->second->name())) {
				throw clipp::error::RequiredOption("Option required: " + string(it->second->isShortOption() ? "-" : "--") + it->second->name(), it->second->name());
			}
		}
	}

	// Comprobamos los parámetros de las opciones procesadas.
	for(OptionMap::const_iterator it = fOptions.begin(); it != fOptions.end(); ++it) {
		it->second->checkArguments();
	}
}

void
OptionManager::checkConflicts() const {
	for(OptionMap::const_iterator it = fOptions.begin(); it != fOptions.end(); ++it) {
		StringSet conflicts = it->second->optionDefinition()->conflictsWith();
		for(StringSet::const_iterator sit = conflicts.begin(); sit != conflicts.end(); ++sit) {
			if(hasOption(*sit)) {
				throw clipp::error::Conflict("Option '" + string(it->second->optionDefinition()->isShortOption() ? "-" : "--") + it->second->name() +
						"' conflicts with option '" + string((*sit).length() > 1 ? "--" : "-") + (*sit) + "'.");
			}
		}
	}
}

void
OptionManager::processArguments() const {
	if(fArgumentsCount.min > 0 && (int)fArguments.size() < fArgumentsCount.min) {
		throw clipp::error::Length("Must have " + StringFrom<int>(fArgumentsCount.min) + " arguments at least and " + StringFrom<int>(fArguments.size()) + " arguments have been supplied.");
	} else if(fArgumentsCount.max > 0 && (int)fArguments.size() > fArgumentsCount.max) {
		throw clipp::error::Length("Must have " + StringFrom<int>(fArgumentsCount.max) + " arguments or less and " + StringFrom<int>(fArguments.size()) + " arguments have been supplied.");
	}
}

void
OptionManager::executeFunctionsForOptions() const {
	for(OptionMap::const_iterator it = fOptions.begin(); it != fOptions.end(); ++it) {
		const OptionDefinition* od = it->second->optionDefinition();
		if(od->execute() != NULL) {
			od->execute()(it->second);
		}
	}
}

void
OptionManager::executeFunctionForArguments() const {
	if(fExecute != NULL) {
		int i = 0;
		for(Strings::const_iterator it = fArguments.begin(); it != fArguments.end(); ++it, i++) {
			fExecute(*it,i);
		}
	}
}

void
OptionManager::process() {
	bool lastArgument = false;

	// El primer elemento es el nombre del programa, que metemos como el argumento posicional cero.
	fArguments.push_back(popRawOption());

	// Procesamos todas las opciones.
	while(remainRawOptions() > 0) {
		string option = popRawOption();
		if(lastArgument) {
			fArguments.push_back(option);
		} else {
			if(isShortOption(option)) {
				// Es una opción corta.
				decodeShortOption(option);
			} else if(isLongOption(option)) {
				// Es una opción larga.
				decodeLongOption(option);
			} else if(option == consts::ShortOptionMarker) {
				// Argumento de entrada estándar.
				fArguments.push_back(option);
			} else if(option == consts::LongOptionMarker) {
				// Opción que indica que el resto de opciones son argumentos.
				lastArgument = true;
			} else {
				// Si no es nada de lo anterior es que es un argumento.
				fArguments.push_back(option);
			}
		}
	}

	// Procesamos si están todas las opciones que tiene que estar.
	checkProcessedOptions();

	// Procesamos si hay conflictos entre opciones.
	checkConflicts();

	// Procesamos los argumentos (el número correcto).
	processArguments();

	// Ejectua las funciones definidas para las opciones.
	executeFunctionsForOptions();

	// Ejecuta la función para los argumentos.
	executeFunctionForArguments();

	// Actualizamos el iterador de opciones por si se procesan por ID.
	fOptionsIterator = fOptionsList.begin();
}

int
OptionManager::countDefinedOptions() const {
	return fDefinitions.size();
}

int
OptionManager::countProcessedOptions() const {
	return fOptions.size();
}

void
OptionManager::credits(const string credits) {
	fCredits = credits;
}

const string
OptionManager::credits() const {
	return fCredits;
}

void
OptionManager::usage(const string usage) {
	fUsage = usage;
}

const string
OptionManager::usage() const {
	return fUsage;
}

const string
OptionManager::description(const string title, unsigned int padding) const {
	if(padding > 32) {
		// No nos pasemos con el padding... :)
		padding = 2;
	}
	int option_padding = 0;
	// Hacemos cálculos...
	for(OptionDefinitionMap::const_iterator it = fDefinitions.begin(); it != fDefinitions.end(); ++it) {
		int calculated = (it->second->isShortOption() ? consts::ShortOptionMarker.length() : consts::LongOptionMarker.length()) +	// Longitud de "-" o "--" dependiendo de si es opción corta o larga.
						 it->second->name().length() +	// Longitud del nombre de la opción.
						 (it->second->hasAlias() ? (it->second->alias().length() == 1 ? consts::ShortOptionMarker.length() : consts::LongOptionMarker.length()) : 0) +	// Longitud del alias más "-" o "--" dependiendo de si es corto o largo.
						 (it->second->hasArgument() ? 2 : 0) +	// Longitud de "[%a]" (lleva argumento opcional).
						 (it->second->isArgumentRequired() ? 2 : 0);	// Si es requerido, se suma 2.
		if(calculated > option_padding) {
			option_padding = calculated;
		}
	}

	stringstream result;
	string padding_string;
	padding_string.insert(0,padding,' ');
	// Imprimimos las opciones. Primero el título, si hay.
	if(!title.empty()) {
		result << title << "\n";
	}
	// Luego el resto de opciones.
	const OptionDefinition* optdef = NULL;
	for(OptionDefinitionList::const_iterator it = fOrderedDefinitions.begin(); it != fOrderedDefinitions.end(); ++it) {
		optdef = *it;
		if(optdef->name() != optdef->alias()) {	// Si la opción del mapa es diferente al alias de la definición, se imprime.
			string option = (optdef->isShortOption() ? consts::ShortOptionMarker : consts::LongOptionMarker) +	// El separador ("-" o "--").
							optdef->name() +	// El nombre de la opción.
							(optdef->hasAlias() ? (consts::AliasSeparator + (optdef->alias().length() == 1 ? consts::ShortOptionMarker : consts::LongOptionMarker) + optdef->alias()) : "") +	// El alias (con todo_ lo que lleva).
							(optdef->hasArgument() ? " " + argumentHelpDescription(optdef) : "");	// El argumento.
			result << padding_string << std::setw(option_padding) << std::left << option << " " << optdef->description() << "\n";	// Componemos la ayuda con la opción y la descripción.
		}
	}
	return result.str();
}

const string
OptionManager::description(unsigned int padding) const {
	return description("", padding);
}

Range<int>&
OptionManager::argumentCountRange() {
	return fArgumentsCount;
}

OptionDefinition&
OptionManager::option(const string option, int id) {
	if(option.empty()) {
		throw clipp::error::OptionDefinition("Cannot add an empty option definition.");
	}
	OptionDefinition* od = getOptionDefinition(option);
	if(od == NULL) {
		od = new OptionDefinition(id, option);
		od->fOwner = this;
		fDefinitions[option] = od;
		fOrderedDefinitions.push_back(od);
	} else {
		throw clipp::error::AlreadyExists("OptionManager: Cannot add option '" + option + "'. " + (option.length() == 1 ? "Short" : "Long") + " option already exists.");
	}
	return *od;
}

void
OptionManager::alias(const string option, const string alias, bool callOptionDefinitionAliasFunction) {
	if(!hasOptionDefinition(option)) {
		throw clipp::error::AlreadyExists("OptionManager: Cannot assign alias '" + alias + "' for option '" + option + "'. Option does not exist.");
	} else if(alias.empty() || hasOptionDefinition(alias)) {
		throw clipp::error::AlreadyExists("OptionManager: Cannot assign alias '" + alias + "' for option '" + option + "'. Alias already exists.");
	} else {
		OptionDefinition* od = getOptionDefinition(option);
		// FIXME: Puede que ya tenga un alias puesto, ¿qué hacemos? ¿No permitimos o modificamos OptionDefinition
		// para que pueda tener un número indeterminado de alias? Mejor lo segundo, digo yo...
		if(callOptionDefinitionAliasFunction) {
			od->alias(alias);
		}
		fDefinitions[alias] = od;
	}
}

void
OptionManager::alias(const string option, const string alias) {
	this->alias(option, alias, true);
}

bool
OptionManager::hasOption(const string name) const {
	return fOptions.find(name) != fOptions.end();
}

bool
OptionManager::hasOption(const int id) const {
	// No hay mapa de opciones por id, así que la búsqueda es secuencial.
	for(OptionList::const_iterator it = fOptionsList.begin(); it != fOptionsList.end(); ++it) {
		if((*it)->id() == id) {
			return true;
		}
	}
	return false;
}

const Option*
OptionManager::getOption(const string name) const {
	OptionMap::const_iterator finder = fOptions.find(name);
	if(finder != fOptions.end()) {
		return finder->second;
	} else {
		return NULL;
	}
}

int
OptionManager::countArguments() const {
	return fArguments.size();
}

const string
OptionManager::getArgument(int index) const {
	if(index >= 0 && index < (int)fArguments.size()) {
		return fArguments[index];
	} else {
		throw clipp::error::OutOfBounds("OptionManager: Cannot get argument at index " + StringFrom<int>(index) + ". Index out of bounds (0.." + StringFrom<int>(fArguments.size() - 1) + ").");
	}
}

const string
OptionManager::firstArgument() const {
	if(fArguments.size() > 0) {
		return fArguments[0];
	} else {
		throw clipp::error::OutOfBounds("OptionManager: Cannot get first argument. There is no arguments.");
	}
}

const string
OptionManager::lastArgument() const {
	if(fArguments.size() > 0) {
		return fArguments[fArguments.size()-1];
	} else {
		throw clipp::error::OutOfBounds("OptionManager: Cannot get last argument. There is no arguments.");
	}
}

const Option*
OptionManager::next() {
	if(fRawOptions.size() > 0) {
		// Si hay opciones es que no han sido procesadas, así que no se pueden obtener los ID sin procesar primero.
		throw clipp::error::Unprocessed("OptionManager: Options are unprocessed. Use 'process()' first.");
	}
	if(fOptionsIterator == fOptionsList.end()) {
		// Si el iterador está al final, devolvemos NULL.
		return NULL;
	}
	Option* option = (*fOptionsIterator);
	++fOptionsIterator;
	return option;
}

void
OptionManager::rewind() {
	fOptionsIterator = fOptionsList.begin();
}

void
OptionManager::executeOnArgument(OnArgumentProcessed executeThis) {
	fExecute = executeThis;
}

static
const string
implode(const string glue, const Strings strings) {
	string result;
	for(Strings::const_iterator it = strings.begin(); it != strings.end(); ++it) {
		if(!(*it).empty()) {
			result += *it + glue;
		}
	}
	result = result.substr(0,result.length()-2);
	return result;
}

const string
OptionManager::dump() const {

	stringstream result;
	result << "OptionManager {\n";
	result << "     Command line arguments {\n        argc: " << fArgc << "\n";
	result << "        argv:";
	stringstream lineargv;
	stringstream itemsargv;
	for(int i = 0; i < fArgc; i++) {
		lineargv << " " << fArgv[i];
		itemsargv << "        argv[" << i << "]=" << fArgv[i] << "\n";
	}
	result << lineargv.str() << "\n" << itemsargv.str();
	result << "\n    }\n";
	result << "    Options definitions (" << fDefinitions.size() << ") {\n";
	for(OptionDefinitionMap::const_iterator it = fDefinitions.begin(); it != fDefinitions.end(); ++it) {
		result << "        Option '" << it->first << "' (id:" << it->second->id() << ")";
		Strings items;
		items.push_back(!it->second->alias().empty() && it->first != it->second->alias() ? ("with alias '" + it->second->alias() + "'") : "");
		items.push_back(it->second->isRequired() ? "is required" : "");
		items.push_back(it->second->hasArgument() ? (it->second->isArgumentRequired() ? "has required argument" : "has argument") : "");
		items.push_back(it->second->isMultiple() ? "is multiple" : "");
		items.push_back(it->second->isExclusive() ? "is exclusive" : "");
		result << " " << implode(", ",items) << ".\n";
	}
	result << "    }\n";
	result << "    Options processed (" << fOptions.size() << ") {\n";
	for(OptionMap::const_iterator it = fOptions.begin(); it != fOptions.end(); ++it) {
		result << "        Option '" << it->second->name() << "' {\n";
		result << "              Real name: " << it->second->name() << "\n";
		result << "            Occurrences: " << it->second->occurrences() << "\n";
		result << "             Arg. count: " << it->second->countValues() << "\n";
		result << "              Arguments:";
		for(int i = 0; i < it->second->countValues(); i++) {
			result << " " << it->second->getAt(i);
		}
		result << "\n        }\n";
	}
	result << "    }\n";
	result << "    Positional arguments(" << fArguments.size() << ") {\n";
	int i = 0;
	for(Strings::const_iterator it = fArguments.begin(); it != fArguments.end(); ++it, i++) {
		result << "        [" << i << "]: " << *it << "\n";
	}
	result << "    }\n";
	result << "}";
	return result.str();
}

ostream&
operator << (ostream& os, const OptionManager& om) {
	string credits = om.credits();
	os << (credits.empty() ? "" : credits + "\n");
	string usage = om.usage();
	os << (usage.empty() ? "" : "Usage: " + usage + "\n");
	os << om.description("Options:",2);
	return os;
}

}	// namespace clipp
