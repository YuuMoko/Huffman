#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <string>
using namespace std;

class Tree
{
public:
	char c = '\0';
	int w;
	Tree *l = NULL;
	Tree *r = NULL;
};

class Node
{
public:
	int w;
	char c;
	Tree *p = NULL;

	bool operator<(const Node &b) const
	{
		return w > b.w;
	}
};

unordered_map<char, string> cTob;

void dfs(Tree *u, string way)
{
	if (u == NULL)
		return;
	char c = u->c;
	if (c != '\0')
		cTob[c] = way;
	dfs(u->l, way + "0");
	dfs(u->r, way + "1");
}

void Encode()
{
	ifstream fin;
	fin.open("./Encode/input.txt");
	string s, str;
	while (getline(fin, s))
	{
		if (str.size())
			str += '\n' + s;
		else
			str += s;
	}
	unordered_map<char, int> Weight;
	priority_queue<Node> heap;

	for (int i = 0; i < str.size(); i++)
		Weight[str[i]]++;

	for (auto t : Weight)
	{
		int w = t.second;
		Tree *p = new Tree({t.first, w});
		heap.push({w, t.first, p});
	}

	while (heap.size() > 1)
	{
		auto t1 = heap.top();
		heap.pop();
		auto t2 = heap.top();
		heap.pop();
		int w = t1.w + t2.w;
		Tree *p = new Tree({'\0', w, t1.p, t2.p});
		heap.push({w, '\0', p});
	}

	Tree *root = heap.top().p;

	dfs(root, "");

	string output;

	for (int i = 0; i < str.size(); i++)
		output += cTob[str[i]];

	fstream fout;
	fout.open("./Encode/output.txt", ios::out);

	fout << output;

	fout.close();

	fout.open("./Encode/PassWord.txt", ios::out);

	for (auto t : cTob)
	{
		fout << t.first << " " << t.second << "\n";
	}
	fout.close();
	fin.close();
	cout << "Finish"
		 << "\n";
}

void build(Tree *u, char word, string way)
{
	if (way.empty())
	{
		u->c = word;
		return;
	}
	char op = way.back();
	way.pop_back();

	if (op == '0')
	{
		if (u->l == NULL)
			u->l = new Tree;
		build(u->l, word, way);
	}
	else if (op == '1')
	{
		if (u->r == NULL)
			u->r = new Tree;
		build(u->r, word, way);
	}
}

void Decode()
{
	fstream fin;
	fin.open("./Decode/PassWord.txt", ios::in);

	char word;
	string way;

	Tree *root = new Tree;

	string line;

	while (getline(fin, line))
	{
		word = line[0];
		way = "";
		if (line[0] == ' ' && line[1] != ' ')
			word = '\n';
		for (int i = 1; i < line.size(); i++)
			if (line[i] == '0' || line[i] == '1')
				way += line[i];
		reverse(way.begin(), way.end());
		build(root, word, way);
	}

	fin.close();
	fin.open("./Decode/input.txt", ios::in);
	fstream fout;
	fout.open("./Decode/output.txt", ios::out);
	Tree *p = root;
	string str;
	getline(fin, str);

	string output;

	for (int i = 0; i < str.size(); i++)
	{
		char op = str[i];

		if (op == '0')
		{
			p = p->l;
			if (p->c != '\0')
			{
				output += p->c;
				p = root;
			}
		}
		else if (op == '1')
		{
			p = p->r;
			if (p->c != '\0')
			{
				output += p->c;
				p = root;
			}
		}
	}

	fout << output << "\n";

	fin.close();
	fout.close();

	cout << "Finish"
		 << "\n";
}

int main()
{
	cout << "1, Encode\n2, Deconde\n0, exit\n";
	int se;
	while (true)
	{
		cTob.clear();
		cin >> se;
		if (se == 1)
			Encode();
		else if (se == 2)
			Decode();
		else if (se == 0)
			exit(0);
		else
			cout << "ERROR" << endl;
	}

	return 0;
}