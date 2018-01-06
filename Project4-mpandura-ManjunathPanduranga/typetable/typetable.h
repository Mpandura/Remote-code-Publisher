#pragma once

#include "../Parser/ActionsAndRules.h"
#include<iostream>
#include<stack>
#pragma warning (disable : 4101)

namespace CodeAnalysis {
	class Tuple
	{
	public:
		using Type_name = std::string;
		using Namespace_name = std::string;
		using File_name = std::string;

		Tuple() {};
		Tuple(std::string typ, std::string nmespce, std::string fle) : type_of(typ), namespace_of(nmespce), file_of(fle) {};// Tuple is created using a initializer sequence
		bool setType(Type_name);			//sets the type of the tuple
		bool setNamespace(Namespace_name);  //sets the namespace of the tuple
		bool setFile(File_name);			//sets the file name to which the type belongs to
		std::string getType();				//returns the type of the tuple
		std::string getNamespace();			//returns the namespace of the tuple
		std::string getFileName();			//returns the file name to which the type belongs to
	private:

		Type_name type_of;
		Namespace_name namespace_of;
		File_name file_of;



	};


	using Key = std::string;
	using Keys = std::vector<Key>;
	class  TypeTable
	{
	public:

		using SPtr = std::shared_ptr<ASTNode>;

		TypeTable();
		~TypeTable();

		Keys getKeys();														//Returns all the keys in the TypeTable
		bool insert(Key, std::vector< Tuple>);								//inserts a tuple into the TypeTable
		std::vector<Tuple> value(Key);									    //Returns a tuple for a key in the TypeTable
		int count();														//Count of number of entries in the TypeTable
		std::unordered_map<std::string, std::vector<Tuple>>& getTable();	//Returns a TypeTable Reference
		void doTableAnal();													// Creates the typeTable


	private:

		std::string correct_namespace;
		std::stack<std::string> nameSpaceStack;								//stack to record the name spaces
		void pushIntoMap(ASTNode* pNode);									// pushes required AST nodes into the TypeTable
		void DFS(ASTNode* pNode);											//Walks the Abstract Syntax tree in a Depth First Serach way
		bool doDisplay1(ASTNode* pNode);									//checks for the required types
		AbstrSynTree& ASTref_;
		std::unordered_map<std::string, std::vector<Tuple>> T_Table;
		std::string pType;
	};



}