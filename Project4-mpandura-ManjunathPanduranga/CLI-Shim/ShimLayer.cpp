/////////////////////////////////////////////////////////////////////////
// CLIShim.cpp - C++\CLI layer is a proxy for calls into MockChannel   //
//                                                                     //
// author : Manjunath Panduranaga                                      //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //
/////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4075 )
#include "ShimLayer.h"
#include <iostream>

String^ Shim::Tosys(const std::string& str)
{
  return gcnew String(str.c_str());
}

void Shim::PostMessage(Message^ msg)
{
	Sendin_Cli->postMessage(FromSYS(msg));
}

Shim::Shim(int Rpor, Message^ Dfolder)
{
  ObjectFactory Of;
  Sendin_Cli = Of.createSendr();
  Recv_Cli = Of.createRecvr();
  MockChn_Clin = Of.createMockChannel(Sendin_Cli, Recv_Cli, Rpor,FromSYS( Dfolder));
  MockChn_Clin->start();
}

std::string Shim::FromSYS(String^ str)
{
	std::string s;
	for (int m = 0; m < str->Length; ++m)
		s += char(str[m]); return s;
}



String^ Shim::GetMessage()
{
  std::string msg = Recv_Cli->getMessage();
  return Tosys(msg);
}

#ifdef TEST_CLISHIM

int main()
{
  String^ ss = "Hello from sender";
  Shim^ Clin = gcnew Shim(8080,"downloads");
  std::cout << "\n  Msg is Passing \"" << Clin->FromSYS(ss) << "\"";
  Clin->PostMessage(ss);
  String^ pReply = Clin->GetMessage();
  std::cout << "\n  Msg has been Received \"" << Clin->FromSYS(pReply) << "\"";std::cout << "\n\n";
}
#endif
