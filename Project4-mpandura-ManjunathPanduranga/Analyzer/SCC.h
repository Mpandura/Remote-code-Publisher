#pragma once
/////////////////////////////////////////////////////////////////////
// SCC.h - Finds the strongly connected components               //
// ver 1.0                                                         //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Platform:    Lenovo, Windows 10                                 //
// Application: Project #2, CSE687 - Object Oriented Design, S2015 //
// Author:		Manjunath Panduranga               				   //
//		       					                                   //
/////////////////////////////////////////////////////////////////////
#ifndef STRONGCOMP_H
#define STRONGCOMP_H

//Module Operations :
//== == == == == == == == ==
//It defines two classes Graph database class and an findingSCC class
//*findingSCC works as a container class finding all the strongly connected
//components using the Tarjan's algorithm
//*Graph class creates a graph containing a vertex set and a adjacency
//list at run time.
#include "../Parser/ActionsAndRules.h"
#include <stack>
#include <set>
#include"Graph.h"

using namespace GraphAnal;
typedef Graph<std::string, std::string> graph;


namespace CodeAnalysis
{
	class strongly_connected_components
	{
		using StrongMap = std::unordered_map<std::string, std::set<std::string>>;
		using StrongMap_ = std::unordered_map<int, std::vector<int>>;
	public:
		void findingSCC(); // finding of findingSCC
		// builds the graph of all the files
		strongly_connected_components::strongly_connected_components(graph& g) : graph_(g) { 
			count = 0;
			Vertex_ = graph_.size();
		}
		void StrongRecur(int m, int disc[], int low[], std::stack<int> *stack, bool visited[]);// A Recursive function used by findingSCC()
		StrongMap& getSCC() // the strong connected components maps is being returned
		{ 
			return strongMap_;
		}  

		StrongMap_& getCC() // the strong connected components maps is being returned
		{
			return strongMap;
		}
		std::unordered_map<std::string, int> strongly_connected_components::getUniquetable()
		{
			return uniques;
		}
		std::string strongly_connected_components::getName(int i)
		{
			for (auto row : uniques)
				if (uniques[row.first] == i)
					return row.first;
			return std::string();
		}
		void persistsIntoXml(std::string filePa);
	private:
		Graph<std::string, std::string> &graph_;
		StrongMap strongMap_;
		StrongMap_ strongMap;
		int count; // counting the strongly connected components
		size_t Vertex_;
		std::unordered_map<std::string, int> uniques;
	};
}
#endif