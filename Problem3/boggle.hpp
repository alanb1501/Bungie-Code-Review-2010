#if !defined(__BOARD_HPP__)
#define __BOARD_HPP__
//
// Board.h 
// 
// Class that holds a boggle board
#include <letter_node.hpp>
#include <dictionary.hpp>
#include <string>
#include <vector>
#include <hash_set>


namespace bungie
{
	/// helper typedefs
	typedef std::vector<letter_node*>			board_row;
	typedef std::vector<board_row*>				board_grid;

	typedef board_grid::iterator	   grid_it;
	typedef board_grid::const_iterator const_grid_it;

	/// Main Boggle class. Represents the game of Boggle
	class boggle
	{
	public:

		/// Main constructor
		/// @param data the board containing the letters for the game
		/// @param dimension the board
		boggle(char** data,int dimension);
		~boggle();

		/// Returns a list of all the words found in the boggle board
		/// using the supplied dictionary
		/// @param word_lookup a dictionary pre-loaded with words
		std::list<std::string> get_all_words(const dictionary& word_lookup);

	private:
		// disallow compiler defaults
		boggle(const boggle&) { };
		boggle() { };

		std::list<letter_node*> board_letters;

		static letter_node* get_left			(const board_grid& board, int row, int col);
		static letter_node* get_right			(const board_grid& board, int row, int col);
		static letter_node* get_top				(const board_grid& board, int row, int col);
		static letter_node* get_bottom			(const board_grid& board, int row, int col);
		static letter_node* get_top_right		(const board_grid& board, int row, int col);
		static letter_node* get_top_left		(const board_grid& board, int row, int col);
		static letter_node* get_bottom_right	(const board_grid& board, int row, int col);
		static letter_node* get_bottom_left		(const board_grid& board, int row, int col);

		static std::list<std::string> recurse_letter(const letter_node* const letter,
													std::hash_set<const letter_node*const >& visited_letters,
													std::string current_word,
													const dictionary& word_lookup);
	};

	//inline here instead of in the class definition
	inline letter_node* boggle::get_left  (const board_grid& board, int row, int col) { return (*board[row])[col-1];  }
	inline letter_node* boggle::get_right (const board_grid& board, int row, int col) { return (*board[row])[col+1];  } 

	inline letter_node* boggle::get_top	   (const board_grid& board, int row, int col) { return (*board[row-1])[col];  }
	inline letter_node* boggle::get_top_right (const board_grid& board, int row, int col) { return (*board[row-1])[col+1];}
	inline letter_node* boggle::get_top_left  (const board_grid& board, int row, int col) { return (*board[row-1])[col-1];}

	inline letter_node* boggle::get_bottom		  (const board_grid& board, int row, int col) { return (*board[row+1])[col];  }
	inline letter_node* boggle::get_bottom_right (const board_grid& board, int row, int col) { return (*board[row+1])[col+1];}
	inline letter_node* boggle::get_bottom_left  (const board_grid& board, int row, int col) { return (*board[row+1])[col-1];}
}



#endif //__BOARD_HPP__
