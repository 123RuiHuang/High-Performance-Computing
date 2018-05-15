using namespace std;
#ifndef HELPER_H
#define HELPER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <sstream>
#include <chrono>
#include <mpi.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
const int LINE_MESSAGE_SIZE = 1000;
const int RESULT_MESSAGE_SIZE = 1500;
typedef list<boost::filesystem::path> path_list_type;
typedef std::map<string,string> book_type; 
path_list_type getFiles(boost::filesystem::path inputFiles); 
void invertFiles(boost::filesystem::path inputFiles, boost::filesystem::path outputFile);
book_type parseFileToBook(string& filename, string &bookname);
void mergeBook(book_type &bigBook,book_type book);
map<string,string> invert(book_type bigBook);
void mergeResult(map<string,string> &invertResult,string line);
void workerJob();
string convert(string& line,int index);
#endif
