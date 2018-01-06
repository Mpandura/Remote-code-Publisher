/////////////////////////////////////////////////////////////////////
// TypeTable.cpp                                                   //
// ver 1.0                                                         //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Platform:    Lenovo, Windows 10                                 //
// Application: Project #2, CSE687 - Object Oriented Design, S2015 //
// Author:		Manjunath Panduranga               				   //
//		       					                                   //
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TypeTable.h"

using Name = std::string;
using DefineMap = std::unordered_map<Name, Type_Table*>;
using Rslt = Logging::StaticLogger<0>;
void TypeTable::elements(std::string s, Type_Table *ele)
{
	map.insert(make_pair(s, ele));
}

DefineMap& TypeTable::getmap()
{
	return map;
}

void TypeTable::show()
{

	std::ostringstream out;
	out << "\n ";
	out << std::right;
	out << std::setw(15) << "-------------------------------------------------------------------------------------------------------------------------------";
	out << "\n ";
	
	out << std::setw(3) << "|";
	out << std::setw(15) << "Element key";
	out << std::setw(3) << "|";
	out << std::setw(15) << "type name";
	out << std::setw(3) << "|";
	out << std::setw(19) << "namespace";
	out << std::setw(3) << "      |";
	out << std::setw(35) << "filename";
	out << std::right;
	out << "\n ";
	out << std::setw(15) << "-------------------------------------------------------------------------------------------------------------------------------";
	out << std::right;
	out << "\n ";
	Rslt::write(out.str());
	for (auto it = map.begin(); it != map.end(); ++it)
	{
		if (it->first == "")
		{
			std::cout << "No table found";
		}
		std::ostringstream out;

		out << std::setw(2) << "|";
		out << std::setw(17) << it->first;
		out << std::setw(3) << "|";
		out << std::setw(13) << it->second->type_;
		out << std::setw(3) << "|";
		out << std::setw(23) << it->second->name_ << "   ";
		out << std::setw(3) << "|   ";
		out << std::setw(35) << it->second->file_;
		
		out << "\n ";
		Rslt::write(out.str());	
	}
}
#ifdef TEST_TYPETABLE
int main()
{

	TypeTable table;
	table.show();

}
#endif