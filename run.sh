#!/bin/bash

###########################################################################
#
# name :	run.sh
#
# usage :	./run.sh
#
# purpose : 	corrupt images, make gif
#
# description :
#
###########################################################################


help_menu="run.sh\n"
help_menu+="Options:\n"
help_menu+="\t-b    Create BMPs\n"
help_menu+="\t-g    Create GIFs\n"
help_menu+="\t-a    Create BMPs then GIFs\n"
help_menu+="\t-h    Print options\n"


# create bmps
category_name=stickman
number_bmp=10
number_corruptions=90
number_runs=10
create_bmp () {

	# first corruption from Images directory 
	for (( i=1; i<=$number_bmp; i++ )) 
	do

		input_file="Input/$category_name$i.bmp"

		output_dir="Output/$category_name$i"
		final_dir="$output_dir/final"
		if [ ! -f $input_file ]
		then
			echo "--> Input file \"$input_file\" does not exist"
			continue
		else
			echo "--> Processing file: $input_file"
		fi

		if [ ! -d $output_dir ]
		then
			echo "--> Creating output directory: $output_dir"
			mkdir $output_dir
		fi

		if [ ! -d $final_dir ]
		then
			echo "--> Creating output directory: $final_dir"
			mkdir $final_dir
		fi 

		# first corruption
		j=0
		echo "--> Run: $j"
		for (( num=1; num<=$number_corruptions; num++ )) 
		do
			output_file="Output/$category_name$i/$category_name$i_$num.bmp"
#			echo $input_file
#			echo $output_file
#			echo $num
#			echo "--> Corruption: $num"
			./file_fuzzer --InputFile $input_file\
				--Density 0.00$num\
				--OutputFile $output_file\
				--MarkovData MarkovData/markov_data.dat\
				--Replace\
				--Shift\
				--Shotgun\
				--ShotgunShift\
				--Swap\
				--Translate\
				--CorruptUncommonLength\
				--CorruptCommonLength\
				--CorruptCommonLengthMostProb\
				--CorruptCommonLengthLeastProb\
				--CorruptUncommonLengthMostProb\
				--CorruptUncommonLengthLeastProb\
				--OverwriteMkvMP\
				--OverwriteMkvLP
		done


		# corrupt in a loop 
		# repeat corruptions on files written in Output directory 
		for (( j=1; j<$number_runs; j++ )) 
		do
			echo "--> Run: $j"
			for (( num=1; num<=$number_corruptions; num++ )) 
			do
#				echo "--> Corruption: $num"
				./file_fuzzer --InputFile Output/$category_name$i/$category_name$i_$num.bmp \
					--Density "0.00$num" \
					--OutputFile Output/$category_name$i/$category_name$i_$num.bmp \
					--MarkovData MarkovData/markov_data.dat \
					--Replace\
					--Shift\
					--Shotgun\
					--ShotgunShift\
					--Swap\
					--Translate\
					--CorruptUncommonLength\
					--CorruptCommonLength\
					--CorruptCommonLengthMostProb\
					--CorruptCommonLengthLeastProb\
					--CorruptUncommonLengthMostProb\
					--CorruptUncommonLengthLeastProb\
					--OverwriteMkvMP\
					--OverwriteMkvLP
			done
		done
	done
}


# create gif
create_gif () {
	for (( i=1; i<=$number_bmp; i++ )) 
	do
		convert -loop 0 -delay 10 Output/$category_name$i/*bmp $category_name$i.gif
	done
}

if [[ $@ == '' ]]; then
	echo "--> Error: supply command line arguments"
	echo $help_menu
fi

if [[ $@ =~ .*"-h".* ]]; then
	echo -e $help_menu
fi

if [[ $@ =~ .*"-b".* ]]; then
	create_bmp 
fi

if [[ $@ =~ .*"-g".* ]]; then
	create_gif 
	exit 0
fi

if [[ $@ =~ .*"-a".* ]]; then
	create_bmp
	create_gif 
	exit 0
fi
