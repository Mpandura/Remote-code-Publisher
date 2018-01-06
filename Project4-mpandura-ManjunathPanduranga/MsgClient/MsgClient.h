#pragma once
/////////////////////////////////////////////////////////////////////////
// MsgClient.h   - Demonstrates simple one-way HTTP messaging          //
// Author: Manjunath Panduranga                                   m    //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/

#include <string>
#include <iostream>
#include <thread>
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"

using Show = Logging::StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
/*
class ClientCounter
{
public:
	ClientCounter() { ++clientCount; }
	size_t count() { return clientCount; }
private:
	static size_t clientCount;
};
size_t ClientCounter::clientCount = 0;
*/
/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class MsgClient
{
public:
	using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
	void Channel_text(std::string, std::string bareMessage, Socket& socket);
	void Message_client( std::string bareMessage, Socket& socket);
	void Point_end(EndPoint ep);
	void ID_msgClient(EndPoint id);
private:
	
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	EndPoint endPt;
	bool sendFile(const std::string& fqname, Socket& socket);
	std::string findMsgAttribute(std::string msg, std::string attr);
	EndPoint IDclient;
};
