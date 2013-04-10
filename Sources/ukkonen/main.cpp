
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <iostream>

using std::string;


enum ActionType
{
	DONOTHING,
	CONTINUE,
	SPLIT,
	ADDEDGE
};


struct Node;

struct Edge
{
	Edge* next; // for linked list
	Node* parentnode;
	Node* node; // node on the end of edge // if nullptr -> leaf
	int first; //index of beginning char
	int last; // index of last char
};

struct Node
{
	Edge* edgies;
	Node* suffixLink;  
};

struct Position
{
	Edge* edge;
	Node* node;
	int offset;
	bool process;
};


Node *root;
Node *current;
string currenttext;
int lenstr = -1;
int itemcount = 0;
char* format = "(%d:%s)";
char* formatN = "(%d:%s):leaf\n\n";


bool  is_leaf(Edge* edge)
{
	return edge->node == nullptr;
}

Node* create_root()
{
	Node* node = new Node();
	node->edgies = nullptr;
	node->suffixLink = nullptr;
	return node;
}

Edge* create_edge(Node* parent)
{
	Edge* edge = new Edge();
	edge->next = nullptr;
	edge->parentnode = parent;
	edge->node = nullptr;
	edge->first = -1;
	edge->last = -1;

	return edge;
}

Node* create_node()
{
	auto node = new Node();
	node->suffixLink = nullptr;
	node->edgies = nullptr;
	return node;
}

Position* create_position(Edge* edge,Node* node, int offset, bool process)
{
	auto pos = new Position();
	pos->edge = edge;
	pos->node = node;
	pos->offset = offset;
	pos->process = process;
	return pos;
}

void add_edge(Node* node, int index,int last, Node* cont = nullptr)
{
	if(!node->edgies)
	{
		Edge* edge = create_edge(node);
		edge->first = index;
		edge->last = last;
		node->edgies = edge; // add first children edge to node
		edge->node = cont;
	}
	else
	{
		Edge* lastEdge = node->edgies;
		while(lastEdge->next)
			lastEdge = lastEdge->next;
		Edge* newedge = create_edge(node);
		newedge->last = last;
		newedge->first = index;
		lastEdge->next = newedge;
		newedge->node = cont;
	}
}

Position* find_recursive(Node* node, int j, int i, int offset)
{
	auto edge = node->edgies;
	while(edge)
		if(currenttext[j] == currenttext[edge->first]) 	break;
		else edge = edge->next;
	if(!edge)
		return nullptr;
	int f = edge->first;
	while(f <= edge->last && j <= i)
		if(currenttext[f++] != currenttext[j++])
			break;
	if(edge->node && j <= i)
	{
		find_recursive(edge->node,j,i,i-j);
	}
	else
		return create_position(edge,edge->node,f < edge->last ? f : edge->last,f == edge->last);//f < edge->last ? f : edge->last
}

Position* find_pos(int j, int i)
{
	int offset = i - j;
	return find_recursive(root,j,i,offset);
}

void split_edge(Edge* edge,int offset,int i)
{
	auto newnode = create_node();
	auto temp = edge->node;
	edge->node = newnode;
	// children edge
	int newfirst = offset;
	int newlast = edge->last;
	edge->last = offset - 1;
	add_edge(newnode,newfirst,newlast,temp);
	add_edge(newnode,i,i);
}

void show(Node* src, int deep)
{
	if(!src)
		return;
	Edge* edge = src->edgies;
	while(edge)
	{
		
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
		edge = edge->next;
	}
}

bool is_need_continue(Node *node, int i)
{
	Edge* edge = node->edgies;
	while(edge)
	{
		if(currenttext[i] == currenttext[edge->first])
			return false;
		edge = edge->next;
	}
	return true;
}

ActionType get_action(Position* pos, int j, int i)
{
	if(!pos->process)
	{
		if(is_leaf(pos->edge) && pos->edge->last == pos->offset)
			return CONTINUE;
		if(!is_leaf(pos->edge) && pos->edge->last == pos->offset && j < i && is_need_continue(pos->node,i))
			return ADDEDGE;
		if(pos->offset < pos->edge->last)
		{
			if(currenttext[pos->offset] != currenttext[i])
				return SPLIT;
			if((currenttext[pos->offset] == currenttext[i] && j < i) || j == i)
				return DONOTHING;
		}
	}
	else if(!is_leaf(pos->edge) && pos->offset == pos->edge->last && currenttext[pos->offset] != currenttext[i])
	{
		return SPLIT;
	}
	return DONOTHING;
}


int main()
{
	currenttext = "ASTALAVISTABABY";// abaabx MISSISSIPPI   ABRACADABRA    Woolloomooloo ASTALAVISTABABY THEGREATALBANIANFUTURE  AAAAABCDEAAAAA
	root = create_root();
	add_edge(root,0,0);
	lenstr = currenttext.length();
	for(int i = 1; i < lenstr;i++)
	{
		//phase
		for(int j = 0; j <= i ; j++)
		{
			//extension
 			auto pos = find_pos(j,i-1);
			if(!pos)
			{
				add_edge(root,i,i);
			}
			else
			{
				ActionType act = get_action(pos,j,i);
				switch(act)
				{
					case CONTINUE:
						pos->edge->last = i;
						break;
					case SPLIT:
						split_edge(pos->edge,pos->offset,i);
						break;
					case ADDEDGE:
						add_edge(pos->node,i,i);
						break;
				}
			}
			delete pos;
		}
	}
	itemcount++;
	printf("tree:\n");
	show(root,1);
	system("pause");
}
