#pragma once
/////////////////////////////////////////////////////////////////////
// TypeTable.h                                                     //
// ver 1.0                                                         //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Platform:    Lenovo, Windows 10                                 //
// Application: Project #2, CSE687 - Object Oriented Design, S2015 //
// Author:		Manjunath Panduranga               				   //
//		       					                                   //
/////////////////////////////////////////////////////////////////////
#ifndef TYPE_TABLE
#define TYPE_TABLE

#include<iostream>
#include<stack>
#include "../Parser/ConfigureParser.h"
#include <set>
#include "../Parser/ActionsAndRules.h"
using namespace CodeAnalysis;

struct Type_Table
{
	using FileName = std::string;
	FileName file_;
	using Namespace = std::string;
	Namespace name_;
	using TypeName = std::string;
	TypeName type_;
};

class TypeTable
{
public:
	using Name = std::string;
	using DefineMap = std::unordered_map<Name, Type_Table*>;
	void elements(std::string name, Type_Table *element);
	DefineMap& getmap();
	void show();
	
private:
	DefineMap map;
};
#endif


