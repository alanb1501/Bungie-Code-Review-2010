#include <hash_dictionary.hpp>

using namespace bungie;

hash_dictionary::hash_dictionary(std::ifstream& file, int max_word_size)
{
	std::string line;

	if(!file.is_open())
	{
		throw; //file isn't open
	}

	// straightforward file reading.
	// read a line at a time, assumption is that
	// one word per line
	// and if the word is bigger than the max size, skip it
	// otherwise add it to our hash
	// the max size is an optimization. which we can figure out by the fact that
	// in a given n x n board, the max sized word is n^2
	while( getline(file,line) ) 
	{
		if(line.size() > max_word_size)
		{
			continue;
		}
		
		_dictionary.insert(line);
	}
}

bool hash_dictionary::find_word(const std::string& word) const
{
	// a O(1) search on a hash_map
	return (_dictionary.find(word) != _dictionary.end());
}

std::pair<bool,bool> hash_dictionary::find_partial_word(const std::string &word) const
{
	// the hash_dictionary does not support partial words...
	throw "method not supported";
}