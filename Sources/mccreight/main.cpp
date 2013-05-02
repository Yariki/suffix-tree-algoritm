#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <time.h>

using std::string;
using std::vector;

enum POS_TYPE
{
	EDGE,
	NODE
};


struct Node;

struct Edge
{
	Node* parent;
	Node* node;
	int first;
	int last;

	int getLenght()
	{
		return first == last ? 1 : (last - first) + 1;
	}
};

struct Node
{
	Edge* owner;
	vector<Edge*> children;
	Node* suffix;
};

struct Label
{
	Label(){}
	Label(int s, int e)
	{
		pos1 = s;
		pos2 = e;
	}
	int pos1;
	int pos2;

	int getLenght() const
	{
		return pos1 == pos2 ? 1 : (pos2 - pos1) + 1;
	}
};

struct Pos
{
	Pos(){}
	void* element;
	POS_TYPE type;
	Label label;
};



Node* root;
Node* head;
int tail;
string currenttext;
int lenght = -1;

int itemcount = 0;
char* format = "(%d:%s)";
char* formatN = "(%d:%s):leaf\n\n";


void create_init_tree()
{
	root = new Node();
	root->suffix = root;
	root->owner = nullptr;
	Edge* edge = new Edge();
	edge->parent = root;
	edge->first = 0;
	edge->last = currenttext.length() - 1;
	edge->node = nullptr;
	root->children.push_back(edge);
}

Node* get_parent(Node* node)
{
	return node->owner->parent;
}

bool is_leaf(Edge* edge)
{
	return edge->node == nullptr;
}

Label get_label(Node*parent,Node* node)
{
	Label l(node->owner->first,node->owner->last);
	return l;
}

Node* get_suffix(Node* node)
{
	return node->suffix;
}

Pos* create_pos(void* element, POS_TYPE type,Label& label)
{
	Pos* pos = new Pos();
	pos->element = element;
	pos->type = type;
	pos->label = Label(label.pos1,label.pos2);
	return pos;
}

void set_suffix(Node* head, Node* suf)
{
	if(head != NULL)
		head->suffix = suf;
}

void add_nodechild(Node* parent, Edge* edge)
{
	if(parent != nullptr && edge != nullptr)
	{
		parent->children.push_back(edge);
	}
}

void set_edgenode(Edge* edge, Node* newNode)
{
	if(edge != nullptr && newNode != nullptr)
	{
		edge->node = newNode;
	}
}

Node* add_element(Pos* pos, int index)
{
	switch(pos->type)
	{
	case NODE: // just add leaf
		{
			auto edge = new Edge();
			edge->first = index;
			edge->last = lenght-1;
			auto node = (Node*)pos->element;
			edge->parent = node;
			add_nodechild(node,edge);
			return (Node*)pos->element;
		}
	case EDGE: // split edge with adding new inner node, which will be new head(i);
		{
			Edge* edge = (Edge *)pos->element;
			Node* node = edge->node;
			int last = edge->last;
			edge->last = pos->label.pos1 - 1;
			Node* innerNode = new Node();
			edge->node = innerNode;
			innerNode->owner = edge;
			// two edge for inner node;
			Edge* splitEdge = new Edge();
			splitEdge->first = pos->label.pos1;
			splitEdge->last = node ? last : lenght - 1;
			splitEdge->parent = innerNode;
			splitEdge->node = node;
			add_nodechild(innerNode,splitEdge);
			Edge* newEdge = new Edge();
			newEdge->first = pos->label.pos2;
			newEdge->last = lenght - 1;
			newEdge->parent = innerNode;
			add_nodechild(innerNode,newEdge);
			return innerNode;
		}
	}
}

Node* add_element_edge_fast(Pos* pos, int tail)
{
	Edge* edge = (Edge*)pos->element;
	edge->last = pos->label.pos1;
	Node* innerNode = new Node();
	edge->node = innerNode;
	innerNode->owner = edge;
	// two edges for node
	Edge* split = new Edge();
	split->first = pos->label.pos1+1;
	split->last = lenght - 1;
	split->parent = innerNode;
	add_nodechild(innerNode,split);
	Edge* newEdge = new Edge();
	newEdge->first = tail + pos->label.pos2;
	newEdge->last = lenght - 1;
	newEdge->parent = innerNode;
	add_nodechild(innerNode,newEdge);
	return innerNode;
}

Pos* slowscan_recurce(Node* node, Label& label)
{
	int startIndex = label.pos1;
	vector<Edge*>::iterator iter;
	Edge* forContinue = nullptr;
	for(iter = node->children.begin(); iter != node->children.end();++iter)
	{
		forContinue = (Edge*)*iter;
		if(currenttext[startIndex] == currenttext[forContinue->first])
			break;
		forContinue = nullptr;
	}
	if(forContinue)
	{
		int f = forContinue->first+1;
		bool match = true;
		while (f <= forContinue->last && startIndex < lenght)
		{
			if(currenttext[++startIndex] != currenttext[f++])
			{
				match = false;
				break;
			}
		}
		
		if(f < forContinue->last ||(!match && f - 1 == forContinue->last))
			return create_pos(forContinue,EDGE,Label(f-1,startIndex));
		label.pos1 = ++startIndex;
		if(f >= forContinue->last && forContinue->node != nullptr && match)
			return slowscan_recurce(forContinue->node,label);//create_pos(forContinue->node,POS_TYPE::NODE,Label(startIndex++,0));
	}
	return create_pos(node,NODE,Label(0,0));
}

Pos* slowscan(Node* startNode, Label& label)
{
	Pos* pos = slowscan_recurce(startNode,label);
	return pos;
}


Pos* fastscan_recurse(Node* node, Label& label)
{
	Edge* forCont = nullptr;
	int  startIndex = label.pos1;
	for(int i = 0; i < node->children.size();i++)
	{
		forCont = node->children[i];
		if(currenttext[startIndex] == currenttext[forCont->first])
			break;
	}
	if(label.getLenght() < forCont->getLenght())
		return create_pos(forCont,EDGE,Label(label.pos2,label.pos2));
	if(label.getLenght() == forCont->getLenght())
		return  create_pos(forCont->parent,NODE,Label(0,0));
	if(label.getLenght() > forCont->getLenght())
		return fastscan_recurse(forCont->node,Label(forCont->last + 1,label.pos2));
}

Pos* fastscan(Node* node, Label& label)
{
	Pos* pos = fastscan_recurse(node,label);
	return pos;
}

void show(Node* src, int deep)
{
	if(!src)
		return;

	vector<Edge*>::iterator iter;
	for(iter = src->children.begin(); iter != src->children.end();++iter)
	{
		Edge* edge = (Edge*)*iter;
		if(is_leaf(edge))
		{
			for(int i = 0; i < deep	;i++ )printf("\t");
			int f = edge->first ? edge->first : 1;
			printf(formatN,itemcount,currenttext.substr(edge->first,edge->last - edge->first + 1).c_str());
			itemcount++;
		}
		else if(!is_leaf(edge))
		{
			for(int i = 0; i < deep;i++ )printf("\t");
			int f = edge->first ? edge->first : 1;
			printf(format,itemcount,currenttext.substr(edge->first,edge->last - edge->first + 1).c_str());
			itemcount++;
			show(edge->node,deep + 1);
		}
	}
}

int main()
{
	currenttext = "AAAXA"; // baxab  abaab  abaabx   AAAXA
	currenttext += "$";
	lenght = currenttext.length();
	if(lenght == -1)
	{
		printf( "Wrong lenght...");
		system("pause");
	}
	create_init_tree();
	head = root;
	tail = 0;
	for(int i = 0; i < lenght-1 ;i++)
	{

		if(head == root)
		{
			Pos* pos = slowscan(root,Label(i == lenght - 1 ? lenght - 1 : i+1,lenght-1));
			auto newnode = add_element(pos,i+1);
			head = newnode;
			continue;
		}
		Node* u = get_parent(head);
		Label v = get_label(u,head);
		Pos* w = nullptr;
		if(u == root)
			w = fastscan(get_suffix(u),v);
		else
		{
			w = fastscan(root,v);
		}
		Node* newNode = nullptr;
		switch(w->type)
		{
		case EDGE:
			newNode = add_element_edge_fast(w,i+1);
			set_suffix(head,newNode);
			head = newNode;
			break;
		case NODE:
			Pos* p = slowscan((Node*)w->element,Label( i == lenght - 1 ? lenght - 1 : i+1,lenght-1));
			newNode = add_element(p,i+1);
			set_suffix(head,(Node*)w->element);
			head = newNode;
			break;
		}
	}
	itemcount++;
	show(root,1);
	system("pause");
}