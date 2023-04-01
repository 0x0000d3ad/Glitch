#include <iostream>
#include <vector>
#include "MTX.h"

#pragma once

using namespace std;

static int MaxCt = 10000;
static double ErrDelta = 0.1;
static int ct2 = 0;
#define inc2(){ ct2 = ct2++%100; }

#ifndef NN_H
#define NN_H

class NN
{

	friend class NN;
	friend MTX GenerateDelta( int, NN &, MTX, MTX );
	friend void AdjustWeights( NN &, MTX, MTX );
	friend void LearningModule3( NN &, char *, std::ifstream::pos_type);
	friend void aquire_NN_single( NN &, char *, std::ifstream::pos_type);
	friend void aquire_NN_multiple( NN & );
	friend void save_NN( NN &, const char *);
	friend bool open_NN( NN &, const char *);

	//corruption mechanisms

	public:

	NN();						//constructor
	void print();				//prints structure
	void print_dot();			//prints structure in dot format
	void init_v( vector<int> );	//init structure w vector, randomly init weights
	void init_r();				//randomly initialize structure and weights
	void set_rho(double);		//sets learning rate
	MTX h_i( MTX, int );
	MTX DSIG_net_i( MTX, int );
	MTX output( MTX );

	private:
	
	int dim_input;				//Dimension of input vector
	int dim_output;				//Dimension of output vector
	vector< MTX > W;			//Array of matrices for weights
	int length_W;				//Length of the weight array (dynamic) how many hidden nodes + 1

	double rho;					//learning rate
};

#endif
