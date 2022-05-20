#include <bits/stdc++.h>
#include "./Library/SymbolTableFactory.h"
#include "./Library/SymbolTable.h"

using namespace std;

constexpr unsigned int str2int(const char* str, int h=0) {
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

bool beginsWith(string s, string special) {
    for (int i = 0; i < special.length(); i++)
        if (s.front() == special[i])
            return true;
    return false;
}

bool endsWith(string s, string special) {
    for (int i = 0; i < special.length(); i++)
        if (s.back() == special[i])
            return true;
    return false;
}

int main() {
    char structure[3];
    cin >> structure;

    SymbolTable<string,int>* st = SymbolTableFactory<string,int>().createST(structure);
    // cout << st->show() << endl;
    int n_words = 0;
    cin >> n_words;
    string words[n_words];
    for (int i = 0; i < n_words; i++) {
        string input;
        cin >> input;
        words[i] = input.substr((beginsWith(input,"{[(")?1:0),(endsWith(input,"}]):;.,!?")?-1:0)+input.length());
    }
    int n_commands = 0, iterator = 0;
    cin >> n_commands;
    while (n_commands-- > 0) {
        int command = 0;
        cin >> command;
        switch (command) {
        case 1: {
            int i_value = 0;
            cin >> i_value;
            while ((--i_value) >= 0) {
                if (iterator >= sizeof(words)/sizeof(words[0]))
                    break;

                // st->add(words[iterator++],1);
                int _rank = st->rank(words[iterator]);
                string search_key = st->select(_rank);
                if (words[iterator] == search_key)
                    st->add(words[iterator++],st->value(search_key)+1);
                else
                    st->add(words[iterator++],1);
            }
            break;
        } case 2: {
            string s_value = "";
            cin >> s_value;
            string search_key = st->select(st->rank(s_value));
            if (search_key == s_value)
                cout << st->value(s_value) << endl;
            else
                cout << "0" << endl;
            break;
        } case 3: {
            string s_value;
            cin >> s_value;
            cout << st->rank(s_value) << endl;
            break;
        } case 4: {
            int i_value;
            cin >> i_value;
            cout << st->select(i_value-1) << endl;
            break;
        } default:
            break;
        }
    }
}