///////////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015                         //
// - build as static library showing how C++\CLI client can use native code channel  //
// - MockChannel reads from sendQ and writes to recvQ                                //
//                                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015                         //
///////////////////////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4267 )
#pragma warning( disable : 4075 )
#pragma warning( disable : 4221 )
#define IN_DLL
#include "MockChannel.h"
#include "../MsgClient/MsgClient.h"
#include "../MsgServer/MsgServer.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>



using BQueue = BlockingQueue < Message >;
using namespace Async;
/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const Message& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
  sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
  Message getMessage();
  BQueue& queue();
  Recvr();
  Recvr(int rcvPort);
private:
  BQueue recvQ_;
  int rcvPort_;
};

Message Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}
Recvr::Recvr()
{
}
Recvr::Recvr(int rcvPort)
{
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr,int recvAddr,std::string downLoadFolder);
  void start();
  void stop();
private:
	std::string downloadFolder;
	int  rcvPort;
  std::thread sendThread_;
  std::thread recieveThred_;
  void start_sending();
  void start_Receiver();
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
  std::string getMsgAttribute(std::string msg, std::string attribute);
};



void MockChannel::start_sending()
{
	std::cout << "\n  mockchannel sender starting up\n";
	sendThread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start\n\n";
			return;
		}
		BQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		while (!stop_)
		{
			std::cout << "\n  channel deQing message";
			Message msg = sendQ.deQ();  // will block here so send quit message when stopping
			std::string msgType = getMsgAttribute(msg, "MSGTYPE");		
			std::string sPort = getMsgAttribute(msg, "fromPort");
			std::string dPort= getMsgAttribute(msg, "toPort");
			std::string clntID= getMsgAttribute(msg, "clntID");
			MsgClient cl;
			cl.ID_msgClient(clntID);
			cl.Point_end(sPort);
			std::cout <<"Starting HttpMessage client" + clntID +" on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id());
			try
			{
				SocketSystem ss;
				SocketConnecter si;
				if (msgType == "text")
				{
					while (!si.connect("localhost", std::stoi(dPort))) //this(2nd parameter) is where  the message seems to go
					{
						std::cout << "\n client waiting to connect";
						::Sleep(100);
					}
					cl.Channel_text(msgType, msg, si);
				}
				if (msgType == "fPth") 
				{
					while (!si.connect("localhost", std::stoi(dPort))) //this(2nd parameter) is where  the message seems to go
					{
						std::cout << "\n client waiting to connect";
						::Sleep(100);
					}
					cl.Message_client(msg, si);
				}
			}
			catch (const std::exception& a)
			{
				Show::write("\n  Exeception: ");
				std::string aa = "\n  " + std::string(a.what()) + "\n\n";Show::write(aa);
			}
		}
		std::cout << "\n  mockchannel sender stopping\n";
	});

}

//----< pass pointers to Sender and Receiver >-------------------------------
MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr, int recieve, std::string a) : pISendr_(pSendr), pIRecvr_(pRecvr)
{
	this->rcvPort = recieve;
	this->downloadFolder = a;
}


void MockChannel::start_Receiver()
{
	std::cout << "\n  mockchannel reciever starting up\n";
	std::string dFolder = this->downloadFolder;
	recieveThred_ = std::thread(
		[this,dFolder] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}
		BlockingQueue<HttpMessage> msgQ;
		BQueue& Recevir = pRecvr->queue();
		try
		{
			SocketSystem ss; SocketListener sl(rcvPort, Socket::IP6);
			MsgServer Ms(msgQ);
			Ms.File_Downloader(dFolder);sl.start(Ms);
			while (true)
			{
				HttpMessage msg = msgQ.deQ();
				std::cout << "\n\n  Message has been received from server side :\n" + msg.bodyString() + "\n";
				Recevir.enQ(msg.bodyString());
			}
		}
		catch (std::exception& b)
		{
			std::cout << "\n  Exeception: ";
			std::string bb = "\n  " + std::string(b.what()) + "\n\n"; std::cout << bb;
		}
	});

}

std::string MockChannel::getMsgAttribute(std::string msg, std::string attribute)
{
	int sI = (msg.find("<" + attribute + ">") + (attribute.length() + 2));
	int eI = msg.find("</" + attribute + ">");
	return msg.substr(sI, eI - sI);
}
//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	std::cout<<  "\n  MockChannel starting up\n";
	this->start_sending();
	this->start_Receiver();
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr,int rcvPort, std::string downloadFolder) 
{ 
  return new MockChannel(pISendr, pIRecvr,rcvPort,downloadFolder); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
