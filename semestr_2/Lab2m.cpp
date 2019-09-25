#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <deque>

#define FINISH -1

using namespace std;

struct Connect
{
	char value;
	float weight;
	bool was = 0;
};
class Vertex
{
public:
	char key;
	Connect *connects;
	size_t size_v = 0;

	void resize_con(Vertex& ver)
	{
		if(ver.size_v%3 != 0 && ver.size_v) return;
		Connect *tmp = new Connect[ver.size_v + 3];
		for(int i = 0; i < ver.size_v; i++)
			tmp[i] = ver.connects[i];
		delete [] ver.connects;
		ver.connects = tmp;
	};
};

class Directed_Graph{
private:
	Vertex *list;
	size_t size_l = 0;
public:
	Directed_Graph()
	: list(new Vertex[10])
	{};

	void print()
	{
		for(int i = 0; i < size_l; i++)
		{
			cout << list[i].key << " : ";
			for(int j = 0; j < list[i].size_v; j++)
				cout << list[i].connects[j].value << "(" << list[i].connects[j].weight << ") ";
			cout << endl;
		}
	};

	void push(const char key, const char value, const float weight)
	{
		find_or_add(value);
		Vertex &v = list[find_or_add(key)];
		v.resize_con(v);
		v.connects[v.size_v].value = value;
		v.connects[v.size_v].weight = weight;
		v.size_v++;
	};
	size_t find_or_add(const char litter)
	{
		for(int i = 0; i < size_l; i++)
			if(list[i].key == litter)
				return i;
		resize_list();
		list[size_l].key = litter;
		return size_l++;
	};
	void resize_list()
	{
		if(size_l%10 != 0 || !size_l) return;

		Vertex *tmp = new Vertex[size_l + 10];
		copy(tmp, tmp + size_l, list);
		for(int i = 0; i < size_l; i++)
			delete [] list[i].connects;
		delete [] list;
		list = tmp;
	};

	int find_way(const size_t start, const size_t finish, deque <char> &way)
	{
		way.push_back(list[start].key);
		if(start == finish)
			return FINISH;
		size_t min;
		while(find_min(list[start], min))
		{
			size_t new_start = find_or_add(list[start].connects[min].value);
			if(find_way(new_start, finish, way) == FINISH)
				return FINISH;
			way.pop_back();
		}
//		way.pop_back();
		return 0;
	};
	bool find_min(Vertex &ver, size_t &min)
	{
		if(ver.size_v == 0) return 0;
		min = -1;
		for(int i = 0; i < ver.size_v; i++)
			if(!ver.connects[i].was)
				min = i;
		if(min == -1)
			return 0;
		for(int i = 0; i < ver.size_v; i++)
		{
			if(ver.connects[i].weight < ver.connects[min].weight && !ver.connects[i].was)
				min = i;
		}
		ver.connects[min].was = 1;
		return 1;
	}
};

int main(){
	char start, finish;
	cin >> start >> finish;

	Directed_Graph graf;
	char from, to;
	float weight;
	while(cin >> from >> to >> weight)
	{
		if(from == '^')
			break;
		graf.push(from, to, weight);
	}

//	graf.print();
	deque <char> way;
	if(!graf.find_way(graf.find_or_add(start), graf.find_or_add(finish), way))
		return 0;
//	cout << endl;

	while(!way.empty())
	{
		cout << way.front();
		way.pop_front();
	}
	return 0;
}
