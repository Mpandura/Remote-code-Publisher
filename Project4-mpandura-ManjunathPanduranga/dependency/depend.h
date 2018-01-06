#pragma once
#pragma once
/////////////////////////////////////////////////////////////////////////
// DepAnal.h - Demonstrate how to start developing dependency analyzer //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //
/////////////////////////////////////////////////////////////////////////
/*
* You need to provide all the manual and maintenance informnation
*/

#include <iostream>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <set>
#include "../Parser/ActionsAndRules.h"
#include "../FileSystem/FileSystem.h"
//#include "../typetable/typetable.h"
#include"../Analyzer/TypeTable.h"
#include "../Publisher/Publisher.h"
#pragma warning (disable : 4101)  // disable warning re unused variable x, below


namespace CodeAnalysis
{
	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;
		using fileset = std::set<std::string>;
		using NewMap = std::unordered_map<std::string, Type_Table*>;

		std::string k;
		std::unordered_map<std::string, std::set<std::string>> returnmap();
		void ShowFunction_DA();
		void ShowFunction_DA1();
		std::set<std::string> GetFile();
		using depen = std::unordered_map<std::string, std::set<std::string>>;
		void Dependency(std::string filespec);
		void Table(ASTNode *pNode, std::string ns);
		TypeAnal();
		void doTypeAnal();
		void TypeAnal::subset(ASTNode* pNode);
		TypeTable& GetTypeTable()
		{
			return typetable;
		}


	private:
		void DFS(ASTNode* pNode);
		NewMap map_;
		depen maps;
		fileset getfile;
		TypeTable typetable;
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
	};




	inline std::unordered_map<std::string, std::set<std::string>> TypeAnal::returnmap()
	{
		return maps;
	}

	inline void TypeAnal::Table(ASTNode * pNode, std::string s)
	{
		Type_Table *element1 = new Type_Table;
		element1->type_ = pNode->type_;
		element1->file_ = pNode->path_;
		element1->name_ = s;
		typetable.elements(pNode->name_, element1);
	}

	inline void TypeAnal::Dependency(std::string s)
	{
		try
		{
			map_ = typetable.getmap();
			std::ifstream in(s);
			if (!in.good())
			{
				std::cout << "\n  could not open " << s << "\n\n";

			}
			{
				toker_.returnComments();
				toker_.attach(&in);
				do
				{
					Type_Table* element2;
					std::string tok = toker_.getTok();
					if (tok == "\n")
						tok = "newline";
					if (map_.find(tok) != map_.end())
					{
						element2 = map_[tok];
						maps[s].insert(element2->file_);
					}
				} while (in.good());
			}
		}
		catch (std::logic_error& ex)
		{
			std::cout << "\n  " << ex.what();
		}
	}

	inline std::set<std::string> TypeAnal::GetFile()
	{
		return getfile;
	}


	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
		std::function<void()> test = [] { int x; };
	}

	inline bool doDisplay(ASTNode* pNode)
	{
		static std::string toDisplay[] = {
			"function", "lambda", "class", "struct", "enum", "alias", "typedef" , "namespace"
		};
		for (std::string type_ : toDisplay)
		{
			if (pNode->type_ == type_)
				return true;
		}
		return false;
	}


	inline void TypeAnal::ShowFunction_DA()
	{

		CodePublisher cp;
		std::ofstream myfile1;
		std::ofstream myfile2;


		for (auto j = maps.begin(); j != maps.end(); j++)
		{

			std::string getcontent;
			k = j->first;

			cp.showHomePage(k);  // display of HTML


								 // HTML design and specifications
			myfile2.open(k + ".html");
			myfile2 << "<!DOCTYPE html><html>";
			myfile2 << "	<head>" << "\n" << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">" << "\n" << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">" << "\n" << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js\"></script>" << "\n" << "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>" << "\n" << "<style>.collapsing{background-color: yellow;}body{margin: 20px;color: black;background-color: #f4f2c6;font-family: Consolas;font - weight: 600;font - size: 90 % ;}.indent{margin - left: 20px;margin - right: 20px;}h4{margin - bottom: 3px;margin - top: 3px;}</style></head>";
			myfile2 << "	<h3>" << k << "</h3>";


			for (auto j : j->second)
			{
				// HTML Depencencies
				myfile2 << "<div class = \"indent\">";
				myfile2 << "<hr /><h4>Dependencies:</h4>";


				myfile2 << "<a href =\"" << j << ".html\">" << j << "</a>" << std::endl;

				myfile2 << "</div><hr />";
			}

			myfile2 << "<body><pre>";

			std::ifstream is(k);

			if (is.is_open())
			{

				char str[255];
				//cp.Replacement(k);
				char c;
				std::stringstream jd;
				//cp.Javascript(k);
				int v1, v2;
				int count = 0;
				int counter = 50;

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

				myfile2 << jd.str();

			}
			myfile2 << "</pre>";

			std::ifstream openfile1(j->first);
			if (openfile1.is_open())
			{
				char str[255];

				while (openfile1) {
					openfile1.getline(str, 255);  // delim defaults to '\n'
					if (openfile1) myfile1 << str << std::endl;
				}
			}

			myfile2 << "</body></html>";
			myfile2.close();
			myfile2.clear();
		}

		myfile1 << "</pre></body></html>";
		myfile1.close();
	}



	// Home page of HTML display
	inline void TypeAnal::ShowFunction_DA1()
	{

		std::ofstream myfile1;
		myfile1.open("../HtmlFiles\\HomeFile.html");
		myfile1 << "<!DOCTYPE html><html>";
		myfile1 << "	<head>" << "\n" << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">" << "\n" << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">" << "\n" << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js\"></script>" << "\n" << "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>" << "\n" << "<style>.collapsing{background-color: yellow;}body{margin: 20px;color: black;background-color: yellow;font-family: Consolas;font - weight: 600;font - size: 90 % ;}.indent{margin - left: 20px;margin - right: 20px;}h4{margin - bottom: 3px;margin - top: 3px;}</style></head>";


		myfile1 << "	<h3>" << "Home page:  Analysing Files" << "</h3>";
		myfile1 << "<body><pre>";
		CodePublisher cp;
		cp.showHomePage("../HtmlFiles\\HomeFile");
		for (auto j = maps.begin(); j != maps.end(); j++)
		{
			myfile1 << "<hr /><a href =\"" << j->first << ".html\">" << j->first << "</a>" << "	has dependency on	" << std::endl;
			for (auto j : j->second)
			{
				std::cout << j << std::endl;
				myfile1 << "<div class = \"indent\">";      // dependencies
				myfile1 << "<h4>Dependencies:</h4>";
				myfile1 << "<a href =\"" << j << ".html\">" << j << "</a>" << std::endl;
				myfile1 << "</div>";
			}
			myfile1 << "<hr />";
		}
		myfile1 << "</pre></body></html>";
		myfile1.close();

	}


	inline void TypeAnal::DFS(ASTNode* pNode)
	{
		static std::string path = "";
		static std::string ns = "";
		if (pNode->path_ != path)
		{
			std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			path = pNode->path_;
		}
		if (pNode->type_ == "namespace") {
			ns = pNode->name_;
		}
		if (doDisplay(pNode))
		{
			std::cout << "\n  " << pNode->name_;
			std::cout << ", " << pNode->type_;
			Table(pNode, ns);
		}
		for (auto pChild : pNode->children_)
			DFS(pChild);
	}


	inline void TypeAnal::doTypeAnal()
	{
		std::cout << "\n  starting type analysis:\n";
		ASTNode* pRoot = ASTref_.root();
		DFS(pRoot);
		subset(pRoot);
	}

	inline void TypeAnal::subset(ASTNode * pNode)
	{
		static std::string path = "";
		if (pNode->path_ != path)
		{
			getfile.insert(pNode->path_);
		}
		for (auto pChild : pNode->children_)
			subset(pChild);
	}
}