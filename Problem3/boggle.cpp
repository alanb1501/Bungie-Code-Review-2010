#include <boggle.hpp>

using namespace bungie;
using namespace std; //TODO: remove this, and stick with std:: (my C++ muscle memory is coming back :) )


// intialize the static member in the cpp

boggle::boggle(char** data, int dimensions)
{
	// create a vector of vectors that represents the dim x dim grid
	board_grid board(dimensions);

	// we iterate through the boards dimensions
	// creating a row at a time
	// and each row, create a node, set it's letter
	// and place it in its proper column
	// finally inserting that row into the board
	for(int i = 0; i < dimensions; ++i)
	{
		board_row* row = new vector<letter_node*>(dimensions);

		for(int j = 0; j < dimensions; ++j)
		{
			letter_node* letter = new letter_node(data[i][j]); 
			(*row)[j] = letter; //store the node in its proper place in the row


			board_letters.push_back(letter); //store permanently in a linear list
		}
		board[i] = row;
	}


#pragma region adjecent
	// now build adjacents, using index math to find all adjacent letters
	// and set them
	// this is tedious, and likely there is a better way to set the adjacents...
	for(int i = 0; i < dimensions; ++i)
	{
		for(int j=0; j < dimensions; ++j)
		{
			//first row
			if(i == 0)
			{
				//top left corner
				if(j == 0)
				{
					(*board[i])[j]->add_adjacent_letter(get_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_right(board,i,j));
				}
				else if(j+1 != dimensions) //top row, non corner letter
				{
					(*board[i])[j]->add_adjacent_letter(get_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom(board,i,j));
				}
				else //top row, right corner
				{
					(*board[i])[j]->add_adjacent_letter(get_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom(board,i,j));
				}
			}
			else if(i+1 == dimensions) //last row
			{
				if(j == 0) //bottom left corner
				{
					(*board[i])[j]->add_adjacent_letter(get_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top(board,i,j));
				}
				else if(j+1 != dimensions) //last row, non corner
				{
					(*board[i])[j]->add_adjacent_letter(get_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top(board,i,j));
				}
				else // last row, right corner
				{
					(*board[i])[j]->add_adjacent_letter(get_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_left(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top(board,i,j));
				}
			}
			else //any middle row
			{
				if(j == 0) //first column
				{
					(*board[i])[j]->add_adjacent_letter(get_top(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_right(board,i,j));
				}
				else if(j+1 != dimensions) //middle row, middle column
				{
					(*board[i])[j]->add_adjacent_letter(get_top(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_left(board,i,j));

					(*board[i])[j]->add_adjacent_letter(get_bottom(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_left(board,i,j));
					
					(*board[i])[j]->add_adjacent_letter(get_right(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_left(board,i,j));
				}
				else //middle row, last column
				{
					(*board[i])[j]->add_adjacent_letter(get_top(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_top_left(board,i,j));

					(*board[i])[j]->add_adjacent_letter(get_bottom(board,i,j));
					(*board[i])[j]->add_adjacent_letter(get_bottom_left(board,i,j));

					(*board[i])[j]->add_adjacent_letter(get_left(board,i,j));
				}

			}
		}
	}

#pragma endregion adjacent builder

	//finally delete the grid, as we don't need it anymore.
	grid_it it = board.begin();
	grid_it end = board.end();

	while(it != end)
	{
		delete (*it);
		++it;
	}
}


boggle::~boggle()
{
	// remove all the nodes in the list
	for(letter_it it = board_letters.begin(); it != board_letters.end() ; ++it)
	{
		delete (*it);
	}
}

// this method traverse all the letters and returns all possible combinations
list<string> boggle::get_all_words(const dictionary& word_lookup)
{
	// For this search, we will do a depth first search of the graph
	// keeping track of letters we have visited along the way (boggle rules specify the same letter-cube cant
	// be used twice)
	// as well the current word, which is the concatination of all the letters up to a point
	// and the dictionary
	// once done, we return the list of words
	const_letter_it it = board_letters.begin();
	const_letter_it end = board_letters.end();

	list<string> return_words;


	while(it!= end)
	{
		hash_set<const letter_node* const> visited_letters;
		return_words.splice(return_words.end(),recurse_letter((*it),visited_letters,"",word_lookup));
		++it;
	}

	return return_words;
}

list<string> boggle::recurse_letter(const letter_node* const letter,
								   hash_set<const letter_node*const >& visited_letters,
								   string current_word,
								   const dictionary& word_lookup)
{
	// This is a recursive method that starts at a node, and travels down all adjacent nodes
	// in an depth first search, the order of which, solely depends on the order
	// of the adjacent_nodes list for a given node
	// we're not order dependant, so long as the node ordering doesnt
	// change in midst of iteration (which is unlikely to occur)

	
	list<string> return_list;

	// if the list of visited letters includes this letter already, return
	if(visited_letters.find(letter) != visited_letters.end())
	{
		//error condition, but no need to throw
		return return_list;
	}

	// append the current letter to the accumulation of letters (the word)
	// one minor optimization would be to use a stream, instead of string concatination
	current_word.push_back(letter->get_letter());

	// create the new word, and add it to the list if it's >= than 3
	if(current_word.size() >= 3) // could make this a parameter
	{
		// the supplied dictionary supports partial word lookups
		// which means that if no words in the dictionary start with this letter combination
		// we can return and avoid iterating through the rest of the combinations
		// which save *A LOT* of processing time for larger grids
		if(word_lookup.partial_lookup_support())
		{
			std::pair<bool,bool> result = word_lookup.find_partial_word(current_word);
			if(!result.first) //the partial word wasn't found, so don't continue down this branch
			{
				return return_list;
			}

			if(result.second) //the partial word, was found, and it's an real word, store it, and continue
			{
				return_list.push_back(current_word);
			}
		}
		else
		{
			// no partial word support, so if it's found
			// store it and continue
			if(word_lookup.find_word(current_word))
			{
				return_list.push_back(current_word);
			} 
		}
	}

	const_letter_it it  = letter->begin();
	const_letter_it end = letter->end();
	
	// add the letter_node to the visited letters
	visited_letters.insert(letter);

	// for each adjacent node for this letter
	// if the letter hasn't been visited yet,
	// recursively call recurse_letter, passing in the visited letters
	// splicing the return lists as they go
	for(;it != end;++it)
	{

		if(visited_letters.find((*it)) != visited_letters.end())
		{
			//we've visited this letter already
			continue;
		}

		return_list.splice(return_list.end(),(recurse_letter((*it),visited_letters,current_word,word_lookup)));
	}
	// as we unwind from the recursive call, we need to *remove* the letter
	// otherwise the algorithm won't iterate over all possible letter combos.
	// it's also possible to instead just find a single combination of letters, and use an
	// anagram algorithm to find all possible combinations, however I think it wouldn't
	// necessarily offer any performance gains (besides partial word support speeds up this immensely)
	visited_letters.erase(visited_letters.find(letter)); 

	return return_list; //hopefully RVO prevents any unecessary copies...
}