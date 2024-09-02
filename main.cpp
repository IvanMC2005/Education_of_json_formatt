#define NOMIXMAX
#include <cstdio>
#include <cstring>
#include<limits>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>
#include <clocale>
#include <ctime>
#include <string>
#include <regex>
#include <algorithm>
#include <random>
#include <list>
#include "json.h"
using namespace std;
int max_valuepar2 = 0;
struct Filelist
{
    char name[10];
    int main_par;
    int secondary_par;
};
Filelist temper;
bool compare(const Filelist& a, const Filelist& b)
{
    return strlen(a.name) > strlen(b.name);
}
bool is_even(Filelist);
void print(Filelist & item)
{
    cout << "Название " << " Основной параметр " << " Вторичный параметр " << endl;
    cout << left << setw(10) << item.name << left << setw(19) << item.main_par << left << setw(10) << item.secondary_par<< endl;
    cout << endl;
}
bool isFilled(const Filelist& item)
{
    if (item.name[0]) return true;
    return false;
}
int main()
{
    setlocale(0, "rus");
    int menu;
    char ch;
    do
    {
        cout << "1 - работать с существующим файлом json\n2 - Создать новый на основе заданного файла\n3 - Выход\n";
        cin >> menu;
        while((ch = getchar()) != '\n');
        while(cin.fail())
        {
            cin.clear();
            cout << "1 - работать с существующим файлом json\n2 - Создать новый на основе заданного файла\n3 - Выход\n";
            cin >> menu;
            while((ch = getchar()) != '\n');
        }
        switch(menu)
        {
            case 1:
            {
                Filelist temper;
                ifstream in;
                list<Filelist>temp;
                in.open("Result.json");
                if (!in.is_open())
                {
                    cout << "Ошибка открытия файла\n";
                    return -1;
                }
                while(in.read(( char *)&temper.name, sizeof(char)*10))
                {
                    in.read(( char *)&temper.main_par, sizeof(int));
                    in.read(( char *)&temper.secondary_par, sizeof(int));
                    print(temper);
                }
                in.close();
                break;
            }
            case 2:
            {
                ifstream in;
                ofstream out;
                in.open("Text.txt");
                if (!in.is_open())
                {
                    cout << "Ошибка открытия файла!\n";
                    return -1;
                }
                set<string> Names;
                int num1, num2, num3;
                auto it = Names.begin();
                list<Filelist> task_list;
                set<char> temp;
                map<char, int> freq_dict;
                auto pdict = freq_dict.begin();
                string mystr;
                getline(in, mystr);
                smatch result;
                for (char i = 65; i < 91; i++)
                {
                    freq_dict.emplace(i, 0);
                }
                for (char i = 97; i < 123; i++)
                {
                    freq_dict.emplace(i, 0);
                }
                while (regex_search(mystr, result, regex("([^#]+){1}")) || regex_search(mystr, result, regex("[^#]+#*([^#]+)")))
                {
                    Names.insert(result.str());
                    mystr = result.suffix();
                }
                for (auto i: Names)
                {
                    for (auto j:i)
                        temp.insert(j);
                    for (auto j:temp)
                    {
                        for (auto& z:freq_dict)
                        {
                            if (j == z.first)
                            {
                                z.second++;
                                break;
                            }
                        }
                    }
                    temp.clear();
                }
                for (int n{0}; n < 100; n++)
                {
                    it = Names.begin();
                    advance(it, rand() % Names.size());
                    int j = 0;
                    for (char * i = const_cast<char *>((*it).c_str()); j < 10; i++, j++)
                    {
                        temper.name[j] = *i;
                    }
                    advance(pdict, rand() % freq_dict.size());
                    num1 = (*pdict).second;
                    pdict = freq_dict.begin();
                    advance(pdict, rand() % freq_dict.size());
                    num2 = (*pdict).second;
                    pdict = freq_dict.begin();
                    advance(pdict, rand() % freq_dict.size());
                    num3 = (*pdict).second;
                    pdict = freq_dict.begin();
                    temper.main_par = num1 * num2 * num3;
                    temper.secondary_par = freq_dict[temper.name[0]];
                    max_valuepar2 = max(temper.secondary_par, max_valuepar2);
                    task_list.push_back(temper);
                }
                task_list.sort(compare);
                cout << endl << endl;
                list<Filelist> newtask_list(task_list.size());
                copy_if(begin(task_list), end(task_list), begin(newtask_list), is_even);
                newtask_list.erase(std::remove_if(newtask_list.begin(), newtask_list.end(), [](const Filelist& item) { return isFilled(item); }), newtask_list.end());
                out.open("Result.json");
                if (!out.is_open())
                {
                    cout << "Не удалось открыть файл\n";
                    return -1;
                }
                for (auto i:task_list)
                    print(i);
                for (auto i:task_list)
                {
                    out.write(( char *)&i.name, sizeof(char)*10);
                    out.write(( char *)&i.main_par, sizeof(int));
                    out.write(( char *)&i.secondary_par, sizeof(int));
                }
                out.close();
                in.close();
                break;
            }
        }
    }
    while(menu != 3);
    return 0;
}
bool is_even(Filelist n)
{
    return max_valuepar2 == n.secondary_par;
}
