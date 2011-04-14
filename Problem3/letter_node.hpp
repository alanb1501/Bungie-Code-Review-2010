#if !defined(__LETTER_NODE_HPP__)
#define __LETTER_NODE_HPP__

#include <list>

namespace bungie
{

	/// Represents a node in our boggle grid graph
	/// containing value of the letter, and a list
	/// of adjacent nodes
	class letter_node
	{
	public:

		/// Main construct
		/// @param letter the letter this node represents
		letter_node(char letter);
		
		/// Copy construct
		letter_node(const letter_node& copy);
		~letter_node();

		/// Returns the letter associated with this node
		/// @return the letter
		char get_letter() const;

		/// Returns a constant iterator to the begining of the adjacent nodes list
		/// used for list traversal
		/// @return begin iterator
		std::list<letter_node*>::const_iterator begin() const;
		
		/// Returns a constant iterator to the end of the adjacent nodes list
		/// used for list traversal
		/// @return end iterator
		std::list<letter_node*>::const_iterator end()   const;

		/// Adds an adjacent node to this ndoe, with the specified letter
		/// @param letter_node_to_add The letter to add
		void add_adjacent_letter(letter_node* letter_node_to_add);

	private:
		char letter;
		std::list<letter_node*> adjacent_letters;
	};

	/// Helper typedefs
	typedef std::list<letter_node*>::iterator		letter_it;
	typedef std::list<letter_node*>::const_iterator const_letter_it;

	/// Inline methods
	inline const_letter_it	letter_node::begin() const { return adjacent_letters.begin(); }
	inline const_letter_it	letter_node::end()   const { return adjacent_letters.end();   }

	inline char letter_node::get_letter() const  { return letter; }
}

#endif //__LETTER_NODE_HPP__
