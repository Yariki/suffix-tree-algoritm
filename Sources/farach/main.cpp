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

bool compareStr (const string& left,const string& right)
{
	return left.compare(right) < 0;
}

bool equal_to_Str(const string& left,const string& right)
{
	return left.compare(right) == 0;
}


int main()
{
	string str = "mississippi";

	int i = str.length() % 2;
	printf("%d\n",i);
	if(i){
		str += "$";
	}
	i = str.length()  % 2;
	printf("%d\n",i);
	printf("\n");

	vector<string> vecString;
	i = 0;
	while(i < str.length())
	{
		string s = str.substr(i,2);
		vecString.push_back(s);
		i+=2;
	}


	vector<string>::iterator begin = vecString.begin();
	vector<string>::iterator end = vecString.end();

	std::sort(begin,end,compareStr);

	for(int i = 0; i < vecString.size();i++)
	{
		printf("%s\n",vecString.at(i).c_str());
	}
	printf("%s\n","-----------------");

	vector<string>::iterator last = unique(vecString.begin(),vecString.end(),equal_to_Str);

	for(auto iter = vecString.begin(); iter != last;++iter)
	{
		printf("%s\n",((string)*iter).c_str());
	}

	system("pause");
}