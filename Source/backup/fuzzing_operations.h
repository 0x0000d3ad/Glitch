#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>

#pragma once
using namespace std;

namespace fuzzing_operations
{
	void shotgun( char *, float, ifstream::pos_type );					//shotgun blast of hex
	void shotgun_shift( char *, float, ifstream::pos_type );			//shotgun blast of shifts
	void replace( char *, float, ifstream::pos_type );					//replaces one byte with another from somewhere in the buffer
	void swap( char *, unsigned int, ifstream::pos_type );						//swaps two bit regions of a given size
	void insert_rand( char *, unsigned int, ifstream::pos_type );				//inserts a string of random characters
	void insert_A( char *, unsigned int, ifstream::pos_type );				//inserts a  string of A's
	void translate( char *, unsigned int, ifstream::pos_type );					//translates a string of bytes by a randomly chosen amount
	void shift( char *, unsigned int, ifstream::pos_type );						//translate function but with shifts
}