#include <letter_node.hpp>
using namespace bungie;

letter_node::letter_node(char c) : letter(c)
{
}

letter_node::~letter_node() { }

letter_node::letter_node(const letter_node& copy) : letter(copy.letter), 
							   adjacent_letters(copy.adjacent_letters)
{
	//may need to modify this...
	// only does a shallow copy (which is okay for STL containers)
}

void letter_node::add_adjacent_letter(letter_node* node_to_add) 
{
	// add the adjacent node to our node list
	adjacent_letters.push_back(node_to_add);	
}