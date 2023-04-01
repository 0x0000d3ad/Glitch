#include "CommandLineOptions.h"
#include "FuzzingOperations.h"
#include "MarkovFuzzing.h"

using namespace std;
using namespace fuzzing_operations;
using namespace markov_fuzzing;

int main( int argc, char **argv )
{
	options.cmd.parse( argc, argv );

	float density = options.Density.getValue();
	unsigned int max_length = 0;
	ifstream input_file;
	ifstream::pos_type file_size;
	ofstream output_file;	
	unsigned int markov[ 256 ][ 256 ];
	unsigned int benchmark=0;
	char * memblock;

	// initialize markov matrix structure
	zero_markov( markov );

	//////////////////////////////////////////////////////////
	// Training Mode
	// - reads in training input data and stores to db
	// - if db is already there, reads data in and adds to it
	//////////////////////////////////////////////////////////

	if ( options.Training.isSet() )
	{
		if ( ! options.MarkovInput.isSet() )
		{
			cerr << "Error: Please supply markov training input." << endl;
			return 1;
		}

		if ( ! options.MarkovData.isSet() )
		{
			cerr << "Error: Please supply markov training data file." << endl;
			return 1;
		}

		// try to open markov data file to see if there is any previousely stored training data
		open( markov, options.MarkovData.getValue().c_str() );

		input_file.open( options.MarkovInput.getValue().c_str(), ios::in | ios::binary | ios::ate );
		if ( input_file.is_open() )
		{
			file_size = input_file.tellg();
			max_length = ( unsigned int ) ( density * ( float ) file_size );
			memblock = ( char * ) malloc( file_size * sizeof( char ) );
			if( ! memblock )
			{
				cerr << "Memory could not be freed" << endl;
				exit(0);
			}
			input_file.seekg ( 0, ios::beg );
			input_file.read ( memblock, file_size );
			input_file.close();
		}
		else 
		{
			cerr << "Unable to open file" << endl;
			return 1;
		}		

		aquire_markov( markov, memblock, file_size );

		save( markov, options.MarkovData.getValue().c_str() );

		// return.  This is training mode.  Do not continue.
		return 0;
	}

	//////////////////////////////////////////////////////////
	// Get input from file 
	//////////////////////////////////////////////////////////

	if ( ! options.InputFile.isSet() )
	{
		cerr << "Error: Please specify an input file." << endl;
		return 1;
	}

	input_file.open( options.InputFile.getValue().c_str(), ios::in|ios::binary|ios::ate );
	if ( input_file.is_open() )
	{
		file_size = input_file.tellg();
		max_length = ( unsigned int ) ( density * ( float ) file_size );
		memblock = ( char * ) malloc( file_size * sizeof( char ) );
		if( ! memblock )
		{
			cerr << "Memory could not be freed" << endl;
			exit(0);
		}
		input_file.seekg ( 0, ios::beg );
		input_file.read ( memblock, file_size );
		input_file.close();
	}
	else 
	{
		cerr << "Unable to open file" << endl;
		return 1;
	}

	//////////////////////////////////////////////////////////
	// Get markov data.  Either from data file or from
	// memblock
	//////////////////////////////////////////////////////////

	if ( options.MarkovData.isSet() )
	{
		// if we can open the markov data file, get the contents, 
		// otherwise continue as though no markov data specified
		if ( open( markov, options.MarkovData.getValue().c_str() ) )
		{
			find_benchmark( benchmark, markov );
		}
		else
		{
			goto aquisition;
		}
	}
	else
	{
		aquisition :
			aquire_markov(markov, memblock, file_size);
			find_benchmark( benchmark, markov );
	}

	//////////////////////////////////////////////////////////
	// Basic file corruption techniques
	//////////////////////////////////////////////////////////

	//shotgun blast of hex
	if ( options.Shotgun.isSet() )
	{
		shotgun( memblock, density, file_size );
	}

	//shotgun blast of shifts
	if ( options.ShotgunShift.isSet() )
	{
		shotgun_shift( memblock, density, file_size );
	}

	//replaces one byte with another from somewhere in the buffer
	if ( options.Replace.isSet() )
	{
		replace( memblock, density, file_size );
	}

	//swaps two bit regions of a given size
	if ( options.Swap.isSet() )
	{
		swap( memblock, max_length, file_size );
	}

	//translates a string of bytes by a randomly chosen amount
	if ( options.Translate.isSet() )
	{
		translate( memblock, max_length, file_size );
	}

	//translate function but with shifts
	if ( options.Shift.isSet() )
	{
		shift( memblock, max_length, file_size );
	}

	//////////////////////////////////////////////////////////
	// markov file corruptions 
	// - markov data and benchmark have been set above
	//////////////////////////////////////////////////////////

	if ( options.CorruptCommonLength.isSet() )
	{
		corrupt_common_length( markov,  max_length, memblock, file_size,  benchmark );
	}

	if ( options.CorruptUncommonLength.isSet() )
	{
		corrupt_uncommon_length( markov,  max_length, memblock, file_size,  benchmark );
	}

	if ( options.CorruptCommonLengthMostProb.isSet() )
	{
		corrupt_common_length_most_prob( markov,  max_length, memblock, file_size, benchmark );
	}

	if ( options.CorruptUncommonLengthMostProb.isSet() )
	{
		corrupt_uncommon_length_most_prob( markov,  max_length, memblock, file_size,  benchmark );
	}

	if ( options.CorruptCommonLengthLeastProb.isSet() )
	{
		corrupt_common_length_least_prob( markov,  max_length, memblock, file_size,  benchmark );
	}

	if ( options.CorruptUncommonLengthLeastProb.isSet() )
	{
		corrupt_uncommon_length_least_prob( markov,  max_length, memblock, file_size,  benchmark );
	}
	
	if ( options.OverwriteMkvMP.isSet() )
	{
		overwrite_with_mkv_mp( markov,  max_length, memblock, file_size,  benchmark);
	}

	if ( options.OverwriteMkvLP.isSet() )
	{
		overwrite_with_mkv_lp( markov,  max_length, memblock, file_size,  benchmark);
	}

	// write results to corrupted file
	output_file.open( options.OutputFile.getValue().c_str(), ios::out | ios::binary );
	if ( output_file.is_open() )
	{
		output_file.write ( memblock, file_size );
		output_file.close(); 
	}
	else
	{
		cerr << "Error: Unable to open output file: " << options.OutputFile.getValue() << endl;
		return 1;
	}

	free( memblock );

	return 0 ;

}
