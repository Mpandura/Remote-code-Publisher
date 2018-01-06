#pragma warning( disable : 4267 )

///////////////////////////////////////////////////////////////////////
// Pub_Exec.cpp - small, generally useful, helper classes             //
// ver 1.4                                                           //
// Language:    C++, Visual Studio 2015                              //
// Platform:    Dell XPS 8900, Windows 10                            //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Manjunath Panduranga                                 //
//              Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include "Pub_Exec.h"
#include <fstream>
#include <sstream>
#include <string>
#include"../Analyzer/publisherExecutive.h"
#include"../HttpMessage/HttpMessage.h"
#include"depen.h"


std::string m;
std::string k ;
std::string Publish_Exec::attribute_msg(std::string msgBody, std::string attribute)
{
	int sI = (msgBody.find("<" + attribute + ">") + (attribute.length() + 2));
	int eI = msgBody.find("</" + attribute + ">");
	return msgBody.substr(sI, eI - sI);
}
std::string Publish_Exec::msgproperty(std::string s, std::string msg1, std::string msg2)
{
	return s += "<" + msg1 + ">" + msg2 + "</" + msg1 + ">";	
}  
std::string Publish_Exec::list_cat()
{
	std::string stng = "";
	std::vector<std::string> dir = FileSystem::Directory::getDirectories(this->repo);
	for each (auto m in dir)
	{
		if (m == "." || m == ".."|| m=="htmlFolder")continue;
		stng += ":" + m;
	}
	return stng;
}
void Publish_Exec::client_msg(int mm, std::string bm, std::string dpor)
{
	Message message;
	switch (mm)
	{
	case 1:
		message = msgproperty(message, "toPort", dpor);
		message = msgproperty(message, "fromPort", "8082");
		message = msgproperty(message, "command", "5");
		message = msgproperty(message, "MSGTYPE", "text"); 
		this->shim_get().a4msg(message);
		break;
	case 2: 
		message = msgproperty(message, "toPort", dpor);
		message = msgproperty(message, "fromPort", "8082");
		message = msgproperty(message, "command", "6");
		message = msgproperty(message, "catg", list_cat());
		message = msgproperty(message, "MSGTYPE", "text"); // 
		this->shim_get().a4msg(message);
		break;
	default:
		break;
	}
}
std::string Publish_Exec::Repos()
{
	return repo;
}
std::string Publish_Exec::listingforCat(std::string category)
{
	std::vector<std::string> fl = FileSystem::Directory::getFiles(this->repo + "\\" + category); return "";
}
void Publish_Exec::Repo_p4(std::string Rpath)
{
	repo = FileSystem::Path::getFullFileSpec(Rpath);
	repoArchive = repo ;
}
Publish_Exec::~Publish_Exec()
{
}
Handler_Pub& Publish_Exec::shim_get()
{
	return this->commHandler_shim;
}
std::string Publish_Exec::RepoArch()
{
	return repoArchive;
}
void Publish_Exec::frmChannel(Message m, int argc, char* argv[])
{
	std::string mCom = attribute_msg(m, "command");
	std::string clientID = attribute_msg(m, "clntID");
	std::string sPort = attribute_msg(m, "fromPort");
	std::string dPort = attribute_msg(m, "toPort");
	switch (std::stoi(mCom))
	{
	case 1:
		project3_display(clientID, argc, argv); 
		this->client_msg(1, m, sPort); 
		break;
	case 2:
		this->client_msg(6, m, sPort);
		break;
	default:
		break;
	}
}
void Publish_Exec::countr(int argc, char* argv[])
{
	this->commHandler_shim = Handler_Pub(8082, argv[1]);
	rcvThread = std::thread(
		[this, argc, argv] {
		while (true)
		{
			Message msg = shim_get().gt_msg();
			frmChannel(msg, argc, argv);
		}
	});
}
Publish_Exec::Publish_Exec()
{

}
void Publish_Exec::argset(int argc, char* argv[])
{
	this->Repo_p4(argv[1]);
}
void Publish_Exec::project3_display(std::string clientID, int argc, char* argv[]) //acts like the main of publisherExecutive
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
int main(int argc, char* argv[])
{
	Publish_Exec av;
	int a = 5;	
	av.argset(argc, argv);
	av.countr(argc, argv); 
	while (true)
	{	}
	return 0;
}