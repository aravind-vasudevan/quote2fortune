#ifndef PARSER_CPP
#define PARSER_CPP

#include "parser.hpp"

using namespace std;

string sanitizeInput( string input )
{
	string ret;
	int i, size=input.size();
	bool leftBracketFound=false, leftAngleFound=false;
	for( i=0; i<size; ++i)
	{
		if( input[i] == '<' )
			leftAngleFound=true;
		else if( input[i] == '>' )
			leftAngleFound=false;
		else if( input[i] == '[' )
			leftBracketFound=true;
		else if( input[i] == ']' )
			leftBracketFound=false;
		else if( leftBracketFound == false && leftAngleFound == false )
			ret.push_back( input[i] );
	}
	return ret;	
}

bool findStringInVector( vector < string > vec, string str )
{
	bool found=false;
	for(int i=0; i<vec.size(); ++i)
	{
		if( str.compare(vec[i]) == 0 )
		{
			found=true;
			break;
		}
		else
		{
		}
	}
	return found;
}

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
	const string& delimiters = ">";
	ifstream input_file;
	input_file.open( fileName.c_str() );
	string quoter, title;
	bool ignoreInputs = false;
	vector < string > exitableQuoters, ignorableQuoters;
	exitableQuoters.push_back(string("External links"));
	ignorableQuoters.push_back(string("Contents"));
	ignorableQuoters.push_back(string("See also"));
	ignorableQuoters.push_back(string("Cast"));
	ignorableQuoters.push_back(string("Taglines"));
	ofstream fileWriter;
	int64_t line_no = 0, numberOfNodes = 0, numberOfEdges = 0;
	int i, blade = 0;
	while( input_file.good() )
	{
		string line;
		vector < string > tokens;
		getline( input_file, line );
		int64_t milestone = 0;
		for( i=0; i<line.size(); ++i )
		{
			string temp = line.substr(i, delimiters.size());
			if( temp.compare(delimiters) == 0 ) // delimiter found
			{
				tokens.push_back( line.substr(milestone, i-milestone+1) );
				milestone = i+1;
			}
		}
		if( line.substr(milestone, i-milestone+1).size() != 0 )
			tokens.push_back( line.substr(milestone, i-milestone+1) );

/*
		string::size_type lastPos = line.find_first_not_of( delimiters, 0 );
		string::size_type pos = line.find_first_of( delimiters, lastPos );
		while( string::npos != pos || string::npos != lastPos )
		{
			string temp = line.substr( lastPos, pos - lastPos );
			tokens.push_back( temp );
			lastPos = line.find_first_not_of( delimiters, pos );
			pos = line.find_first_of( delimiters, lastPos );
		}
*/

		if( tokens.size() == 0 )
		{
		}
		else if(tokens.size() == 1)
		{
//			if( tokens[0].find("<Nodes>") != string::npos )
//			{
//				blade = 1;
//				ncon = 1;
//			}
		}
		else
		{
			if( tokens[0].find("<h2>") != string::npos || ignoreInputs == true )
			{
				if( tokens[0].find("<h2>") != string::npos )
				{
					quoter = sanitizeInput( line.substr( tokens[0].size(), line.size() ) );
					while( quoter[quoter.size()-1] == ' ') // stupid lil hack to remove trailing white spaces, if any
					{
						quoter = quoter.substr( 0, quoter.size()-1 );
					}
					if( findStringInVector( exitableQuoters, quoter ) == true )
					{
						break;
					}
					if( findStringInVector( ignorableQuoters, quoter ) == false )
					{
						ignoreInputs = false;
					}
					else
						ignoreInputs = true;
				}
			}
			else if( tokens[0].find("<title>") != string::npos )
			{
				title = sanitizeInput( tokens[1] );
				transform(title.begin(), title.end(), title.begin(), ::tolower);
				for( i=0; i<title.size(); ++i )
					if( title[i] == ' ' )
						title.erase(i,1);
				fileWriter.open( title.c_str(), ios::trunc);
			}
			else if( tokens[0].find("<li>") != string::npos )
			{
				fileWriter<<sanitizeInput(line.substr( tokens[0].size(), line.size()+1 ));
				fileWriter<<endl<<"\t\t-- "<<quoter.c_str()<<endl<<"%\n";
			}
			else
			{
			}
		}
		line_no++;
	}


	cout<<"FileName:"<<title;
	fileWriter.close();	
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
