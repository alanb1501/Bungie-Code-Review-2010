#if !defined(__DICTIONARY_LETTER_NODE_HPP__)
#define __DICTIONARY_LETTER_NODE_HPP__
#include <hash_map>

namespace bungie
{
	// It turns out, our letter_node class used for boggle
	// is essentially what we want for our faster dictionary lookup
	// but instead of storing the nodes in a list, we'll store them in a hash, for constant time lookups
	// Since this is a tree, and not a graph, we use the child nomenclature, instead of adjacent
	// Also, since our nodes will be words in the english language, we'll at most have 26 adjacent nodes

	/// Defines a "node" in our dictionary tree
	class dictionary_letter_node
	{
	
	public:

		/// Main constructor
		/// @param letter the character that this node represents
		dictionary_letter_node(char letter);

		/// The copy constructor (iirc needed for STL containers)
		dictionary_letter_node(const dictionary_letter_node& copy);
		
		/// Destructor
		~dictionary_letter_node();

		/// the letter this node represents
		/// @return the letter
		char get_letter()		const;

		/// Whether this node is a word
		/// @return true if the root to this node is a word
		bool is_word()			const; 

		/// Sets the word property for this node
		/// @param word the value
		void is_word(bool word);

		/// Find the child letter node that matches this letter
		/// @return the next child node, NULL if not found
		const dictionary_letter_node* find_child_letter(char letter_to_find) const;


		/// Add's a child node that matches this letter
		/// creating a new one, and returning it
		/// if one exists, find it, then return it
		/// @param letter_to_add the letter
		/// @return the corresponding node that matches the input letter
		dictionary_letter_node* add_child_letter(char letter_to_add);


	private:
		char letter;
		bool word;
		std::hash_map<char,dictionary_letter_node*> child_letters; //using a hash_map gives us near constant time lookup
	};

	// inline below
	inline char dictionary_letter_node::get_letter()		const	{ return letter; }
	inline bool dictionary_letter_node::is_word()			const	{ return word;   } 
	inline void dictionary_letter_node::is_word(bool is_word)		{ word = is_word;}

	typedef std::hash_map<char,dictionary_letter_node*> NODE_MAP;
}

#endif //__DICTIONARY_LETTER_NODE_HPP__
