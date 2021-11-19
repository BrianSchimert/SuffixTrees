#pragma once
#include "SuffixNode.h"
#include <string>


class SuffixTree {


public:
	int numLeaves;
	int numNodes;

	SuffixNode *root;
	SuffixTree(std::string s, std::string alphabet);
	void FindPath(SuffixNode* u, std::string s, int index);
	void NodeHops();
	void printChildren(SuffixNode *u);
	void DFSDisp(SuffixNode* T);
	SuffixNode* findChildMatch(SuffixNode* node, std::string searchTerm, std::string s);
	void addChild(SuffixNode* parent, SuffixNode* child);
	void removeChild(SuffixNode* parent, SuffixNode* child);
	void DFSDisplay(SuffixNode* T);
	std::string getStringLabel(SuffixNode* T);
	void printStringPath(SuffixNode* T);
	void BWTPopulate(SuffixNode *T);
	void printBWTToFile(std::string filename);

};