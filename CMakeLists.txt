cmake_minimum_required( VERSION 2.8.12 )

project( glitch )

set( INPUT  Input/input.txt          )
set( OUTPUT Output/output.txt        )
set( M      MarkovData/Markov.dat    )
set( MT     Input/training_input.txt )

include_directories( ${RANDOM}/Random/Source
	${UTILITY}/Utility/Source )

set( SOURCE Source/FuzzingOperations.cpp
	Source/main.cpp
	Source/MarkovFuzzing.cpp 
	Source/RandomFunction.cpp )

set( HEADERS Source/CommandLineOptions.h
	Source/FuzzingOperations.h
	Source/MarkovFuzzing.h 
	Source/RandomFunction.h
	Source/Random.hpp
	Source/CommandLineParser.hpp )

add_executable( ${PROJECT_NAME} ${SOURCE} ${HEADERS} )

enable_testing()

###############################################
# corruption techniques without markov training
###############################################
add_test( test_Replace                        ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --Replace                            )
add_test( test_Shift                          ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --Shift                              )
add_test( test_Shotgun                        ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --Shotgun                            )
add_test( test_ShotgunShift                   ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --ShotgunShift                       )
add_test( test_Swap                           ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --Swap                               )
add_test( test_Translate                      ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --Translate                          )
                                                                                                                                                
###############################################                                                                                                 
# markov corruption single file                                                                                                                 
###############################################                                                                                                 
add_test( test_CorruptCommonLength            ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptCommonLength                )
add_test( test_CorruptUncommonLength          ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptUncommonLength              )
add_test( test_CorruptCommonLengthMostProb    ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptCommonLengthMostProb        )
add_test( test_CorruptCommonLengthLeastProb   ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptCommonLengthLeastProb       )
add_test( test_CorruptUncommonLengthMostProb  ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptUncommonLengthMostProb      )
add_test( test_CorruptUncommonLengthLeastProb ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptUncommonLengthLeastProb     )
add_test( test_OverwriteMkvMP                 ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --OverwriteMkvMP                     )
add_test( test_OverwriteMkvLP                 ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --OverwriteMkvLP                     )

###############################################
# markov training 
###############################################
add_test( test_Training                       ./${PROJECT_NAME} --MarkovInput ${MT} --MarkovData ${M} --Training                                 )

###############################################
# markov corruption with training 
###############################################
add_test( test_CorruptCommonL_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptCommonLength            --MarkovData ${M} )
add_test( test_CorruptUncommo_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptUncommonLength          --MarkovData ${M} )
add_test( test_CorruptCommonL_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptCommonLengthMostProb    --MarkovData ${M} )
add_test( test_CorruptCommonL_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptCommonLengthLeastProb   --MarkovData ${M} )
add_test( test_CorruptUncommo_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptUncommonLengthMostProb  --MarkovData ${M} )
add_test( test_CorruptUncommo_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --CorruptUncommonLengthLeastProb --MarkovData ${M} )
add_test( test_OverwriteMkvMP_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --OverwriteMkvMP                 --MarkovData ${M} )
add_test( test_OverwriteMkvLP_M ./${PROJECT_NAME} --InputFile ${INPUT} --OutputFile ${OUTPUT} --OverwriteMkvLP                 --MarkovData ${M} )
