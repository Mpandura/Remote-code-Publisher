///////////////////////////////////////////////////////////////////////////
// Publisher.cpp - Demonstrate  creation of web pages                   //
//                                                                     //
// Manjunath , CSE687 - Object Oriented Design, Spring 2017           //
///////////////////////////////////////////////////////////////////////
/*
* You need to provide all the manual and maintenance informnation
*/

#include "Publisher.h"


//------------------Creation of Html Page-----------------//
void CodeAnalysis::CodePublisher::getHtml()
{
	std::ofstream myfile1;
	myfile1.open("C:\\Users\\Manjunath Gowda\\CodeAnalyzer (1)\\Publisher\\Parser\\man.html");
	myfile1 << "<!DOCTYPE html><html><head></head><body><pre>";
}


//-----------------automatic display of the Home Page-----------------//
void CodeAnalysis::CodePublisher::showHomePage(std::string k)
{
	std::ofstream as(k + ".html");
	std::stringstream jd;
	as << jd.str();
	as.close();
	std::string homeLink = k + ".html";
	std::wstring wHomwLink = std::wstring(homeLink.begin(), homeLink.end());
	LPCWSTR sw = wHomwLink.c_str();
	ShellExecute(0, 0, sw, 0, 0, SW_SHOW);
}


//---------------This is the required Html format with specifications-----------//
void CodeAnalysis::CodePublisher::openingHtml(std::string k)
{
	std::ofstream myfile2;
	myfile2.open(k + ".html");
	myfile2 << "<!DOCTYPE html><html>";
	myfile2 << "	<head>" << "\n" << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">" << "\n" << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">" << "\n" << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js\"></script>" << "\n" << "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>" << "\n" << "<style>body{margin: 20px;color: black;background - color: #eee;font - family: Consolas;font - weight: 600;font - size: 90 % ;}.indent{margin - left: 20px;margin - right: 20px;}h4{margin - bottom: 3px;margin - top: 3px;}</style></head>";

	myfile2 << "<body><pre>";
}



//----------------------Any markup characters in the code text will have to be escaped, e.g., replace < with &lt; and > with &gt;.------------------//
void CodeAnalysis::CodePublisher::Replacement(std::string k)
{
	
	std::ifstream is(k);
	std::stringstream jd;
	char c;
	while (is.get(c))          // loop getting single characters
	{
		switch (c)
		{
		case '<':
			jd << "&lt";
			break;
		case '>':
			jd << "&gt";
			break;

		case '–':
			jd << "&ndash";
			break;
		case'—':
			jd << "&mdash";
			break;
		default:
			jd << c;
			break;
		}

	}
	is.close();
	std::ofstream os(k+".html");
	os << jd.str();
	os.close();
	jd.clear();
}


//-------------optionally2 provide the facility to expand or collapse class bodies, methods, and global functions using JavaScript and CSS properties.-------------//

void CodeAnalysis::CodePublisher::Javascript(std::string k)
{
	int v1, v2;
	int count = 0;
	int counter = 50;
	char c;
	std::ifstream is(k);
	std::stringstream jd;
	while (is.get(c))          // loop getting single characters
	{
		switch (c)
		{

		case'{':
			v1 = rand() % 100;
			v2 = rand() % 30 + 1985;
			counter++;
			count++;
			jd << "<div" << v1 << " class=\"container\">" << "\n" << "<button type=\"button\" class = \"btn btn-info\" data-toggle=\"collapse\" data-target =\"#demo" << v1 << "\">+</button>" << "\n" << "<div" << v2 << " id=\"demo" << v1 << "\" class=\"collapse\">" << c;

			break;
		case'}':
			jd << c << "</div" << v1 << "></div" << v2 << ">";
			counter--;
			count--;

			break;
		default:
			jd << c;

			break;
		}
		
	}
	is.close();
	std::ofstream jdd(k+".html");
	jdd << jd.str();
	jdd.close();
	jd.clear();

}


void CodeAnalysis::CodePublisher::closingbody(std::ofstream myfile2)
{
	
	myfile2 << "</pre>";
}



#ifdef TEST_PUB

int main()
{

}
#endif