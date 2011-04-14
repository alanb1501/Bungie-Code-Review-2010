//
// Alan R. Balasundaram
//
// Problem 2
// 5/27/2010
//
// Write an algorithm to duplicate a singly linked list, optimized for space and time 
//
// Given: 
// struct s_node
// {
// 	 struct s_node *next;
//	 struct s_node *reference;
// };
//
//
// Assumptions:
//
// 1) *next and *reference will be initialized to NULL if they don't point to anything valid
// 
// 2) the list is not circular (ie, the last node will have a NULL *next ptr)
//
//
#include <vector>
#include <iostream>
#include <hash_map>
#include <time.h>
#include <sstream>
#include <string>

struct s_node
{
	struct s_node *next;
	struct s_node *reference;
};

struct options
{
	options() : use_quick_and_dirty(false),
				node_count(5),
				do_help(false),
				error(false)
	{}

	bool use_quick_and_dirty;
	int  node_count;
	bool do_help;
	bool error;

};

// forward declrations
void do_help(char** argv);
options parse_cmd_line(int argc, char** argv);

s_node* duplicate_list_quick_and_dirty(struct s_node* list)
{
	if(list == NULL)
	{
		throw; //error
	}
	
	// Not quick by runtime standards, but quick in terms of writing
	// I came up with this solution quickly, when I got blocked thinking about
	// the more optimal solution.

	// for this algorithm, we use a lookup to store the nodes
	// For each node, we create a new node, then store it in the lookup
	// mapping the old node to the new node
	// 2nd time through the list, we use the lookup to set the references

	// Analysis
	// This algorithms run time is O(n), we traverse the list twice
	// the storage for this algorithm is O(n), because we make a copy of
	// each original s_node pointer, and a copy the new s_node pointer

	// This algorithm is fairly straightforward. It's biggest drawback is using the hash_map
	// to quickly find nodes
	std::hash_map<s_node*,s_node*> nodes;

	s_node* return_list = new s_node(); 

	s_node* temp = list;
	s_node* temp_new = return_list;

	while(temp->next != NULL)
	{
		nodes[temp] = temp_new;

		temp_new->next = new s_node();
		temp = temp->next;
		temp_new = temp_new->next;
	}

	temp = list;
	temp_new = return_list;

	while(temp->next != NULL)
	{
		if(temp->reference != NULL)
		{
			temp_new->reference = nodes[temp->reference];
		}

		temp = temp->next;
		temp_new = temp_new->next;
	}

	return return_list;
}

s_node* duplicate_list(s_node* list)
{

	if(list == NULL)
	{
		throw; //error
	}
	
	s_node* to_return = new s_node();
	s_node* org_list = list;
	s_node* new_list = to_return;

	// The trick was given in the problem description:
	// "You may modify the input provided that it is restored before your function completes."
	// foreach node in the list
	// we create the new node,
	// set the new nodes next ptr to the original node's next ptr
	// then set the original nodes next ptr to the new node
	//
	// Analysis
	//
	// The run time for this problem is O(n),requiring us to traverse the list three times
	// With storage space O(1). While the storage requirement is low, it requires
	// moving pointers, which can be error prone.
	// Also, I believe it may be possible to do this in two or even one pass.
	// However, it's hard to ignore the nearly 4x runtime improvement with not using the hash_map (based on ctime)

	while(org_list != NULL)
	{
		// set new node's next pointer to old node's next
		new_list->next	     = org_list->next;
		// set old nodes next to the current node
		org_list->next			 = new_list;
		// set cur pointer to the next one in the list (pointed to by new_list->next)
		org_list = new_list->next; 
		

		if(org_list == NULL)
		{
			break;
		}

		// create a new node
		new_list = new s_node();
	}

	// now we set references, then reset the original pointers	
	org_list = list;
	new_list = to_return;

	while(org_list != NULL)
	{
		// if we have a reference
		// follow it, and it's next pointer should point
		// to the new node
		// so set the current new node's reference to it
		if(org_list->reference != NULL)
		{
			new_list->reference = org_list->reference->next;
		}

		//go to the next original node
		org_list = org_list->next->next;
		if(org_list != NULL)
		{
			new_list = org_list->next;
		}
	
	}

	org_list = list;
	new_list = to_return;

	while(org_list != NULL)
	{
		s_node* temp = new_list->next;

		// if the next old node isn't null
		if(temp != NULL)
		{
			// set net_list_temp's next to old_node next (which poitns to the new nodes next value)
			new_list->next = temp->next;
		}

		// set old node's next to the next old node
		org_list->next = temp;

		// move to the next item
		org_list = org_list->next;

		// move the new list point to the next new list item
		new_list = new_list->next;
	}

	return to_return;
}


bool test_duplicate_list(int node_count, bool use_q_n_d)
{
	srand(static_cast<unsigned int>(time(NULL)));

	int NUM_NODES = node_count;
	//first create the list...
	s_node* head = new s_node();
	
	head->next = NULL;
	head->reference = NULL;

	s_node* tmp = head;

	std::vector<s_node*> nodes;
	nodes.push_back(head);
	
	//create a list of NUM_NODES...
	for(int i = 0; i < NUM_NODES; i++)
	{
		
		tmp->next = new s_node();
		tmp->reference = NULL;
		tmp = tmp->next;
		nodes.push_back(tmp);
	}

	//do random idexes..
	int count = rand() %NUM_NODES + 1;
	for(int i = 0; i < count; ++i)
	{
		int index = rand() % NUM_NODES;
		int ref_index  = rand() % NUM_NODES;
		
		nodes[index]->reference = nodes[ref_index];
	}

	s_node* dupe_list;

	unsigned int start = clock();

	
	if(use_q_n_d)
	{
		dupe_list = duplicate_list_quick_and_dirty(head);		
	}
	else
	{
		dupe_list = duplicate_list(head);
	}			

    std::cout << "Runtime: " << clock()-start << " msec" << std::endl;


	bool to_return = true;
	if(dupe_list == NULL)
	{
		to_return = false;
	}

	s_node* head_ptr = head;
	s_node* dupe_ptr = dupe_list;
	//verify that the two lists are identical

	std::hash_map<s_node*,int> map1;
	std::hash_map<s_node*,int> map2;

	int id = 0;

	// first create id's for each node
	while(head_ptr != NULL)
	{
		map1[head_ptr] = ++id;
		map2[dupe_ptr] = id;
		dupe_ptr = dupe_ptr->next;
		head_ptr = head_ptr->next;
	}

	head_ptr = head;
	dupe_ptr = dupe_list;
	
	// now ensure the reference nodes have the same id
	// which they will if the ndoes were created in the same order
	// and the references point to the proper nodes
	while(head_ptr != NULL)
	{
		if(head_ptr->reference != NULL)
		{
			int head_id = (*map1.find(head_ptr->reference)).second;
			int dupe_id = (*map2.find(dupe_ptr->reference)).second;
			if(head_id != dupe_id)
			{
				to_return = false;
				break;
			}
		}

		head_ptr = head_ptr->next;
		dupe_ptr = dupe_ptr->next;
	}

	for (std::vector<s_node*>::iterator it = nodes.begin(); it!=nodes.end(); ++it) 
	{
		delete *it;
		*it = NULL;
	}
	
	s_node* temp = dupe_list;
	while(temp != NULL)
	{
		s_node* temp2 = temp;
		temp = temp2->next;
		delete temp2;
		temp2 = NULL;
	}

	return to_return;
}

int main(int argc, char** argv)
{
	options opts = parse_cmd_line(argc,argv);

	if(opts.do_help)
	{
		if(opts.error)
		{
			std::cout << "Error parsing the command line." << std::endl;
		}

		do_help(argv);
		return -1;
	}

	if(test_duplicate_list(opts.node_count,opts.use_quick_and_dirty))
	{
		std::cout << "Contratulations. duplicate_list is a success." << std::endl;
	}
	else
	{
		std::cout << "Unfortunately, duplicate_list failed." << std::endl;
	}

	return 0;
}

void do_help(char** argv)
{
	std::cout << std::endl;

	std::cout << "Solution for Problem 2: Write a function to duplicate a linked list" << std::endl;
	std::cout << "that is time and space constrained "								   << std::endl;

	std::cout																			<< std::endl;
	std::cout << "Usage: " << argv[0]  << " <OPTIONS>"									<< std::endl;
	std::cout																			<< std::endl;
	std::cout << "OPTIONS"																<< std::endl;
	std::cout << "\t    --help"															<< std::endl;
	std::cout << "\t       Print this output message."									<< std::endl;
	std::cout																			<< std::endl;
	std::cout << "\t-q, --quick-and-dirty"												<< std::endl;
	std::cout << "\t       Use the quick and dirty duplicate algorithm (not optimal)."	<< std::endl;
	std::cout << "\t       If -r is specified, this option is ignored."					<< std::endl;
	std::cout																			<< std::endl;
	std::cout << "\t-n, --node-count NUM"												<< std::endl;
	std::cout << "\t       Generate NUM nodes for use"									<< std::endl;
	std::cout																			<< std::endl;
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

		if(arg == "-n" || arg == "--node-count")
		{
			if(++i >= argc)
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}

			std::istringstream str(argv[i]);

			int nodes;

			if( !(str >> nodes))
			{
				opts.error = true;
				opts.do_help = true;
				break;
			}

			opts.node_count = nodes;
			continue;
		}

		std::cout << "Unknown command line argument: " << argv[i] << std::endl;
		opts.error = true;
		opts.do_help = true;
		break;

	}
	
	return opts;
}