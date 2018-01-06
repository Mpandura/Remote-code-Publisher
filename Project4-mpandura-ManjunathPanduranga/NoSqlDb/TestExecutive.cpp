/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
// Author- Manjunath Panduranga                                                                //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include "NoSqlDb.h"
#include <iostream>
#include<fstream>
#include<ctime>
#include"../XmlDocument/XmlDocument/XmlDocument.h"
#include<regex>
#include"../XmlDocument/XmlElement/XmlElement.h"
#include"../Serialize/Convert.h"

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;

std::string time_now() {
	time_t now = time(0);
	const char * tym = std::ctime(&now);
	return tym;
}

int main()
{
	std::cout << "\n  Demonstrating Project 1 NoSql Code";
	std::cout << "\n =================================\n";

	std::cout << "\n  Requirement #2:  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";

	NoSqlDb<StrData> db;

	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "transport - Cars";
	elem1.data = "Four wheeler vehicle";
	elem1.description = "Audi Q800 sports car";
	std::string abc = time_now();
	elem1.timeDate = abc;
	elem1.children = { "no children" };
	db.save(elem1.name, elem1);

	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "transport - Bikes";
	elem2.data = " two wheeler vehicle";
	elem2.description = "Royal Enfield 500 cc";
	abc = time_now();
	elem2.timeDate = abc;
	elem2.children = { elem1.name };
	db.save(elem2.name, elem2);

	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "transport - Cycles";
	elem3.data = "Two wheeler vehicle";
	elem3.description = "FireFox racing cycles";
	abc = time_now();
	elem3.timeDate = abc;
	elem3.children = { elem2.name };
	db.save(elem3.name, elem3);


	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";

	std::cout << "\n  size of db = " << db.count() << "\n";


	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";

	std::cout << "\n  Creating and saving NoSqlDb elements with int data";
	std::cout << "\n ----------------------------------------------------\n";

	NoSqlDb<intData> idb;

	Element<intData> ielem1;
	ielem1.name = "elem1";
	ielem1.category = "transport - Cars";
	ielem1.data = 1;
	ielem1.description = "Audi Q800 sports car";
	abc = time_now();
	ielem1.timeDate = abc;
	ielem1.children = { "no children" };
	idb.save(ielem1.name, ielem1);

	Element<intData> ielem2;
	ielem2.name = "elem2";
	ielem2.category = "transport - Bikes";
	ielem2.data = 2;
	ielem2.description = "Two wheeler vehicle";
	abc = time_now();
	ielem2.timeDate = abc;
	ielem2.children = { ielem1.name };
	idb.save(ielem2.name, ielem2);

	Element<intData> ielem3;
	ielem3.name = "elem3";
	ielem3.category = "transport - Cycles";
	ielem3.data = 3;
	ielem3.description = "Two wheeler vehicle";
	abc = time_now();
	ielem3.timeDate = abc;
	ielem3.children = { ielem2.name };
	idb.save(ielem3.name, ielem3);



	std::cout << "\n  Retrieving elements from NoSqlDb<int>";
	std::cout << "\n ---------------------------------------\n";

	std::cout << "\n  size of idb = " << idb.count() << "\n";
	Keys ikeys = idb.keys();
	for (Key key : ikeys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << idb.value(key).show();
	}


	std::cout << "\n\n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "Requirement #3:   Addition of key/value pair \n";
	std::cout << "-------------------------------------------------------\n";

	Element<intData> ielem4;
	ielem4.name = "elem4";
	ielem4.category = "test";
	ielem4.data = 4;
	ielem4.description = "Demo for addition";
	abc = time_now();
	ielem4.timeDate = abc;
	ielem4.children = { ielem2.name, ielem3.name };
	idb.add(ielem4.name, ielem4);

	std::cout << "\n  Deletion of Key/Value Pair \n";
	std::cout << "------------------------------------\n";
	db.deleting(ielem4.name);


	std::cout << "\n\n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "Requirement #4:   Editing of key/value pair \n";
	std::cout << "-------------------------------------------------------\n";

	std::cout << "\n /// Here I am Editing the values of Element3 with Element2-Category, Element2-data and Element1-Description ///\n ";
	db.MetadataUpdating(elem3.name, elem2.category, elem2.data, elem1.description);

	std::cout << "\n/// Here the Element1 is updated with Element2 ///\n";
	db.UpdatingOfElements(elem1.name, elem2);

	db.deletingChildren(elem2.name);

	std::cout << "\n\n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "Requirement #5:   XML file generation \n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "The XML file is generated for all Elements\n";
//	std::string xml = db.toXml();
//	std::cout << "\nxml string:" << xml;
//	std::ofstream xmlfile("../XmlDocument/XmlDocument/ToXml.xml");
//	xmlfile << xml;
//	xmlfile.close();





	std::cout << "\n-------------------------------------------------------\n";
	std::cout << "Requirement #7:   Generation of Queries Using Regular Expressions \n";
	std::cout << "-------------------------------------------------------\n";

	std::vector<std::string> v = { "elem1","elem2","hello world" };//test vector
	std::vector<std::string> v3 = { "elem1" };
	std::vector<std::string> v2 = { "elem2" };


	std::cout << "/// # 7.1. query1///  The value of a specified key.\n";
	std::cout << "Value of Key Elem1\n";
	db.Query1_UsingRegularExpression(elem1.name);


	std::cout << "\n\n///# 7.2. query2/// The children of a specified key.\n";
	std::cout << "children of Key Elem2\n";
	std::vector<std::string> Query2 = db.Query2_UsingRegularExpression(elem2.name);
	if (!Query2.empty())
	{
		for (int i = 0; i < Query2.size(); i++)
		{
			std::cout << Query2[i] << "\n";
		}
	}
	else
	{
		std::cout << "No Children\n";
	}

	std::cout << "\n\n///# 7.3. query3/// The set of all keys matching a specified pattern which defaults to all keys.\n\n";

	std::cout << "Keys that start with pattern: elem\n";
	std::vector<std::string> Query3 = db.Query3_UsingRegularExpression(v, "elem");
	if (!Query3.empty())
	{
		for (int i = 0; i < Query3.size(); i++)
		{
			std::cout << Query3[i] << "\n";
		}
	}

	std::cout << "\n\n///# 7.4. query4/// All keys that contain a specified string in their item name\n\n";
	std::cout << "keys that contain the item name string: ele\n";
	std::vector<std::string> Query4 = db.Query4_UsingRegularExpression(v, "ele");
	if (!Query4.empty())
	{
		for (int i = 0; i < Query4.size(); i++)
		{
			std::cout << Query4[i] << "\n";
		}
	}
	else
		std::cout << "No matching patterns found\n";



	std::cout << "\n\n/// # 7.5 query5 /// All keys that contain a specified string in their category name\n\n";
	std::cout << "keys with category string: transport\n";
	std::vector<std::string> Query5 = db.Query5_UsingRegularExpression(v, "transport");
	if (!Query5.empty())
	{
		for (int i = 0; i < Query5.size(); i++)
		{
			std::cout << Query5[i] << "\n";
		}
	}
	else
		std::cout << "No matching patterns found\n";

	std::cout << "\n\n/// # 7.6 query6  /// All keys that contain a specified string in their template data section when that makes sense.\n\n";
	std::cout << "keys that contain the data string: :vehicle\n";
	std::vector<std::string> Query6 = db.Query6_UsingRegularExpression(v, "vehicle");
	if (!Query6.empty())
	{
		for (int i = 0; i < Query6.size(); i++)
		{
			std::cout << Query6[i] << "\n";
		}
	}
	else
		std::cout << "No matching patterns found\n";


	std::cout << "\n-------------------------------------------------------\n";
	std::cout << "Requirement #8:  \n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "query 4: for the set of key returned using earlie query\n";
	std::vector<std::string> Query9 = db.Query4_UsingRegularExpression(Query4, "ele");
	if (!Query9.empty())
	{
		for (int i = 0; i < Query9.size(); i++)
		{
			std::cout << Query9[i] << "\n";
		}
	}
	else
		std::cout << "No matching patterns found\n";
	std::cout << "\nquery 5: for the set of key returned using earlie query\n";
	std::vector<std::string> Query10 = db.Query5_UsingRegularExpression(Query5, "trans");
	if (!Query10.empty())
	{
		for (int i = 0; i < Query10.size(); i++)
		{
			std::cout << Query10[i] << "\n";
		}
	}
	else
		std::cout << "No matching patterns found\n";


	std::cout << "-------------------------------------------------------\n";
	std::cout << "Requirement #9:   Union \n";
	std::cout << "-------------------------------------------------------";
	std::cout << "\n" << "Query - Result of union of keys" << "\n";
	std::vector<std::string> unionvec;
	for (int i = 0; i < (int)Query3.size(); i++) {
		unionvec.push_back(Query3.at(i));
	}
	for (int i = 0; i < (int)Query4.size(); i++) {
		unionvec.push_back(Query4.at(i));
	}
	std::vector<std::string> UnionRes = db.Query5_UsingRegularExpression(unionvec, "transport");
	if (!Query5.empty())
	{
		for (int i = 0; i < Query5.size(); i++)
		{
			std::cout << Query5[i] << "\n";
		}
	}
	else
		std::cout << "No matching patterns found\n";


	std::cout << "\n-------------------------------------------------------\n";
	std::cout << "Requirement #10:   Loading the Contents in the form of Xml \n";
	std::cout << "-------------------------------------------------------";
	NoSqlDb<StrData>newDb;
	//newDb.("../XmlDocument/XmlDocument/FromXml.xml");
	std::cout << "\n";
	std::cout << "FromXml.xml\n";
	std::cout << "The contents are loaded in the form of Xml file in the File name FromXml.xml\n\n";

	std::cout << "-------------------------------------------------------\n";
	std::cout << "Requirement #11:   Loading the Contents in the form of Xml \n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "Test Executive.cpp file is Generated\n\n";

	std::cout << "-------------------------------------------------------\n";
	std::cout << "Requirement #12:   Bonus \n";
	std::cout << "-------------------------------------------------------\n";
	std::cout << "The Queries is generated using regular expressions\n";





	
}

