#pragma once

using namespace std;

#include <string>
#include <vector>
#include "sqlite3.h"
#include <stdexcept>
#include <iostream>
#include <sstream>

class StyleDatabase
{
public:
	StyleDatabase(string dbName);
	~StyleDatabase(void);

	//Use this to create a new Document
	//  if a Style (author) doesn�t exist and add it to the Style table.  
	//returns the DocumentID of the new Document.
	int insertDocument(string Author, string Title, string publishDate);

	// Used to add a sentence
	// This would:
	//	1) convert the vector of words to a vector of integer tokens
	//	2) add any new words to the Token table(if needed)
	//	3) add these tokens to the Sentence table creating a new row.
	//	4) add these tokens to the HMMtokenPath table (along with the StyleID (derived from DocumentID) 
	//     and SentenceID (which is remembered from #3).
	//     Each word would be a row in the HMMtokenPath table along with the word before it and the word after it.
	//	5) Returns the new sentenceID. 
	int insertSentence(int DocumentID, vector<string> words);

	// Returns the string of the selected sentence. 
	string getSentence(int sentenceID);

	// returns the number of instances (the count) in all documents of that series of 3 words.
	//	Returns a list of the SentenceID�s for every occurance of that string. 
	int getWordGroupCount(string prevWord, string currWord, string nextWord);
		
	//	same as above but restricts the count to a specific style.
	int getWordGroupCountByStyle(int StyleID, string prevWord, string currWord, string nextWord);
	//		 getWordGroupCountByStyle() / getWordGroupCount() = the percentage of that word group by style.
		
	//Returns a list of all of the SentenceIDs where that combination of words exists in all documents of the selected style. 
	vector<int> getWordGroupListByStyle(int StyleID, string prevWord, string currWord, string nextWord);

	void clearDatabase();
	vector<string> testFunc(); // Remove after testing

private:
	sqlite3* db;
	sqlite3_stmt *statement;
	//stringstream strm;

	/* Use these minimally -- use the wrapper functions instead */
	void insert(string query);
	int retrieve(string table, string data, string searchType, string searchData);

	/* Wrapper Functions */
	void insertAuthor(string author);
	int retrieveAuthorStyleID(string author);
	void insertDocument( int styleID, string title);
	void insertIntoSentences(int docId);
	void addHMMTokenPath(int SentID, int StyleID, int CurToken, int NextToken, int PrevToken);
	int getStyleID(int docID);
	int getSentenceID(int docID);
	bool doesWordExist(string word);
	void addWord(string word);
	int getWordID(string word);
	int getDocumentID(string title);


};

