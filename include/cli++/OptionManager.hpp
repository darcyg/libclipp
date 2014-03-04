/*
 * OptionManager.hpp
 *
 *  Created on: 04/05/2011
 *        User: diego
 *      Author: Diego Lago <diego.lago.gonzalez@gmail.com>
 */

#ifndef OPTIONMANAGER_HPP_
#define OPTIONMANAGER_HPP_

#include <iostream>
#include <deque>

#if __cplusplus >= 201103L
#include <unordered_map>
#else
#include <map>
#endif

using std::ostream;

#include <cli++/OptionDefinition.hpp>
#include <cli++/Option.hpp>
#include <cli++/Utils.hpp>
#include <cli++/Exceptions.hpp>

namespace clipp {

/**
 * Tipo de función que se ejecuta cuando se procesen los argumentos.
 *
 * @param string argument El argumento procesado.
 * @param int position Posición del argumento.
 * @return int Resultado.
 */
typedef int (*OnArgumentProcessed)(const string argument, int position);

/**
 * Clase que gestiona las definiciones de las opciones de la línea de comandos
 * y que analiza dicha línea para ver si coinciden los valores pasados con
 * las definiciones que mantiene.
 */
class OptionManager {
	public:

		/**
		 * Tipo de error que se puede producir.
		 */
		enum Error {
			ERROR_INVALID_OPTION,
			ERROR_LAST_ERROR
		};

	private:

		/**
		 * Tipo de dato que mantiene las definiciones de opciones. Si está
		 * disponible C++11 se usa 'unordered_map' porque es más rápido a la
		 * hora de obtener las opciones, aunque realmente no mantiene el orden
		 * de inserción, sino un orden basado en una función hash.
		 */
#if __cplusplus >= 201103L
		typedef std::unordered_map<string, OptionDefinition*>	OptionDefinitionMap;
#else
		typedef std::map<string, OptionDefinition*>				OptionDefinitionMap;
#endif

		/**
		 * Tipo de dato que mantiene las opciones leídas. Si está
		 * disponible C++11 se usa 'unordered_map' porque es más rápido a la
		 * hora de obtener las opciones, aunque realmente no mantiene el orden
		 * de inserción, sino un orden basado en una función hash.
		 */
#if __cplusplus >= 201103L
		typedef std::unordered_map<string, Option*>				OptionMap;
#else
		typedef std::map<string, Option*>						OptionMap;
#endif

		typedef std::list<OptionDefinition*>					OptionDefinitionList;
		typedef	std::list<Option*>								OptionList;

		std::deque<string>			fRawOptions;				/// Opciones sin procesar.

		OptionDefinitionMap			fDefinitions;				/// Las definiciones.
		OptionDefinitionList		fOrderedDefinitions;		/// Las definiciones ordenadas según la inserción (para mostrarlas en la ayuda).
		OptionMap					fOptions;					/// Las opciones.
		OptionList					fOptionsList;				/// Lista de opciones (ordenadas por proceso).
		OptionList::iterator		fOptionsIterator;			/// Iterador para recorrer las opciones por ID.
		Strings						fArguments;					/// Los argumentos sin opción.

		string						fErrors[ERROR_LAST_ERROR];	/// Las descripciones de los errores.

		Range<int>					fArgumentsCount;			/// Número mínimo y máximo de argumentos posicionales.
		OnArgumentProcessed			fExecute;					/// Función a ejecutar por cada argumento.

		string						fCredits;					/// Cadena con la descripción del título de la aplicación y los créditos.
		string						fUsage;						/// Cadena con la información de uso.

		int							fArgc;						/// Contador de argumentos de la línea de comandos.
		char**						fArgv;						/// Argumentos de la línea de comandos.

		/**
		 * Clase amiga para usar la función "alias()".
		 */
		friend class	OptionDefinition;

		/**
		 * Devuelve una cadena representnado el argumento para la ayuda. Si el
		 * argumento no tiene tipo, devuelve '%a'; si es un entero devuelve
		 * '%i'; si es un flotante devuelve '%f'; si es un booleano devuelve
		 * '%b'; y si es una cadena devuelve '%s'. Si no es requerido, se
		 * devuelve entre corchetes.
		 *
		 * @param OptionDefinition* optdef Definición.
		 * @return string Cadena representando el argumento.
		 */
		const string				argumentHelpDescription(const OptionDefinition* optdef) const;

		/**
		 * Crea un alias para una opción.
		 *
		 * @param string option Nombre de la opción.
		 * @param string alias Alias de la opción.
		 * @param bool callOptionDefinitionAliasFunction True para llamar
		 * a la función "alias" de la OptionDefinition, false para no llamarlo.
		 */
		void						alias(const string option, const string alias, bool callOptionDefinitionAliasFunction);


		/**
		 * Devuelve si una opción o alias existe.
		 *
		 * @param string nameOrAlias Nombre o alias de la opción.
		 * @return bool True si existe, false en otro caso.
		 */
		bool						hasOptionDefinition(const string nameOrAlias) const;

		/**
		 * Devuelve una definición de opción, o NULL si no existe.
		 *
		 * @param string nameOrAlias Nombre o alias de la opción.
		 * @return OptionDefinicion* Definición de opción o NULL.
		 */
		OptionDefinition*			getOptionDefinition(const string nameOrAlias) const;

		/**
		 * Devuelve la primera opción de la lista de opciones sin procesar
		 * y la saca de dicha lista.
		 *
		 * @return string Opción.
		 */
		const string				popRawOption();

		/**
		 * Devuelve la primera opción de la lista de opciones sin procesar
		 * pero no la saca de la lista.
		 *
		 * @return string Opción.
		 */
		const string				peekRawOption() const;

		/**
		 * Devuelve si todavía quedan opciones sin procesar.
		 *
		 * @return uint Número de opciones que quedan sin procesar.
		 */
		inline unsigned int			remainRawOptions() const;

		/**
		 * Devuelve si un parámetro de la línea de comandos es una opción,
		 * es decir, comienza por "-" y más caracters o por "--" y más
		 * caracteres.
		 *
		 * @param string option Opción.
		 * @return bool True si es opción, false en otro caso.
		 */
		inline bool					isOption(const string option) const;

		/**
		 * Devuelve si una opción es corta (si comienza por "-" y tiene más
		 * caracteres).
		 *
		 * @param string option La opción.
		 * @return bool True si es corta, false en otro caso.
		 */
		inline bool					isShortOption(const string option) const;

		/**
		 * Devuelve si una opción es larga (comienza por "--" y tiene más
		 * caracteres).
		 *
		 * @param string option Opción.
		 * @return bool True si es larga, false en otro caso.
		 */
		inline bool					isLongOption(const string option) const;

		/**
		 * Devuelve una opción normalizada. Si es corta quita el "-" y si
		 * es larga quita el "--".
		 *
		 * @param string option La opción de la línea de comandos.
		 * @param bool isShort True para indicar que es corta, false para
		 * indicar que es larga.
		 * @return string La opción normalizada.
		 */
		inline const string			normalizeOption(const string option, bool isShort = false) const;

		/**
		 * Procesa una opción corta. Puede obtener más parámetros de la línea
		 * de comandos mediante la función "pop()".
		 *
		 * @param string option La opción.
		 * @param bool normalize True para normalizar (-a -> a), false para
		 * no normalizar.
		 */
		void						decodeShortOption(const string option, bool normalize = true);

		/**
		 * Procesa una opción larga. Puede obtener más parámetros de la línea
		 * de comandos mediante la función "pop()".
		 *
		 * @param string option La opción.
		 * @param bool normalize True para normalizar (--help -> help), false
		 * para no normalizar.
		 */
		void						decodeLongOption(const string option, bool normalize = true);

		/**
		 * Postprocesa una opción, una vez decodificada la cadena de la línea
		 * de comandos, ya está la definición y se comprueba si es exclusiva,
		 * etc. y se añade a las opciones.
		 *
		 * @param OptionDefinition* od Definición de la opción.
		 * @param string realOption Opción real que llegó en la línea de
		 * comandos (por si es un alias).
		 * @param bool isNegated True si la opción es la negada, false en otro
		 * caso.
		 */
		void						postProcessOption(const OptionDefinition* optdef, const string realOption = string(), bool isNegated = false);

		/**
		 * Añade una opción ya procesada. Si la opción ya existe, añade el
		 * argumento.
		 *
		 * @param string OptionDefinition* Definición de la opción.
		 * @param string argument Argumento.
		 * @param bool True si la opción es la negada, false en otro caso.
		 */
		void						addOption(const OptionDefinition* optdef, const string argument = string(), bool isNegated = false);

		/**
		 * Procesa si están todas las opciones que tiene que estar.
		 *
		 * @throw RequiredOptionException Si hace falta alguna opción.
		 */
		void						checkProcessedOptions() const;

		/**
		 * Chequea los conflictos entre opciones.
		 */
		void						checkConflicts() const;

		/**
		 * Procesa los argumentos una vez procesada la línea de comandos,
		 * es decir, mira a ver si el número de ellos es correcto.
		 */
		void						processArguments() const;

		/**
		 * Ejecuta las funciones de las opciones (una por cada opción que
		 * se fija en la definición).
		 */
		void						executeFunctionsForOptions() const;

		/**
		 * Ejecuta la función por cada argumento (una sóla función que se
		 * ejecuta por cada argumento sin contar las opciones, sólo los
		 * argumentos posicionales).
		 */
		void						executeFunctionForArguments() const;

	public:

		/**
		 * Constructor de la clase.
		 *
		 * @param int argc Contador de opciones.
		 * @param char** argv Array de char* con las opciones.
		 */
		OptionManager(int argc, char** argv);

		/**
		 * Destructor de la clase.
		 */
		virtual 				~OptionManager();

		/**
		 * Procesa todas las opciones de la línea de comandos verificando
		 * que su estructura coincida con las definiciones y lanzando
		 * excepciones en caso de error.
		 */
		void					process();

		/**
		 * Devuelve el número de opciones definidas, incluidos los alias.
		 *
		 * @return int Número de opciones definidas.
		 */
		int						countDefinedOptions() const;

		/**
		 * Devuelve el número de opciones procesadas.
		 *
		 * @return int Número de opciones procesadas.
		 */
		int						countProcessedOptions() const;

		/**
		 * Fija y devuelve la cadena con el nombre de la aplicación y los
		 * créditos.
		 *
		 * @param string credits Nombre de la aplicación y créditos.
		 * @return string Nombre de la aplicación y créditos.
		 */
		void					credits(const string credits);
		const string			credits() const;

		/**
		 * Fija y devuelve la cadena con la información de uso de la utilidad.
		 *
		 * @param string usage Información de uso.
		 * @return string Información de uso.
		 */
		void					usage(const string usage);
		const string			usage() const;

		/**
		 * Devuelve una cadena con las descripciones de todas las opciones.
		 *
		 * @param unsigned int padding Separación, en caracteres, del borde izquierdo.
		 * @return string Descripciones de todas las opciones.
		 */
		const string			description(const string title, unsigned int padding = 2) const;
		const string			description(unsigned int padding = 2) const;

		/**
		 * Devuelve el rango (Range<int>) con el mínimo y el máximo de
		 * número de argumentos posicionales. Hay que tener en cuenta que
		 * como mínimo siempre hay un argumento posicional, el cero, que es
		 * el nombre del programa.
		 *
		 * @return Range<int> Par con el mínimo y el máximo.
		 */
		Range<int>&				argumentCountRange();

		/**
		 * Crea una opción. Si el nombre tiene más de 1 caracter, será
		 * una opción larga, si tiene un sólo caracter será una opción corta.
		 *
		 * @param string option Nombre de la opción.
		 * @param int id Id de la opción.
		 * @return OptionDefinition& Referencia a la opción creada.
		 */
		OptionDefinition&		option(const string option, int id = 0);

		/**
		 * Crea un alias para una opción.
		 *
		 * @param string option Nombre de la opción.
		 * @param string alias Alias de la opción.
		 */
		void					alias(const string option, const string alias);

		/**
		 * Indica si existe una opción con dicho nombre.
		 * 
		 * @param string name Nombre de la opción.
		 * @return bool True si existe, false en otro caso.
		 */
		bool					hasOption(const string name) const;
		
		/**
		 * Indica si existe una opción dado su id. Este método no es
		 * fiable ya que puede haber opciones que no tengan id.
		 * 
		 * @param int id Id de la opción.
		 * @return bool True si existe, false en otro caso.
		 */
		bool					hasOption(const int id) const;

		/**
		 * Devuelve una opción dado el nombre.
		 *
		 * @param string name Nombre de la opción.
		 * @return Option* Puntero a la opción o NULL si no existe.
		 */
		const Option*			getOption(const string name) const;

		/**
		 * Cuenta el número de argumentos.
		 *
		 * @return int Número de argumentos.
		 */
		int						countArguments() const;

		/**
		 * Devuelve un argumento dado su índice.
		 *
		 * @param int index Índice del argumento a obtener.
		 * @return string Argumento.
		 * @throw OutOfBoundsException Si el índice es incorrecto.
		 */
		const string			getArgument(int index) const;

		/**
		 * Devuelve el primer argumento.
		 *
		 * @return string Argumento.
		 * @throw OutOfBoundsException Si no hay argumentos.
		 */
		const string			firstArgument() const;

		/**
		 * Devuelve el último argumento.
		 *
		 * @return string Argumento.
		 * @throw OutOfBoundsException Si no hay argumentos.
		 */
		const string			lastArgument() const;

		/**
		 * Devuelve la opción actual o NULL si ya no hay más incrementando
		 * el iterador a la siguiente opción.
		 *
		 * @return Option* Opción actual o NULL si ya no hay más.
		 */
		const Option*			next();

		/**
		 * Resetea el contador de id's de opciones hasta el principio para
		 * poder volver a usar el bucle de procesado.
		 */
		void					rewind();

		/**
		 * Fija la función a ejecutar por cada argumento procesado.
		 *
		 * @param OnArgumentProcessed executeThis Función a ejecutar.
		 */
		void					executeOnArgument(OnArgumentProcessed executeThis);

		/**
		 * Devuelve una representación del OptionManager como cadena para
		 * depuración.
		 *
		 * @return string Cadena representando el OptionManager.
		 */
		const string			dump() const;
};

/**
 * Operador << para imprimir la ayuda de las opciones de forma lo más
 * automática posible. Ejemplo:
 *
 * OptionManager om;
 * -- fijar opciones de om --
 * cout << om << endl;
 *
 * @param ostream& os Dónde hay que imprimir.
 * @param OptionManager& om Qué hay que imprimir.
 * @return ostream& Dónde se imprimió.
 */
ostream&
operator << (ostream& os, const OptionManager& om);

}	// namespace clipp

#endif /* OPTIONMANAGER_HPP_ */
