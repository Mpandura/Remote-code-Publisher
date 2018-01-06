#pragma once
//#pragma warning( disable : LNK4217 )
#include "../MockChannel/MockChannel.h"
#include "../FileSystem/FileSystem.h"
/////////////////////////////////////////////////////////////////////////
// Handler.h                                                           //
// Author : Manjunath Panduranga                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////

class Handler_Pub
{
public:
	using Message = std::string;
	void a4msg(Message msg);
	Message gt_msg();
	~Handler_Pub();
	Handler_Pub(int Rpor, std::string Downld);
	Handler_Pub();
	
private:
	void FolderCreation(std::string s);
	ISendr* Pub_sender;
	IRecvr* Pub_recv;
	IMockChannel* pMockChan;
	
};

