#include "MD5.hpp"
#include <iostream>
#include <bitset>
#include <math.h>
using namespace std;

//divide get its subset by range
template<size_t bits>
unsigned long subset(std::bitset<bits> set, int min, int max)
{
	std::bitset<bits> range = ~std::bitset<bits>();
	const int ignore_hi = bits - max;
	range = (range << ignore_hi) >> ignore_hi;
	set &= range;

	return (set >> min).to_ulong();
}
//by me,using the shift
bitset<32>& ModeTwoAdd(bitset<32>& a, bitset<32>& b)
{
	bitset<32> c, c2, s;
	//the first carrier
	c = a & b;
	s = a ^ b;
	// the hightest carrier is neglected
	c2 = c << 1;
	c = s & c2;
	s = s ^ c2;
	// the hightest carrier is neglected
	c2 = c << 1;
	s = s ^ c2;
	// TODO: 在此处插入 return 语句
	return s;
}

MD5::MD5()
{
	CV[0] = 0x67452301;
	CV[1] = 0xEFCDAB89;
	CV[2] = 0x98BADCFE;
	CV[3] = 0x10325476;
	int temp_index[64] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 1, 6,11, 0, 5,10,15, 4, 9, 14, 3, 8,13, 2, 7,12,5, 8,11,14, 1, 4, 7,10,13, 0, 3, 6, 9,12,15, 2,0, 7,14, 5,12, 3,10, 1, 8,15, 6,13, 4,11, 2, 9 };
	int temp_s[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20 , 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23 , 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
	for (int i = 0; i < 64; i++)
	{
		index.push_back(temp_index[i]);
		s.push_back(temp_s[i]);
	}

}

MD5::~MD5()
{
	index.clear();
	s.clear();
}

void MD5::strTobit(string a)
{
	bitset<512> bits;
	string subStr = "";
	MessageFlow.clear();
	int index = 0;
	for (int i = 0; i < a.length(); i++)
	{

		for (int j = 0; j<8; ++j)
			bits[index++] = ((a[i] >> j) & 1);
		if (index == 512)
		{
			index = 0;
			MessageFlow.push_back(bits);
		}
	}
	char str[65] = "";
	long int k = MessageFlow.size() * 512 + index;
	_itoa_s(k, str, 65, 2);
	bitset<64> temp(str);

	//padding
	int t = 0;
	bits[index++] = 1;
	t++;
	while ((t + index) % 512 != 448)
	{
		if (index == 512)
		{
			index = 0;
			MessageFlow.push_back(bits);
		}
		bits[index++] = 0;
	}
	//add message length
	for (int i = 0; i < 64; i++)
	{
		bits[index++] = temp[i];
	}
	MessageFlow.push_back(bits);
}

void MD5::HMD5(int q)
{
	bitset<32> X;
	int signal = 0;
	bitset<32> T;
	bitset<32> temp1;
	bitset<32> g_return;
	for (int j = 0; j < 64; j+=16)
	{
		for (int i = j; i < j + 16; i++)
		{
			X = subset(MessageFlow[q], index[i] * 32, i * 32 + 32);
			T = abs(int(pow(2, 32)*sin(i)));
			g_return = g(signal);
			temp1 = ModeTwoAdd(CV[0],g_return);
			temp1 = ModeTwoAdd(temp1, X);
			temp1 = ModeTwoAdd(temp1, T);
			temp1 = temp1 << s[i];
			CV[0] = ModeTwoAdd(CV[1], temp1);
			temp1 = CV[0];
			CV[0] = CV[1];
			CV[1] = CV[2];
			CV[2] = CV[3];
			CV[3] = temp1;
		}
		signal++;
	}

}

bitset<32> MD5::g(int i)
{
	switch (i)
	{
	case 0:return ( CV[1] & CV[2] ) | (CV[1].flip() & CV[4]);
	case 1:return ( CV[1] & CV[3] ) | (CV[2] & CV[4].flip());
	case 2:return CV[1] ^ CV[2] ^ CV[3];
	case 3:return CV[2] ^ ( CV[1] | CV[4].flip());
	default:
		break;
	}
	return bitset<32>();
}

bitset<128> MD5::Encryption()
{
	CV[0] = 0x67452301;
	CV[1] = 0xEFCDAB89;
	CV[2] = 0x98BADCFE;
	CV[3] = 0x10325476;

	for (int i = 0; i < MessageFlow.size(); i++)
	{
		HMD5(i);
	}
	bitset<128> temp;
	bitset<128> result;
	for (int i = 0; i < 4; i++)
	{
		cout << CV[i] << endl;
	}
	for (int i = 0; i < 32; i++)
	{
		result[i] = CV[3][i];
		result[i+32] = CV[2][i];
		result[i+64] = CV[1][i];
		result[i+96] = CV[0][i];

	}
	return result;
}




