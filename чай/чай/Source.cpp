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

//Сеть Фейстеля 
void Feistel(unsigned int* v, unsigned int* w, unsigned int* k) {
    unsigned int v0 = v[0], v1 = v[1], i, sum = 0;
    unsigned int delta = 0x9E3779B9;//Константа δ
    //РАУДОВ 64,НО ЭТО 32 ЦИКЛА (2 РАУНДА В 1 ЦИКЛЕ)
    for (i = 0; i < 32; i++) {
        cout << "\nНомер цикла в сете Фейстеля=" << i << endl;
        sum += delta;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);//НЕЧЕТНЫЕ РАУНДЫ
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);//ЧЕТНЫЕ РАУНДЫ
        cout << "L(левая часть блока)=" << v0 << endl;
        cout << "R(правая часть блока)=" << v0 << endl;
    }
    w[0] = v0; w[1] = v1;
  
}

void TeaEncode(const std::string& str, const std::string& key, std::string* out)
{
    unsigned int v[2];
    unsigned int w[2];
    unsigned int k[4];
    unsigned int keybuffer[4];
    //memset - заполняет массива (1 параметр) элементами(2 пармаметр) длингой (3 параметр)
    memset(v, 0, sizeof(v));
    memset(w, 0, sizeof(w));
    memset(k, 0, sizeof(k));
    memset(keybuffer, 0, sizeof(keybuffer));
    out->clear();
    int len = key.length();
    if (len > 16)
        len = 16;
    //Копирует байты от ключа в ключ_буфера
    memcpy(keybuffer, key.c_str(), len);
    for (int i = 0; i < 4; ++i) {
        k[i] = keybuffer[i];
    }
    
    //Скопируйте входную строку в буфер размером, кратным 4
    int strbuflen = str.length();
    if (strbuflen == 0)
        return;
    if ((strbuflen % 4) > 0)
        strbuflen += 4 - (strbuflen % 4);

    unsigned char* strbuf = new unsigned char[strbuflen];
    // сначала очищается а потом как заполняется 
    memset(strbuf, 0, strbuflen);
    memcpy(strbuf, str.c_str(), str.length());
    v[1] = 0;
    for (int i = 0; i < strbuflen; i += 4)
    {
        v[0] = *(unsigned int*)&strbuf[i];
        Feistel(&v[0], &w[0], &k[0]);
        out->append((char*)&w[0], 4);
        v[1] = w[1];
    }

    out->append((char*)&v[1], 4);
    delete[] strbuf;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    ifstream fileIn;
    ofstream fileOut;
    string f1, f2, f3;
    string* fileName = &f1; // имя файла
    string* key = &f2; // ключ 
    string* data = &f3; // строка 
    string* name = new string;
    cout << "Введите ключ=";// << endl;
    getline(cin, *key);
    cout << "Введите название файла=";// << endl;
    getline(cin, *fileName);

    fileIn.open(*fileName, ios::in | ios::binary);
    string str((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
    fileIn.close();
    string outdata;
    cout << "Поступившая строка=" << str << " Поступивший ключ" << f2 << endl;
    TeaEncode(str, f2, &outdata);
    str = "0";
    cout << "Введите название файла для сохранения результатов=";// << endl;
    getline(cin, *fileName);
    fileOut.open(*fileName, ios::out | ios::binary);
    fileOut.write(outdata.c_str(), outdata.size());
    // End
    cout << "Пока!" << endl;
    system("pause");

}
