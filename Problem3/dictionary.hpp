#if !defined(__DICTIONARY_HPP__)
#define __DICTIONARY_HPP__
#include <string>
#include <utility>

namespace bungie
{
	/// Defines a dictionary class interface
	class dictionary
	{
		public:
			virtual ~dictionary() {}

			/// Searches the dictionary for the supplied word
			/// @return true if found
			virtual bool find_word(const std::string& word) const = 0;
			
			/// Whether the dictionary supports looking up words
			/// that are partially complete
			/// @return true if supported
			virtual bool partial_lookup_support() const = 0;

			/// Searches the dictionary for the supplied partial word, and indicates
			/// that the word is also a "whole" word
			/// @return pair object: first = the partial word was found
			///                      second = the partial word is a whole word
			virtual std::pair<bool,bool> find_partial_word(const std::string& word) const = 0;
	};

}

#endif //__DICITONARY_HPP__

