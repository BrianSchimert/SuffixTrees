#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>



class SuffixNode {

public:

	int id;  //4 bytes
	int stringDepth;  //4 bytes
	//SuffixNode *suffixLink;
	SuffixNode *parent; //8 bytes
	std::pair<int, int> edgeLabel; //8 bytes
	SuffixNode *leftChild; //8 bytes
	SuffixNode *rightSibling;	 //8 bytes
	//total of 40bytes per node
};