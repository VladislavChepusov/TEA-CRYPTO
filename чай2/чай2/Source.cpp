#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstddef>
#include <vector>
#include <map>
#include <array>

using namespace std;



void Feistel(unsigned int* v, unsigned int* w, unsigned int* k) {
    register unsigned int v0 = v[0], v1 = v[1], i, sum = 0xC6EF3720;
    register unsigned int delta = 0x9E3779B9;
    for (i = 0; i < 32; i++) {
        cout << "\n����� ����� � ���� ��������=" << i << endl;
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum -= delta;
        cout << "L(����� ����� �����)=" << v0 << endl;
        cout << "R(������ ����� �����)=" << v0 << endl;

    }
    w[0] = v0; w[1] = v1;
}

void TeaDecode(const std::string& str, const std::string& key, std::string* out)
{
    unsigned int v[2];
    unsigned int w[2];
    unsigned int k[4];
    unsigned int keybuffer[4];


    memset(v, 0, sizeof(v));
    memset(w, 0, sizeof(w));
    memset(k, 0, sizeof(k));
    memset(keybuffer, 0, sizeof(keybuffer));
    out->clear();


    int numBlocks = str.length() / 4;
    int numPasses = numBlocks - 1;

    if (numPasses <= 0)
        return;

    int len = key.length();
    if (len > 16)
        len = 16;
    memcpy(keybuffer, key.c_str(), len);
    for (int i = 0; i < 4; ++i)
        k[i] = keybuffer[i];

   
    unsigned char* buffer = new unsigned char[numPasses * 4 + 4];
    memset(buffer, 0, numPasses * 4 + 4);


    const char* p = str.c_str();
    v[1] = *(unsigned int*)&p[numPasses * 4];
    for (int i = 0; i < numPasses; ++i)
    {
        v[0] = *(unsigned int*)&p[(numPasses - i - 1) * 4];
        Feistel(&v[0], &w[0], &k[0]);
        *(unsigned int*)&buffer[(numPasses - i - 1) * 4] = w[0];
        v[1] = w[1];
    }

    out->assign((char*)buffer, numPasses * 4);
    delete[] buffer;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    ifstream fileIn;
    ofstream fileOut;

 
    string f1, f2, f3;
    string* fileName = &f1; 
    string* key = &f2; 
    string* data = &f3; 
    string* name = new string;
    cout << "������� ����=";// << endl;
    getline(cin, *key);
    cout << "������� �������� �����=";// << endl;
    getline(cin, *fileName);
    fileIn.open(*fileName, ios::in | ios::binary);
    string str((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
    fileIn.close();
    string outdata;
    cout << "����������� ���������=" <<str<< "����������� ����=" << f2<< endl;
    TeaDecode(str, f2, &outdata);
    str = "0";
    cout << "������� �������� ����� ��� ���������� �����������=";// << endl;
    getline(cin, *fileName);
    fileOut.open(*fileName, ios::out | ios::binary);
    fileOut.write(outdata.c_str(), outdata.size());

 
    cout << "����!" << endl;
    system("pause");

}