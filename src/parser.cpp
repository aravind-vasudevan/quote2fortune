#ifndef PARSER_CPP
#define PARSER_CPP

#include "parser.hpp"

using namespace std;

int
parser :: parseXML( std::string fileName )
{
	enum
	{
		_NODE = 1,
		_EDGE = 1,
		_NEND = 3,
		_NSTART = 5,
		_WEIGHT = 7
	};
	const string& delimiters = "\"";
	ifstream input_file;
	input_file.open( fileName.c_str() );
	int64_t line_no = 0, numberOfNodes = 0, numberOfEdges = 0;
	int i, blade = 0;
	while( input_file.good() )
	{
		string line;
		vector < string > tokens;
		getline( input_file, line );
		string::size_type lastPos = line.find_first_not_of( delimiters, 0 );
		string::size_type pos = line.find_first_of( delimiters, lastPos );
		while( string::npos != pos || string::npos != lastPos )
		{
			string temp = line.substr( lastPos, pos - lastPos );
			tokens.push_back( temp );
			lastPos = line.find_first_not_of( delimiters, pos );
			pos = line.find_first_of( delimiters, lastPos );
		}
		if(tokens.size() == 1)
		{
			if( tokens[0].find("<Nodes>") != string::npos )
			{
				blade = 1;
				ncon = 1;
			}
			else if( tokens[0].find("</Nodes>") != string::npos )
			{
				nvtxs = numberOfNodes;
				for(int i=0; i<nvtxs; ++i)
				{
					vector < double > tempVector( nvtxs );
					connectivityMatrix.push_back( tempVector );
				}
				singleConstraintVwgt.resize( nvtxs, 0 );
				cout<<"\nTotal number of nodes found = "<<numberOfNodes;
				fflush( stdout );
			}
			else if( tokens[0].find("<Edges>") != string::npos )
				blade = 3;
			else if( tokens[0].find("</Edges>") != string::npos )
			{
				blade = 4;
//				createMETISFile( numberOfNodes, numberOfEdges, ncon, fileName );
			}
			else if( tokens[0].find("</Graph>") != string::npos )
				break;
		}
		else
		{
			switch( blade )
			{
				case 1:
				{
					IDMap.push_back( atoll( tokens[_NODE].c_str() ) );
					for(int i = 0; i<ncon; ++i)
						vwgt.push_back( 0 ); // Assume equal weights for all intersections
					++numberOfNodes;
					break;
				}
				case 3:
				{
					int64_t temp = atoll( tokens[ _NSTART ].c_str() );
					int64_t nStart, nEnd;
					for(i=0; i<IDMap.size(); ++i)
						if( IDMap[i] == temp )
						{	nStart = i; break;	}
					temp = atoll( tokens[ _NEND ].c_str() );
					for(i=0; i<IDMap.size(); ++i)
						if( IDMap[i] == temp )
						{	nEnd = i; break;	}
					connectivityMatrix[ nStart ][ nEnd ] = atoll( tokens[ _WEIGHT ].c_str() );
					vwgt[ nStart*ncon ] += connectivityMatrix[ nStart ][ nEnd ];
					vwgt[ nEnd*ncon ] += connectivityMatrix[ nStart ][ nEnd ];
					++numberOfEdges;
					break;
				}
				default:
					break;
			}
		}
		line_no++;
	}
	singleConstraintVwgt.resize(nvtxs, 0);
	for(i=0; i<nvtxs; ++i)
		for(int j=0; j<ncon; ++j)
			singleConstraintVwgt[i] += vwgt[i*ncon + j];
		
	input_file.close();
}

parser::parser()
{
	nvtxs = ncon = 0;
}

parser::~parser()
{
	// free memory for the all the variables. call vector.clear();
}

parser parser::operator = (parser param)
{
	parser temp;
	temp.nvtxs = param.nvtxs;
	temp.ncon = param.ncon;
	temp.vwgt = param.vwgt;
	return (temp);
}

#endif
