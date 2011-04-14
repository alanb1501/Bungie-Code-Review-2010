//
// Alan R. Balasundaram
//
// Problem 1
// 5/26/2010
//
// Write an algorithm to reverse the words in a char[], that is optimized for both space & time
//
// Given: Only allowed to use ANSI C library for string manipulation
//
// Assumptions:
//
// 1: Although we are restricted to ANSI C string libary calls for this problem, 
//    we are allowed to use C++ idioms (cout vs printf(), new vs malloc, etc) as appropriate
//    as well as C++ strings for *non-problem* related functions (arg processing etc)
//

#include <string.h> 

// only used for ARG processing, and console output
#include <string>
#include <sstream>
#include <iostream> 
#include <fstream>


//in production, this would likely be a configuration item
const char WORD_SEPARATOR = ' ';
const char NULL_TERM = '\0'; //though this is unlikely to ever need to change, we'll keep it here

struct options
{
	options() : use_quick_and_dirty(false),
				run_tests(false),
				input_file(""),
				sentence("Now is the winter of our discontent made glorious summer by this son of York"),
				do_help(false),
				error(false)
	{}

	bool use_quick_and_dirty;
	bool run_tests;
	std::string input_file;
	std::string sentence;
	bool do_help;
	bool error;

};

// for command line processing
options parse_cmd_line(int argc, char** argv);
void do_help(char** argv);

//buffer allocation helper
int create_larger_buffer(char** buffer, int size);


void reverse_words(char* sentence)
{
	if(strcmp("",sentence) == 0 || sentence == NULL)
	{
		//invalid input. return.
		return;
	}

	// c string functions use size_t datatype, but we need signed values
	// for various comparisons
	int len = strlen(sentence);
	
	int f_start = len -1; //index to where the front words will start to be written (-1 to account for index math vs size math)
	int f_word_start = 0; //indexes for the current front word
	int f_word_end = 0;

	int b_start = 0; //index to where the end words will start to be written
	int b_word_start = len-1; //indexes for the current last word
	int b_word_end = len-1;

	int temp_front_size = 5; //the median english word size is 5, according to google.
	int temp_back_size = 5;
	char* temp_front = new char[temp_front_size]; //temp pointers
	char* temp_back = new char[temp_back_size]; 
	temp_front[0] = NULL_TERM; //user the null char to indicate buffer has junk data
	temp_back[0] = NULL_TERM;


	bool f_word_found = false;
	bool b_word_found = false;

	// The algorithm for reversing words is as follows
	// moving forward and backwards through the sentence
	// find a word that is delianated by the WORD_SEPARATOR
	// continue, until you find both a word from the beginning, and from the end
	// once found, check to see if the front word will fit in the space that the backword is currently occupying
	// if so, save off the backword into a temp variable
	// and write the front word in its place, prepending it with the WORD_SEPERATOR
	// otherwise, if the backword fits in the front words space, save off the front word in a temp variable
	// then write the backword in the front words place
	// continue moving towards the middle, replacing words as necessary until the front word index and the backword index cross
	// at which time, write out any remaining temp values in the proper place
	// and delete temp memory
	//

	// Analysis
	// The runtime for this alrogithm is O(n). We traverse n/2 items (since we use a front and end index), 
	// and we make n copies.
	// The space requirement is (assuming the original space for the sentence is accounted for else where
	// is O(1). We need to allocate two temporary buffers, which may grow depending on the size of the largest word in the sentence

	// In order to make this better, it should be possible to make due with fewer index variables. 
	// The index/point math used here, makes this algorithm more error prone, as well as harder to maintain and debug
	// It may even be possible to make do with a single temporary variable, or even possibly without any at all.

	while(true )
	{
		if(!f_word_found)
		{
			//found the front word.
			if(sentence[++f_word_end] == WORD_SEPARATOR)
			{
				f_word_found = true;
			}
		}
		
		if(!b_word_found)
		{
			if(sentence[--b_word_start] == WORD_SEPARATOR)
			{
				//found the next back word
				b_word_found = true;
			}
		}

		if(b_word_found && f_word_found)
		{
			int f_size = f_word_end - f_word_start;
			int b_size = b_word_end - b_word_start;

			if(f_word_end - b_start >= b_size)
			{
				//if the space that the front word occupies is big enough for the back word to be written into
				//we save off the front word, then write the backword in the front words position

				// we can key off our indexes, and the temp_data to know
				// if we need to store off the current back data
				// if we have temp_data already, we don't want to overwrite it
				if(f_word_start != f_word_end && temp_front[0] == NULL_TERM)
				{
					//+1 for null term
					if(f_size+1 > temp_front_size)
					{
						temp_front_size = create_larger_buffer(&temp_front,f_size+1);
					}

					memcpy(temp_front,&sentence[f_word_start],f_size); //now we've saved off temp_front
					temp_front[f_size] = NULL_TERM; //set the null term
				}

				// we've overloaded the null_term to indicate that the temp variable has or does not have
				// meaningful data.
				// if it doesn't, do a memcpy of the backword to its new position
				// otherwise, copy the temp buffer to the new position
				// and set the null_term flag
				if(temp_back[0] == NULL_TERM)
				{
					memcpy(&sentence[b_start],&sentence[b_word_start+1],b_size);
				}
				else
				{
					
					memcpy(&sentence[b_start],temp_back,b_size);
					temp_back[0] = NULL_TERM;
				}
			
				
				//update indices
				b_start += b_size;

				sentence[b_start++] = WORD_SEPARATOR; //add the space

				b_word_end = --b_word_start;
				b_word_found = false;
			}

			else if(f_start - b_word_start >= f_size)
			{
				//if the space that the backword occupies is big enough for the front word to be written into
				//save off the backword (if necessary), then write the front word in the backwords place

				// much like the case with the back word,
				// we key off the index values, and the temp_data
				// to see if we need to save off the front word
				if(b_word_start != b_word_end && temp_back[0] == NULL_TERM)
				{
					if(b_size+1 > temp_back_size)
					{
						temp_back_size = create_larger_buffer(&temp_back,b_size+1);
					}

					memcpy(temp_back,&sentence[b_word_start+1],b_size);
					temp_back[b_size] = NULL_TERM; //set the null term
				}
				

				sentence[f_start-f_size] = WORD_SEPARATOR; //add the space

				// keying off the temp_data
				// we either copy the temp_data to it's new position in the back
				// or directly copy the data from the original buffer ontop of itself
				// in the new starting position.
				if(temp_front[0] == NULL_TERM)
				{
					memcpy(&sentence[f_start-f_size+1],&sentence[f_word_start],f_size);
				}
				else
				{
					memcpy(&sentence[f_start-f_size+1],temp_front,f_size);
					temp_front[0] = NULL_TERM;
				}
				
				// update indexes and continue
				f_start -= f_size+1;
				f_word_start = ++f_word_end;
				f_word_found = false;
			}
		}

		if(f_word_end >= b_word_start)
		{
			// we've come to the end of parsing. write out all that's left.
			// again, keying on the data inside the temp variables
			if( temp_front[0] != NULL_TERM)
			{
				size_t size = strlen(temp_front);
				memcpy(&sentence[f_start-size+1],temp_front,size);
				temp_front[0] = NULL_TERM;
			}
			
			if(temp_back[0] != NULL_TERM)
			{
				size_t size = strlen(temp_back);
				memcpy(&sentence[b_start],temp_back,size);
				temp_back[0] = NULL_TERM;
			}
			break;
		}
	}

	delete [] temp_front;
	delete [] temp_back;
}

void reverse_words_quick_and_dirty(char* sentence)
{
	// Not quick by runtime standards, but quick in terms of writing
	// I came up with this solution quickly, when I got blocked thinking about
	// the more optimal solution.

	if(strcmp("",sentence) == 0 || sentence == NULL)
	{
		//invalid input. return.
		return;
	}

	size_t len = strlen(sentence);

	// allocate a temp buffer
	char* temp = new char[len];

	// we start from the end of the sentence, using two pointers, that move backwards
	// one that points to the start of the word
	// and one that points to the current end of the word
	//
	// When we come to the start of the word, we memcpy it, to our temp buffer
	// taking into account the differences between array math (start 0) and size math (start 1)
	// then set the end pointer to the current start position
	// then continue
	//
	// We have a special case when the pointer hits the start of the sentence; we just take the remaining characters
	// and copy it to the temp buffer

	// Analysis
	// this reverse word uses O(n) space (we traverse the string 3 times):
	// once for the while loop
	// then each word found in the while loop, we do a memcpy
	// then the final memcpy
	// and O(n) memory (we make a temp copy of the string)
	//
	// While this reverse word algorithm is less efficient than the one presented as the solution,
	// This method is more straight forward, less error prone, and easier to maintain, as there are fewer
	// index variables to keep track of, and fewer array indexes, and memcpy's

	// intialize the indexes
	size_t new_buff_ptr = 0;
	size_t start(len),end(len);
	
	while(start != 0)
	{
		// if we found the separator character
		// copy the word (where word = sentence[start:end])
		if(sentence[start] == WORD_SEPARATOR)
		{
			//copy the word into the buffer, update the indexes, and set the separator
			memcpy(&temp[new_buff_ptr],&sentence[start+1],end-start+1);
			new_buff_ptr += end-start-1;
			temp[new_buff_ptr++] = WORD_SEPARATOR;
			end = start;
		}

		// decrement the index. If we hit the begining of the sentence
		// copy the remaining chars to the buffer
		if(--start == 0)
		{
			memcpy(&temp[new_buff_ptr],&sentence[start],end-start);
		}
	}

	// copy the temp buffer back onto the original array, delete the temp, and return
	memcpy(sentence,temp,len);
	delete [] temp;
}


// Unit test.
bool test_reverse_word(std::ifstream &file)
{
	if(!file.is_open())
	{
		throw; //file not open
	}

	std::string line;

	while(getline(file,line))
	{
		//make a deep copy...
		std::string expected = line + "";

		char* exp = &expected[0];
		char* actual = &line[0];

		reverse_words_quick_and_dirty(exp);
		reverse_words(actual);
		
		if(strcmp(actual,exp) != 0)
		{

			std::cout << "expected: " << exp << std::endl;
			std::cout << "actual  : " << actual << std::endl;
			std::cout << "Test Failed!" << std::endl;
			return false;
		}
	}

	std::cout << "Test Passed!" << std::endl;

	return true;
}

// Main driver for program1
int main(int argc, char** argv)
{
	options opts = parse_cmd_line(argc, argv);

	if(opts.do_help)
	{
		if(opts.error)
		{
			std::cout << "Error parsing command line." << std::endl;
		}

		do_help(argv);
		return -1;
	}

	if(opts.run_tests)
	{
		std::ifstream file;
		file.open(opts.input_file,std::ifstream::out);
		
		test_reverse_word(file);
		file.close();
		return 0;
	}

	char* sentence = &opts.sentence[0];
	std::cout << "Input sentence:  " << opts.sentence << std::endl;

	if(opts.use_quick_and_dirty)
	{
		reverse_words_quick_and_dirty(sentence);
	}
	else
	{
		reverse_words(sentence);
	}

	std::cout << "Output sentence: " << sentence << std::endl;

	return 0;
}

int create_larger_buffer(char** buffer, int size)
{
	char* tmp = *buffer; //save the current buffer

	*buffer = new char[size + size/2];

	delete tmp;

	return size+size/2;
}

void do_help(char** argv)
{
	std::cout << std::endl;

	std::cout << "Solution for Problem 1: Write a function to reverse the words in a sentence,"	<< std::endl;
	std::cout << " that is time and space constrained "											<< std::endl;

	std::cout																		<< std::endl;
	std::cout << "Usage: " << argv[0]  << " <OPTIONS> SENTENCE"						<< std::endl;
	std::cout																		<< std::endl;
	std::cout << "OPTIONS"															<< std::endl;
	std::cout << "\t    --help"														<< std::endl;
	std::cout << "\t       Print this output message."								<< std::endl;
	std::cout																		<< std::endl;
	std::cout << "\t-q, --quick-and-dirty"											<< std::endl;
	std::cout << "\t       Use the quick and dirty reverse algorithm (not optimal)."<< std::endl;
	std::cout << "\t       If -r is specified, this option is ignored."             << std::endl;
	std::cout																		<< std::endl;
	std::cout << "\t-r, --run-tests FILE"											<< std::endl;
	std::cout << "\t       Runs the reverse_words on a series of input sentences."	<< std::endl;
	std::cout << "\t       contained in the FILE."									<< std::endl;
	std::cout																		<< std::endl;
	std::cout << "SENTENCE"															<< std::endl;
	std::cout << "\t       The sentence to reverse. "								<< std::endl;
	std::cout << "\t       If -r is specified, this is not used."					<< std::endl;
}


options parse_cmd_line(int argc, char** argv)
{
	options opts;

	if(argc == 1)
	{
		opts.do_help = true;
		return opts;
	}

	std::stringstream str;

	for(int i = 1; i < argc; ++i)
	{
		const std::string& arg(argv[i]);

		if(arg == "--help")
		{
			opts.do_help = true;
			break;
		}

		if(arg == "-q" || arg == "--quick-and-dirty")
		{
			opts.use_quick_and_dirty = true;
			continue;
		}

		if(arg == "-r" || arg == "--run-tests")
		{
			if(++i >= argc)
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}

			opts.input_file = std::string(argv[i]);
			opts.run_tests = true;
			continue;
		}

		str << argv[i];

		if(i+1 < argc)
		{
			str << " ";
		}
	}

	const std::string& temp = str.str();

	if(temp.size() != 0)
	{
		opts.sentence = temp;
	}

	return opts;
}