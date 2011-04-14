#if !defined(__TREE_DICTIONARY_HPP__)
#define __TREE_DICTIONARY_HPP__

#include <dictionary_letter_node.hpp>
#include <dictionary.hpp>
#include <fstream>
#include <hash_map>
#include <string>

namespace bungie
{

	// This class uses a tree to store words
	// It stores at most 26 trees
	// with the root node of each, representing a letter
	// in the english alphabet
	// the child nodes for each tree, represent subsequent letters in 
	// a word
	// for example: apple
	// Our "A" tree (tree of all words that start with A) has the structure {a}->{p}->{p}->{l}->{e}
	// if we add the word 'ape'
	// our new "A" tree looks like {a}->{p}->{e}{p}->{l}->{e}
	// where {e} and {p} are sibling nodes, which are child nodes of the first {p}
	// each tree is stored in a hash
	// and each node stores it's children in a hash
	// so we have an amortized lookup time of O(1)
	// Of course, the hash_dictionary has O(1) as well,
	// the advantage of the tree structure however is support for partial word lookups
	// because this dictionary supports partial word lookups
	// we need a way to differentiate between a partial word
	// and a complete word
	// we do this by setting a child node:is_word value to true
	// which we will key off of when searching for words...


	/// A dictionary that uses a tree structure
	/// to store words
	class tree_dictionary : public dictionary
	{
	
	public:
		/// Main constructor
		/// @param file an pre-opened file stream containing words, one line at a time
		/// @max_word_size the max size word allowed in the dictionary
		tree_dictionary(std::ifstream& file,int max_word_size = INT_MAX);
		~tree_dictionary();
		

		/// Searches the dictionary for a word
		/// @param word the word to find
		/// @return true if found, false otherwise
		bool find_word(const std::string& word) const;

		/// The tree_dictionary supports partial word lookups
		bool partial_lookup_support() const { return true; }

		/// searches the dictionary for the partial word
		/// @param word to find
		/// @return a pair object: pair.first = the partial word was found
		/// pair.second = the partial word is also a word
		std::pair<bool,bool> find_partial_word(const std::string& word) const;

	private:
		tree_dictionary() {}
		tree_dictionary(const tree_dictionary&) {}

		NODE_MAP _dictionary;

	};
}

#endif //__TREE_DICTIONARY_HPP__
