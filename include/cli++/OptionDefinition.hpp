/*
 * OptionDefinition.hpp
 *
 *  Created on: 04/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#ifndef OPTIONDEFINITION_HPP_
#define OPTIONDEFINITION_HPP_

#include <list>
#include <set>

namespace clipp {

/**
 * Lista y set de cadenas.
 */
typedef std::set<string>		StringSet;

/**
 * Declaración de la clase que contiene estas clases.
 */
class OptionManager;
class Option;

/**
 * Tipo de función que se ejecutará cuando se procesen las opciones.
 *
 * @param Option* option La opción procesada.
 * @return int Resultado.
 */
typedef int (*OnOptionProcessed)(const Option* option);

/**
 * Definición de una opción.
 */
class OptionDefinition {
	public:

		/**
		 * Tipo del valor del argumento de la opción.
		 */
		enum OptionType {
			TypeNone,
			TypeString,
			TypeInteger,
			TypeFloat,
			TypeBoolean
		};

	private:
		OptionManager*		fOwner;				/// Puntero al OpcionManager que tiene esta instancia.
		int					fId;				/// El id numérico de la opción.
		string				fName;				/// Nombre de la opción.
		OptionType			fType;				/// Tipo de la opción.
		string				fDescription;		/// Descripción de la opción.
		bool				fHidden;			/// Si está oculta.
		bool				fRequired;			/// Si es obligatoria.
		bool				fHasArgument;		/// Si tiene argumento.
		bool				fArgumentRequired;	/// Si el argumento es obligatorio.
		bool				fMultiple;			/// Si es múltiple.
		bool				fExclusive;			/// Si es exclusiva.
		bool				fAllowNoPreffix;	/// Si permite prefijo "no-" en opciones.
		string				fAlias;				/// Si tiene un alias (sólo uno).
		StringSet			fValidStrings;		/// Cadenas válidas para su argumento.
		StringSet			fConflictsWith;		/// Opciones con las que tiene conflicto (no pueden estar juntas).
		bool				fHasMinValue;		/// Si tiene valor mínimo si es numérico el argumento.
		float				fMinValue;			/// Valor mínimo.
		bool				fHasMaxValue;		/// Si tiene valor máximo.
		float				fMaxValue;			/// Valor máximo.
		bool				fHasDefaultValue;	/// Indica si tiene valor predeterminado o no.
		string				fDefaultValue;		/// Valor predeterminado de esta opción.
		OnOptionProcessed	fExecute;			/// Función ejecutada cuando se analizan todas las opciones.
		void*				fVar;				/// Pointer to variable to be updated.

		/**
		 * Clase amiga que puede acceder a las propiedades de esta clase.
		 */
		friend class	OptionManager;

		/**
		 * Si la opción es exclusiva no puede ser múltiple ni obligatoria.
		 * Se lanza excepción en caso de que se produzca.
		 *
		 * @throw OptionDefinitionException Si hay error en la opción.
		 */
		void			checkExclusivity() const;

		/**
		 * Clase que puede acceder a las siguientes funciones privadas.
		 */
		friend class	Option;

		/**
		 * Comprueba el tipo de argumento.
		 *
		 * @param string argument El argumento a comprobar.
		 * @throw clipp::Exception En caso de fallo.
		 */
		void			checkArgumentType(const string argument) const;

		/**
		 * Comprueba el valor del argrumento (rango y valores de cadena).
		 *
		 * @param string value Valor a comprobar.
		 * @throw clipp::Exception En caso de fallo.
		 */
		void			checkArgumentValue(const string value) const;

	public:

		/**
		 * Constructor de copia de la clase.
		 *
		 * @param OptionDefinition& od OptionDefintion a copiar.
		 */
		OptionDefinition(const OptionDefinition& od);

		/**
		 * Constructor de la clase.
		 *
		 * @param string name Nombre de la opción.
		 * @param OptionType type Tipo de la opción.
		 * @param string description Descripción.
		 */
		OptionDefinition(const string name, OptionType type = TypeString, const string description = string());

		/**
		 * Constructor de la clase.
		 *
		 * @param int id Id de la opción.
		 * @param string name Nombre de la opción.
		 * @param OptionType type Tipo de la opción.
		 * @param string description Descripción.
		 */
		OptionDefinition(int id, const string name, OptionType type = TypeString, const string description = string());

		/**
		 * Destructor de la clase.
		 */
		virtual 			~OptionDefinition();

		/**
		 * Operador de comparación de igualdad.
		 *
		 * @param OptionDefinition/string od Opción a comparar.
		 * @return bool True si son iguales, false en otro caso.
		 */
		bool				operator == (const OptionDefinition& od) const;
		bool				operator == (const string on) const;

		/**
		 * Operador de comparación de desigualdad.
		 *
		 * @param OptionDefinition/string od Opción a comparar.
		 * @return bool True si son distintos, false en otro caso.
		 */
		bool				operator != (const OptionDefinition& od) const;
		bool				operator != (const string on) const;

		/**
		 * Fija y devuelve el ID de la opción.
		 *
		 * @param int id ID.
		 * @return int ID.
		 */
		OptionDefinition&	id(int id);
		int					id() const;

		/**
		 * Fija y devuelve el nombre de la opción.
		 *
		 * @param string name Nombre de la opción.
		 * @return string Nombre de la opción.
		 */
		OptionDefinition&	name(const string name);
		const string		name() const;

		/**
		 * Fija y devuelve el tipo de la opción.
		 *
		 * @param OptionType ot Tipo de la opción.
		 * @return OptionDefinition& *this
		 * @return OptionType Tipo de la opción.
		 */
		OptionDefinition&	type(OptionType ot = TypeString);
		OptionType			type() const;

		/**
		 * Devuelve si es una opción larga o corta.
		 *
		 * @return bool True si se cumple la propiedad.
		 */
		bool				isLongOption() const;
		bool				isShortOption() const;

		/**
		 * Fija el tipo de la opción. Similar a "type(...)" pero con los
		 * parámetros ya definidos.
		 *
		 * @return OptionDefinition& *this
		 */
		OptionDefinition&	typeString();
		OptionDefinition&	typeInteger();
		OptionDefinition&	typeFloat();
		OptionDefinition&	typeBool();

		/**
		 * Fija un puntero a la variable que será actualizada con esta opción.
		 * 
		 * @param [type]* Puntero a la variable.
		 * @return OptionDefinition& *this
		 */
		OptionDefinition&	var(string* value);
		OptionDefinition&	var(char** value);
		OptionDefinition&	var(int* value);
		OptionDefinition&	var(float* value);
		OptionDefinition&	var(double* value);
		OptionDefinition&	var(bool* value);

		/**
		 * Devuelve el puntero a la variable a ser actualizada, pero no el tipo.
		 */
		void*				var() const;

		/**
		 * Devuelve un puntero a la variable a actualizar pero con un casting
		 * hecho a su tipo (según la plantilla).
		 */
		template <typename T>
		T*					var() const { return static_cast<T*>(var()); };

		/**
		 * Fija un alias, comprueba que el alias existe y devuelve el alias.
		 *
		 * @param string alias Alias.
		 * @return OptionDefinition& *this
		 * @return string Alias.
		 */
		OptionDefinition&	alias(const string alias);
		bool				hasAlias() const;
		const string		alias() const;

		/**
		 * Fija y devuelve la descripción de la opción.
		 *
		 * @param string description Descripción.
		 * @return OptionDefinition& *this
		 * @return string Descripción.
		 */
		OptionDefinition&	description(const string description);
		const string		description() const;

		/**
		 * Fija y devuelve si la opción está oculta.
		 *
		 * @param bool hidden True para ocultarla.
		 * @return bool True si está oculta.
		 */
		void				hidden(bool hidden);
		bool				hidden() const;

		/**
		 * Fija y devuelve si es obligatoria.
		 *
		 * @param bool value True si es obligatoria.
		 * @return OptionDefinition& *this
		 * @return bool True si es obligatoria.
		 */
		OptionDefinition&	required(bool required = true);
		bool				isRequired() const;

		/**
		 * Fija y devuelve si una opción es requerida sólo si ya existe una
		 * opción indicada. Este método se puede invocar varias veces.
		 *
		 * @param string option La opción requerida para que esta lo sea.
		 * @return OptionDefinition& this
		 * @return bool True si esta opción es requerida ante la pasada
		 * como parámetro.
		 */
		OptionDefinition&	requiredIfOption(const string option);
		bool				isRequiredIfOption(const string option) const;

		/**
		 * Fija y devuelve si una opción NO es requerida sólo si ya existe una
		 * opción indicada. Este método se puede invocar varias veces.
		 *
		 * @param string option La opción requerida para que esta lo sea.
		 * @return OptionDefinition& this
		 * @return bool True si esta opción NO es requerida ante la pasada
		 * como parámetro.
		 */
		OptionDefinition&	requiredIfNotOption(const string option);
		bool				isRequiredIfNotOption(const string option) const;

		/**
		 * Fija y devuelve las opciones con las que tiene conflicto (no pueden
		 * estar juntas).
		 *
		 * @param StringSet conflicts Lista de opciones con las que tiene
		 * conflicto.
		 * @return OptionDefinition& this
		 * @return StringSet& Lista de opciones con conflicto (referencia).
		 */
		OptionDefinition&	conflictsWith(const string conflicts);
		OptionDefinition&	conflictsWith(StringSet conflicts);
		StringSet			conflictsWith() const;

		/**
		 * Fija y devuelve si esta opción tiene argumento.
		 *
		 * @param bool value True si tiene argumento.
		 * @param bool required True si el argumento es obligatorio.
		 * @return OptionDefinition& *this
		 * @return bool True si tiene argumento.
		 */
		OptionDefinition&	argument(bool has = true, bool required = false);
		bool				hasArgument() const;

		/**
		 * Fija y devuelve si el argumento de esta opción es obligatorio.
		 *
		 * @param bool value True si el argumento es obligatorio.
		 * @return OptionDefinition& *this
		 * @return bool True si el argumento es obligatorio.
		 */
		OptionDefinition&	argumentRequired(bool required = true);
		bool				isArgumentRequired() const;

		/**
		 * Fija y devuelve si la opción es múltiple.
		 *
		 * @param bool value True si es múltiple.
		 * @return OptionDefinition& *this
		 * @return bool True si es múltiple.
		 */
		OptionDefinition&	multiple(bool multiple = true);
		bool				isMultiple() const;

		/**
		 * Fija y devuelve si la opción es exclusiva.
		 *
		 * @param bool value True si es exclusiva.
		 * @return OptionDefinition& *this
		 * @return bool True si es exclusiva.
		 */
		OptionDefinition&	exclusive(bool exclusive = true);
		bool				isExclusive() const;

		/**
		 * Permite opciones tal que --option y --no-option.
		 *
		 * @param bool allow True para permitir, false en otro caso.
		 * @return OptionDefinition& *this
		 * @return bool True si se permite el prefijo.
		 */
		OptionDefinition&	allowNoPreffix(bool allow = true);
		bool				allowNoPreffix() const;

		/**
		 * Fija y devuelve el valor predeterminado de esta opción.
		 *
		 * @param string defaultValue Valor por defecto.
		 * @return string Valor por defecto.
		 */
		void				defaultValue(const string defaultValue);
		const string		defaultValue() const;

		/**
		 * Elimina el valor predeterminado.
		 */
		void				removeDefaultValue();

		/**
		 * Indica si el valor predeterminado está fijado o no.
		 *
		 * @return bool True si está fijado, false en otro caso.
		 */
		bool				hasDefaultValue() const;

		/**
		 * Fija y devuelve un puntero a la función que hay que ejecutar
		 * cuando se procese esta opción.
		 *
		 * @param OnOptionProcessed executeThis Puntero a la función a
		 * ejecutar.
		 * @return OnOptionProcessed Puntero a la función a ejecutar.
		 */
		void				execute(OnOptionProcessed executeThis);
		OnOptionProcessed	execute() const;

		/**
		 * Fija las cadenas válidas para esta opción.
		 *
		 * @param string validString Cadena válida.
		 * @param StringSet validStrings Cadenas válidas.
		 * @param string[] validStrings Cadenas válidas.
		 * @param int length Longitud del array de cadenas válidas.
		 * @return OptionDefinition& *this
		 * @return StringSet Cadenas válidas.
		 */
		OptionDefinition&	validString(const string validString);
		OptionDefinition&	validStrings(const StringSet validStrings);
		OptionDefinition&	validStrings(const string validStrings[], int length);
		StringSet			validStrings() const;

		/**
		 * Devuelve si esta opción tiene valor mínimo en caso de que su
		 * argumento sea numérico.
		 *
		 * @param float& value Devuelve el valor mínimo en caso de que tenga
		 * valor mínimo.
		 * @return bool True si tiene valor mínimo, false en otro caso.
		 */
		bool				hasMinValue(float& value) const;

		/**
		 * Fija el valor mínimo.
		 *
		 * @param float value Valor mínimo.
		 * @return OptionDefinition& *this
		 */
		OptionDefinition&	minValue(float value);

		/**
		 * Devuelve si esta opción tiene valor máximo en caso de que su
		 * argumento sea numérico.
		 *
		 * @param float& value Devuelve el valor máximo en caso de que tenga
		 * valor máximo.
		 * @return bool True si tiene valor máximo, false en otro caso.
		 */
		bool				hasMaxValue(float& value) const;

		/**
		 * Fija el valor máximo.
		 *
		 * @param float value Valor máximo.
		 * @return OptionDefinition& *this
		 */
		OptionDefinition&	maxValue(float value);
};

}	// namespace clipp

#endif /* OPTIONDEFINITION_HPP_ */
