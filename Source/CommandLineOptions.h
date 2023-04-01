#include "CommandLineParser.hpp"

using namespace Utility::CommandLineParser;
using namespace std;

struct CommandLineOptions
{
	// instantiate parser
	CmdLine cmd;

	SwitchArg		CorruptCommonLength;
	SwitchArg		CorruptUncommonLength;
	SwitchArg		CorruptCommonLengthMostProb;
	SwitchArg		CorruptCommonLengthLeastProb;
	SwitchArg		CorruptUncommonLengthMostProb;
	SwitchArg		CorruptUncommonLengthLeastProb;
	SwitchArg		OverwriteMkvMP;
	SwitchArg		OverwriteMkvLP;
	SwitchArg		Replace;
	SwitchArg		Shift;
	SwitchArg		Shotgun;
	SwitchArg		ShotgunShift;
	SwitchArg		Swap;
	SwitchArg		Translate;
	SwitchArg		Training;
	ValueArg<unsigned int>   MaxOuterEpochs;
	ValueArg<float>          Density;
	ValueArg<string>         MarkovInput;
	ValueArg<string>         MarkovData;
	ValueArg<string>         InputFile;
	ValueArg<string>         OutputFile;

	CommandLineOptions( void ) :
		cmd	                       ( "file_fuzzer", ' ', "0.0" ),	
		CorruptCommonLength            ( "", "CorruptCommonLength",            "CorruptCommonLength",                                            cmd ),
		CorruptUncommonLength          ( "", "CorruptUncommonLength",          "CorruptUncommonLength",                                          cmd ),
		CorruptCommonLengthMostProb    ( "", "CorruptCommonLengthMostProb",    "CorruptCommonLengthMostProb",                                    cmd ),
		CorruptCommonLengthLeastProb   ( "", "CorruptCommonLengthLeastProb",   "CorruptCommonLengthLeastProb",                                   cmd ),
		CorruptUncommonLengthMostProb  ( "", "CorruptUncommonLengthMostProb",  "CorruptUncommonLengthMostProb",                                  cmd ),
		CorruptUncommonLengthLeastProb ( "", "CorruptUncommonLengthLeastProb", "CorruptUncommonLengthLeastProb",                                 cmd ),
		OverwriteMkvMP                 ( "", "OverwriteMkvMP",                 "OverwriteMkvMP",                                                 cmd ),
		OverwriteMkvLP                 ( "", "OverwriteMkvLP",                 "OverwriteMkvLP",                                                 cmd ),
		Replace                        ( "", "Replace",                        "Replace byte with another from file",                            cmd ),
		Shift                          ( "", "Shift",                          "Translate with shifts",                                          cmd ),
		Shotgun                        ( "", "Shotgun",                        "Shotgun blast of random bits",                                   cmd ),
		ShotgunShift                   ( "", "ShotgunShift",                   "Shift file contents randomly",                                   cmd ),
		Swap                           ( "", "Swap",                           "Swaps two bit regions",                                          cmd ),
		Translate                      ( "", "Translate",                      "Translate bit string by given amount",                           cmd ),
		Training                       ( "", "Training",                       "Markov training mode",                                           cmd ),
		MaxOuterEpochs                 ( "", "MaxOuterEpochs",                 "Max epochs outer loop",               false, 100, "u_int",       cmd ),
		Density                        ( "", "Density",                        "Percent of file to corrupt",          false,  0.1,     "u_int",  cmd ),
		MarkovInput                    ( "", "MarkovInput",                    "Training file for markov",            false, "string", "string", cmd ),
		MarkovData                     ( "", "MarkovData",                     "Markov data file",                    false, "string", "string", cmd ),
		InputFile                      ( "", "InputFile",                      "File to fuzz",                        false, "string", "string", cmd ),
		OutputFile                     ( "", "OutputFile",                     "File for fuzzed output",              false, "string", "string", cmd ) {}
};

CommandLineOptions options;
