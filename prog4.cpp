/*
 * main.cpp
 */

#include "tokens.h"
#include "parse.h"
#include <fstream>
#include <iostream> 
#include <cctype>
#include <string>
#include <ctype.h>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <map>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;



bool isFileOpen(string fileName)
{
    ifstream infile;
    infile.open(fileName);
    
    if(infile.is_open() == false){ return false;}
    else{return true;}
    
    
    return 0;    
}


bool is_textfile_empty( const char* filename ) //stackoverflow
{
  string   s;
  ifstream f( filename, ios::binary );

  // Check for UTF-8 BOM
  if (f.peek() == 0xEF)
    {
    f.get();
    if (f.get() != 0xBB) return false;
    if (f.get() != 0xBF) return false;
    }

  // Scan every line of the file for non-whitespace characters
  while (getline( f, s ))
    {
    if (s.find_first_not_of(
      " \t\n\v\f\r" // whitespace
      "\0\xFE\xFF"  // non-printing (used in various Unicode encodings)
      ) != string::npos)
      return false;
    }

  // If we get this far, then the file only contains whitespace
  // (or its size is zero)
  return true;
}


//stole Ryans code 
int main(int argc, char *argv[])
{
		

	istream *in = &cin;
	ifstream file;
    int linenum = 0;

	for( int i=1; i<argc; i++ ) {
		string arg( argv[i] );
		if( in != &cin ) {
			cout << "TOO MANY FILE NAMES" << endl;
			return 0;
		}
		else {
			file.open(arg);
			if( file.is_open() == false ) {
				cout << "UNABLE TO OPEN " << arg << endl;
				return 0;
			}

			in = &file;
		}
	}
    /**
    ParseTree *prog = Prog(in, &linenum);
    prog->IdentMap(&identCountMap);
    
    
    if(prog==0){return 0;}

    
    int maxcnt = 0;
    
    for(std::map<string,int>::iterator it=identCountMap.begin(); it!=identCountMap.end(); ++it)
    {
        if(it->second > maxcnt)
        {
             maxcnt = it->second;
             mostids = it->first;
        }
        else if(it->second == maxcnt)
        {
             mostids+= ", " + it->first;   
         }
    }
    
    cout<<"LEAF COUNT: "<< prog->LeafCount()<<endl;
    cout<<"STRING COUNT: "<< prog->StringCount()<<endl;
    
    if(prog->IdentCount()!=0)
    {
        cout<<"IDENT COUNT: "<< prog->IdentCount()<<endl;
        cout<<mostids<<endl;
    }
    **/
    ParseTree *prog = Prog(in, &linenum);
    if(prog==0){return 0;}
    
    map<string, Value> symbolsMap;
    prog->Eval(&symbolsMap);

	return 0;
}
