#include <iostream>
#include "SuffixTree.h"
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;


int stringLength;
int nodeCounter;
string originalString;
int numLeaves;
int numInternalNodes;
int leafCounter;
vector<int> SA;
vector<char> BWT;

SuffixTree::SuffixTree(string s, string alphabet) {
	SuffixNode *root = new SuffixNode();
	root->id = 0;
	root->stringDepth = 0;
	root->edgeLabel = make_pair(0,0);
	root->leftChild = NULL;
	root->rightSibling = NULL;
	root->parent = NULL;
	nodeCounter = s.size()+1;
	leafCounter = 1;
	numLeaves = 0;
	numInternalNodes = 1;

	s = s + "$";
	originalString = s;
	stringLength = s.size();
	for (int i = 0; i < stringLength; i++) {
		//cout << "Inserting suffix: " << s.substr(i) << endl;
		FindPath(root, s, i);
		//DFSDisplay(root);
		//cout << "\n";
	}
	//DFSDisp(root);
	//cout << "DFS print: ";
	//DFSDisplay(root);
	BWTPopulate(root);
	//for (int i = 0; i < SA.size(); i++)
	//	cout << SA.at(i) << " ";
	
	cout << "Number of Leaves: " << numLeaves << "\n";
	cout << "Number of Internal Nodes: " << numInternalNodes << "\n";
	cout << "Total number of Nodes : " << numLeaves + numInternalNodes << "\n";
	//cout << sizeof(root);
	//cout << nodeCounter << endl;
	//cout << leafCounter << endl;
}
void SuffixTree::printBWTToFile(string filename) {
	ofstream ofile(filename);
	for (int i = 0; i < SA.size(); i++) {
		int curr = SA.at(i);
		if (curr == 1) {
			curr = originalString.size() - 1;
		}
		else {
			curr = curr - 2;
		}
		BWT.push_back(originalString[curr]);
	}
	cout << "\n";
	for (int i = 0; i < BWT.size(); i++)
		ofile << BWT.at(i) << endl;
}
void SuffixTree::BWTPopulate(SuffixNode *T) {
	if (T->leftChild == NULL) {
		SA.push_back(T->id);
	}
	SuffixNode* temp = T->leftChild;
	while (temp != NULL) {
		BWTPopulate(temp);
		temp = temp->rightSibling;
	}
}

void SuffixTree::DFSDisp(SuffixNode* T) {
	if (T->leftChild == NULL) {
		cout << T->id;
		//<< " " << T->edgeLabel.first << " " << T->edgeLabel.second << "\n";
	}
	//cout << getStringLabel(T);
	SuffixNode* temp = T->leftChild;
	while (temp != NULL) {
		DFSDisp(temp);
		temp = temp->rightSibling;
		
	}
}

void SuffixTree::DFSDisplay(SuffixNode* T) {
	if (T == NULL) {
		return;
	}
	while (T) {
		cout << T->id << ": ";
		//printStringPath(T);
		//cout << "\n";
		if (T->leftChild) {
			DFSDisplay(T->leftChild);
		}
		T = T->rightSibling;
	}

}

//finds and returns the child node whose edge label matches the first letter in the search term
SuffixNode* SuffixTree::findChildMatch(SuffixNode* node, string searchTerm, string s) {
	SuffixNode* temp = node->leftChild;
	string edgeString;
	int start, end;
	while (temp) {
		start = temp->edgeLabel.first;
		end = temp->edgeLabel.second;
		edgeString = s.substr(start, end - start+1);
		if (edgeString[0] == searchTerm[0]) {
			return temp;
		}
		temp = temp->rightSibling;
	}
	return temp;
}

//adds a suffixnode child below another suffixnode parent, in lexicographical order
void SuffixTree::addChild(SuffixNode* parent, SuffixNode* child) {  
	SuffixNode* temp = parent->leftChild;
	child->parent = parent;
	if (temp != NULL) {
		int start = temp->edgeLabel.first;
		int end = temp->edgeLabel.second;
		int substrLen;
		if (end - start == 0) {
			substrLen = 1;
		}
		else {
			substrLen = end - start+1;
		}
		string edgeStringChildren = originalString.substr(start, substrLen);
		start = child->edgeLabel.first;
		end = child->edgeLabel.second;
		if (end - start == 0) {
			substrLen = 1;
		}
		else {
			substrLen = end - start+1;
		}
		string edgeStringToAdd = originalString.substr(start, substrLen);
		if (edgeStringToAdd[0] < edgeStringChildren[0]) { //if we want the child to be to the left
			child->rightSibling = temp;
			parent->leftChild = child;
			return;
		}
		while (temp) {
			if (temp->rightSibling == NULL) { //insert at the end of the child list
				temp->rightSibling = child;
				return;
			}
			else {  //inserting between two existing children
				start = temp->edgeLabel.first;
				end = temp->edgeLabel.second;
				edgeStringChildren = originalString.substr(start, end - start+1);
				start = temp->rightSibling->edgeLabel.first;
				end = temp->rightSibling->edgeLabel.second;
				string nextChildString = originalString.substr(start, end - start+1);
				if (edgeStringToAdd[0] > edgeStringChildren[0] && edgeStringToAdd[0] < nextChildString[0]) { //insert in between nodess
					child->rightSibling = temp->rightSibling;
					temp->rightSibling = child;
					return;
				}
				temp = temp->rightSibling;
			}
		}
	}
	else {
		parent->leftChild = child;
		//child->parent = parent;
	}
}

//Find path function called to insert suffixes into the tree
void SuffixTree::FindPath(SuffixNode* u, string s, int i) {
	SuffixNode* v = u;
	string x = s.substr(i);  //THIS IS THE SUFFIX BEING ADDED
	//cout << x << endl;
	SuffixNode* children = u->leftChild;
	SuffixNode* newNode = new SuffixNode();
	int start, end;
	//node u might have children, so traverse them to see if matching path 
		//return;
		SuffixNode* matcher = findChildMatch(v, x, s);

		if (matcher == NULL) { //no suitable outgoing edges were found from u, so simply add as u's child
			//add child
			newNode->id = leafCounter;
			leafCounter++;
			numLeaves++;
			newNode->edgeLabel = make_pair(i, s.size()-1);
			//cout << s.size();
			//cout << newNode->edgeLabel.first << " ";
			//cout << newNode->edgeLabel.second << " ";
			newNode->stringDepth = s.size() - i;
			addChild(u, newNode);  //uncomment
			
			return;
		}
		else { //there was a match found outgoing from u
			start = matcher->edgeLabel.first;
			end = matcher->edgeLabel.second;
				string edgeString = s.substr(start, end - start+1);  //get the string edge label from the input string
					//cout << "hello";
					for (int j = 0; j < edgeString.size(); j++) {//
						if (edgeString[j] != x[j]) { //if there's a mismatch along the edge label of the child of u
							//break, create a new internal node and rearrange
							//create new internal node, set the child to matcher as well as another new leaf
							//for the new suffix, parent set to u (or v)
							SuffixNode *newInternalNode = new SuffixNode();
							SuffixNode* newLeafNode = new SuffixNode();
							int newInternalNodeStart = start;
							int newInternalNodeEnd = start+j-1;
							newLeafNode->id = leafCounter;
							leafCounter++;
							newInternalNode->id = nodeCounter;
							nodeCounter++;
							newInternalNode->edgeLabel = make_pair(newInternalNodeStart, newInternalNodeEnd);
							newInternalNode->parent = matcher->parent;
							numInternalNodes++;
							newLeafNode->edgeLabel = make_pair(i + j, s.size()-1);
							int oldMatchingLeafNodeStart = matcher->edgeLabel.first + j;
							int oldMatchingLeafNodeEnd = end;
							SuffixNode* parent = matcher->parent;
							numLeaves++;
							//printChildren(parent);
							//cout << endl;
							removeChild(matcher->parent, matcher);
							//printChildren(parent);
							//cout << endl;
							matcher->edgeLabel = make_pair(oldMatchingLeafNodeStart, oldMatchingLeafNodeEnd);
							matcher->rightSibling = NULL;
							matcher->parent = NULL;
							addChild(newInternalNode, matcher);
							addChild(newInternalNode, newLeafNode);
							addChild(parent, newInternalNode);
							//printChildren(parent);
							//cout << endl;
							

							return;
						}
					}
					FindPath(matcher, s, i + end - start+1); //edge label is exhausted, so findpath on the next node
			}
	//return;
}


//removes a specified suffixnode child from the parent
void SuffixTree::removeChild(SuffixNode* parent, SuffixNode* child) {
	SuffixNode* temp = parent->leftChild;
	if (temp == NULL) {
		cout << "No children exist to remove";
		return;
	}
	int start;
	int end;
	string edgeStringChildren;
	string edgeStringToRemove;
	start = child->edgeLabel.first;
	end = child->edgeLabel.second;
	edgeStringToRemove = originalString.substr(start, end - start);
	start = temp->edgeLabel.first;
	end = temp->edgeLabel.second;
	edgeStringChildren = originalString.substr(start, end - start);
	if (edgeStringToRemove[0] == edgeStringChildren[0]) {  //if the node to remove is the first node in the child list
		parent->leftChild = child->rightSibling;
		child->parent = NULL;
		return;
	}
	else {
		SuffixNode* prev = temp;
		temp = temp->rightSibling;
		while (temp) {
			start = temp->edgeLabel.first;
			end = temp->edgeLabel.second;
			edgeStringChildren = originalString.substr(start, end - start);
			if (edgeStringToRemove[0] == edgeStringChildren[0]) {
				prev->rightSibling = temp->rightSibling;
				return;
			}
			prev = temp;
			temp = temp->rightSibling;
		}

	}	
	cout << "no child removed";

}

void SuffixTree::NodeHops() {

	return;
}
//returns the string that corresponds to the edgelabel of a particular node
string SuffixTree::getStringLabel(SuffixNode* T) {
	int start = T->edgeLabel.first;
	int end = T->edgeLabel.second;
	return originalString.substr(start, end - start + 1);

}

//prints all the children suffixnodes of the given parent
void SuffixTree::printChildren(SuffixNode* u) {
	SuffixNode* temp = u->leftChild; //start at the leftmost child of node u
	while (temp) {
		cout << temp->id;
		temp = temp->rightSibling;
	}

}

//prints the string path of the given suffixnode
void SuffixTree::printStringPath(SuffixNode* T) {
	if (T == NULL) {
		return;
	}
	printStringPath(T->parent);
	int start = T->edgeLabel.first;
	int end = T->edgeLabel.second;
	string print;
	if (end - start == 0)
		print = originalString.substr(start, 1);
	else
		print = originalString.substr(start, end - start+1);
	cout << print << "";
}

//
//void SuffixTree::FindPath(SuffixNode* u, string s, int i) {
//	SuffixNode* v = u;
//	string x = s.substr(i);  //THIS IS THE SUFFIX BEING ADDED
//	//cout << x << endl;
//	SuffixNode* left = u->leftChild;
//	SuffixNode* newNode = new SuffixNode();
//	int start, end;
//	if (left == NULL) {  //if there are no children under u, then directly insert new leaf for suffix x		
//		newNode->id = i;
//		newNode->edgeLabel = make_pair(i, s.size());
//		cout << newNode->edgeLabel.first << " ";
//		cout << newNode->edgeLabel.second << " ";
//		newNode->leftChild = NULL;
//		newNode->rightSibling = NULL;
//		newNode->parent = u;
//		newNode->stringDepth = 1;
//		u->leftChild = newNode;
//		return;
//	}
//	else { //node u has children, so traverse them to see if matching path
//		//return;
//		SuffixNode* temp = left;
//		string edgeString;
//		while (temp) { //while there are siblings to search
//			start = temp->edgeLabel.first;
//			end = temp->edgeLabel.second;
//			edgeString = s.substr(start, end - start);  //get the string edge label from the input string
//			if (edgeString[0] == x[0]) {  //the edge is matching
//				//TODO: run through edgestring until there is a mismatch
//				int counter = 0;
//				while (edgeString[counter] == x[counter] && counter < (end-start)) { //while the characters are matching and edge label isn't exhausted
//					counter++;
//				}
//
//				newNode->id = i;
//				newNode->edgeLabel = make_pair()
//
//				return;
//			}
//			else { //move on to the next sibling node to check it
//				temp = temp->rightSibling;
//			}
//		}
//		//now we know that there was no matching branch
//	}
//	//TODO: if gets here, then no match was found and we need to just add it as a child of v
//
//	return;
//}




//void SuffixTree::FindPath(SuffixNode* u, string s, int i) {
//	SuffixNode* v = u;
//	string x = s.substr(i);  //THIS IS THE SUFFIX BEING ADDED
//	//cout << x << endl;
//	SuffixNode* children = u->leftChild;
//	SuffixNode* newNode = new SuffixNode();
//	int start, end;
//	//node u might have children, so traverse them to see if matching path
//		//return;
//	if (children == NULL) {  //if there are no children under u, then directly insert new leaf for suffix x		
//		newNode->id = i;
//		newNode->edgeLabel = make_pair(i, s.size());
//		//cout << newNode->edgeLabel.first << " ";
//		//cout << newNode->edgeLabel.second << " ";
//		newNode->leftChild = NULL;
//		newNode->rightSibling = NULL;
//		newNode->parent = u;
//		newNode->stringDepth = i;
//		u->leftChild = newNode;
//		return;
//	}
//	else {
//		SuffixNode* matcher = findChildMatch(v, x, s);
//		SuffixNode* temp = children;
//		string edgeString;
//		while (temp) { //while there are siblings to search, also should check to see if lexographic ordering is kept
//			start = temp->edgeLabel.first;
//			end = temp->edgeLabel.second;
//			edgeString = s.substr(start, end - start);  //get the string edge label from the input string
//			if (edgeString[0] == x[0]) {  //the edge is matching
//				//TODO: run through edgestring until there is a mismatch
//				cout << "hello";
//				int counter = 0;
//				for (int j = 0; j < edgeString.size(); i++) {//
//					if (edgeString[j] != x[j]) { //if there's a mismatch along the edge label of the child of u
//						//break, create a new internal node and rearrange
//						//TODO: add stuff here for above notes
//
//						return;
//					}
//				}
//				FindPath(temp, s, i + end-start); //find path on whatever child temp is on
//			}
//			else { //move on to the next sibling node to check it
//				temp = temp->rightSibling;
//			}
//		}
//	}
//		//now we know that there was no matching branch
//	//TODO: if gets here, then no match was found and we need to just add it as a child of v
//		/*newNode->id = i;
//		newNode->edgeLabel = make_pair(i, s.size());
//		cout << newNode->edgeLabel.first << " ";
//		cout << newNode->edgeLabel.second << " ";
//		newNode->leftChild = NULL;
//		newNode->rightSibling = NULL;
//		newNode->parent = u;
//		newNode->stringDepth = 1;
//		u->leftChild = newNode;
//		return;*/
//	return;
//}
