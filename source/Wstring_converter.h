#ifndef WSTRINGCONVERTER
#define WSTRINGCONVERTER

#include <iostream>
#include <string>
using namespace std;

//Convert the string in the first param in a wstring and assign it at the wstring in the second param
void string_to_wstring(string s, wstring& w)
{
 wstring temp(s.begin(), s.end());
 w = temp;
 return;
}

//Convert the string in the first param in a wstring and return it
wstring string_to_wstring(string s)
{
 wstring temp(s.begin(), s.end());
 return temp;
}

//Convert the wstring in the first param in a string and assign it at the string in the second param
void wstring_to_string(wstring w, string& s)
{
 string temp(w.begin(), w.end());
 s = temp;
 return;
}

//Convert the wstring in the first param in a string and return it
string wstring_to_string(wstring w)
{
 string temp(w.begin(), w.end());
 return temp;
}

#endif
