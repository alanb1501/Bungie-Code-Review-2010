#if !defined(__HASH_DICTIONARY__HPP__)
#define __HASH_DICTIONARY__HPP__
#include <fstream>
#include <hash_set> 
#include <string>
#include <dictionary.hpp>

namespace bungie
{
	// this dictionary uses a hash_set in the backend
	// to store words requiring words to be "complete" before lookup
	// so it doesn't support partial word lookups
	
	/// Represents a dictionary, that uses a hash structure to
	/// store all words
	class hash_dictionary : public dictionary
	{
		public:
			/// Main constructor
			/// @param file pre-opened filestream that contains words on a single line
			/// @param max_word_size the maximum sized words allowd in the dictionary
			/// which is an optimization, since in a 3x3 grid, the max word possible is 9
			/// so avoid loading in anywords with size > 9
			hash_dictionary(std::ifstream& file,int max_word_size = INT_MAX);

			/// searchs the dictionary for the word
			/// @return true if found, false otherwise
			bool find_word(const std::string& word) const;

			/// The hash_dictionary does not support looking up partially formed words
			bool partial_lookup_support() const { return false;}

			/// The hash_dictionary does not support looking up partially formed words
			std::pair<bool,bool> find_partial_word(const std::string& word) const;

		private:
			hash_dictionary() {}
			hash_dictionary(const hash_dictionary&) {}

			std::hash_set<const std::string> _dictionary;
	};

}

#endif //__HASH_DICTIONARY__HPP__
