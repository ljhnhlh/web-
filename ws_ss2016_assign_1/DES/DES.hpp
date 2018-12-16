#ifndef DES_HPP
#define DES_HPP
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
using namespace std;
class DES
{
public:
	DES();
	~DES();
	void initial(string str);
	void initialChiper(string str);
	void IP(int);
	string IP1();
	bitset<32> Feistel(bitset<32>& R0,bitset<48> &k);//轮函数
	void Generate_key();
	int S_Block(int i, int row, int col);
	bitset<32> P_permutation(bitset<32> &feistel);//P置换
	void Transfer();//加密过程的子密钥调度
	void Intial_Sbox();//从本地加载S-盒数据
	void InverseTransfer();//解密过程的子密钥调度
	bitset<64> StrToBit(string );
	string BitToStr(bitset<64> a);
private:
	std::bitset<64> PlainText;
	std::bitset<64> ChiperText;
	std::bitset<64> ChiperKey;
	std::bitset<64> Temp;
	std::bitset<48> key[17];
	vector<int> SBox;
	
};
#endif