///////////////////////////////////////////////////////////////////////////
// Publisher.h - Demonstrate  creation of web pages                     //
//                                                                     //
// Manjunath , CSE687 - Object Oriented Design, Spring 2017           //
///////////////////////////////////////////////////////////////////////
/*
* You need to provide all the manual and maintenance informnation
*/



#pragma once
#include <iostream>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <set>
#include "../Parser/ActionsAndRules.h"
#include "../FileSystem/FileSystem.h"

namespace CodeAnalysis
{
	class CodePublisher {
	public:

		//------------------Creation of Html Page-----------------//
		void getHtml();
		//-----------------automatic display of the Home Page-----------------//
		void showHomePage(std::string k);
		//---------------This is the required Html format with specifications-----------//
		void openingHtml(std::string k);
		//----------------------Any markup characters in the code text will have to be escaped, e.g., replace < with &lt; and > with &gt;.------------------//
		void Replacement(std::string k);
		//-------------optionally2 provide the facility to expand or collapse class bodies, methods, and global functions using JavaScript and CSS properties.-------------//
		void Javascript(std::string k);

		void closingbody(std::ofstream myfile2);




	};
}

