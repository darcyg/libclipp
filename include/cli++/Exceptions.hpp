/*
 * Exceptions.hpp
 *
 *  Created on: 09/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#ifndef CLIPP_EXCEPTIONS_HPP_
#define CLIPP_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

using std::runtime_error;
using std::length_error;
using std::string;

namespace clipp {

namespace error {

/**
 * Clase base de las excepciones de esta librería.
 */
class Exception : public runtime_error {
	protected:
		string	_optionName;	/// Nombre de la opción que produce el error.
	public:

		/**
		 * Constructor de la clase.
		 * @param string msg Mensaje de la excepción.
		 */
		Exception(const string msg) : runtime_error(msg), _optionName() {}

		/**
		 * Constructor de la clase.
		 *
		 * @param string msg Mensaje de la excepción.
		 * @param string optionName Nombre de la opción que produce el error.
		 */
		Exception(const string msg, const string optionName) : runtime_error(msg), _optionName(optionName) {}

		/**
		 * Destructor de la clase.
		 */
		virtual ~Exception() throw() {}

		/**
		 * Devuelve el nombre de la opción que ha producido el error.
		 *
		 * @return string Nombre de la opción.
		 */
		virtual const string&	getOptionName() const { return _optionName; }

		/**
		 * Fija el nombre de la opción que produce el error.
		 *
		 * @param string option Nombre de la opción.
		 */
		virtual void setOptionName(const string option) { _optionName = option; }
};

/**
 * Excepción que se lanza cuando hay un fallo en la definición de una opción.
 */
class OptionDefinition : public Exception {
	public:
		OptionDefinition(const string msg) : Exception(msg) {}
		OptionDefinition(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando una opción es inválida.
 */
class InvalidOption : public Exception {
	public:
		InvalidOption(const string msg) : Exception(msg) {}
		InvalidOption(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando un argumento de una opción es inválido.
 */
class InvalidArgument : public Exception {
	public:
		InvalidArgument(const string msg) : Exception(msg) {}
		InvalidArgument(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando una opción es requerida y no está presente.
 */
class RequiredOption : public Exception {
	public:
		RequiredOption(const string msg) : Exception(msg) {}
		RequiredOption(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando un argumento es requerido y no está presente.
 */
class RequiredArgument : public Exception {
	public:
		RequiredArgument(const string msg) : Exception(msg) {}
		RequiredArgument(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando una opción es exclusiva y hay más.
 */
class ExclusiveOption: public Exception {
	public:
		ExclusiveOption(const string msg) : Exception(msg) {}
		ExclusiveOption(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando una opción no permite más de una aparición.
 */
class MultipleOption : public Exception {
	public:
		MultipleOption(const string msg) : Exception(msg) {}
		MultipleOption(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando una opción es ambigua.
 */
class AmbiguousOption : public Exception {
	public:
		AmbiguousOption(const string msg) : Exception(msg) {}
		AmbiguousOption(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando una opción está duplicada.
 */
class AlreadyExists : public Exception {
	public:
		AlreadyExists(const string msg) : Exception(msg) {}
		AlreadyExists(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando hay conflicto entre opciones.
 */
class Conflict : public Exception {
	public:
		Conflict(const string msg) : Exception(msg) {}
		Conflict(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando nos pasamos de los bordes de un conjunto.
 */
class OutOfBounds : public Exception {
	public:
		OutOfBounds(const string msg) : Exception(msg) {}
		OutOfBounds(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando hay un error en la longitud de algún elemento.
 */
class Length : public Exception {
	public:
		Length(const string msg) : Exception(msg) {}
		Length(const string msg, const string optionName) : Exception(msg,optionName) {}
};

/**
 * Excepción que se lanza cuando no han sido procesadas las opciones y se usan
 * métodos que lo necesitan.
 */
class Unprocessed : public Exception {
	public:
		Unprocessed(const string msg) : Exception(msg) {}
		Unprocessed(const string msg, const string optionName) : Exception(msg,optionName) {}
};

}	// namespace clipp::error

}	// namespace clipp

#endif /* CLIPP_EXCEPTIONS_HPP_ */
