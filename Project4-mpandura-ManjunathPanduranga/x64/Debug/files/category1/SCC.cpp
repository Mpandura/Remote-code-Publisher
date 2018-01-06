/////////////////////////////////////////////////////////////////////
// SCC.cpp - Finds the strongly connected components               //
// ver 1.0                                                         //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Platform:    Lenovo, Windows 10                                 //
// Application: Project #2, CSE687 - Object Oriented Design, S2015 //
// Author:		Manjunath Panduranga               				   //
//		       					                                   //
/////////////////////////////////////////////////////////////////////
#include <vector>
#include <sstream>
#include <algorithm>
#define NIL -1
#include "SCC.h"
#pragma warning( disable : 4267 )
using namespace CodeAnalysis;

void strongly_connected_components::findingSCC()
{
	int *prev = new int[Vertex_];
	std::stack<int> *stack = new std::stack<int>();
	int *storage = new int[Vertex_];
	bool *visited = new bool[Vertex_];
	for (int m = 0; m < Vertex_; m++)
	{
		prev[m] = NIL;
		storage[m] = NIL;
		visited[m] = false;
	}
	for (int j = 0; j < Vertex_; j++)
	{
		if (storage[j] == NIL)
			StrongRecur(j, storage, prev, stack, visited);
	}
}



void strongly_connected_components::StrongRecur(int next, int storage[], int prev[], std::stack<int> *stack, bool visited[])
{
	int storing = 0;
	static int tym = 0;
	storage[next] = prev[next] = ++tym;
	stack->push(next);
	visited[next] = true;
	Vertex<std::string, std::string> &vertex = graph_[next];
	for (size_t i = 0; i < vertex.size(); i++)
	{
		Vertex<std::string, std::string>::Edge edge = vertex[i];
		Vertex<std::string, std::string> &vertex_ = graph_[edge.first];
		for (size_t j = 0; j < graph_.size(); j++)
		{
			Vertex<std::string, std::string> &vertex = graph_[j];
			if (vertex.value() == vertex_.value())
			{
	    		 if (visited[vertex.id()] == true)
					prev[next] = min(prev[next], storage[vertex.id()]);
				
				else if (storage[vertex.id()] == -1)
				{
					StrongRecur(vertex.id(), storage, prev, stack, visited);
					prev[next] = min(prev[next], prev[vertex.id()]);
				}
				break;
			}
		}
	}
  
	if (storage[next]==prev[next] )
	{
		std::cout << "\n\n";
		std::stringstream stringstream;
		stringstream << "strongly connected components" << count;
		while (stack->top() != next)
		{
			storing = (int)stack->top();
			Vertex<std::string, std::string>& v = graph_[storing];
			visited[storing] = false;
			strongMap_[stringstream.str()].insert(v.value());
			stack->pop();
		}
		storing = (int)stack->top();
		Vertex<std::string, std::string> &vertex = graph_[storing];
		visited[storing] = false;
		stack->pop();
		strongMap_[stringstream.str()].insert(vertex.value());
		count++;
		stringstream.str("");
	}
}

void CodeAnalysis::strongly_connected_components::persistsIntoXml(std::string filePa)
{
	std::string theDocument;
	XmlWriter wrg;
	try {
		int count = 1;
		std::unordered_map<int, std::vector<int>>& sCGraph = getCC();
		wrg.indent();
		wrg.start("Strongly Connected Components");
		for (auto vert : sCGraph) {
			wrg.start("SCC");			
			std::vector<int> x = sCGraph[vert.first];
			for (auto edge : x) {
				wrg.start("files");
				wrg.addBody(getName(edge));
				wrg.end();
			}
			wrg.end();
		}
		wrg.end();
		theDocument = wrg.xml();
		std::ofstream input_file(filePa);
		input_file << theDocument;
		input_file.close();
	}
	catch (const std::exception& e) {
		std::cout << "exception:" << e.what();
	}
}

#ifdef TEST_STRONGCOMPONENTS
int main()
{
	return 0;
}
#endif