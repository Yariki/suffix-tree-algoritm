
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <vector>

using namespace std;
using std::string;
using std::vector;

#define SAFE_DELETE(o) if(o != NULL){ delete o;}


string _mainPattern;
int _lenght = -1;


bool compareStr (const string& left,const string& right)
{
	return left.compare(right) < 0;
}

string shift(string& input)
{
	char ch = input.front();
	string temp (input.substr(1,input.length() - 1));
	temp.push_back(ch);
	return temp;
}


vector<string>* make_shift_array(string& input)
{
	auto vect = new vector<string>();
	string last = input;
	vect->push_back(last);
	for(int i = 1; i < input.length(); i++)
	{
		last = shift(last);
		vect->push_back(last);
	}
	return vect;
}

int compare_str(string& str1, string& str2)
{
	int count = 0;
	for(int i = 0; i < str1.length();i++)
		if(str1[i] == str2[i]) 	count++;
		else break;

	return count;
}

float calculate_acp(vector<string>* vect, vector<int>* vectCalc, int lenght)
{
	int sum = 0;
	for(int i = 0 ; i < vect->size() - 1; i++)
	{
		int count = compare_str(vect->at(i), vect->at(i + 1));
		sum+=count;
		vectCalc->push_back(count);
	}

	return (float)sum / (float) (lenght - 1);
}


void show_info()
{
	printf(" Average Common Prefix \n");	
	printf("Input:  The first line of the input contains integer n (1 < n < 250001). The second line contains string T.\n");	
	printf("Output: The only line of the output should contain the Average LCP of T with 3 digits after decimal point.\n");	
	
}


void main()
{
	show_info();
	char* input = new char[255];
	printf("The input data read from 'in.txt'.\n");
	
	ifstream in("in.txt");
	if(!in.is_open())
	{
		printf("Error while opening file.");
		system("pause");
		return;
	}
	char lenght[255];
	in >> lenght;
	in >> input;

	_mainPattern = string(input);//"MISSISSIPPI";

	if(_mainPattern.empty())
	{
		printf("String is empty.");
		system("pause");
		return;
	}


	auto v = make_shift_array(_mainPattern);

	vector<string>::iterator begin = v->begin();
	vector<string>::iterator end = v->end();
	std::sort(begin,end,compareStr);
	vector<int>* coutForEach = new vector<int>();
	float acp = calculate_acp(v,coutForEach,_mainPattern.length());

	ofstream out;
	out.open("out.txt");
	for(int i = 0; i < v->size();i++)
	{
		printf("%s\t%d\n",v->at(i).c_str(), i >= coutForEach->size() ? 0 : coutForEach->at(i));
		int s = i >= coutForEach->size() ? 0 : coutForEach->at(i);
		out << v->at(i).c_str() << "\t" << s << "\n";
	}
	printf("Average LCP =  %f\n",acp);		
	out << "Average LCP = " << acp << "\n";

	in.close();
	out.close();
	SAFE_DELETE(coutForEach);
	SAFE_DELETE(v);
	SAFE_DELETE(input);
	system("pause");
}
