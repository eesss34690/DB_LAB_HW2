#ifndef INDEX_H_
#define INDEX_H_
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
class Node
{
	protected:
		bool isLeaf;
		vector<int> keys;

	public:
		bool Get_IsLeaf();
		vector<int> Get_Keys();
		virtual void Insertkv(int key, int value){}
		virtual void Insertkn(int key, Node* rightChild){}
		virtual void Insertknn(int key, Node* leftChild, Node* rightChild){}
		virtual void Search(int key){}
		virtual void Search(int key1, int key2){}
		virtual Node* Split(int* keyToParent){}
		virtual vector<Node*> Get_Children(){}
		virtual vector<int> Get_Values(){}
		virtual Node* Get_Next(){}
};


// internal node
class InternalNode : public Node
{
	private:
		vector<Node*> children;

	public:
		InternalNode();
		void Insertkn(int key, Node* rightChild);
		void Insertknn(int key, Node* leftChild, Node* rightChild);
		Node* Split(int* keyToParent);
		vector<Node*> Get_Children();
};


// leaf node
class LeafNode : public Node
{
	private:
		LeafNode* prev;
		LeafNode* next;
		vector<int> values;

	public:
		LeafNode();
		void Insertkv(int key, int value);
		Node* Split(int* keyToParent);
		vector<int> Get_Values();
		Node* Get_Next();
};


// B+ tree
class Index
{
	private:
		int order= 50;
		Node* root;
		ofstream outputFile;
		void Search_Path(Node* node, int key, stack<Node*>* path);
		void Destroy(Node* node);

	public:
		void Initialize();
		Index(int num, vector<int> keys, vector<int> val);
		void Insert(int key, int value);
		int Search1(int key);
		vector<int> Search2(int key1, int key2);
		void key_query(vector<int> query_keys);
		void range_query(vector<pair<int,int>> query_pairs);
		void clear_index();

};

#endif // INDEX_H_INCLUDED
