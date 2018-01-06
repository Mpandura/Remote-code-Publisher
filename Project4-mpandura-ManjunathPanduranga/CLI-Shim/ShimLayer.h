#pragma once
///////////////////////////////////////////////////////////////////////
// CLIShim.h - C++\CLI layer is a proxy for calls into MockChannel   //
// author: Manjunath Panduranga                                      //                          
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017         //
///////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4075 )
#include "../MockChannel/MockChannel.h"
#include <string>
using namespace System;

public ref class Shim
{
public:
  using Message = String;
  Shim(int Rpor,Message^ Dfolder);
  void PostMessage(Message^ msg);
  String^ GetMessage();
  String^ Tosys(const std::string& s);
  std::string FromSYS(String^ s);
  ~Shim()
  {
  }
private:
  IMockChannel* MockChn_Clin;
  ISendr* Sendin_Cli;
  IRecvr* Recv_Cli;
 
};


