#include "helper.h"
path_list_type getFiles(boost::filesystem::path dir)
{
	
	path_list_type directoryContents; // is a list
	boost::filesystem::path dirPath(dir);
	boost::filesystem::directory_iterator end_iter;
	for ( boost::filesystem::directory_iterator dir_itr( dirPath );dir_itr != end_iter;++dir_itr )
	{
   directoryContents.push_back(dir_itr->path());
	}
	return directoryContents;
}

book_type parseFileToBook(string& filename) 
{

	book_type book;
	string line;
	ifstream datafile(filename.c_str()); // open the file for input
  if (datafile.is_open()) // test for open failure
	{

		unsigned int linesRead = 0;
   vector<string> partOfName;
   boost::split(partOfName, filename, boost::is_any_of("/"));
   string name = partOfName[partOfName.size()-1];// size of partOfName is 9
   name += ":";
	 
		// Process file into memory
 	while ( datafile.good() ) // read while the stream is good for reading
		{
	
			// Lines are typically formatted as chapter:verse: Text
			getline(datafile,line);
			if (line.empty())
				continue; // do not try to parse an empty line
			vector<string> chapterVerseStory;
			boost::split(chapterVerseStory, line, boost::is_any_of(":"));
    //  cout<<chapterVerseStory.size()<<endl;
     // for(int i =0; i < chapterVerseStory.size();i++) cout<<chapterVerseStory[i]<<endl;
	   // cout<<endl;
			if (chapterVerseStory.size() < 3)
			{
				continue;
			}
      
      
			// Trim
      
			const string chapter(boost::algorithm::trim_copy(chapterVerseStory[0]));
			const string verse = boost::algorithm::trim_copy(chapterVerseStory[1]);
      string text = name;
      for(int i = 2; i < chapterVerseStory.size(); i++) 
      {
		   text += boost::algorithm::trim_copy(chapterVerseStory[i]);
      if(i < chapterVerseStory.size() -1)
      {
        text += ": ";
      }
      }
      
			// Create a line index	
			string chapterVerseIndex = name + to_string(boost::lexical_cast<int>(chapter) * 1000 +  boost::lexical_cast<int>(verse)); // combine name and index because key need to be unique in a map
			book[chapterVerseIndex] = text;
		//  cout<<chapterVerseIndex<<" "<<text<<endl;
			++linesRead;
		}
		datafile.close();

  }

	return book;
}



void mergeBook(book_type &bigBook,book_type book)
{
 book_type::iterator elem;
 elem = book.begin();
 while(elem != book.end())
 {
  bigBook[elem->first] = elem->second;
  elem++;
 }

}

map<string,string> invert(book_type bigBook)
{
  // Master
  int threadCount;
  MPI_Comm_size(MPI_COMM_WORLD,&threadCount);
  map<string,string> invertResult;
  book_type::const_iterator record = bigBook.begin();
  for(int rank = 1; rank < threadCount && record != bigBook.end(); ++rank,++record)
  {
     const string indexAndName(record->first); // genesis:1001
     vector<string> indexAndNameArray;
     boost::split(indexAndNameArray, indexAndName, boost::is_any_of(":"));
     int index = atoi(indexAndNameArray[1].c_str());
     const string line(record->second);//genesis:In the beginning God created the heaven and the earth.
     const size_t length = line.size();
     char msg[LINE_MESSAGE_SIZE];
     line.copy(msg,length);
     msg[length] = '\0';
     MPI_Send(msg,LINE_MESSAGE_SIZE,MPI_CHAR,rank,index,MPI_COMM_WORLD);
  }
  
  for(;record != bigBook.end(); ++record)
  {   
      char resultMsg[RESULT_MESSAGE_SIZE];
      MPI_Status status;
      MPI_Recv(resultMsg,RESULT_MESSAGE_SIZE,MPI_CHAR,MPI_ANY_SOURCE,
      MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      const int incomingIndex = status.MPI_TAG; // 1000 * chapter + verse
      const int sourceCaught = status.MPI_SOURCE;// id of Workers which is rank
      string returnLine(resultMsg);
      mergeResult(invertResult,returnLine);
      string outgoingIndexAndName(record->first);
      vector<string>outgoingIndexAndNameArray;
      boost::split(outgoingIndexAndNameArray, outgoingIndexAndName, boost::is_any_of(":"));
      int outgoingIndex = atoi(outgoingIndexAndNameArray[1].c_str());
      const string line(record->second);
      const size_t length = line.size();
      char msg[LINE_MESSAGE_SIZE];
      line.copy(msg,length);
      msg[length] = '\0';
      MPI_Send(msg,LINE_MESSAGE_SIZE,MPI_CHAR,sourceCaught,outgoingIndex,MPI_COMM_WORLD);
  }
 
  // no more work need to be done, so receive all the results from workers
  for(int rank = 1; rank < threadCount; ++rank)
  {
      char resultMsg[RESULT_MESSAGE_SIZE];
      MPI_Status status;
      MPI_Recv(resultMsg, RESULT_MESSAGE_SIZE,MPI_CHAR,rank,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      string line(resultMsg);
      mergeResult(invertResult,line);
  }
  for(int rank = 1; rank < threadCount; ++rank)
  {
   MPI_Send(0,0,MPI_INT,rank,0,MPI_COMM_WORLD);
  }
  return invertResult;

}
void mergeResult(map<string,string> &invertResult,string line)
{  //cout<<line<<endl;
   vector<string> items;// each item is a sting : word1=file1,chp:vsX
   boost::split(items, line, boost::is_any_of("||"));
   int size = items.size();
   
   for(int i = 0; i < size; i++)
   { if(items[i].empty()) continue;
     //cout<<items[i]<<endl;
     vector<string> wordAndLlocation;
     boost::split(wordAndLlocation, items[i], boost::is_any_of("="));
     string word = wordAndLlocation[0];
     string location = wordAndLlocation[1]; 
     if(invertResult.find(word) == invertResult.end()) {
       invertResult[word] = location;
     }
     else
     {
      invertResult[word] = invertResult[word] + "; " + location;
     }
     
   }
  
   
}

void workerJob()
{// worker function
 int rank;
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 char msg[LINE_MESSAGE_SIZE];
 MPI_Status status; 
 while(1)
 {
    // receive a message from master
    MPI_Recv(msg,LINE_MESSAGE_SIZE,MPI_CHAR,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    if(status.MPI_TAG == 0)
    {
      return;
    }
    const int index = status.MPI_TAG;
    // convert msg to a string for parseing
    string line(msg);
    const string resultLine = convert(line,index);// word1=file1,chp:vsX;file1,chp:vsY || word2= file1,chp:vsX;file1:vsY
    const size_t length = resultLine.size();
    char resultMsg[RESULT_MESSAGE_SIZE];
    resultLine.copy(resultMsg,length);
    resultMsg[length] = '\0';
    MPI_Send(resultMsg,RESULT_MESSAGE_SIZE,MPI_CHAR,0,index,MPI_COMM_WORLD);
    
 }
  return;

}

string convert(string& line,int index) // this is what worker do
{
 int chapter = index / 1000;
 int verse = index % 1000;
 map<string,string> wordCounts;
 vector<string> words;
 boost::split(words, line, boost::is_any_of(": ,.;!?)("));// the first element of words is the name of the book
 vector<string>::const_iterator w = words.begin();
 string bookname(boost::algorithm::to_lower_copy(*w));
 w++;
 bookname += ".";
 bookname += boost::algorithm::to_lower_copy(*w); //txt
 w++;
 for(;w!=words.end();++w)
 {
  string term(boost::algorithm::to_lower_copy(*w));
  if (boost::algorithm::trim_copy(term).empty()) continue;
  if(wordCounts.find(term) == wordCounts.end()) // first time
  {
      wordCounts[term] = bookname + "," + to_string(chapter) + ":" + to_string(verse);
  }
  
 }
 stringstream result;
 for(map<string,string>::const_iterator t = wordCounts.begin();t !=wordCounts.end();++t)
 {
   string key(t->first);
   string value(t->second);
   result << key << "=" << value << "||";
 } 
 return result.str();
}
void createOutPut(map<string,string> invertResult,boost::filesystem::path outputFile)
{
 ofstream output;
 string filename = outputFile.string();
 output.open(filename,ios::out|ios::app);
 map<string,string>::iterator elem = invertResult.begin();
 for(int i = 0; i < invertResult.size(); i++) {
   output<<elem->first<<"="<<elem->second<<endl;
       elem++;
   }
 output.close();

}
void invertFiles(boost::filesystem::path inputFiles,boost::filesystem::path outputFile){
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank == 0)
  {   chrono::high_resolution_clock c;
      chrono::high_resolution_clock::time_point start4 = c.now();
      
      cout<<" Begin to read books(parse all files to book_type(books) and merge books to a big book ) ";
      chrono::high_resolution_clock::time_point start1 = c.now();
      path_list_type files = getFiles(inputFiles);
      path_list_type::iterator file = files.begin();
      book_type bigBook;
      for(;file != files.end();file++) 
      {    boost::filesystem::path filecopy(*file);
           string filename = filecopy.string();
           book_type book(parseFileToBook(filename));
           mergeBook(bigBook,book);
      }
      chrono::high_resolution_clock::time_point stop1 = c.now();
		  double t = (double)chrono::duration_cast<chrono::microseconds>(stop1 - start1).count() / 1000000.0;
      cout<<" Finished. It cost " << t <<" s"<<endl;
      
      cout<<" Begin to build in-memory inverted file";
      chrono::high_resolution_clock::time_point start2 = c.now();
      map<string,string> invertResult = invert(bigBook);
      MPI_Barrier(MPI_COMM_WORLD);
      chrono::high_resolution_clock::time_point stop2 = c.now();
      t = (double)chrono::duration_cast<chrono::microseconds>(stop2 - start2).count() / 1000000.0;
      cout<<" Finished. It cost " << t <<" s"<<endl;
      
      
      // write back file
      cout<<" Begin to write the output file ";
      chrono::high_resolution_clock::time_point start3 = c.now();
      createOutPut(invertResult,outputFile);
      chrono::high_resolution_clock::time_point stop3 = c.now();
      t = (double)chrono::duration_cast<chrono::microseconds>(stop3 - start3).count() / 1000000.0;
      cout<<" Finished. It cost " << t <<" s"<<endl;
      
      chrono::high_resolution_clock::time_point stop4 = c.now();
      t = (double)chrono::duration_cast<chrono::microseconds>(stop4 - start4).count() / 1000000.0;
      cout<<" It totally cost " << t <<" s"<<endl<<endl<<endl;
   }
  else 
  {
   workerJob();
   MPI_Barrier(MPI_COMM_WORLD);
  }

}
