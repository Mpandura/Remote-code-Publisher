#pragma once
#include "Handler.h"
/////////////////////////////////////////////////////////////////////////
// Handler.cpp                                                         //
// Author : Manjunath Panduranga                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////


void Handler_Pub::a4msg(Message m)
{
	Pub_sender->postMessage(m);
}

Handler_Pub::Handler_Pub(int rpor, std::string Dfolder)
{
	FolderCreation(FileSystem::Path::getFullFileSpec(Dfolder));
	ObjectFactory of;
	Pub_sender = of.createSendr();
	Pub_recv = of.createRecvr();
	pMockChan = of.createMockChannel(Pub_sender, Pub_recv, rpor, Dfolder);
	pMockChan->start();
}

Handler_Pub::Handler_Pub()
{
}

Message Handler_Pub::gt_msg()
{
	return Pub_recv->getMessage();
}


void Handler_Pub::FolderCreation(std::string dPath)
{
	size_t siz = dPath.find('\\');
	while (siz != std::string::npos)
	{
		if (!FileSystem::Directory::exists(dPath.substr(0, siz)))
			FileSystem::Directory::create(dPath.substr(0, siz));
		siz = dPath.find('\\', siz + 1);
	}
	if (dPath[dPath.size() - 1] != '\\')
	{
		if (!FileSystem::Directory::exists(dPath))
			FileSystem::Directory::create(dPath);
	}
}

Handler_Pub::~Handler_Pub()
{
}
#ifdef TEST_PUBHANDLER
int main()
{
	return 0;
}
#endif


