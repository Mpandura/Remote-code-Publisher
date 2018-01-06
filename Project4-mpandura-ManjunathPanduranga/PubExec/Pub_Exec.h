#pragma once
///////////////////////////////////////////////////////////////////////
// Pub_Exec.h - small, generally useful, helper classes              //
// ver 1.4                                                           //
// Language:    C++, Visual Studio 2015                              //
// Platform:    Dell XPS 8900, Windows 10                            //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Manjunath Panduranga                                 //
//              Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
#include "../Handler/Handler.h"
#include "../HttpMessage/HttpMessage.h"
#include <thread>
#include <iostream>
#include <string>
#include <time.h>
#include<vector>
#include<ctime>
#include<exception>
#include"../Publisher/Publisher.h"
#include"../PubExec/depen.h"
using namespace Logging;
using namespace CodeAnalysis;
class Publish_Exec 
{
	using File = std::string;
	using Files = std::vector<File>;
	using depen = std::unordered_map<std::string, std::set<std::string>>;
	using NewMap = std::unordered_map<std::string, Type_Table*>;

public:
	std::string Repos();
	void Repo_p4(std::string repo);
	void countr(int argc, char* argv[]);
	Handler_Pub& shim_get();
	void frmChannel(Message msg, int argc, char* argv[]);
	void argset(int argc, char* argv[]);
	Publish_Exec();
	std::string RepoArch();
	~Publish_Exec();
private:
	NewMap map_;
	depen maps;
	std::thread processThread;
	std::string repoArchive;
	Handler_Pub commHandler_shim;
	std::thread rcvThread;
	void project3_display(std::string clnt, int argc, char* argv[]);
	std::string repo;
	std::string attribute_msg(std::string msg, std::string a);
	void client_msg(int msg, std::string bm, std::string dpor);
	std::string list_cat();
	std::string listingforCat(std::string cat);
	std::string msgproperty(std::string message, std::string str1, std::string str2);
};

