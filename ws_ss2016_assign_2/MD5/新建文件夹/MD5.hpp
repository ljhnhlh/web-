#pragma once
#include <iostream>
#include <bitset>
#include <vector>
using namespace std;

class MD5
{  
public:
	MD5();
	~MD5();
	void strTobit(string a);//used to transfer string to bit,each 64 words make up an item of vector
	void HMD5(int q);
	bitset<32> g(int i);
	bitset<128> Encryption();
private:
	vector<bitset<512>> MessageFlow;
	vector<int> index;
	vector<int> s;
	bitset<32> CV[4];
	
};
