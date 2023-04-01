#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "NN.h"
#include "MTX.h"

#pragma warning(disable:4996)

using namespace std;

double SUM( vector<MTX> sum )
{

	double y = 0;

	for( int i = 0; i < (int)sum.size(); i++ ) y += SUM_MTX( sum[i] );

	return( y );

}

NN::NN()
{

	dim_input = 0;
	dim_output = 0;
	W.clear();
	length_W = 0;

}
						
void NN::print()
{
	cout << length_W << endl;
	for( int i = 0; i < length_W; i++ ) 
	{	
		cout << "Matrix" << i+1 << ":" << endl;
		cout << W[i].ii << "\t" << W[i].jj << endl;
		W[i].print(); 
	}

}
				
void NN::print_dot()
{

	ofstream out("neural_structure.dot");
	string temp;
	int i, j, k, l, m = 0;
	
	out << "digraph{" << endl;
	
	//graph all interconnections
	for(  i = 0; i < (int)W.size(); i++ )
	{
		for(  j = 0; j < W[i].ii; j++ )
		{
			for(  k = 0; k < W[i].jj; k++ )
			{
				for(  l = 0; l < W[i].ii; l++ )
				{
					for(  m = 0; m < W[i].jj; m++ )
					{
						if(j==l&&k==m) continue;
						else
						{
							out << i << j << k << "->";
							out << i << l << m << ";" << endl;
						}
					}
				}
			}
		}
	}
	//graph intercnctns between input/output of subsequent weight matrices
	for(  i = 1; i < (int)W.size(); i++ )
	{
		for(  j = 0; j < W[i].ii; j++ )		//W[i].ii should == W[i-1].jj
		{
			out << i << j << j << "->";
			out << i-1 << j << j << ";" << endl;
		}
	}
	
	out << "}" << endl;
	out.close();

}

void NN::init_v( vector<int> V )
{

	MTX temp;
	int temp1 = 0;

	W.clear();
	dim_input = V.front();
	dim_output = V.back();
	
	for( size_t i = 0; i < V.size() - 1; i++ )
	{
	
		temp.rinit(V[i+1], V[i]);
		W.push_back(temp);
		
	}
	
	length_W = (int)V.size()-1;
	rho = 0.5;

}
			
void NN::init_r()
{

	inc2();
	int mod = 15;
	int low = 3;
	vector<int> temp;
	temp.clear();
	
	for(int i = 0; i < 3+rand()%15; i++)
	{

		temp.push_back(3+rand()%15); 
	}
	
	NN::init_v( temp );
	
}						

void NN::set_rho(double r) { rho=r; }

MTX NN::DSIG_net_i( MTX input, int place )  //runs from [0,length)
{

	MTX in;
	MTX hidden_node;
	in = input;

	hidden_node = W[0]*in;
	for( int i = 1; i <= place; i++ )
	{
		in = SIG( hidden_node );	
		hidden_node = W[i]*in;
	}
	
	return(DSIG(hidden_node));

}

MTX NN::h_i( MTX input, int place )  //runs from [0,length)
{

	MTX in;
	MTX hidden_node;
	in = input;

	for( int i = 0; i <= place; i++ )
	{
		hidden_node = W[i]*in;
		in = SIG( hidden_node );
	}
	
	return(in);

}

MTX NN::output( MTX input ){return(NN::h_i(input,length_W-1));}


MTX GenerateDelta( int place, NN &ann, MTX out, MTX in )
{
	MTX O;		//output of using "in" as input
	MTX neti;	//output before round of ith weight matrix
	MTX netn;	//output of last node
	MTX dO;		//err output: (netn - "out").SIG'(netn)
	MTX di;		//err ith layer: SIG'(neti)*(di+1)^T*(Wi+1)
				//error vector in front of d+1
	MTX di_plus_1;
				//Wi+1 weight matrix following Wi
	MTX Wi_plus_1;
	MTX temp1;
	MTX temp2;
	
	int i = 0;	//index for weight matrices
				//last W matrix
	int n = ann.length_W;
	//Generate first output delta
	O = ann.h_i(in,n-1);

	dO = COMP( (out - O), ann.DSIG_net_i(in, n-1) );

	di = dO;
	if( place <= n-2 )
	{
		for( i = n-2; i >= place; i-- )
		{
			//temp var's for delta(i+1) and W(i+1)
			di_plus_1 = di;
			Wi_plus_1 = ann.W[i+1];
		
			//SIG'( net(i) )
			temp1 = ann.DSIG_net_i(in, i);
			//matrix multiplication: delta(i+1)*W(i+1)
			temp2 = ((di_plus_1^T)*Wi_plus_1)^T;
			//term by term (linear) product of above two terms
			//term1: SIG'( net(i) )
			//term2: delta(i+1)*W(i+1)
			//SIG'( net(i) ) * W(i+1)*delta(i+1)
			//Note: term1 and term2 should be same dimensionality because
			//matrix multiplication is column vector * operator, not vice versa
			di =  COMP(temp1,temp2);
		}	
	}
	return( di );

}

void AdjustWeights(NN &ann, MTX out, MTX in)
{

	MTX di;
	double rho = ann.rho;
	
	int i = 0;	//index for weight matrices
				//last W matrix
	int n = ann.length_W;

	for( i=n-1; i>=0; i--)
	{
		di=GenerateDelta(i,ann,out,in);
		ann.W[i]+=rho*(di*(ann.h_i(in,i-1)^T));
	}
}

//sliding window format, runs through buffer, and adjusts with last element as output
void LearningModule3( NN &ann, char * buffer, std::ifstream::pos_type size )
{

	MTX input;
	MTX output;
	int input_length = ann.dim_input;
	int output_length = ann.dim_output;
	vector<double> in;
	vector<double> out;

	input.init(input_length, 1);
	output.init(output_length,1);

	in.reserve(input_length);
	out.reserve(output_length);

	//start the base case
	//grab first input set from buffer
	for( int i = 0; i < input_length; i++ )
	{
		in.push_back( 1/((double)(unsigned char)*(buffer+i)) );
	}
	input = in;

	//grab the element in the buffer following all of the elements above
	for( int i = 0; i < output_length; i++ )
	{
		out.push_back( 1/((double)(unsigned char)*(buffer+input_length+i)) );
	}
	output = out;

	//adjust weights
	AdjustWeights( ann, output, input );

	//continue iteratively untill the end of the buffer
	for( int i = 0; i < (int)size - (input_length + output_length); i++ )
	{

		in.clear();
		out.clear();

			for( int j = 0; j < input_length; j++ )
			{
				in.push_back( ((double)2/256)*((double)(unsigned char)(*(buffer+i+j)))-1 );


//FIX NN'S!!!!!!!!

			}
			input = in;

			for( int k = 0; k < output_length; k++ )
			{
				out.push_back( ((double)2/256)*((double)(unsigned char)(*(buffer+i+k)))-1 );
			}
			output = out;

		AdjustWeights( ann, output, input );

	}

}

void aquire_NN_single( NN &ann, char *buffer, std::ifstream::pos_type size)
{

	LearningModule3(ann, buffer, size);

}

void aquire_NN_multiple( NN & ann )
{

		char * memblock = NULL;
		char * name = NULL;
		float temp = 0;
		std::ifstream::pos_type size;
		int sizem = 256;


		WIN32_FIND_DATA FileData;
		HANDLE hSearch;

		int count = 0;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! fix problem with specifying data type !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		hSearch = FindFirstFile("C:\\fc09\\build\\BMP\\*.bmp", &FileData);

		while(hSearch!=INVALID_HANDLE_VALUE)
		{
			name = (char*)malloc(strlen("C:\\fc09\\build\\BMP\\")+strlen(FileData.cFileName));
			strcpy(name,"C:\\fc09\\build\\BMP\\");
			strcat(name, FileData.cFileName);

			cout << "Scanning File: " << FileData.cFileName << endl;			

			ifstream file(name, ios::in|ios::binary|ios::ate);
			if (file.is_open())
			{
				size = file.tellg();
				memblock = (char*)malloc(size*sizeof(char)+64);
				if(memblock==NULL)
				{
					cerr << "Memory could not be freed" << endl;
					exit(0);
				}
				file.seekg(0, ios::beg);
				file.read (memblock, size);
				file.close();
			}
			else 
			{
				cerr << "NN multiple acquisition error: unable to open file" << endl;
				return;
			}

			aquire_NN_single(ann, memblock, size);
			free(memblock);

			if(FindNextFile(hSearch, &FileData)==0) break;
		}


}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! scrutinize these damn things!  Prolly v v v wrong!! !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void save_NN( NN & ann, const char* filename )
{
	int sizem = ann.length_W;
	ofstream out(filename, ios::binary|ios::out|ios::ate );

	if(out)
	{
		//write out how many layers of the NN
		out.write((char*)&sizem, 1*sizeof(int));

		//write out dim of inputs
		out.write((char*)&ann.dim_input, 1*sizeof(int));

		//write out the length of each column vector representing the NN
		for(int i=0;i<sizem-1;i++)
		{
			out.write((char*)&ann.W[i].jj, 1*sizeof(int));
		}

		//write out all elements of the NN
		for( int k = 0; k < sizem; k++ ) 
		{	
			for(int i = 0; i < ann.W[k].ii; i++ )
			{
				for(int j = 0; j < ann.W[k].jj; j++ )  
				{
					out.write((char*)&ann.W[k][i][j], 1*sizeof(double));
				}
			}
		}

		out.close();
	}

}

bool open_NN( NN & ann, const char* filename )
{
	int sizem = 0;
	vector <int> NN_vector;
	int temp = 0;
	ifstream in(filename, ios::binary );

	if(in)
	{
		//read in how many layers of the NN
		in.read((char*)&sizem, 1*sizeof(int));
		NN_vector.reserve(sizem);

		//read in the length of each column vector representing the NN
		for(int i=0;i<sizem;i++)
		{
			in.read((char*)&temp, 1*sizeof(int));
			NN_vector.push_back(temp);
		}

		//initialize a NN with respect to the NN_vector
		ann.init_v(NN_vector);

		//fill in the elements of the NN
		for( int k = 0; k < sizem; k++ ) 
		{	
			for(int i = 0; i < ann.W[k].ii; i++ )
			{
				for(int j = 0; j < ann.W[k].jj; j++ )  
				{
					in.read((char*)&ann.W[k][i][j], 1*sizeof(double));
				}
			}
		}

		in.close();
		return true;
	}
	else
	{
		return false;
	}

}

