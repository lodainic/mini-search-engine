#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

vector<string> readFile(string docsName);
vector<string> mySteamming(vector<string> words, vector<string> suff,
                           vector<string> pref);
vector<string> eraseSym(vector<string> words);
vector<string> minimizeLetts(vector<string> words);
string eraseSuff(string word, string suff);
string erasePref(string word, string pref);
int wrdFreq(vector<string> words, int indx);

int main()
{
    vector<string> words, suff, pref, docsName;
    /*  wordsMap = hash table with keys as words from the documents 
        and values as a vector containing occurences of the word 
        in the documents whose name coressponds to index */
    unordered_map<string, vector<int>> wordsMap;
    int ind, wordFreq;
    string srchWord;

    docsName = readFile("docs/docsName.txt");
    suff = readFile("docs/suffixes.txt");
    pref = readFile("docs/prefixes.txt");

    for (auto itv = docsName.begin(); itv != docsName.end(); itv++)
    {
        words = readFile("docs/" +*itv);
        words = mySteamming(words, suff, pref);
        for (int i = 0; i < words.size(); i++)
        {
            wordFreq = wrdFreq(words, i);
            wordsMap[words[i]].push_back(wordFreq);
        }
    }

    cout << "********************SEARCH********************" << endl;
    cout << "***************  ";
    getline(cin, srchWord);

    for (int i = 0; i < suff.size(); i++)
        srchWord = eraseSuff(srchWord, suff[i]);
    for (int i = 0; i < pref.size(); i++)
        srchWord = erasePref(srchWord, pref[i]);

    if (wordsMap.find(srchWord) != wordsMap.end())
    {
        for (int i = 0; i < docsName.size(); i++)
        {
            cout << "Doc" << i + 1 << " has " << wordsMap[srchWord][i] << " occurrences." << endl;
        }
    }
    else
    {
        cout << "No occurrences of " << srchWord << " found." << endl;
    }

    return 0;
}

vector<string> readFile(string docsName)
{
    vector<string> words;
    ifstream fin(docsName);
    string w;
    if (!fin)
    {
        cerr << "Unable to open file" << docsName;
        exit(1);
    }
    while (fin >> w)
    {
        words.push_back(w);
    }
    fin.close();
    return words;
}

int wrdFreq(vector<string> words, int indx)
{
    int freq = 1;

    for (int j = 0; j < words.size(); j++)
    {
        if (words[indx] == words[j] && indx != j)
        {
            freq++;
        }
    }
    return freq;
}

//***************************MY_STEAMMING***************************
vector<string> mySteamming(vector<string> words, vector<string> suff,
                           vector<string> pref)
{
    words = eraseSym(words);
    words = minimizeLetts(words);
    for (int i = 0; i < words.size(); i++)
        for (int j = 0; j < suff.size(); j++)
            words[i] = eraseSuff(words[i], suff[j]);
    for (int i = 0; i < words.size(); i++)
        for (int j = 0; j < pref.size(); j++)
            words[i] = erasePref(words[i], pref[j]);
    return words;
}
vector<string> minimizeLetts(vector<string> words)
{
    for (int i = 0; i < words.size(); i++)
    {
        if (words[i][0] > '@' && words[i][0] < '[')
            words[i][0] = words[i][0] + 32;
    }
    return words;
}
vector<string> eraseSym(vector<string> words)
{
    int len, pos, i = -1;
    string str;
st:
    i++;
    len = words[i].length() - 1;
    //erase punctuation marks & numbers:
    while (len >= 0)
    {
        if (words[i][len] >= '!' && words[i][len] <= '.' ||
            words[i][len] >= '0' && words[i][len] <= '@' ||
            words[i][len] >= '[' && words[i][len] < 'a' ||
            words[i][len] >= '{' && words[i][len] <= '~')
        {
            if (words[i][len + 1] >= 'A' && words[i][len + 1] <= 'Z' ||
                words[i][len + 1] >= 'a' && words[i][len + 1] <= 'z' &&
                    words[i][len - 1] >= 'A' && words[i][len - 1] <= 'Z' ||
                words[i][len - 1] >= 'a' && words[i][len - 1] <= 'z' &&
                    words[i][len] == '-' ||
                words[i][len] == '.')
            {
                words[i][len] = ' ';
                pos = words[i].find(' ');
                str = words[i].substr(pos + 1);
                words.push_back(str);
                words[i].erase(words[i].begin() + pos, words[i].end());
            }
            words[i].erase(len, 1);
        }
        len--;
    }
    if (i <= words.size())
        goto st;

    //delete left 0 length elements :
    for (i = words.size() - 1; i >= 0; i--)
    {
        if (words[i].length() == 0)
            words.erase(words.begin() + i);
    }
    return words;
}
string eraseSuff(string word, string suff)
{
    int n = 0, len, slen;
    //particular
    if (word == "has")
        word = "have";
    if (word == "is" || word == "the" ||
        word == "have" || word == "m/s" ||
        word == "with" || word == "not")
    {
        return word;
    }
    else
    {
        len = word.length() - 1;
        slen = suff.length() - 1;
        while (slen >= 0)
        {
            if (word[len] == suff[slen])
                n++;

            len--;
            slen--;
        }
        if (n == suff.length())
            word.erase(word.length() - n, n);

        return word;
    }
}
string erasePref(string word, string pref)
{
    int n = 0, i = 0, j = 0, len, plen;
    //particular
    if (word == "a" || word == "in" ||
        word == "and" || word == "at")
    {
        return word;
    }
    else
    {
        len = word.length() - 1;
        plen = pref.length() - 1;
        while (i <= plen)
        {
            if (word[i] == pref[j])
                n++;

            i++;
            j++;
        }
        if (n == pref.length())
            word.erase(0, n);

        return word;
    }
}
