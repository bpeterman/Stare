using namespace std;



#include <sstream> 
#include <fstream>
#include <iostream>
#include <thread>
#include "Stopwatch.h"
#include "CMDparser.h"
#include "Tokenizer.h"
#include "HMMengine.h"
#include "PHPsocket.h"

// initialize static variable
int CMDparser::cmdParserInstanceCount = 0;

CMDparser::CMDparser()
{
    cmdParserInstanceCount++;
}

CMDparser::~CMDparser()
{
    cmdParserInstanceCount--;
}


bool CMDparser::parseCMD(vector<string> cmdList)
{
	//MetaData metaData;

	// Change command string to lower case
	//std::transform(cmdList[0].begin(), cmdList[0].end(), cmdList[0] , (int(*)(int))tolower);
	if (cmdList.size() <= 0)
		return true;  // The string was empty.

	// Select the task
	if (cmdList[0] == "learn")
		Learn(cmdList);
	else if (cmdList[0] == "compare")
		Compare(cmdList);
	else if (cmdList[0] == "create")
		Create(cmdList);
	else if (cmdList[0] == "test")
		test();
	else if (cmdList[0] == "Brandon")
		Brandon(cmdList);
	else if (cmdList[0] == "Blake")
		Blake(cmdList);
	else if (cmdList[0] == "Sam")
		Sam(cmdList);
	else if (cmdList[0] == "Hiep")
		Hiep(cmdList);
	else if (cmdList[0] == "Leven")
		Leven(cmdList);
	else if (cmdList[0] == "Brian")
		Brian(cmdList);
	else if (cmdList[0] == "Show")
		show(cmdList);
	else if ((cmdList[0] == "Create") && (cmdList[1] == "Database"))
		doCreateDatabase(true);
	else if (cmdList[0] == "Execute")
		Execute(cmdList[1]);
	else if (cmdList[0] == "help")
	{
		cout << "The commands are : 'quit', 'learn', 'compare', 'create', 'help'" << endl;
		cout << "For help on a specific command, type the command without arguments." << endl;
		cout << "All commands are case sensitive." << endl;
	}
	else if (cmdList[0] == "quit")
		// return false to end message loop
		return false;
	else 
	{
		cout << "'" << cmdList[0] << "'" << " is not a valid command." << endl;
		cout << "Type 'help' for a list of commands." << endl;
	}
	// load the file from the database and insert it into DocumentText
	return true;
}

void CMDparser::doCreateDatabase(bool confirm)
{
	hmm.dataBase.CreateDatabase(confirm);
	hmm.tokenizer.tdb.LoadTokenMap();
}

void CMDparser::show(vector<string> cmdList)
{
	if (cmdList[1]=="Documents")
	{
	  vector<MetaData> docs = getDocuments();
	  for(std::vector<MetaData>::iterator d = docs.begin(); d != docs.end();++d)
	  {
	      cout << d->Title << endl;
	  }
	}
    else if (cmdList[1]=="Styles")
    {
        std::vector<std::string> styles = getStyles();
        for(std::vector<std::string>::iterator s = styles.begin(); s != styles.end();++s)
        {
            cout << *s << endl;
        }

    }

	  
}

void CMDparser::Learn(vector<string> cmdList)
{
	if (cmdList.size() != 5)
	{
		cout << "'learn' adds the document with the selected params to the database." << endl;
		if (cmdList.size() != 1) // it is not a request for help
			cout << "Invalid Parameters" << endl;
		cout << "example: learn [filename][Author][Title][PublishDate]" << endl;
		cout << "All arguements should be in quotes." << endl;
		return;
	}
	MetaData metaData;
	metaData.DocumentText = cmdList[1]; // ReadFile(cmdList[1]);
	metaData.action = ActionType::Learn;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];
	cout << "Learning document -> " << metaData.Author << " : " << metaData.Title;
	hmm.learnFromFile(metaData);
}
void CMDparser::Compare(vector<string> cmdList)
{
	if (cmdList.size() != 5)
	{
		cout << "'compare' uses the selected document pointed at by the params and" << endl;
		cout << "compares it to the rest of the documents in the database" << endl;
		cout << "to try to recognize the style of the selected document." << endl;
		if (cmdList.size() != 1) // it is not a request for help
			cout << "Invalid Parameters" << endl;
		cout << "example: compare [filename][Author][Title][PublishDate]" << endl;
		cout << "All arguements should be in quotes." << endl;
		return;
	}
	MetaData metaData;
	metaData.DocumentText = cmdList[1]; // ReadFile(cmdList[1]);
	metaData.action = ActionType::Compare;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];
	cout << "Comparing document -> " << metaData.Author << " : " << metaData.Title;
	//hmm(db, tokenizer);
	hmm.compareWithFile(hmm,metaData);
}

void CMDparser::Create(vector<string> cmdList)
{
	//if (cmdList.size() != 3)
	//{
	//	if (cmdList.size() != 1) // it is not a request for help
	//		cout << "Invalid Parameters" << endl;
	//	cout << "example: learn [filename][Author]" << endl;
	//	cout << "All arguements should be in quotes." << endl;
	//	return;
	//}
	int StyleID = atoi( cmdList[1].c_str());
	int size = atoi(cmdList[2].c_str());

	//MetaData metaData;
	//metaData.DocumentText = ReadFile(cmdList[1]);
	//metaData.action = ActionType::Create;
	//metaData.Author = cmdList[2];
	////HMMengine hmm(db, tokenizer);
	cout << hmm.createDoc(StyleID, size) << endl;
}

string CMDparser::ReadFile(string fileName)
{
	using std::ifstream;

	ifstream t(fileName);
	std::stringstream buffer;

	if (!t.is_open())
	{
		cout << "can't open '" << fileName << "' file for input" << endl;
		return "";
	}
	else
	{
		buffer << t.rdbuf();
		return buffer.str();
	}

}

void CMDparser::Execute(string file)
{
	string line;
	vector<string> cmdLst;
	string fileWithExtension = file + ".stare";
	string commandString = ReadFile(fileWithExtension);
	if (commandString != "")
	{
		istringstream commandStream(commandString);
		while (getline(commandStream, line))
		{
			cout << "<< " << line << endl;
			cmdLst = CMDparser::getCommands(line);
			parseCMD(cmdLst);
			cout << endl << endl;
		}
	}
}

vector<string> CMDparser::getCommands(string cmdStr)
{
	vector<string> words;
	stringstream ss;

	// I was too lazy to implement a full state machine.  However...
	// Uses 3 states to determine the action at a character position.  They are:
	// inWord==false, inQuotes==false :  this state means that any non Alphanums are dumped until a proper alphanum is encountered
	// inWord==true, inQuotes==false  :  This state meant that any alphanums are added to the current word until a non-alphanum is encountered
	// inWord==true, inQuotes==true  :  Anything, whether alphanum or not, is added to the current word until another '"' is encountered.

	bool inWord = false;   // if inWord, all alphanumeric 
	bool inQuotes = false; // if inQuotes is true, all characters are recorded into the current ss until another quote is encountered.

	for (unsigned int i = 0; i < cmdStr.size(); i++)
	{
		if (!inWord)
		{
			if (isAlphaNumeric(cmdStr[i]))
			{
				// start word
				ss << cmdStr[i];
				inWord = true;
			}
			else if (cmdStr[i] == '"')
			{
				// start quotes
				inWord = true;
				inQuotes = true;
			}
		}
		else
		{
			// inWord is true
			if (inQuotes)
			{
				if (cmdStr[i] != '"')
					ss << cmdStr[i];
				else
				{
					// the quotes have ended
					words.push_back(ss.str());
					ss.str("");  // empty ss
					inWord = false;
					inQuotes = false;

				}
			}
			else
			{
				if (isAlphaNumeric(cmdStr[i]))
					ss << cmdStr[i];
				else
				{
					// the word has ended
					words.push_back(ss.str());
					ss.str("");  // empty ss
					inWord = false;
				}

			}
		}
	}

	// The last word can end with the end of the string.  
	// It would not be detected then, so add it here.
	if (inWord)
		words.push_back(ss.str());

	return words;
}

bool CMDparser::isAlphaNumeric(char c)
{
	if (((c >= 'a') && (c <= 'z')) ||
		((c >= 'A') && (c <= 'Z')) ||
		((c >= '0') && (c <= '9')))
		return true;

	else
		return false;
}

/******************************   PHPconnector functions  ************************************/
//***********  STUBS    STUBS
//string CMDparser::getSentences(int rangeStart, int rangeEnd)
//{
//	return "This is sentence 3.  This is sentence 4.";
//} 

//string CMDparser::getDocument(int DocumentID)
//{
//	return"This is sentence 1.  This is sentence 2. This is sentence 3.  This is sentence 4.";
//} 


//string CMDparser::create(int clientID, string author, int numOfSentences)
//{
//	return "Now is the time for all good men to come to the aid of their country.";
//}
//
//comparethreadEngine
//
//vector<MetaData> CMDparser::getDocuments()
//{
//	MetaData md("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt");
//	std::vector<MetaData> test;
//	test.push_back(md);
//	test.push_back(md);
//	test.push_back(md);
//	return test;
//
//}
//
//std::vector<MetaData> CMDparser::getStyles()
//{
//	MetaData md("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt");
//	std::vector<MetaData> test;
//	test.push_back(md);
//	test.push_back(md);
//	test.push_back(md);
//	return test;
//}

// these are no longer needed
int CMDparser::checkCompareStatus(int clientID)  // done is 100
{
	return 25;
}
int CMDparser::checkCreateStatus(int clientID)     // done is 100
{
	return 25;
}

string CMDparser::getSentences(int rangeStart, int rangeEnd)
{
	std::stringstream ss;
	if ((int) hmm.dataBase.TotalSentenceList.size() <= rangeStart)
		ss << "These sentences do not exist in the database.";
	else
	{

		if ((int) hmm.dataBase.TotalSentenceList.size() <= rangeEnd)
			rangeEnd = hmm.dataBase.TotalSentenceList.size();

		for (int i = rangeStart; i < rangeEnd; i++)
		{
			ss << hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[i]);
		}
	}
	return ss.str();

}



string CMDparser::getDocument(int DocumentID)
{
	std::stringstream ss;

	//int documentIndex = hmm.dataBase.getDocumentListIndex(DocumentID);
	if ((DocumentID >= 0) && (DocumentID < (int)hmm.dataBase.documentList.size()))
	{
		for (int i = hmm.dataBase.documentList[DocumentID].startSentenceID;
			i < hmm.dataBase.documentList[DocumentID].endSentenceID;
			 i++)
		{
			ss << hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[i]);
		}
	}
	else
	{
		ss << "Cannot find this document in the database";
	}

	return ss.str();
}

void CMDparser::learn(string author, string title, string date, string text)
{
	hmm.learn(MetaData(author, title, date, text));
}

/*
CompareResult CMDparser::compare(int clientID, string text)
{
	//HMMengine hmm;
	//hmm.compare(MetaData("", "", "", text));
	vector<SentenceRanking> test;

	SentenceBlob sbSource;
	sbSource.NextNextSentenceStr = "Sentence 9";
	sbSource.NextSentenceStr = "Sentence 8";
	sbSource.SentenceStr = "Sentence 7";
	sbSource.PrevSentenceStr = "Sentence 6";
	sbSource.PrevPrevSentenceStr = "Sentence 5";
	sbSource.SentenceID = 7;

	SentenceBlob sbFound;
	sbFound.NextNextSentenceStr = "Sentence 9";
	sbFound.NextSentenceStr = "Sentence 8";
	sbFound.SentenceStr = "Sentence 7";
	sbFound.PrevSentenceStr = "Sentence 6";
	sbFound.PrevPrevSentenceStr = "Sentence 5";
	sbFound.SentenceID = 7;

	test.push_back(SentenceRanking("Dickens", "A tale of two cities", sbSource, sbFound, .57));
	test.push_back(SentenceRanking("Mark Twain", "Huckleberry Finn", sbSource, sbFound, .57));

	StyleCertaintyItem style1;
	style1.certainty = .73;
	style1.StyleID = 2;
	style1.StyleName = "Mark Twain";

	vector<StyleCertaintyItem> ci;
	ci.push_back(style1);

	CompareResult cr;
	cr.documentCertainties = ci;
	cr.sentenceRankings = test;
        cr.percentComplete = 100;
	return cr;
}
*/



CompareResult CMDparser::compare(int clientID, string text)
{
      EngineStatus engineStatus(clientID);
      hmm.compareThreadEngine(&hmm,&engineStatus, text);
      CompareResult cr =  engineStatus.getResult();
      cr.percentComplete=100;
      cr.print();
      // write cr to file
      std::string file_name = "..//HTML//output//" + to_string(clientID);
      ofstream result_file(file_name);
      result_file << PHPsocket::JSONtoStringCompare(cr);
      result_file.close();
      return cr;
  
  
		//HMMengine hmm;
		//hmm.compare(MetaData("", "", "", text));

	// search for the active process
  
  /*
	CompareResult cr;
	cr.percentComplete = 0;
	int foundProcessIndex = -1; // if not found
	std::cout << "received compare request from: " << clientID << std::endl;
	std::cout << "Current number of threads:" << engineProcesses.size() << std::endl;
	std::cout << "list: " << std::endl;
	
	std::cout << "the current CMDparser instance count is " << cmdParserInstanceCount << std::endl;
	for (size_t p = 0; p < engineProcesses.size(); ++p)
	    std::cout << "    " << engineProcesses[p]->getClientID() << std::endl;
	for (unsigned int i = 0; i < engineProcesses.size(); i++)
	{
		if (engineProcesses[i]->getClieOutgoing:
{
   "command" : "checkCompare",
   "overallCertainty" : "N/A",
   "ranking" : [
      {
         "certainty" : 27,
         "dataBaseSnip" : [ "", "", "" ],
         "documentTitle" : "",
         "origSnip" : [
            "--whoever you\nare, give it back!",
            "\"\n\n\n\nO For a Muse of Fire, that would ascend\nThe brightest Heauen of Inuention\n",
            ""
         ]
      }
   ]
}
ntID() == clientID)
		{
			// We found it.
			foundProcessIndex = i;
			if (engineProcesses[i]->getPercentComplete() == 100)
			{
				// the process is complete, so return it
				std::cout << "Process is complete, return CR and delete process" << std::endl;
				std::cout<< "printing CR";
				cr = engineProcesses[i]->getResult();
				cr.print();
				cr.percentComplete = 100;
				// delete the object
				delete(engineProcesses[i]);
				engineProcesses.erase(engineProcesses.begin() + i);
			}
			else
			{
				//process is not complete, so return status.
				//CompareResult cr;
				cr.percentComplete = engineProcesses[i]->getPercentComplete();
			}
		}
	}
	if (foundProcessIndex == -1)
	{
		// it was not found in the list.
		std::cout << "the session ID was not found" << std::endl;
		if (text == "")
		{
			std::cout << "Error:  No text was sent.  Either timed out or the first request was sent empty. " << std::endl;
			// this is an error.  Either it timed out, they never sent it before.  
			//CompareResult cr;
			cr.percentComplete = -1;
		}
		else
		{
			std::cout << "New request initiated" << std::endl;
			// this is a new request so we should get started.
			EngineStatus* engineStatus = new EngineStatus(clientID);
			engineProcesses.push_back(engineStatus);
			std::cout << "Currently there are " << engineProcesses.size() << " active processes";
			//std::thread t(testThread, hmm, engineStatus, text);
			std::thread t(&HMMengine::compareThreadEngine,&hmm,engineStatus, text); //
			t.detach();
			//t.join();
			std::cout << "post detach" << std::endl;
		}
	}
	std::cout << "percentComplete" << cr.percentComplete << std::endl;
	return cr;


*/






	//int status = hmm.compareCheck(clientID);

	//if (hmm.compareCheck(clientID))
	//{
	//	cr.percentComplete = status;
	//}
	//else
	//{
	//}
	//return hmm.compare()

}


CreateResult CMDparser::create(int clientID, string author, int numOfSentences)
{
    CreateResult cr;
    cr.percentComplete = 0;
    int styleID = hmm.dataBase.getStyleID(author);
    //std::cout << "StyleID for "<< author << " is " << styleID << std::endl;
    cr.newDocument = hmm.createDoc(styleID,numOfSentences);
    //std::cout << "The created string is: " << std::endl;
    std::cout << cr.newDocument;
    return cr;
} 



vector<MetaData> CMDparser::getDocuments()
{
	//MetaData md("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt");
	//std::vector<MetaData> test;
	//test.push_back(md);
	//test.push_back(md);
	//test.push_back(md);
	vector<MetaData> docList;
	for (unsigned int i = 0; i < hmm.dataBase.documentList.size(); i++)
        if (hmm.dataBase.documentList[i].Author != "")
		    docList.push_back(MetaData(hmm.dataBase.documentList[i].Author, hmm.dataBase.documentList[i].Title, hmm.dataBase.documentList[i].PublishDate, ""));
	return docList;

}

std::vector<std::string> CMDparser::getStyles()
{
	//MetaData md("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt");
	//std::vector<MetaData> test;
	//test.push_back(md);
	//test.push_back(md);
	//test.push_back(md);
	std::vector<std::string> styles;
	for (unsigned int i = 0; i < hmm.dataBase.StyleList.size(); i++) {
        if (hmm.dataBase.StyleList[i].Author != "")
            styles.push_back(hmm.dataBase.StyleList[i].Author);
    }
	return styles;
}

/******************************   PHPconnector functions  ************************************/

void CMDparser::test()
{
	std::cout << std::endl << "test the console 'test' command - passed" << std::endl << std::endl;
	std::cout << std::endl << "Clearing the database for clean test." << std::endl << std::endl;
	hmm.dataBase.CreateDatabase(false);  // command often fails under windows.  just delete the file manually.

	std::cout << std::endl << "Tokenizing test document" << std::endl;
	std::deque<std::vector<int>> BrianTokenizedDoc = hmm.tokenizer.tokenizeDoc("This is a test doc.  You still need.  The tokenizer will convert these sentences to tokens");

	std::cout << std::endl << "Inserting metadata Brian's Book" << std::endl;
	int docID = hmm.dataBase.insertDocument("Brian", "Brian book Title", "2014");
	std::cout << "Inserting Brian's book text" << std::endl << std::endl;
	hmm.dataBase.insertDocumentText(docID, BrianTokenizedDoc);

	std::deque<std::vector<int>> BradonTokenizedDoc = hmm.tokenizer.tokenizeDoc("This is a test doc.  You still need.  The tokenizer will convert these sentences to tokens");
	std::cout << std::endl << "Inserting metadata Brandons's Book" << std::endl;
	docID = hmm.dataBase.insertDocument("Brandon", "Brandon book Title", "2014");
	std::cout << "Inserting Brandon's book text" << std::endl << std::endl;
	hmm.dataBase.insertDocumentText(docID, BradonTokenizedDoc);

	std::cout << std::endl << "Printing the list of styles in the database." << std::endl;
	vector<string> cmdLst;
	cmdLst.push_back("show");
	cmdLst.push_back("Styles");
	show(cmdLst);

	std::cout << std::endl << "Printing the list of Documents in the database." << std::endl;
	cmdLst[1] = "Documents";
	show(cmdLst);
}



void CMDparser::Brandon(vector<string> cmdParams)
{
	//cout << "The int on our platform is " << (sizeof(int)*8) << " bits" << endl;
	//cout << "The long on our platform is " << (sizeof(long)* 8) << " bits" << endl; 
	//cout << "The long long on our platform is " << (sizeof(long long)* 8) << " bits" << endl;
	//StyleDatabase& test = StyleDatabase::getInstance();
	//test.open("BrianAIsql.db3");
	//StyleDatabase db;
	//db.clearDatabase();
	//db.insertAuthor("Brian");
	//int StyleID = db.retrieveAuthorStyleID("Brian");
	////test.insertDocument(StyleID, "Brian book", "1996");
	//db.retrieve("Styles", "Author", "Author", "Sam");
	//int DocumentID = db.insertDocument("Sam", "Sams Book", "1900");  // test new path
	//DocumentID = db.insertDocument("Brian", "Brians other book.", "1998"); // test same author, new book
	//DocumentID = db.insertDocument("Brian", "Brian book", "1996"); // test same author, same book.
	//vector<string> sentVect1;
	//sentVect1.push_back("This");
	//sentVect1.push_back("is");
	//sentVect1.push_back("a");
	//sentVect1.push_back("test");
	//db.insertSentence(DocumentID, sentVect1);
	//db.insertSentence(DocumentID, sentVect1); // test duplicate words.
	//vector<string> sentVect2;
	//sentVect2.push_back(".");
	//db.insertSentence(DocumentID, sentVect2); // test short sentence.
	//db.close();
}

void CMDparser::Blake(vector<string> cmdParams)
{

}

void CMDparser::Sam(vector<string> cmdParams)
{

}

void CMDparser::Hiep(vector<string> cmdParams)
{

}

void CMDparser::Leven(vector<string> cmdParams)
{

}

void CMDparser::Brian(vector<string> cmdParams)
{
	learn("Brian", "Test of Compare", "2014", "I want to prove that the learn interface works properly.");
	getDocuments();
	Execute("test");
	getDocuments();
	getStyles();
	std::string showSent = getSentences(50, 56);
	std::string showDoc =  getDocument(1);
	//DocumentDatabase dBase;
	//dBase.CreateDatabase(false);
	////Tokenizer t;
	////t.tokenizeFile(string("../StareProject/Documents/HenryV.txt"));
	//////std::deque<std::vector<int>> 
	////t.save_BAV(string("e:/tmp/HenryVreconstruct.txt"));

	////HMMengine hmm(db, tokenizer);
	//Stopwatch sw;


	//MetaData metaData;
	//metaData.DocumentText = "../StareProject/Documents/HenryV.txt"; // ReadFile(cmdList[1]);
	//metaData.action = ActionType::Learn;
	//metaData.Author = "Shakespere";
	//metaData.Title = "Henry V";
	//metaData.PublishDate = "1619";
	//cout << "Learning document -> " << metaData.Author << " : " << metaData.Title;

	////hmm.learn(metaData);

	////Execute("test");
	////dBase.CreateDatabase(false);
	//hmm.learn(metaData);
	//dBase.insertDocument("fred", "sam book", "2341");
	//sqlite3* db = dBase.openDB();
	//int styleID = dBase.getStyleID(db, "fred");
	//std::cout << styleID;

	//sw.start();
	//Tokenizer tokenizer = Tokenizer();
	//std::vector <std::vector<int>> Midsummer = tokenizer.tokenizeDoc("../StareProject/Documents/AMidsummerNightsDream.txt");
	//sw.end();

	//int tokenizerTime = sw.getTimeInMicroseconds();

	////	vector<string> sentenceVect;
	//sw.start();

	//int docID = db.insertDocument("Shakespere", "A Midnight Summer Dream", "a long long ago");
	//sw.end();
	//int dbDocTime = sw.getTimeInMicroseconds();

	//sw.start();
	//db.insertDocumentText(docID, Midsummer);
	//sw.end();

	//int dbTextTime = sw.getTimeInMicroseconds();

	//  Add more books
	//db.clearDatabase();

	//sw.start();
	//cout << "Loading Henry V" << endl;
	//hmm.learn(MetaData("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt"));
	//cout << "Loading Romeo and Juliet" << endl;
	//hmm.learn(MetaData("Shakespere", "Romeo and Juliet", "1597", "../StareProject/Documents/RomeoAndJuliet.txt"));
	//cout << "Loading A Tale of Two Cities" << endl;
	//hmm.learn(MetaData("Charles Dickens", "A Tale of Two Cities", "1859", "../StareProject/Documents/ATaleOfTwoCities.txt"));
	//cout << "Loading Great Expectation" << endl;
	//hmm.learn(MetaData("Charles Dickens", "Great Expectations", "1860", "../StareProject/Documents/GreatExpectations.txt"));
	//sw.end();
	//int learnTime = sw.getTimeInMicroseconds();

	//hmm.compare(MetaData("Shakespere", "A Midnight Summer Dream", "1605", "../StareProject/Documents/AMidsummerNightsDream.txt"));
}

	//do
	//{
	//	sentenceVect = tokenizer.getNextSentence();
	//	db.insertSentence(docID, sentenceVect);
	//} while (sentenceVect.size() > 0);
	//sw.end();

	//int dbTime = sw.getTimeInMicroseconds();

	//int sentID = 
	//string sent = db.getSentence(sentID);CreateResult
	//Tokenizer tokenizer2 = Tokenizer("../StareProject/Documents/AChristmasCarol.txt");
	//tokenizer2.tokenizeDoc();
	//int docID2 = db.insertDocument("Charles Dickens", "A Christmas Carol", "1864");
	//do
	//{
	//	sentenceVect = tokenizer2.getNextSentence();
	//	db.insertSentence(docID2, sentenceVect);
	//} while (sentenceVect.size() > 0);


//}