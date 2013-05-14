#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class parser
{
	public:
	int nvtxs,					// Number of vertices in G(V, E)
	    ncon;					// Number of connections in G(V, E)
	std::vector < int64_t > vwgt; 	// Constraint vector.
	std::vector < int64_t > singleConstraintVwgt; 	// Constraint vector.
	std::vector < std::vector < double > > connectivityMatrix;
	std::vector < int64_t > IDMap;
	parser operator = (parser);

	int parseXML(std::string fileName);
	
	parser();
	~parser();
};

#endif
