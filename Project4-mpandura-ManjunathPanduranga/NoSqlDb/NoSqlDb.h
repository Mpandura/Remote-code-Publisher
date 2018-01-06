#pragma once
////////////////////////////////////////////////////////////////////////
// NoSqlDb.h - children/value pair in-memory databas                 //
// Author - Manjunath Panduranga                                    //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4715 )
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include"StrHelper.h"
#include "../CppProperties/CppProperties.h"
#include <regex>
#include <set>
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
//#include "../Analyzer/DepAnal.h"
#include "../Analyzer/SCC.h"
#include"../PubExec/depen.h"

using namespace std;
using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;
  /////////////////////////////////////////////////////////////////////
  // Element class represents a data record in our NoSql database
  // - in our NoSql database that is just the value in a children/value pair
  // - it needs to store child data, something for you to implement
  //
template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::string;
	using Description = std::string;
	using Children = std::vector<std::string>;

	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<Description> description;
	Property<TimeDate> timeDate;    // metadata
	Property<Data> data;            // data
	Children children;
	std::string show();
};

template<typename Data>
std::string Element<Data>::show()
{
	
		std::ostringstream out;
		out.setf(std::ios::adjustfield, std::ios::left);
		out << "\n    " << std::setw(8) << "name" << " : " << name;
		out << "\n    " << std::setw(8) << "category" << " : " << category;
		out << "\n    " << std::setw(8) << "description" << " : " << description;
		out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
		out << "\n    " << std::setw(8) << "data" << " : " << data;
		out << "\n    " << std::setw(8) << "children" << " : ";
		//<< children;

		std::vector<std::string> temp = children;
		for (int m = 0; m < (int)temp.size(); m++) {
			out << temp[m] << " ";
			out << "\n";
		}
		out << "\n";
		return out.str();
	
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a children/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//
template<typename Data>
class NoSqlDb
{
public:
  using Key = std::string;
  using Keys = std::vector<Key>;

  Keys keys();
  bool save(Key key, Element<Data> elem);
  Element<Data> value(Key key);
  size_t count();
  using dep_map = std::unordered_map<std::string, std::set<std::string>>;
  using str_map = std::unordered_map<std::string, std::set<std::string>>;
  std::string toXml(const Element<Data> element);
  std::string NoSqlDb<Data>::strongtoXml(const Element<Data> element);
  Element<Data> NoSqlDb<Data>::element_return(Key key);
  void  NoSqlDb<Data>::fromXml(const std::string& xml);
  void dependency_analysis(CodeAnalysis::TypeAnal &ta);
  void strong(CodeAnalysis::strongly_connected_components &sc);
  void NoSqlDb<Data>::saveExistingElement(Key key, Element<Data> elem);
  bool add(Key key, Element<Data> elem);
  bool NoSqlDb<Data>::deleting(Key key);
  void NoSqlDb<Data>::UpdatingOfElements(Key key, Element<Data> &elem);
  std::vector<std::string> NoSqlDb<Data>::Query2_UsingRegularExpression(Key key);
  std::vector<std::string> NoSqlDb<Data>::Query3_UsingRegularExpression(std::vector<std::string> data, std::string pattern);
  Element<Data> NoSqlDb<Data>::valueOfKey(Key key);
  std::vector<std::string> NoSqlDb<Data>::Query4_UsingRegularExpression(std::vector<std::string> data, std::string pattern);
  std::vector<std::string> NoSqlDb<Data>::Query5_UsingRegularExpression(std::vector<std::string> data, std::string pattern);
  std::vector<std::string> NoSqlDb<Data>::Query6_UsingRegularExpression(std::vector<std::string> data, std::string pattern);
  Element<Data> NoSqlDb<Data>::Query1_UsingRegularExpression(Key key);
  void NoSqlDb<Data>::deletingChildren(Key children);
void NoSqlDb<Data>::MetadataUpdating(Key key, std::string category, std::string data, std::string description);
 
private:
  using Item = std::pair<Key, Element<Data>>;
  dep_map depen;
  str_map stron;
  std::unordered_map<Key,Element<Data>> store; 
};

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
  Keys keys; 
  for (Item item : store)
  {
    keys.push_back(item.first);
  }
  return keys;
}

template<typename Data>
bool NoSqlDb<Data>::add(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store.insert(std::make_pair(key, elem));
	std::cout << key << "\n";
	std::cout << value(key).show();
	return true;
}

template<typename Data>
bool NoSqlDb<Data>::deleting(Key key)
{
	std::cout << " deleting the children \n";

	if (store.find(key) != store.end())
		return false;
	else {
		store.erase(key);
		std::cout << " children is deleted sucessfully\n";
	}
	return true;
}

template<typename Data>
void NoSqlDb<Data>::deletingChildren(Key children)
{
	for (Item s : store)
	{
		std::vector<std::string> temp = s.second.children;
		for (int m = 0; m < (int)temp.size(); m++)
		{
			if (temp[m] == children)
				temp.erase(temp.begin() + m);
		}
		s.second.children = temp;
		Key parentkey;
		parentkey = s.first;
		store[parentkey].children = s.second.children;
	}
	/*std::cout << children << "\n";
	std::cout << value(children).show();*/
}

template<typename Data>
Element<Data> NoSqlDb<Data>::Query1_UsingRegularExpression(Key key)
{
	
	std::regex d("(elem1)(.*)");
	if (std::regex_match(key, d))
	{
		std::cout << "\n  " << key << ":";
		std::cout << value(key).show();
	}
	return Element<Data>();
}



template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
  if(store.find(key) != store.end())
    return false;
  store[key] = elem;
  return true;
}

template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
  if (store.find(key) != store.end())
    return store[key];
  return Element<Data>();
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
  return store.size();
}



template<typename Data>
void  NoSqlDb<Data>::fromXml(const std::string& xml)
{
	try
	{
		XmlDocument doc(xml, XmlDocument::file);
		std::vector<SPtr> desc = doc.descendents("element").select();
		for (unsigned int i = 0; i < desc.size(); i++) {
			std::vector<SPtr> keysDesc = doc.descendents("children").select();
			std::vector<SPtr> valuesDesc = doc.descendents("value").select();

			Element<Data> item;
			std::string name_value = valuesDesc.at(i)->children()[0]->children()[0]->value();
			std::string category_value = valuesDesc.at(i)->children()[1]->children()[0]->value();
			std::string description_value = valuesDesc.at(i)->children()[2]->children()[0]->value();
			std::string timeDate_value = valuesDesc.at(i)->children()[3]->children()[0]->value();
			std::string data_value = valuesDesc.at(i)->children()[4]->children()[0]->value();


			name_value = trim(name_value);
			item.name = name_value;
			category_value = trim(category_value);
			item.category = category_value;
			description_value = trim(description_value);
			item.description = description_value;
			timeDate_value = trim(timeDate_value);
			item.timeDate = timeDate_value;
			data_value = trim(data_value);
			item.data = Convert<Data>::fromString(data_value);


			std::string key_value = keysDesc.at(i)->children()[0]->value();
			key_value = trim(key_value);
			this->save(key_value, item);
		}

	}
	catch (std::exception ex)
	{
		std::cout << "\n error";
		std::cout << "\n " << ex.what();
	}
}


template<typename Data>
inline std::string NoSqlDb<Data>::toXml(const Element<Data> element)
{


	std::string xml;
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("Table");
	doc.docElement() = pRoot;

	SPtr pNameElem = makeTaggedElement("Name");
	pRoot->addChild(pNameElem);
	SPtr pTextElem1 = makeTextElement(element.name);
	pNameElem->addChild(pTextElem1);


	
	SPtr pTimeElem = makeTaggedElement("TimeAndDate");
	pRoot->addChild(pTimeElem);
	SPtr pTextElem4 = makeTextElement(element.timeDate);
	pTimeElem->addChild(pTextElem4);
	
	
	SPtr pChildElem = makeTaggedElement("Children");
	pRoot->addChild(pChildElem);
	if (element.children.size() != 0)
	{
		int i = 1;
		for (Key childkey : element.children)
		{
			SPtr pChildElem1 = makeTaggedElement("Child");
			pChildElem->addChild(pChildElem1);
			SPtr pTextElem = makeTextElement(childkey);
			pChildElem1->addChild(pTextElem);
		}
	}
	else
	{
		SPtr pChildElem1 = makeTaggedElement("Child");
		pChildElem->addChild(pChildElem1);
		SPtr pTextElem = makeTextElement("children are not found");
		pChildElem1->addChild(pTextElem);
	}
	xml = doc.toString(); 
	return xml;
}


template<typename Data>
inline std::string NoSqlDb<Data>::strongtoXml(const Element<Data> element)
{

	std::string xml;
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("Strongly Connected Components");
	doc.docElement() = pRoot;

	SPtr pNameElem = makeTaggedElement("Name");
	pRoot->addChild(pNameElem);
	SPtr pTextElem1 = makeTextElement(element.name);
	pNameElem->addChild(pTextElem1);



	SPtr pChildElem = makeTaggedElement("SCC");
	pRoot->addChild(pChildElem);
	if (element.children.size() != 0)
	{
		int i = 1;
		for (Key childkey : element.children)
		{
			SPtr pChildElem1 = makeTaggedElement("Files");
			pChildElem->addChild(pChildElem1);
			SPtr pTextElem = makeTextElement(childkey);
			pChildElem1->addChild(pTextElem);
		}
	}
	else
	{
		SPtr pChildElem1 = makeTaggedElement("Child");
		pChildElem->addChild(pChildElem1);
		SPtr pTextElem = makeTextElement("children are not found");
		pChildElem1->addChild(pTextElem);
	}
	xml = doc.toString();


	//cout << xml; 
	return xml;
}
template<typename Data>
inline void NoSqlDb<Data>::dependency_analysis(CodeAnalysis::TypeAnal &ta)
{
	depen = ta.returnmap(); 
	Element<Data> ele;
	ofstream myfile;
	myfile.open("C:\\report.html");
	myfile << "<!DOCTYPE html><html><head></head><body>"; //starting html
	for (auto j = depen.begin(); j != depen.end(); j++)
	{
		
		ele.name = j->first;
		
		for (auto i : j->second)
		{
			ele.data = "depenedency data";
			ele.children.push_back(i);
		}
		save(ele.name, ele);

	}
	

	//													  //add some html content
	//													  //as an example: if you have array of objects featuring the properties name & value, you can print out a new line for each property pairs like this:
	//for (int m = 0; m< depen.end(); m++)
	//	myfile << "<p><span style='font-weight: bold'>" << depen[m] << "</span><span>" << depen[m] << "</span></p>";

	////ending html
	myfile << "</pre></body></html>";
	myfile.close();

}

template<typename Data>
inline void NoSqlDb<Data>::strong(CodeAnalysis::strongly_connected_components &sc)
{
	stron = sc.getSCC();
	Element<Data> ele;
	for (auto j = stron.begin(); j != stron.end(); j++)
	{
		ele.name = j->first;
		
		for (auto i : j->second)
		{
			

			ele.data = "depenedency data";
			ele.children.push_back(i);
		}
		save(ele.name, ele);
	}

}



template<typename Data>
void NoSqlDb<Data>::saveExistingElement(Key key, Element<Data> elem)
{
	store[key] = elem;
}


template<typename Data>
void NoSqlDb<Data>::MetadataUpdating(Key key, std::string category,  std::string data, std::string description) {
	if (store.find(key) != store.end())
	{
		store[key].category = category;
		store[key].description = description;
		store[key].data = data;
	
		std::cout << "\n  " << key << ":";
		std::cout << value(key).show();
		std::cout << "\nupdating of Metadata is sucessfully completed \n";
}

	else {
		std::cout << "updating is failed \n";

	}
}


template<typename Data>
void NoSqlDb<Data>::UpdatingOfElements(Key key, Element<Data> &elem) {
	if (store.find(key) != store.end())
	{
		store[key] = elem;
	
		std::cout << "\n  " << key << ":";
		std::cout << value(key).show();
		std::cout << "\n updating of elements is sucessfully completed \n";
	}

	else {
		std::cout << "updating is failed \n";
	}
}


template<typename Data>
std::vector<std::string> NoSqlDb<Data>::Query6_UsingRegularExpression(std::vector<std::string> data, std::string pattern)
{
	std::vector<std::string> q6;
	auto it = store.begin();
	while (it != store.end())
	{
		std::string strKey = it->first;
		Element<Data> elem = it->second;
		std::string stringKey = elem.data;
		std::smatch m;
		std::regex e("\\b(vehicle)([^ ]*)");
		if (std::regex_search(stringKey, m, e)) {
		
			if (std::find(data.begin(), data.end(), strKey) != data.end())
			{
				std::size_t found = stringKey.find(pattern);
				if (found != std::string::npos)
				{
					q6.push_back(strKey);
				}
			}
		}
		++it;
	}
	return q6;
}

template<typename Data>
std::vector<std::string> NoSqlDb<Data>::Query5_UsingRegularExpression(std::vector<std::string> data, std::string pattern)
{
	std::vector<std::string> q5;
	auto it = store.begin();
	while (it != store.end())
	{
		std::string strKey = it->first;
		Element<Data> elem = it->second;
		std::string stringKey = elem.category;

		std::regex h("(transport)(.*)");
	
		if (std::regex_match(stringKey, h))
		{
			if (std::find(data.begin(), data.end(), strKey) != data.end())
			{
				std::size_t found = stringKey.find(pattern);
				if (found != std::string::npos)
				{
					q5.push_back(strKey);
				}
			}
		}
				++it;
			}
			return q5;
		}
	

template <typename Data>
std::vector<std::string> NoSqlDb<Data>::Query4_UsingRegularExpression(std::vector<std::string> data, std::string pattern)
{
	std::vector<std::string> q4;
	auto it = store.begin();
	while (it != store.end())
	{
		Element<Data> elem = it->second;
		std::string stringKey = elem.name;
		std::string key = it->first;

		std::regex h("(elem)(.*)");

		if (std::regex_match(stringKey, h))
		{
			if (std::find(data.begin(), data.end(), key) != data.end())
			{
				std::size_t found = stringKey.find(pattern);
				if (found != std::string::npos)
				{
					q4.push_back(stringKey);
				}
			}
		}
			++it;
		}
		return q4;
	
}
template<typename Data>
Element<Data> NoSqlDb<Data>::element_return(Key key)
{
	Element<Data> element;


	for (Item item : store)
	{
		if (item.first == key)
		{
			element = item.second;

			return element;
		}

	}
}

template <typename Data>
std::vector<std::string> NoSqlDb<Data>::Query3_UsingRegularExpression(std::vector<std::string> data, std::string pattern)
{
	std::vector<std::string> q3;
	std::vector<std::string> defaultToAllKeys;
	auto it = store.begin();
	while (it != store.end())
	{
		std::string stringKey = it->first;
		std::regex h("(elem)(.*)");

		if (std::regex_match(stringKey, h))
		{
			if (std::find(data.begin(), data.end(), stringKey) != data.end())
			{
				defaultToAllKeys.push_back(stringKey);
				std::size_t found = stringKey.find(pattern);
				if (found != std::string::npos)
				{
					q3.push_back(stringKey);
				}
			}
		}
		++it;
	}

	if (!q3.empty())
	{
		return q3;
	}
	else
	{
		std::cout << "No keys found to match the pattern- Default to all keys\n";
		return defaultToAllKeys;
	}
}

template <typename Data>
std::vector<std::string> NoSqlDb<Data>::Query2_UsingRegularExpression(Key key)
{
	std::string c("elem1", "elem2");
	std::regex d("(elem1)(.*)");
	if (std::regex_match(c, d))
	{

		if (store.find(key) != store.end())
		{
			Element < Data > e = store[key];
			return e.children;
		}
	}
	else
	{
		std::cout << "The children is not found\n";
	}
}


template <typename Data>
Element<Data> NoSqlDb<Data>::valueOfKey(Key key)
{
	std::string c("elem1", "elem2");
	std::regex d("(elem1)(.*)");
	if (std::regex_match(c, d))
	{
		if (store.find(key) != store.end())
		{
			return store[key];
		}
	}
	return Element<Data>();
}






