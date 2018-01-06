#pragma once
#pragma once
#include "../Publisher_CommHandler/Publisher_CommHandler.h"
#include "../HttpMessage/HttpMessage.h"
//#include"../Analyzer/DepAnal.h"
#include <thread>
#include <iostream>
#include <string>
#include <time.h>
#include<vector>
#include<ctime>
#include<exception>
#include"../Publisher/Publisher.h"
#include"../dependency/depend.h"
using namespace Logging;
using namespace CodeAnalysis;
class Exec //like the windowmain class
{
	using File = std::string;
	using Files = std::vector<File>;
	using depen = std::unordered_map<std::string, std::set<std::string>>;
	using NewMap = std::unordered_map<std::string, Type_Table*>;
private:
	Publisher_CommHandler commHandler_shim;
	std::thread rcvThread;
	NewMap map_;
	depen maps;
	std::thread processThread;
	// DepAnalysis executerForDa(TypeAnal ta, std::vector<std::string> cppFiles, std::vector<std::string> hFiles);
	void DoAnalysis(std::string clientID, int argc, char* argv[]);
	std::string repository;
	std::string repoArchive;
	std::string addPropertyToMessage(std::string message, std::string v1, std::string v2);
	std::string getMsgAttribute(std::string msg, std::string attribute);
	void sendMessageToClient(int msgCategory, std::string bareMessage, std::string destPort);
	std::string getCatListOnserver();//returns category list seperated by ':'
	std::string getfileListForCategory(std::string cat);

public:
	std::string getRepository();
	std::string getRepositoryArchive();
	void setRepository(std::string repoPath);
	Publisher_CommHandler& getCommHandler();
	void processMsgFromChannel(Message msg, int argc, char* argv[]);
	void setArguments(int argc, char* argv[]);
	void classLoaded(int argc, char* argv[]);
	Exec();
	~Exec();
};
