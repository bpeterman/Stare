#pragma once

using namespace std;

#include <iostream>
#include <vector>
#include <thread>
#include "HMMengine.h"
#include "EngineStatus.h"


//  Takes the command line arguments and uses it to load the MetaData struct.
class CMDparser
{
public:
	static int cmdParserInstanceCount;  
  
	CMDparser();
	~CMDparser();
	
	HMMengine hmm;  
	vector<EngineStatus*> engineProcesses;

	void doCreateDatabase(bool confirm);
	bool parseCMD(vector<string> cmdList);
	void Learn(vector<string> cmdList);
	void Compare(vector<string> cmdList);
	void Create(vector<string> cmdList);

	void Execute(string fileName);
	void show(vector<string> cmdList);
	void Quit(vector<string> cmdList);

	string ReadFile(string fileName);

	void Brandon(vector<string> cmdList);
	void Blake(vector<string> cmdList);
	void Sam(vector<string> cmdList);
	void Hiep(vector<string> cmdList);
	void Leven(vector<string> cmdList);
	void Brian(vector<string> cmdList);

	static vector<string> getCommands(string cmdStr);
	static bool isAlphaNumeric(char c);

	string getSentences(int rangeStart, int rangeEnd);
	string getDocument(int DocumentID);


	void learn(string author, string title, string date, string text);
	CompareResult compare(int clientID, string text);
	CreateResult create(int clientID, string author, int numOfSentences);

	int checkCompareStatus(int clientID);  // done is 100
	int checkCreateStatus(int clientID);   // done is 100

	vector<MetaData> getDocuments();

	std::vector<std::string> getStyles();
	void test();

};

