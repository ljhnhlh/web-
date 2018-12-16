# Attention

因为S-盒数据过多，所以在实现的过程中采取了读取本地的S-盒数据的方法，因此 运行前需要把 `DES.cpp` 中的Initial_Sbox() 的路径改成所在的本地路径，否则无法读取键盘输入

```c++
void DES::Intial_Sbox()
{
	string filename = "C:\\Users\\林俊浩\\Desktop\\DES\\SBoxData.txt";
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
```

## Some problem

有时，任意输入时若输出密文，会导致程序无法继续执行，一般密文以 `?`结尾的都无法继续，个人认为是输出的乱码中有些控制系统的二进制串，导致程序终止，不过出现机率较小，请TA随性输入。