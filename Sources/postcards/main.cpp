//Открытки и конверты
//	Для поздравлений  сотрудников фирмы было куплено N открыток и М конвертов. Конверты и открытки оказались разных размеров, и некоторые открытки помещаются не во все конверты. Написать программу, находящую такое распределение открыток по конвертам, при котором поздравления получат наибольшее количество людей. В один конверт разрешается вкладывать не более одной открытки.
//	В первой строке входного файла записаны числа М и N ( ). Далее записаны высота и ширина каждого из М конвертов, затем — высота и ширина каждой из N открыток. Размеры конвертов и открыток являются натуральными числами, не превосходящими 32767, и разделяются пробелами и/или символами перевода строки.
//	Выведите в выходной файл целое число К — максимальное количество открыток, которые можно разложить по конвертам. Далее выведите К пар целых чисел, означающих номер открытки и номер конверта, в который ее необходимо положить.
//
//


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <vector>

using namespace std;
using std::string;
using std::vector;

#define SAFE_DELETE(o) if(o != NULL){ delete o;}

struct postcard
{
	float _w, _h;
	int _indexEnvelope;
};

struct envelope
{
	float _w,_h;
	int _indexPostcard;
};

bool read_file(vector<postcard*>& vecPostcard, vector<envelope*>& vecEnvelope)
{
	printf("The input data read from 'postcards.in'.\n");
	ifstream in("postcards.in");
	if(!in.is_open())
	{
		printf("Error while opening file.\n");
		system("pause");
		return false;
	}
	string buf;
	std::getline(in,buf);


	std::getline(in,buf);
	vector<float> listForEnvelope;
	char* pchr = strtok((char*)buf.c_str()," ");
	while(pchr != NULL)
	{
		listForEnvelope.push_back(atof(pchr));
		pchr = strtok(NULL," ");
	}

	std::getline(in,buf);
	vector<float> listForPost;
	pchr = strtok((char*)buf.c_str()," ");
	while(pchr != NULL)
	{
		listForPost.push_back(atof(pchr));
		pchr = strtok(NULL," ");
	}


	in.close();
	for(int i = 0; i < listForPost.size(); i+=2)
	{
		auto post= new postcard();
		post->_h = listForPost.at(i);
		post->_w = listForPost.at(i + 1);
		post->_indexEnvelope = -1;
		vecPostcard.push_back(post);
	}

	for(int i = 0; i < listForEnvelope.size(); i+=2)
	{
		auto env= new envelope();
		env->_h = listForEnvelope.at(i);
		env->_w = listForEnvelope.at(i + 1);
		env->_indexPostcard = -1;
		vecEnvelope.push_back(env);
	}
	return true;
}

bool can_include(const postcard* post, const envelope* env)
{
	float hp = post->_h;
	float wp = post->_w;
	float he = env->_h;
	float we = env->_w;
	float temp,d,l,alpha,beta, gamma,width;
	// попытка разместить паралельно сторонам
	if(he > we)
	{
		temp = he;
		he = we;
		we = temp;
	}
	if(hp > wp){
		temp = hp;
		hp = wp;
		wp = temp;
	}
	if(hp <= he && wp <= we){
		return true;
	}
	if(hp >= he){
		return false;
	}
	//размещение под углом
	d = sqrt(pow(hp,2) + pow(wp,2));
	l = sqrt(pow(d,2) - pow(he,2));
	alpha = atan(he / l);
	beta = atan(hp / wp);
	gamma = alpha - beta;
	width = hp* sin(gamma) + wp * cos(gamma);
	if(width <= we){
		return true;
	}
	return false;
}

void main()
{
	vector<postcard*> vectpost;
	vector<envelope*> vecenv;
	if(!read_file(vectpost,vecenv))
	{
		printf("Error while reading file.\n");
		system("pause");
	}

	for(int i = 0; i< vectpost.size();i++)
	{
		
		for(int j = 0; j < vecenv.size();j++)
		{
			envelope* env = vecenv.at(j);
			if(env->_indexPostcard > -1)
				continue;
			postcard* pos = vectpost.at(i);
			if(can_include(pos,env)){
				pos->_indexEnvelope = j;
				env->_indexPostcard = i;
				break;
			}
		}
	}
	int maxPostcards = 0;
	for(auto iter = vecenv.begin(); iter != vecenv.end(); iter++)
	{
		delete *iter;
	}
	
	for(int i = 0; i <  vectpost.size(); i++)
	{
		auto post = vectpost.at(i);
		if(post->_indexEnvelope > -1)
		{
			maxPostcards++;
			int index =  post->_indexEnvelope;
			cout << i + 1 << " " << index + 1 << "\n";
		}
		delete post;
	}
	cout << "Max count: " << maxPostcards << "\n";
	system("pause");
}