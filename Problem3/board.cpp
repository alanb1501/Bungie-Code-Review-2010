#include <boggle.hpp>

using namespace bungie;
using namespace std;


boggle::boggle(char** data, int dimensions)
{
	//create a board that represents the dim x dim grid
	board_grid board(dimensions);

	//first create all the letters
	for(int i = 0; i < dimensions; ++i)
	{
		board_row* row = new vector<boggle_letter*>(dimensions);

		for(int j = 0; j < dimensions; ++j)
		{
			boggle_letter* letter = new boggle_letter(data[i][j]);
			(*row)[j] = letter;

			board_letters.push_back(letter); //store permanently in a linear list
		}
		board[i] = row;
	}


#pragma region adjecent
	//now build adjacents
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

	//delete the grid...

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
	for(letter_it it = board_letters.begin(); it != board_letters.end() ; ++it)
	{
		delete (*it);
	}
}

//inline here instead of in the header file, to completely decouple the definition from the implementation
inline boggle_letter* boggle::get_left  (const board_grid& board, int row, int col) { return (*board[row])[col-1];  }
inline boggle_letter* boggle::get_right (const board_grid& board, int row, int col) { return (*board[row])[col+1];  } 

inline boggle_letter* boggle::get_top	   (const board_grid& board, int row, int col) { return (*board[row-1])[col];  }
inline boggle_letter* boggle::get_top_right (const board_grid& board, int row, int col) { return (*board[row-1])[col+1];}
inline boggle_letter* boggle::get_top_left  (const board_grid& board, int row, int col) { return (*board[row-1])[col-1];}

inline boggle_letter* boggle::get_bottom		  (const board_grid& board, int row, int col) { return (*board[row+1])[col];  }
inline boggle_letter* boggle::get_bottom_right (const board_grid& board, int row, int col) { return (*board[row+1])[col+1];}
inline boggle_letter* boggle::get_bottom_left  (const board_grid& board, int row, int col) { return (*board[row+1])[col-1];}

// this method traverse all the letters and returns all possible combinations
list<string> boggle::get_all_words(const dictionary& word_lookup)
{
	// first this will do a brute force depth first search of the graph
	// the first optimization will require a "smarter" dictionary that allows you to look up words
	// in partial form (ie no word starts with 'xg')
	// This optimization will allow us to essentially "branch and bound" approach, reducing 
	// the number of checks, as each irrelevant "path/branch" is removed

	const_letter_it it = board_letters.begin();
	const_letter_it end = board_letters.end();

	list<string> return_words;


	while(it!= end)
	{
		hash_set<const boggle_letter* const> visited_letters;
		return_words.splice(return_words.end(),recurse_letter((*it),visited_letters,"",word_lookup));
		++it;
	}

	return return_words;
}

list<string> boggle::recurse_letter(const boggle_letter* const letter,
								   hash_set<const boggle_letter*const >& visited_letters,
								   string current_word,
								   const dictionary& word_lookup)
{
	list<string> return_list;

	if(visited_letters.find(letter) != visited_letters.end())
	{
		//error condition;
		return return_list;
	}

	current_word.push_back(letter->get_letter());

	//create the new word, and add it to the list if it's greater than 3 characters or more
	if(current_word.size() > 2)
	{
		if(word_lookup.find_word(current_word))
		{
			return_list.push_back(current_word);
		} 
		else
		{
			// the supplied dictionary supports partial word lookups
			// which means no words in the dictionary start with these
			// letters, thus we can return, and avoid iterating through 
			// the rest of the combinations
			if(word_lookup.partial_lookup_support())
			{
				return return_list;
			}
		}
	}

	const_letter_it it  = letter->begin();
	const_letter_it end = letter->end();
	
	visited_letters.insert(letter);
	for(;it != end;++it)
	{
		if(visited_letters.find((*it)) != visited_letters.end())
		{
			//we've visited this letter already
			continue;
		}

		//TODO: if the letter + current word is not a valid dictionary combo, skip
		return_list.splice(return_list.end(),(recurse_letter((*it),visited_letters,current_word,word_lookup)));
	}
	visited_letters.erase(visited_letters.find(letter)); //remove the letter we just added...

	return return_list; //bank on RVO.
}