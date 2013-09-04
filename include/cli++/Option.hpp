/*
 * Option.hpp
 *
 *  Created on: 04/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#ifndef OPTION_HPP_
#define OPTION_HPP_

#include <string>
#include <vector>

using std::string;

#include <cli++/Utils.hpp>

namespace clipp {

/**
 * Vector de cadenas.
 */
typedef std::vector<string>	Strings;

/**
 * Definiciónes de clases para que puedan ser usadas aquí.
 */
class OptionDefinition;
class OptionManager;

/**
 * Esta clase mantiene el nombre de la opción y los posibles valores de los
 * argumentos de la línea de comandos según las definiciones de las opciones.
 */
class Option {
	private:
		int					fId;			/// Id numérico de la opción.
		string				fName;			/// Nombre de la opción.
		int					fOccurrences;	/// Número de veces que aparece.
		Strings				fValues;		/// Valores de los argumentos, si hay.
		OptionDefinition*	fOptdef;		/// Definición de opción donde está esta opción.

		/**
		 * Evitamos que esta clase se pueda clonar o asignar poniendo el
		 * constructor de copia y el operador de asignación en "private".
		 *
		 * @param Option& Opción a clonar.
		 * @return Option& Opción clonada.
		 */
		Option(const Option&);
		Option& operator = (const Option&);

		/**
		 * Clase que puede acceder a las propiedades internas de esta clase.
		 */
		friend class OptionManager;

	public:

		/**
		 * Constructor de la clase.
		 *
		 * @param string name Nombre de la opción.
		 * @param string value Valor de la opción.
		 */
		Option(const string name, const string value = string());

		/**
		 * Destructor de la clase.
		 */
		virtual 				~Option();

		/**
		 * Devuelve el ID numérico de la opción según se ha fijado en
		 * la definición de la misma.
		 *
		 * @return int Id numérico.
		 */
		int						id() const;

		/**
		 * Devuelve el nombre de la opción.
		 *
		 * @return string Nombre de la opción.
		 */
		const string			name() const;

		/**
		 * Devuelve la definición de opción donde está esta opción.
		 *
		 * @return OptionDefinition* Definición de opción.
		 */
		const OptionDefinition*	optionDefinition() const;

		/**
		 * Incrementa el número de ocurrencias de esta opción (el número de
		 * veces que aparece; sólo para el OptionManager).
		 */
		void					incOccurrenceCount();

		/**
		 * Devuelve el número de veces que aparece esta opción.
		 *
		 * @return int Número de veces que aparece esta opción.
		 */
		int						occurrences() const;

		/**
		 * Añade un valor de un argumento.
		 *
		 * @param string value Valor del argumento.
		 */
		void					add(const string value);

		/**
		 * Fija el valor de un argumento según una posición determinada.
		 *
		 * @param string value Valor del argumento.
		 * @param int index Índice del argumento a fijar.
		 */
		void					set(const string value, int index = 0);

		/**
		 * Devuelve el valor de un argumento según su índice.
		 *
		 * @param int index Índice del argumento.
		 * @return string Valor del argumento.
		 */
		const string			getAt(unsigned short index) const;

		/**
		 * Devuelve el valor del argumento en la posición cero (que suele
		 * ser la más común porque la mayoría de opciones sólo tendrán
		 * un argumento).
		 *
		 * @return string Valor del argumento.
		 */
		const string			get() const;

		/**
		 * Devuelve el valor de un argumento según su índice convertido
		 * al tipo indicado en la plantilla (template).
		 *
		 * @param T default_value Valor por defecto.
		 * @param int index Índice del argumento.
		 * @return T Valor del argumento convertido al tipo indicado.
		 */
		template <typename T>
		const T					getAt(T default_value, unsigned short index) const { return StringTo<T>(getAt(index), default_value); }

		/**
		 * Devuelve el valor de un argumento en el índice cero convertido
		 * al tipo indicado en la plantilla (template).
		 *
		 * @param T default_value Valor por defecto.
		 * @return T Valor del argumento convertido al tipo indicado.
		 */
		template <typename T>
		const T					get(T default_value) const { return StringTo<T>(get(), default_value); }

		/**
		 * Devuelve el número de argumentos.
		 *
		 * @return int Número de argumentos.
		 */
		int						countValues() const;

		/**
		 * Chequea que los argumentos sean correctos. Esta función hace uso
		 * de _optdef para saber qué tipo de parámetros tiene y cómo
		 * chequearlos.
		 */
		void					checkArguments();
};

}	// namespace clipp

#endif /* OPTION_HPP_ */
