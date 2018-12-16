#include <iostream>
#include <string>
#include "DES.hpp"
using namespace std;
int main()
{
	int sign;
	cout << "input 0 to encode\ninput 1 to decode\ninput 2 to get the chiper text\n";
	string a;
	string b;
	string str = "";
	string plain = "";
	DES des;
	while (1)
	{
		cin >> sign;
		if (sign == 0)
		{
			str = "";
			cout << "please input the message" << endl;
			cin >> a;
			for (int i = 0; i < a.length(); i+= 8)
			{
				b = a.substr(i, 8);
				des.initial(b);
				des.IP(0);
				des.Transfer();
				str += des.IP1();
			}
			cerr << str << endl;
		}
		else if (sign == 1) {
			if (str.length() == 0) {
				cout << "please encode first" << endl;
				continue;
			}
			plain = "";
			for (int i = 0; i < str.length(); i += 8)
			{
				b = str.substr(i, 8);  
				des.initialChiper(b);
				des.IP(1);
				des.InverseTransfer();
				plain += des.IP1();
			}
			if (a.length() % 8 != 0)
			{
				plain = plain.substr(0, a.length());
			}
			cout << plain << endl;

		}
		else if (sign == 2) {
			if (str.length() == 0) {
				cout << "please encode first" << endl;
				continue;
			}
			cout << str << endl;
		}


	}
/*
	DES b(a);
	b.IP(0);
	b.Transfer();
	b.IP1(0);
	b.print();
	b.IP(1);
	b.InverseTransfer();
	b.IP1(1);
	b.print1();
	*/
	return 0;
}