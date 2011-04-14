//
// Alan R. Balasundaram
//
// Problem 3
// 5/28/2010
//
// Write a function to find all the words on a generalized Boggle board.
//
// Given: 
//
// The board, the board dimensions, and a dictionary
//
// Assumptions:
//
// 1) Follows the traditional boggle board rules (ie words can't cross the boundaries of the board)
//
// 2) The the columns and rows of the board will always be equal dimensions(ie 3x3, 4x4, etc)
//
// 3) Only english words, no Unicode funkiness, umlauts etc
//
// 4) Plurality will be considered a different word
//
// 5) Free to use the full might of C++ (including TR1 additions, and Boost)
//
// 6) the dictionary is an input file that is one word per line, deliniated with the proper system endline (endl)
//
// 7) If the board has multiple instances of the same letter, duplicate words are allowed. The rules on the Boggle
//    Wiki page are ambiguous about this.
//
// Caveats:
// 
// There are several ways this program could be improved upon.
// 
// The application doesn't do proper exception handling. It is concievable that we could run out of memory
// And we should gracefully let the user know
//
// There are a number of places int's are used, which could easily replaced by short ints, etc.
// However, C++ doesn't specify a fixed size definition for int, instead I believe it's
// tied to the word size of the native architecture. To improve upon this, we could use the C99 types
// int64_t etc, to specifically get the sizes we want, portable across all hardware (eventhough C99 isn't necessarily
// part of the standard)
//
// Related to the integral types, the code may not be fully portable. string uses string::size_type, which
// doesn't define signed or unsigned, so it's possible that on an SGI Irix, you could have signed/unsigned mismatch
// while comparing sizes of strings with integral values that aren't string::size_type
//
// The code hasn't been validated for support for threaded use.
//
// And as robust as I have tried to make it; it hasn't gone through a rigerous QA process (ie error checking could be better)
//
// Errata
//
// The problem description is missing all words that are 3 letters: box,orb,bed,red, etc..., which is (pointwise) 
// the same as a 4 letter word. it is possible that the supplied dictionary only started with 4 letter words
// but more likely the original program constrained output to 4 letter words and higher. This program 
// includes 3 letter words
//
// Copyright
//
// Boggle is a word game trademarked by Parker Brothers, a division of Hasbro. All respective rights are reserved by the 
// trademark owner. The use of their name, brand, and game concept was solely used for testing/demonstration purposes.
//
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <time.h>


#include <hash_dictionary.hpp>
#include <tree_dictionary.hpp>
#include <boggle.hpp>

using namespace bungie;
using namespace std;


///
/// Holds the command line options used by this program
///
struct options
{
	options() : use_tree(true),
				grid_size(4),
				keep_score(false),
				quiet_mode(false),
				do_help(false),
				error(false)
	
	{}

	bool		   use_tree;
	unsigned short grid_size;
	string    file_name;
	bool		   keep_score;
	bool		   quiet_mode;
	bool		   do_help;
	bool		   error;
};

/// 
const int POINTS[6] = {1,1,2,3,5,11};

/// Forward declarations

/// generates a random SIZE x SIZE letter grid
char** generate_random_word_grid(int size,bool quiet);

/// Prints a help/usage message
void do_help(char** argv);

/// Parse the command line arguments
options parse_cmd_line(int argc, char** argv);

/// Main driver
int main(int argc, char** argv)
{
	cout << endl;
	// we could find a 3rd party command line parser
	// but our needs are fairly simplistic
	// so we just roll our own
	options opts = parse_cmd_line(argc,argv);

	if(opts.do_help)
	{
		if(opts.error)
		{
			cout << "Error while parsing command line." << endl;
		}

		do_help(argv);
		return 0;
	}

	if(opts.file_name == "")
	{
		cout << "Missing dictionary filename." << endl;
		do_help(argv);
		return 0;
	}

	int DATA_SIZE = opts.grid_size;
	
	char** data = generate_random_word_grid(DATA_SIZE,opts.quiet_mode);

	

	ifstream file;
	file.open(opts.file_name,ifstream::out);

	if(!file.is_open())
	{
		cout << "Unable to open dictionary file " << opts.file_name << " for reading." << endl;
		do_help(argv);
		return 0;
	}

	cout << "Initializing dictionary..." ;
	
	dictionary* dict;
	
	if(opts.use_tree)
	{
		dict = new tree_dictionary(file,DATA_SIZE * DATA_SIZE);
	}
	else
	{
		dict = new hash_dictionary(file,DATA_SIZE*DATA_SIZE);
	}
	
	cout << "complete." << endl;
	file.close();

	cout << "Initializing boggle..." ;
	boggle b(data,DATA_SIZE);
	cout << "complete." << endl;

	cout << "Searching for all words..." ;
	unsigned int start = clock();
	list<string> words = b.get_all_words(*dict);
	unsigned int stop = clock();    
	cout << "complete." << endl;

	opts.quiet_mode ? cout : cout << "----Words found:----" << endl;
	
	list<string>::const_iterator it = words.begin();
	long long score = 0;

	// this loop should be replaced with TR1's support for bind and lambdas...
	for(;it != words.end();++it)
	{

		int index = (*it).size() - 3; //boggle words start at 3 letters, so our points lookup is indexed at 0
									//so subtract 3 from the word, and use that as the lookup

		if(index < 0) //this shouldn't happen, because we should have returned a 2 letter word
		{
			throw;
		}
		else if(index > 5) //8+ leter words are worth 11 points
		{
			score+= 11;
		}

		score+= POINTS[index]; //return the lookup value

		opts.quiet_mode ? cout : cout << (*it) << endl;
	}

	opts.quiet_mode ? cout :cout << "--------------------" << endl;
	


	if(opts.keep_score)
	{
		cout << "The total possible score for this grid is: [" << score << "]" << endl;
	}

	cout << "Word Search Runtime: " << stop-start << " msec" << endl;
	
	//clear up memory
	for(int i = 0; i < DATA_SIZE; ++i)
	{
		delete[] data[i];
	}

	delete [] data;
	delete dict;

	return 0;
}

char** generate_random_word_grid(int size,bool quiet)
{
	srand(static_cast<unsigned int>(time(NULL)));

	char** data = new char*[size];

	for(int i = 0; i < size; ++i)
	{
		data[i] = new char[size];
	}

	for(int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			data[i][j] = static_cast<char>(rand() % 26 + 'a');
			quiet ? cout : cout << data[i][j] << " ";
		}
		quiet ? cout : cout << endl;
	}

	cout << endl;

	return data;
}

void do_help(char** argv)
{
	cout << endl;

	cout << "Solution for Problem 3: Write a function to find all the words in a generalized Boggle board."	<< endl;
	cout << "Boggle is a trademark of Parker Brothers."														<< endl;

	cout																		<< endl;
	cout << "Usage: " << argv[0]  << " <OPTIONS>"								<< endl;
	cout																		<< endl;
	cout << "OPTIONS"															<< endl;
	cout << "\t    --help"														<< endl;
	cout << "\t       Print this output message."								<< endl;
	cout																		<< endl;
	cout << "\t-g, --grid-size NUM"											<< endl;
	cout << "\t       Generates a random letter grid of NUM x NUM dimensions"	<< endl;
	cout << "\t       Default: 4"												<< endl;
	cout																		<< endl;
	cout << "\t-d, --dictionary FILE"											<< endl;
	cout << "\t       A text file containing a list of words for verification"	<< endl;
	cout																		<< endl;
	cout << "\t-s, --score"													<< endl;
	cout << "\t       Report the maximum score possible using standard Boggle"	<< endl;
	cout << "\t       scoring rules"											<< endl;
	cout   																	<< endl;
	cout << "\t-q, --quiet"													<< endl;
	cout << "\t       suppress nearly all output"						   	    << endl;
	cout   																	<< endl;
	cout << "\t    --dictionary-type {HASH|TREE}"								<< endl;
	cout << "\t       Choose which data structure to use for the dictionary"   << endl;
	cout << "\t       Default: TREE"											<< endl;
	cout   																	<< endl;
	cout << "\t       CAUTION: Choosing HASH, with  grid-size > 4 will cause"	<< endl;
	cout << "\t       long runtimes. For large grids, TREE is recommended"     << endl;
}

options parse_cmd_line(int argc, char** argv)
{
	options opts;

	if(argc == 1)
	{
		opts.do_help = true;
		return opts;
	}

	for(int i = 1; i < argc; ++i)
	{
		string arg(argv[i]);

		if(arg == "--help")
		{
			opts.do_help = true;
			break;
		}

		if(arg == "-g" || arg=="--grid-size" )
		{
			if(++i >= argc)
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}
			
			istringstream str(argv[i]);

			unsigned short size;

			if( !(str >> size))
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}

			opts.grid_size = size;
			continue;
		}

		if(arg == "-d" || arg== "--dictionary")
		{
			if(++i >= argc)
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}

			opts.file_name = string(argv[i]);
			continue;
		}

		if(arg == "-s" || arg== "--score")
		{
			opts.keep_score = true;
			continue;
		}

		if(arg == "-q" || arg== "--quiet")
		{
			opts.quiet_mode = true;
			continue;
		}


		if(arg == "--dictionary-type")
		{
			if(++i >= argc)
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}

			string type = argv[i];
			if(type == "HASH")
			{
				opts.use_tree = false;
			}
			else if(type == "TREE")
			{
				opts.use_tree = true;
			}
			else
			{
				cout << "Unsupported Dictionary Type: " << type << endl;
				opts.error = true;
				opts.do_help = true;
				break;
			}

			continue;
		}

		cout << "Unknown commandline option: " << arg << endl;
		opts.error = true;
		opts.do_help = true;
		break;
	}

	return opts;
}

	
