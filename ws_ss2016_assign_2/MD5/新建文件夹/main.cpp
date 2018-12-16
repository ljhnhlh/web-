#include <iostream>
#include "MD5.hpp"
using namespace std;
int main()
{
	string str;
	int t;
	cin >> t;
	while (t!=0)
	{
		cin >> str;
		MD5 a;
		a.strTobit(str);
		cout << a.Encryption() << endl;
		cin >> t;
	}
}