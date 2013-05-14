#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h> //your directory may be different
#include "parser.hpp"

using namespace std;

string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

int main( int argc, char *argv[] )
{
    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, argv[1] );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);
	ofstream fileWriter;
	string tempFileName("buffer.xml");
	fileWriter.open(tempFileName.c_str(), ios::trunc);
	fileWriter << data;
	fileWriter.close();

	parser p;

	p.parseXML(tempFileName);
    //cout << endl << data << endl;
    cin.get();

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
