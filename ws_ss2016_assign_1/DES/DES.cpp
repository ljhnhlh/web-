#include <iostream>
#include <cstdlib>
#include <fstream>
#include "DES.hpp"
using namespace std;

DES::DES()
{
	bitset<64> ckey("1010101010110101010010101010110101110101010101001001010100010101");
	ChiperKey = ckey;
	Intial_Sbox();
	Generate_key();
}
void DES::initial(string str)
{
	while (str.length() < 8)
	{
		str += '0';
	}
	PlainText = StrToBit(str);
}
void DES::initialChiper(string str)
{
	ChiperText = StrToBit(str);
}
DES::~DES()
{
	SBox.clear();
}
void DES::IP(int num)
{
	int ip[64] = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,
		          46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,
				  25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,
				  13,5,63,55,47,39,31,23,15,7 };
	int t;
	bitset<64> temp;
	if (num == 0)
	{
		temp = PlainText;
	}
	else
	{
		temp = ChiperText;
	}

	for (int i = 0; i < 64; i++)
	{
		t = ip[i]-1;
		Temp[i] = temp[t];			
	}
}

string DES::IP1()
{
	int ip1[64] = { 40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,
					46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,
					52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,
					58,26,33,1,41,9,49,17,57,25 };
	bitset<64> result;
	for (int i = 0; i < 64; i++)
	{
		result[i] = Temp[ip1[i] - 1];
	}

	return BitToStr(result);
}

bitset<32> DES::Feistel(bitset<32>& R0, bitset<48>& k)
{
	//E-expend
	int index[48] = { 32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,
				  	  11,12,13,12,13,14,15,16,17,16,17,
					  18,19,20,21,20,21,22,23,24,25,24,
					  25,26,27,28,29,28,29,30,31,32,1};
	bitset<48> E_expand;
	int t;
	for (int i = 0; i < 48; i++)
	{
		t = index[i] - 1;
		E_expand[i] = R0[t];
	}
	//XOR
	E_expand ^= k;

	//divide into 8 group
	bitset<6> group[8];
	for (int i = 0; i < 8; i++)
	{
		for (int  j = 0; j < 6; j++)
		{
			group[i][j] = E_expand[j + i * 6];
		}
	}

	// use long int strtol(const char *nptr, char **endptr, int base) 
	//to translate any base number to decimalism 
	// use char * itoa(int number , char *str,int base)
	// to translate decimalism to any base number
	bitset<6> row;
	bitset<6> col;
	bitset<6> row_and("100001");
	bitset<6> col_and("011110");
	bitset<6> temp_and("000001");
	vector<bitset<4>> bit;
	int select_row;
	int select_col;
	char **endptr = NULL;
	for (int i = 0; i < 8; i++)
	{
		row = group[i] & row_and;
		row = (row >> 5) | (row & temp_and);
		col = group[i] & col_and;
		col = col >> 1;
		select_row = strtol(row.to_string().c_str(), endptr, 2);
		select_col = strtol(col.to_string().c_str(), endptr, 2);
		int num = S_Block(i, select_row, select_col);
		char str[5] = "";
		//translate num to binary number
		_itoa_s(num, str,5, 2);//4 parameter
		bitset<4> temp(str);
		bit.push_back(temp);
	}

	bitset<32> feistel;
	t = 0;
	for (int i = 0; i < bit.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			feistel[t++] = bit[i][j];
		}
	}
	bit.clear();
	feistel = P_permutation(feistel);

	return feistel;
}


void DES::Generate_key()
{
	int pc[56]  = { 57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,
				   2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,
				   47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,
				   53,45,37,29,21,13,5,28,20,12,4 };

	int pc2[48] = { 14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,
					26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,
					51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32 };
	bitset<28> C0;
	bitset<28> D0;
	int t;
	for (int i = 0; i < 28; i++)
	{
		t     = pc[i]-1;
		C0[i] = ChiperKey[t];
		t     = pc[i + 28] - 1;
		D0[i] = ChiperKey[t];
	}
	t = 1;
	int shift;
	while (t < 17)
	{
		
		switch (t)
		{
		case 1:
		case 2:
		case 9:
		case 16:shift = 1; break;
		default:
			shift = 2;
			break;
		}
		int index;
		C0 = (C0 >> (28 - shift))| (C0 << shift);
		D0 = (D0 >> (28 - shift))| (D0 << shift);
		for (int  i = 0; i < 48; i++)
		{
			index = pc2[i] - 1;
			if (index < 28)
			{
				key[t][i] = C0[index];
			}
			else
			{
				key[t][i] = D0[index - 28];
			}
		}
		t++;
	}
	

}

int DES::S_Block(int i,int row,int col)
{
	int index = i * 64 + row * 16 + col;
	return SBox[index];
}

bitset<32> DES::P_permutation(bitset<32> &feistel)
{
	int p[32] = { 16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,
					24,14,32,27,3,9,19,13,30,6,22,11,4,25 };
	bitset<32>temp;
	for (int i = 0; i < 32; i++)
	{
		temp[i] = feistel[p[i] - 1];
	}
	return temp;
}


void DES::Transfer()
{
	bitset<32> L0;
	bitset<32> R0;
	bitset<32> L1;
	bitset<32> R1;

	for (int i = 0; i < 32; i++)
	{
		L0[i] = Temp[i];
		R0[i] = Temp[i + 32];
	}
	int t = 1;
	bitset<32> feistel;
	while (t <= 16)
	{
		L1 = R0;
		feistel = Feistel(R0, key[t]);
		R1 = L0 ^ feistel;
		L0 = L1;
		R0 = R1;
		t++;
	}
	for (int i = 0; i < 32; i++)
	{
		Temp[i] = R1[i];
		Temp[i + 32] = L1[i];
	}
}

void DES::Intial_Sbox()
{
	string filename = "C:\\Users\\ÁÖ¿¡ºÆ\\Desktop\\DES\\SBoxData.txt";
	ifstream inSboxData;
	inSboxData.open(filename.c_str(), ios::in);
	int t = 0;
	int temp;
	vector<int> VecTemp;
	while (!inSboxData.eof())
	{	
		inSboxData >> temp;
		SBox.push_back(temp);
	}
	inSboxData.close();
}

void DES::InverseTransfer()
{
	bitset<32> A;
	bitset<32> B;
	bitset<32> G;
	bitset<32> H;
	for (int i = 0; i < 32; i++)
	{
		A[i] = Temp[i];
		B[i] = Temp[i + 32];
	}
	int t = 16;
	while (t > 0)
	{
		G = B;//G = L_t = R_t-1;
		H = A ^ Feistel(B, key[t]);//H = Li-1
		B = H;// B = Lt;
		A = G;// A = Rt;
		t--;
	}
	for (int i = 0; i < 32; i++)
	{
		Temp[i] = H[i];
		Temp[i + 32] = G[i];
	}
}
bitset<64>  DES::StrToBit(string a)
{
	bitset<64> bits;
	for (int i = 0; i<8; ++i)
		for (int j = 0; j<8; ++j)
			bits[i * 8 + j] = ((a[i] >> j) & 1);
	return bits;
}

string  DES::BitToStr(bitset<64> a)
{
	bitset<8> temp;
	char **endptr = NULL;
	int num;
	string str = "";
	for (int i = 0; i < 64; i++)
	{
		temp[i % 8] = a[i];
		if (i % 8 == 7)
		{
			num = strtol(temp.to_string().c_str(), endptr, 2);
			char a = num;
			str += a;
		}

	}
	return str;
}