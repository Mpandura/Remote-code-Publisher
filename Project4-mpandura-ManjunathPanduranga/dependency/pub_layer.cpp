#pragma warning( disable : 4267 )
#include "pub_layer.h"
#include <fstream>
#include <sstream>
#include <string>
//#include <vector>
//#include <map>
//#include <unordered_map>
//#include <functional>
//#include <algorithm>
//#include <exception>
//#include <iomanip>
//#include <chrono>
//#include <ctime>
//
//
//
//#include"../Analyzer/TypeTable.h"
//
//#include"../Publisher/Publisher.h"
//#include "../Parser/Parser.h"
//#include "../FileSystem/FileSystem.h"
//#include "../FileMgr/FileMgr.h"
//#include "../Parser/ActionsAndRules.h"
//#include "../Parser/ConfigureParser.h"
//#include "../AbstractSyntaxTree/AbstrSynTree.h"
//#include "../Logger/Logger.h"
//#include "../Utilities/Utilities.h"
//#include"../NoSqlDb/NoSqlDb.h"
//#include"../Analyzer/DepAnal.h"
#include"../Analyzer/publisherExecutive.h"
#include"../HttpMessage/HttpMessage.h"
#include"depend.h"


std::string m;
std::string k;
void Exec::DoAnalysis(std::string clientID, int argc, char* argv[]) //acts like the main of publisherExecutive
{
	argv[0];


	using namespace CodeAnalysis;
	CodeAnalysisExecutive exec;

	try {
		bool succeeded = exec.ProcessCommandLine(argc, argv);
		TypeTable Tt;
		TypeAnal ta;
		if (!succeeded)
		{
			return;
		}
		exec.setDisplayModes();
		exec.startLogger(std::cout);

		std::ostringstream tOut("CodeAnalysis - Version 1.4");
		Utils::sTitle(tOut.str(), 3, 92, tOut, '=');
		Rslt::write(tOut.str());
		Rslt::write("\n     " + exec.systemTime());
		Rslt::flush();
		exec.showCommandLineArguments(argc, argv);
		Rslt::write("\n");

		exec.getSourceFiles();
		exec.processSourceCode(true);
		exec.complexityAnalysis();
		exec.dispatchOptionalDisplays();
		exec.flushLogger();
		exec.displayMetricSummary(50, 10);

		exec.flushLogger();
		Rslt::write("\n");





		ta.doTypeAnal();
		Tt = ta.GetTypeTable();



		Tt.show();
		std::set<std::string> setfiles = ta.GetFile();
		for (std::string it : setfiles) { ta.Dependency(it); }


		std::cout << "\n\n\n\n ---------------<   Dependency Analysis  >-----------------\n" << std::endl;

		ta.ShowFunction_DA();
		ta.ShowFunction_DA1();
	}

	catch (std::exception& except)
	{
		exec.flushLogger();
		std::cout << "\n\n  caught exception in Executive::main: " + std::string(except.what()) + "\n\n";
		exec.stopLogger();
		return;
	}

}




std::string Exec::addPropertyToMessage(std::string message, std::string v1, std::string v2)
{
	return message += "<" + v1 + ">" + v2 + "</" + v1 + ">";

}
std::string Exec::getMsgAttribute(std::string msgBody, std::string attribute)
{
	int sI = (msgBody.find("<" + attribute + ">") + (attribute.length() + 2));
	int eI = msgBody.find("</" + attribute + ">");
	return msgBody.substr(sI, eI - sI);
}
//sends message to client
//msgMeaning:
//   1= analysis done
//    
void Exec::sendMessageToClient(int msgMeaning, std::string bareMessage, std::string destPort)
{
	Message msg;
	switch (msgMeaning)
	{
	case 1: // telling the cleint that the analysis is done
		msg = addPropertyToMessage(msg, "toPort", destPort);
		msg = addPropertyToMessage(msg, "fromPort", "8082");
		msg = addPropertyToMessage(msg, "command", "5");//analyzed command
		msg = addPropertyToMessage(msg, "MSGTYPE", "text"); // 
		this->getCommHandler().P_PostMessage(msg);
		break;
	case 6: // sending the catgegory list
		msg = addPropertyToMessage(msg, "toPort", destPort);
		msg = addPropertyToMessage(msg, "fromPort", "8082");
		msg = addPropertyToMessage(msg, "command", "6");//to update category list
		msg = addPropertyToMessage(msg, "catg", getCatListOnserver());
		msg = addPropertyToMessage(msg, "MSGTYPE", "text"); // 
		this->getCommHandler().P_PostMessage(msg);
		break;
	default:
		break;
	}
}

std::string Exec::getCatListOnserver()
{
	std::vector<std::string> catDirectories = FileSystem::Directory::getDirectories(this->repository);
	std::string catString = "";
	for each (auto cat in catDirectories)
	{
		if (cat == "." || cat == ".." || cat == "htmlFolder")
			continue;
		catString += ":" + cat;
	}
	return catString;
}
std::string Exec::getfileListForCategory(std::string cat)
{
	std::vector<std::string> files = FileSystem::Directory::getFiles(this->repository + "\\" + cat);
	return "";
}





void Exec::setArguments(int argc, char* argv[])
{
	this->setRepository(argv[1]);
}



std::string Exec::getRepository()
{
	return repository;
}

std::string Exec::getRepositoryArchive()
{
	return repoArchive;
}

void Exec::setRepository(std::string repoPath)
{
	repository = FileSystem::Path::getFullFileSpec(repoPath);
	repoArchive = repository;

}

Publisher_CommHandler& Exec::getCommHandler()
{

	return this->commHandler_shim;
}
// message recieved in string format
void Exec::processMsgFromChannel(Message msg, int argc, char* argv[])
{

	//check if the message is a command and process that command
	// also decide what happens if a file comes here
	std::string msgCommand = getMsgAttribute(msg, "command"); // example:  command=1, command=2
	std::string clientID = getMsgAttribute(msg, "clntID");
	std::string sPort = getMsgAttribute(msg, "fromPort");
	std::string dPort = getMsgAttribute(msg, "toPort");
	switch (std::stoi(msgCommand))
	{
	case 1://analyze files
		DoAnalysis(clientID, argc, argv); // happens synchronous from here
										  //tell the client that code analysis is done
		this->sendMessageToClient(1, msg, sPort); //source of the message is the destination for acknowledgement

		break;
	case 2:// send category list
		   //send category list
		this->sendMessageToClient(6, msg, sPort); //source of the message is the destination for acknowledgement
		break;
	case 3:// 
		break;
	default: //
		break;
	}
}

void Exec::classLoaded(int argc, char* argv[]) //window_loaded counterpart
{
	this->commHandler_shim = Publisher_CommHandler(8082, argv[1]);
	rcvThread = std::thread(
		[this, argc, argv] {
		while (true)
		{
			Message rcvdMsg = getCommHandler().P_GetMessage(); // only text messages come here
			processMsgFromChannel(rcvdMsg, argc, argv);
		}
	});
}

Exec::Exec()
{

}

Exec::~Exec()
{
}

int main(int argc, char* argv[])
{
	Exec exec;
	exec.setArguments(argc, argv);
	exec.classLoaded(argc, argv); //like calling windowLoaded function


	int a = 5;
	while (true)
	{

	}
	return 0;
}