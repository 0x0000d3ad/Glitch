#include "parser.h"

using namespace std;
using namespace cvt;

static int ctrp = 0;

unsigned int choose_rand_p( unsigned int size )
{

	ctrp++;
	unsigned int output = (ctrp%2==1)?rand()%size:(size-rand()%size);
	return( (output>=size)?(size-1):output );

}

int parser( int argc, char** argv, fc_options & options )
{

#ifdef WIN32
	srand(GetTickCount());
#endif // WIN32
	int choice = 0;
	int index = 0;

	if( argc <= 3 )
	{
		print_help();
		exit(0);
	}

	if(!options.parsed_already)
	{
		parse( argc, argv, options );
		options.parsed_already = true;
	}

	//if density is not set by the user, choose a random density in (0, 0.1]
	if(!options.density)
	{
		options.dense = ((float)(1+choose_rand_p(10000)))/((float)100000);						//density bounded from above by 0.1
	}
	//max length must be handled outside this function!!
	
	//if number of corruptions is not set by the user, choose a random int in [1, 5]
	if(!options.number)
	{
		options.number=true;
		options.number_of_times = 1+choose_rand_p(5);
	}
	
	//if ai option set, push all of these functions into the corruption stack
	if(options.ai && !options.ai_already)
	{
		options.corruption = true;
		options.ct.push_back(ct_corrupt_common_length);
		options.ct.push_back(ct_corrupt_uncommon_length);
		options.ct.push_back(ct_corrupt_common_length_most_prob);
		options.ct.push_back(ct_corrupt_uncommon_length_most_prob);
		options.ct.push_back(ct_corrupt_common_length_least_prob);
		options.ct.push_back(ct_corrupt_uncommon_length_least_prob);
		options.ct.push_back(ct_overwrite_with_mkv_mp);
		options.ct.push_back(ct_overwrite_with_mkv_lp);
		options.ct.push_back(ct_corrupt_common_length_mult);
		options.ct.push_back(ct_corrupt_uncommon_length_mult);
		options.ct.push_back(ct_corrupt_common_length_most_prob_mult);
		options.ct.push_back(ct_corrupt_uncommon_length_most_prob_mult);
		options.ct.push_back(ct_corrupt_common_length_least_prob_mult);
		options.ct.push_back(ct_corrupt_uncommon_length_least_prob_mult);
		options.ct.push_back(ct_overwrite_with_mkv_mp_mult);
		options.ct.push_back(ct_overwrite_with_mkv_lp_mult);
		//turn ai off so that these options don't get turned on again
		options.ai_already = true;
	}

	//if corruption is specified by the user, push out corruptions
	if( options.corruption )
	{
		if( options.prob )
		{
			choice = choose_probability(options);
		}
		else
		{
			index = choose_rand_p((int)options.ct.size());
			choice = choose( options.ct[index] );
		}
	}

	//if no corruption types are set, then choose randomly, respecting the "turn off ai" mode
	if( !options.corruption && options.turn_off_ai )
	{
		choice = choose_rand_p(7);
	}
	else if( !options.corruption && !options.turn_off_ai)
	{	
		int ai = choose_rand_p(100);
		if(ai >=0 && ai <= 80)													//eighty percent chance of employing AI method
		{
			choice = 7+choose_rand_p(16);	
		}
		else																	//twenty percent chance of choosing non AI corruption method
		{
			choice = choose_rand_p(7);
		}
	}

	return(choice);

}

void parse( int argc , char** argv , fc_options & optns )
{

	//initialize options
	optns.density = false;
	optns.corruption = false;
	optns.prob = false;
	optns.ai = false;
	optns.turn_off_ai = false;

	optns.ct.clear();
	optns.p.clear();

	optns.dense = (float)0.1;
	optns.number_of_times = 0;

	if( argc == 2 && argv[1][0] == '-' && argv[1][1] == 'h' )
	{
		print_help();
	}
	else if( argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v' )
	{
		print_verbose();
	}

	int i = 3;
	while( i < argc )
	{

		//-d: density
		if( argv[i][0] == '-' && argv[i][1] == 'd' )
		{
			optns.density = true;
			get_density(optns, argv[i+1]);
			i+=2;
		}
		//-c: corruptor
		//	since this switch can have multiple arguments,
		//	sweep through argv for all arguments that make sense
		else if( argv[i][0] == '-' && argv[i][1] == 'c' )
		{
			optns.corruption = true;
			++i;
			while(keep_going( argc, argv, i ))
			{
				get_corruptors( optns, argv[i] );
				i++;
			}
			if( i >= argc ) return;
		}
		//-p: probabilities
		//	since this switch can have multiple arguments,
		//	sweep through argv for all arguments that make sense
		else if(  argv[i][0] == '-' && argv[i][1] == 'p' )
		{
			optns.prob = true;
			++i;
			while(keep_going( argc, argv, i ))
			{
				get_probabilities( optns, argv[i] );
				i++;
			}
		}
		//-a: activates all ai corruption mechanisms
		else if(  argv[i][0] == '-' && argv[i][1] == 'a' )
		{
			optns.turn_off_ai = false;
			optns.ai = true;
			i++;
		}
		//-n: how many times to apply corruption mechanisms to a file
		else if(  argv[i][0] == '-' && argv[i][1] == 'n' )
		{
			optns.number = true;
			get_number(optns, argv[i+1]);
			i+=2;
		}
		//-s: turns off ai mechanisms, (-s)tupidifies the fuzzer
		else if(  argv[i][0] == '-' && argv[i][1] == 's' )
		{
			optns.turn_off_ai = true;
			optns.ai = false;
			i++;
		}
		//-m: enter markov info directory and file name extension
		else if(  argv[i][0] == '-' && argv[i][1] == 'm' )
		{
			
			++i;
			int count_markov = 0;
			while(keep_going( argc, argv, i ))
			{
				if(count_markov == 0)
				{
					get_dir(optns, argv[i]);
					i++;
				}
				else
				{
					get_ext(optns, argv[i]);
					i++;
				}
				count_markov++;
			}
		}
		//-h, --help, help: help menu parameters
		else if(  argv[i][0] == '-' && argv[i][1] == '-' )
		{
			print_help();
			return;
		}
		else if(  argv[i][0] == '-' && argv[i][1] == 'h' )
		{
			print_help();
			return;
		}
		else if(  argv[i][0] == 'h' && argv[i][1] == 'e' )
		{
			print_help();
			return;
		}
		else if(  argv[i][0] == '-' && argv[i][1] == 'v' )
		{
			print_verbose();
			return;
		}
		else i++;

	}

}

//produces the choice of a (single) corruptor to use
int choose( corruption_types x )
{
	return( (int)x );
}

void get_density( fc_options & o, char * str )
{
	o.dense = cvt::convertchar2float(str);
}

void get_number( fc_options & o, char * str )
{
	o.number_of_times = convertchar2int(str);
}

int	choose_probability(fc_options oppies)
{
	int sum = 0;
	int result = 0;
	int temp = 0;
	int term = 0;
	int pieces = 0;
	int piece = 0;

	//determine the sum of all the probabilities in options
	for( int i = 0; i < (int)oppies.p.size(); i++ )
	{
		sum+=(int)oppies.p[i];
	}

	//determine if the probabilities vector has more, equal, or less
	//elementes than the corruption types vector
	if(oppies.p.size() > oppies.ct.size())
	{
		term = (int)oppies.ct.size();
	}
	else if( oppies.p.size() == oppies.ct.size() )
	{
		term = (int)oppies.p.size();
	}
	//if there are less probabilities than corruption types
	//take the sum and divide it among the remaining types
	else
	{
		pieces = (int)oppies.ct.size() - (int)oppies.p.size();
		piece = sum/pieces;
		
		for( int i = 0; i < pieces; i++ )
		{
			oppies.p.push_back(piece);
		}
	}

	result = choose_rand_p(sum);

	for( int i = 0; i < term; i++ )
	{
		if( result >= temp && result <= temp + (int)oppies.p[i])
		{
			return(oppies.ct[i]);
		}
		else
		{
			temp+=(int)oppies.p[i];
		}
	}

	return(ct_corrupt_uncommon_length);

}

void get_corruptors( fc_options & o, char * str)
{

	//shotgun
	if(strlen("shotgun") == strlen(str) && str[0] == 's' && str[6] == 'n' )
		o.ct.push_back(ct_shotgun);

	//shotgun_shift
	else if(strlen("shotgun_shift") == strlen(str) && str[0] == 's' && str[6] == 'n' && str[8] == 's')
		o.ct.push_back(ct_shotgun_shift);

	//replace
	else if(strlen("replace") == strlen(str) && str[0] == 'r' && str[6] == 'e')
		o.ct.push_back(ct_replace);

	//swap
	else if(strlen("swap") == strlen(str) && str[0] == 's' && str[3] == 'p')
		o.ct.push_back(ct_swap);

	//translate
	else if(strlen("translate") == strlen(str) && str[0] == 't' && str[8] == 'e')
		o.ct.push_back(ct_translate);

	//shift
	else if(strlen("shift") == strlen(str) && str[0] == 's' && str[4] == 't')
		o.ct.push_back(ct_shift);

	//corrupt_common_length
	else if(strlen("corrupt_common_length") == strlen(str) && str[0] == 'c' && str[15] == 'l')
		o.ct.push_back(ct_corrupt_common_length);

	//corrupt_uncommon_length
	else if(strlen("corrupt_uncommon_length") == strlen(str) && str[0] == 'c' && str[17] == 'l')
		o.ct.push_back(ct_corrupt_uncommon_length);

	//corrupt_common_length_most_prob
	else if(strlen("corrupt_common_length_most_prob") == strlen(str) && str[0] == 'c' && str[22] == 'm')
		o.ct.push_back(ct_corrupt_common_length_most_prob);

	//corrupt_uncommon_length_most_prob
	else if(strlen("corrupt_uncommon_length_most_prob") == strlen(str) && str[0] == 'c' && str[17] == 'l' && str[24] == 'm') 
		o.ct.push_back(ct_corrupt_uncommon_length_most_prob);

	//corrupt_common_length_least_prob
	else if(strlen("corrupt_common_length_least_prob") == strlen(str) && str[0] == 'c' && str[15] == 'l' && str[22] == 'l' )
		o.ct.push_back(ct_corrupt_common_length_least_prob);

	//corrupt_uncommon_length_least_prob
	else if(strlen("corrupt_uncommon_length_least_prob") == strlen(str) && str[0] == 'c' && str[8] == 'u' && str[17] == 'l' && str[24] == 'l' )
		o.ct.push_back(ct_corrupt_uncommon_length_least_prob);

	//overwrite_with_mkv_mp
	else if(strlen("overwrite_with_mkv_mp") == strlen(str) && str[0] == 'o' && str[19] == 'm' )
		o.ct.push_back(ct_overwrite_with_mkv_mp);

	//overwrite_with_mkv_lp
	else if(strlen("overwrite_with_mkv_lp") == strlen(str) && str[0] == 'o' && str[19] == 'l' )
		o.ct.push_back(ct_overwrite_with_mkv_lp);

	//corrupt_common_length_mult
	else if(strlen("corrupt_common_length_mult") == strlen(str) && str[0] == 'c' && str[15] == 'l' && str[22] == 'm')
		o.ct.push_back(ct_corrupt_common_length_mult);

	//corrupt_uncommon_length_mult
	else if(strlen("corrupt_uncommon_length_mult") == strlen(str) && str[0] == 'c' && str[17] == 'l' && str[24] == 'm' )
		o.ct.push_back(ct_corrupt_uncommon_length_mult);

	//corrupt_common_length_most_prob_mult
	else if(strlen("corrupt_common_length_most_prob_mult") == strlen(str) && str[0] == 'c' && str[22] == 'm' && str[22] == 'm' )
		o.ct.push_back(ct_corrupt_common_length_most_prob_mult);

	//corrupt_uncommon_length_most_prob_mult
	else if(strlen("corrupt_uncommon_length_most_prob_mult") == strlen(str) && str[0] == 'c' && str[17] == 'l' && str[24] == 'm' && str[34] == 'm')
		o.ct.push_back(ct_corrupt_uncommon_length_most_prob_mult);

	//corrupt_common_length_least_prob_mult
	else if(strlen("corrupt_common_length_least_prob_mult") == strlen(str) && str[0] == 'c' && str[15] == 'l' && str[28] == 'p' )
		o.ct.push_back(ct_corrupt_common_length_least_prob_mult);

	//corrupt_uncommon_length_least_prob_mult
	else if(strlen("corrupt_uncommon_length_least_prob_mult") == strlen(str) && str[0] == 'c' && str[8] == 'u' && str[17] == 'l' && str[24] == 'l' && str[35] == 'm' )
		o.ct.push_back(ct_corrupt_uncommon_length_least_prob_mult);

	//overwrite_with_mkv_mp_mult
	else if(strlen("overwrite_with_mkv_mp_mult") == strlen(str) && str[0] == 'o' && str[19] == 'm')
		o.ct.push_back(ct_overwrite_with_mkv_mp_mult);

	//overwrite_with_mkv_lp_mult
	else if(strlen("overwrite_with_mkv_lp_mult") == strlen(str) && str[0] == 'o' && str[19] == 'l' )
		o.ct.push_back(ct_overwrite_with_mkv_lp_mult);

	else return;

}

void get_probabilities( fc_options & o, char * str)
{
	o.p.push_back(convertchar2int(str));
}

void get_dir(fc_options & o, char * str )
{
	o.dir = str;
}

void get_ext(fc_options & o, char * str )
{
	o.ext = str;
}

void print_help(void)
{

	cout	<< "fc09 help menu:" << endl
			<< "Minimum command line parameters:" << endl
			<< "fc09 input.ext output.ext <test file directory>" << endl << endl
			<< "Command line switches" << endl
			<< "	-a: ai corruptors" << endl
			<< "	-c: corruption type" << endl
			<< "	-h: help menu" << endl
			<< "	-m: c:\\directory filename_ext (without the \".\")"
			<< "		used for markov analysis of a directory"
			<< "	-n: number of corruptions to apply to a file" << endl
			<< "	-p: density <float>" << endl
			<< "	-s: turn off ai" << endl
			<< "	-v: verbose definition of corruption mechanisms" << endl;

}

void print_verbose(void)
{

	cout	<< "Non AI Corruption Mechanisms:" << endl << endl
			<< "shotgun:\n\tshotgun blast of hex" << endl << endl
			<< "shotgun_shift:\n\tshotgun blast of shifts" << endl << endl
			<< "replace:\n\treplaces a character in the buffer" << endl << endl
			<< "swap:\n\tswaps contiguous blocks of the file around" << endl << endl
			<< "translate:\n\tadds a constant value to a contiguous block in the file" << endl << endl
			<< "shift:\n\tapplies shift operation" << endl << endl

			<< "AI Corruption Types" << endl << endl
			<< "corrupt_common_length:\n\tcorrupts a common structure" << endl << endl
			<< "corrupt_uncommon_length:\n\tcorrupts an uncommon structure" << endl << endl
			<< "corrupt_common_length_most_prob:\n\tcorrupts common elements with common elements" << endl << endl
			<< "corrupt_uncommon_length_most_prob:\n\tcorrupts uncommon elements with common elements" << endl << endl
			<< "corrupt_common_length_least_prob:\n\tcorrupts common elements with uncommon elements" << endl << endl
			<< "corrupt_uncommon_length_least_prob:\n\tcorrupts uncommon elements with uncommon elements" << endl << endl
			<< "overwrite_with_mkv_mp:\n\toverwrites an arbitrary block with common elements" << endl << endl
			<< "overwrite_with_mkv_lp:\n\toverwrites an arbitrary block with uncommon elements" << endl << endl
			<< "corrupt_common_length_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "corrupt_uncommon_length_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "corrupt_common_length_most_prob_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "corrupt_uncommon_length_most_prob_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "corrupt_common_length_least_prob_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "corrupt_uncommon_length_least_prob_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "overwrite_with_mkv_mp_mult:\n\tsame as above but uses information from multiple files" << endl << endl
			<< "overwrite_with_mkv_lp_mult:\n\tsame as above but uses information from multiple files" << endl;

}

bool keep_going( int argc, char ** argv, int i )
{

	if( i >= argc ) return(false);
	if( argv[i][0] == '-' || argv[i][0] == 'h' ) return(false);
	else return(true);

}
