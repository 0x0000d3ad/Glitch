/* parser for command line options */
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "cvt.h"

#ifdef WIN32
#include <windows.h>
#endif // WIN32

typedef int probabilities;

using namespace std;

enum corruption_types
{
	//Non-AI corruption types
	ct_shotgun = 1,
	ct_shotgun_shift,
	ct_replace,
	ct_swap,
	ct_translate,
	ct_shift,

	//AI corruption types
	ct_corrupt_common_length,
	ct_corrupt_uncommon_length,
	ct_corrupt_common_length_most_prob,
	ct_corrupt_uncommon_length_most_prob,
	ct_corrupt_common_length_least_prob,
	ct_corrupt_uncommon_length_least_prob,
	ct_overwrite_with_mkv_mp,
	ct_overwrite_with_mkv_lp,
	ct_corrupt_common_length_mult,
	ct_corrupt_uncommon_length_mult,
	ct_corrupt_common_length_most_prob_mult,
	ct_corrupt_uncommon_length_most_prob_mult,
	ct_corrupt_common_length_least_prob_mult,
	ct_corrupt_uncommon_length_least_prob_mult,
	ct_overwrite_with_mkv_mp_mult,
	ct_overwrite_with_mkv_lp_mult
};

struct fc_options
{

	vector<corruption_types> ct;		//stack of corruption types
	vector<probabilities> p;			//stack of corresponding probabilities
	float dense;						//density
	int number_of_times;				//number of times to corrupt a file (if option is set)
	char * dir;
	char * ext;

	//summary of user defined options
	bool density;						//user defines a density
	bool corruption;					//user defines corruption types
	bool prob;					//user defines probabilities for corruption types
	bool ai;							//user activates ai only mode
	bool number;						//user specifies number of times to apply corruption
	bool turn_off_ai;					//user turns off all ai
	bool ai_already;					//if we already added ai functions
	bool parsed_already;				//if we have already parsed command line options

};

int parser( int , char**, fc_options & );
void parse( int , char**, fc_options & );
int choose( corruption_types );
int choose_probability(fc_options);
void get_density( fc_options & , char *  );
void get_corruptors( fc_options & , char * );
void get_probabilities( fc_options & , char * );
void get_number( fc_options & , char * );
void get_dir( fc_options &, char * );
void get_ext( fc_options &, char * );
void print_help(void);
void print_verbose(void);
bool keep_going( int , char ** , int );
