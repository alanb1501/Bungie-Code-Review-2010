#include <dictionary_letter_node.hpp>
using namespace bungie;

dictionary_letter_node::dictionary_letter_node(char letter) : letter(letter),
															  word(false)
{

}

dictionary_letter_node::~dictionary_letter_node()
{
	NODE_MAP::iterator it = child_letters.begin();

	//iterate through all the child nodes and delete them
	while(it != child_letters.end())
	{
		delete (*it).second;
		++it;
	}

	child_letters.clear();
}

dictionary_letter_node* dictionary_letter_node::add_child_letter(char letter_to_add)
{
	// if there already is a child node for a given letter
	// return it
	// otherwise, create it, then return the newly created node
	if(child_letters.find(letter_to_add) == child_letters.end())
	{
		child_letters[letter_to_add] = new dictionary_letter_node(letter_to_add);
	}

	return child_letters[letter_to_add];
}


const dictionary_letter_node* dictionary_letter_node::find_child_letter(char letter_to_find) const
{
	// search for a child node for the letter, and return it
	// null otherwise
	NODE_MAP::const_iterator it = child_letters.find(letter_to_find);
	if(it == child_letters.end())
	{
		return NULL;
	}

	return (*it).second;
}

