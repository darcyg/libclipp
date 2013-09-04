/*
 * Utils.hpp
 *
 *  Created on: 28/02/2012
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <iostream>
#include <sstream>

using std::stringstream;
using std::ios_base;

namespace clipp {

/**
 * Convierte un valor (el de la plantilla) a cadena. Por ejemplo, para
 * convertir números a cadenas: string i = StringFrom<int>(42);
 *
 * @param T value El valor a convertir.
 * @param ios_base f Si es un número, se puede especificar una base.
 * @return string Cadena representando el valor.
 */
template <typename T>
string
StringFrom(const T value, ios_base& (*f)(ios_base&) = NULL) {
	stringstream result;
	if(f != NULL) {
		result << f;
	}
	result << (T)value;
	return result.str();
}

/**
 * Devuelve un valor (el de la plantilla) dada su cadena. Por ejemplo para
 * convertir cadenas en números:
 *
 * int i = StringTo<int>("42");
 * int j = StringTo<int>("0xBE05",hex);
 *
 * @param string value El valor a convertir.
 * @param T defaultValue El valor por defecto si no se puede convertir.
 * @param ios_base f Una base en caso de usar números.
 * @return T El valor convertido.
 */
template <typename T>
T
StringTo(const string value, T defaultValue, ios_base& (*f)(ios_base&) = NULL) {
	stringstream iss(value);
	T result;
	if(f == NULL) {
		iss >> result;
	} else {
		iss >> f >> result;
	}
	return (iss.fail() ? defaultValue : result);
}

/**
 * Comprueba si una cadena dada es del tipo de dato indicado en T. Por ejemplo,
 * para saber si una cadena contiene un número.
 *
 * bool isNumeric = StringIs<int>("42");	// true
 * bool isNumber = StringIs<int>("42a");	// false
 *
 * @param string value El valor a comprobar.
 * @param ios_base f Una base en caso de que sea numérico.
 * @return bool True si es correcto, false en otro caso.
 */
template <typename T>
bool
StringIs(const string value, ios_base& (*f)(ios_base&) = NULL) {
	stringstream iss(value);
	T result;
	if(f == NULL) {
		iss >> result;
	} else {
		iss >> f >> result;
	}
	return !iss.fail() && iss.rdbuf()->in_avail() <= 0;
}

/**
 * Clase rango. Esta clase representa un rango de dos valores: mínimo y máximo.
 */
template <typename T>
class Range {
	public:
		/**
		 * Typedef para indicar el tipo de rango y que se pueda usar fuera de
		 * la clase; por ejemplo:
		 *
		 * Range<int> r;
		 *
		 * r.RangeType r2;
		 *
		 */
		typedef	T	RangeType;

		T			min;	///	El valor mínimo.
		T			max;	/// El valor máximo.

		/**
		 * Constructor por defecto de la clase.
		 */
		Range() : min(), max() {}

		/**
		 * Constructor de copia de la clase.
		 *
		 * @param Range& range Rango a copiar.
		 */
		Range(const Range& range) : min(range.min), max(range.max) {}

		/**
		 * Constructor de la clase.
		 *
		 * @param min
		 * @param max
		 */
		Range(T min, T max) : min(min), max(max) {}

		/**
		 * Destructor de la clase.
		 */
		virtual ~Range() {}

		/**
		 * Operador de asignación.
		 *
		 * @param Range& range El rango a asignar.
		 * @return Range& El rango asignado.
		 */
		virtual Range&	operator = (const Range& range) {
			if(this == &range) {
				return *this;
			}
			min = range.min;
			max = range.max;

			return *this;
		}

		/**
		 * Operador de igualdad. Un rango es igual a otro si su máximo
		 * y su mínimo son iguales.
		 *
		 * @param Range& range Rango a comparar.
		 * @return bool True si son iguales, false en otro caso.
		 */
		virtual bool operator == (const Range& range) const {
			return min == range.min && max == range.max;
		}

		/**
		 * Operador de desigualdad. Lo contrario al de igualdad.
		 *
		 * @param Range& range El rango a comparar.
		 * @return bool True si son distintos, false en otro caso.
		 */
		virtual bool operator != (const Range& range) const {
			return !(*this == range);
		}

		/**
		 * Comprueba si un valor del tipo T está en el rango.
		 *
		 * @param T value El valor a comprobar.
		 * @return bool True si está en el rango, false en otro caso.
		 *
		 * FIXME: Los rangos pueden ser abiertos o cerrados y esto no está
		 * contemplado.
		 */
		virtual bool in(T value) const {
			return value >= min && value <= max;
		}
};

/**
 * Función de ayuda que crea un rango.
 *
 * @param T min Mínimo del rango.
 * @param T max Máximo del rango.
 * @return Range<T> Rango creado.
 */
template <typename T>
inline Range<T>
MakeRange(T min, T max) {
	return Range<T>(min,max);
}

}	// namespace clipp

#endif /* UTILS_HPP_ */
