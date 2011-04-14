#include <tree_dictionary.hpp>

using namespace bungie;

tree_dictionary::tree_dictionary(std::ifstream& file,int max_word_size)
{
	std::string line;

	if(!file.is_open())
	{
		throw; //file isn't open
	}

	// straight forward file reading
	// assuming a single word per line
	while( getline(file,line) ) 
	{
		
		if(line.size() > max_word_size)
		{
			continue; // the word is to big, skip
		}

		char first_letter = line[0]; 

		// find a "tree" for the first letter
		NODE_MAP::iterator it =_dictionary.find(first_letter);
		
		dictionary_letter_node* node;

		// if the tree doesn't exist, create it, and assign the new tree to node
		if(it == _dictionary.end())
		{
			node = new dictionary_letter_node(first_letter);
			_dictionary[first_letter] = node;
		}
		else // otherwise assign the node to found tree
		{
			node = (*it).second;
		}

		// for each subsequent letters in the word, add a child node
		// and set the node to the next node in the tree
		// eventually you should have:
		// a->p->p->l->e
		for(int i = 1; i < line.size(); ++i)
		{
			node = node->add_child_letter(line[i]);
		}

		// set the node for the last letter of this word, as a word node
		// which means that all the letters from the root node to this node
		// together form a word
		node->is_word(true); 
							
	}
}

tree_dictionary::~tree_dictionary()
{
	NODE_MAP::iterator it = _dictionary.begin();

	// delete the head nodes for each letter tree
	while(it != _dictionary.end())
	{
		delete (*it).second;
		++it;
	}

	// cleaar out the dictionary
	_dictionary.clear();
}

bool tree_dictionary::find_word(const std::string& word) const
{
	char first_letter = word[0];
	NODE_MAP::const_iterator it =_dictionary.find(first_letter);

	// if we don't have a letter tree for the first letter of this word
	// then it's not in the dictionary
	if(it == _dictionary.end())
	{
		return false;
	}
	
	// otherwise, iterate through each letter in the word
	// walking the tree 
	// if no node in the tree matches the letter (indicated by NULL)
	// return false
	// finally returning true
	const dictionary_letter_node* node = (*it).second;

	for(int i = 1; i < word.size(); ++i)
	{
		node = node->find_child_letter(word[i]);
		if(node == NULL)
		{
			return false;
		}
	}

	return true;
}

std::pair<bool,bool> tree_dictionary::find_partial_word(const std::string& word) const
{
	char first_letter = word[0];
	NODE_MAP::const_iterator it =_dictionary.find(first_letter);

	// if no letter tree exists for the first letter
	// return false,X
	// since pair.first is false (partial word not found), pair.second is irrelevant
	if(it == _dictionary.end())
	{
		return std::pair<bool,bool>(false,false);
	}
	
	const dictionary_letter_node* node = (*it).second;

	// flag that indicates the input word is a whole word
	bool is_word = false;
	for(int i = 1; i < word.size(); ++i)
	{
		// for every letter in the word, walk the tree
		// if we dont find a node, return false,X
		// again since pair.first is false (partial word not found), pair.second is irrelevant
		node = node->find_child_letter(word[i]);
		if(node == NULL)
		{
			return std::pair<bool,bool>(false,false);
		}

		// set the flag
		is_word = node->is_word();
	}

	// return that yes, we've found the partial word,
	// and the flag which indicates if it's a whole word (ie add it to the list of found words)
	// or not
	return std::pair<bool,bool>(true,is_word);

}