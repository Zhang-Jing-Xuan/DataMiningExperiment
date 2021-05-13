#include "FPGrowth.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

#pragma - mark public method

FPGrowth::FPGrowth(float _minSupport)
{
    minSupport = _minSupport;
    root = new FPTreeNode("", 0, NULL);
}

void FPGrowth::initFromFile(string fileName)
{
    start = clock();
    cout << "Init from file" << fileName << "." << endl;
    flush(cout);
    buildHeaderTable(fileName);
    buildFPTree(fileName);
    prefix = NULL;
}

void FPGrowth::outputToFile(string fileName)
{
    ofstream fout;
    fout.open(fileName);
    if (!fout)
    {
        cout << "Cannot open the output file." << endl;
        return;
    }
    vector<FreqResult> result;
    miningFPTree(result);
    cout << fixed;
    cout.precision(2);
    cout << "The time use for mining is" << (clock() - start) * 1.0 / CLOCKS_PER_SEC << endl;
    for (auto line : result)
    {
        for (auto i : line.trans)
        {
            fout << i.name << " ";
        }
        fout << " : "<<line.count << endl;
    }
    cout << fixed;
    cout.precision(2);
    cout << "The time use for mining and outputing is" << (clock() - start) * 1.0 / CLOCKS_PER_SEC << endl;
    fout.close();
}

void FPGrowth::initFromTrans(vector<FPTrans> &trans)
{
    buildHeaderTable(trans);
    buildFPTree(trans);
}

#pragma - mark private method

void FPGrowth::buildHeaderTable(string fileName)
{
    ifstream fin;
    fin.open(fileName);
    if (!fin)
    {
        cout << "Cannort open the input file." << endl;
        return;
    }
    string line;
    int transCount = 0;
    int index = 0;
    while (!getline(fin, line).eof())
    {
        transCount++;
        stringstream ss(line);
        string item;
        while (ss >> item)
        {
            if (nameIndex.find(item) != nameIndex.end())
            {
                headerTable[nameIndex[item]]->count++;
            }
            else
            {
                nameIndex[item] = index;
                FPHeaderTable *ft = new FPHeaderTable(item, 1, NULL);
                headerTable.push_back(ft);
                index++;
            }
        }
    }
    fin.close();
    minSupportCount = minSupport * transCount;
    for (auto it = headerTable.begin(); it != headerTable.end(); it++)
    {
        if ((*it)->count < minSupportCount)
        {
            it = headerTable.erase(it);
            it--;
        }
    }
    sortHeaderTable();
}

void FPGrowth::buildHeaderTable(vector<FPTrans> &trans)
{
    int index = 0;
    for (auto line : trans)
    {
        for (auto i : line.trans)
        {
            if (nameIndex.find(i.name) != nameIndex.end())
            {
                headerTable[nameIndex[i.name]]->count += i.count;
            }
            else
            {
                nameIndex[i.name] = index;
                FPHeaderTable *ft = new FPHeaderTable(i.name, i.count, NULL);
                headerTable.push_back(ft);
                index++;
            }
        }
    }
    for (auto it = headerTable.begin(); it != headerTable.end(); it++)
    {
        if ((*it)->count < minSupportCount)
        {
            it = headerTable.erase(it);
            it--;
        }
    }
    sortHeaderTable();
    // cout<<ItemsCount<<endl;
}

bool cmpHeaderTable(FPHeaderTable *a, FPHeaderTable *b)
{
    return a->count > b->count;
}

void FPGrowth::sortHeaderTable()
{
    sort(headerTable.begin(), headerTable.end(), cmpHeaderTable);
    int index = 0;
    nameIndex.clear();
    for (auto i = headerTable.begin(); i != headerTable.end(); i++)
    {
        nameIndex[(*i)->name] = index;
        index++;
    }
    ItemsCount = index;
}

void FPGrowth::insertTrans(vector<string> &trans)
{
    // cout<<"A"<<endl;
    // cout<<trans.size()<<endl;
    FPTreeNode *currentNode = root;
    unordered_map<string, FPTreeNode *> *children = NULL;
    children = &(currentNode->children);
    for (auto it = trans.begin(); it != trans.end(); it++)
    {
        
        if (children->find(*it) != children->end())
        {
            currentNode->count++;
            currentNode = (*children)[*it];
            children = &(currentNode->children);
        }
        else
        {
            
            FPTreeNode *ft = new FPTreeNode((*it), 1, currentNode);
            (*children)[*it]=ft;
            currentNode = ft;
            children = &(ft->children);
            if (headerTable[nameIndex[*it]]->head == NULL)
            {
                headerTable[nameIndex[*it]]->head = currentNode;
                headerTable[nameIndex[*it]]->end = currentNode;
            }
            else
            {
                headerTable[nameIndex[*it]]->end->link = currentNode;
                headerTable[nameIndex[*it]]->end = currentNode;
            }
        }
    }
}

void FPGrowth::insertTrans(vector<FPTransItem *> &trans)
{
    // cout<<"A"<<endl;
    FPTreeNode *currentNode = root;
    unordered_map<string, FPTreeNode *> *children = NULL;
    children = &(currentNode->children);
    for (auto it = trans.begin(); it != trans.end(); it++)
    {
        if (children->find((*it)->name) != children->end())
        {
            (*children)[(*it)->name]->count += (*it)->count;
            currentNode = (*children)[(*it)->name];
            children = &(currentNode->children);
        }
        else
        {
            FPTreeNode *ft = new FPTreeNode(((*it)->name), (*it)->count, currentNode);
            currentNode = ft;
            (*children)[(*it)->name]=ft;
            children = &(ft->children);
            if (headerTable[nameIndex[(*it)->name]]->head == NULL)
            {
                headerTable[nameIndex[(*it)->name]]->head = currentNode;
                headerTable[nameIndex[(*it)->name]]->end = currentNode;
            }
            else
            {
                headerTable[nameIndex[(*it)->name]]->end->link = currentNode;
                headerTable[nameIndex[(*it)->name]]->end = currentNode;
            }
        }
    }
}

unordered_map<string, int> *gindex = NULL;
bool cmpString(string a, string b)
{
    return (*gindex)[a] < (*gindex)[b];
}

bool cmpTransItem(FPTransItem *a, FPTransItem *b)
{
    return (*gindex)[a->name] < (*gindex)[b->name];
}

void FPGrowth::buildFPTree(string fileName)
{
    ifstream fin;
    fin.open(fileName);
    if (!fin)
    {
        cout << "Cannot open the input file" << endl;
        return;
    }

    string line;
    // cout<<"D"<<endl;
    while (!getline(fin, line).eof())
    {
        stringstream ss(line);
        string item;
        vector<string> items;
        // cout<<"C"<<endl;
        while (ss >> item)
        {
            // cout<<"B"<<endl;
            if (nameIndex.find(item) != nameIndex.end())
            {
                items.push_back(item);
            }
        }
        gindex = &nameIndex;
        sort(items.begin(), items.end(), cmpString);
        gindex = NULL;
        insertTrans(items);
        // cout<<"A"<<endl;
    }
    fin.close();
}

void FPGrowth::buildFPTree(vector<FPTrans> &trans)
{
    for (auto line = trans.begin(); line != trans.end(); line++)
    {
        vector<FPTransItem *> items;
        for (auto t = line->trans.begin(); t != line->trans.end(); t++)
        {
            if (nameIndex.find(t->name) != nameIndex.end())
            {
                items.push_back(&(*t));
            }
        }
        gindex = &nameIndex;
        sort(items.begin(), items.end(), cmpTransItem);
        gindex = NULL;
        insertTrans(items);
    }
}

#pragma - mark mining phrase
void FPGrowth::saveSingleResult(vector<FreqResult> &result)
{
    FreqResult resultLine;
    auto it = prefix->end() - 1;
    resultLine.count = (*it)->count;
    for (; it >= prefix->begin(); it--)
    {
        // cout<<(*it)->name<<" ";
        resultLine.trans.push_back((*it)->name);
    }
    // cout<<": "<<resultLine.count<<endl;
    result.push_back(resultLine);
}

void FPGrowth::miningFPTree(vector<FreqResult> &result)
{
    if (prefix==NULL)
    {
        prefix = new vector<FPTransItem *>;
    }
    if (headerTable.size() == 0)
    {
        // cout<<"A"<<endl;
        if (prefix->size() > 0)
        {
            // cout<<prefix->size()<<endl;
            saveSingleResult(result);
        }
        return;
    }
    if (prefix->size() > 0)
    {
        saveSingleResult(result);
    }
    for (auto it = headerTable.end() - 1; it >= headerTable.begin(); it--)
    {
        FPTransItem *newPrefix = new FPTransItem((*it)->name, (*it)->count);
        vector<FPTrans> trans;
        FPTreeNode *cur=NULL;
        for (cur = (*it)->head; cur != (*it)->end; cur = cur->link)
        {
            vector<FPTransItem> items;
            // if(cur==NULL)cout<<"A"<<endl;
            int basicCount = cur->count;
            for (auto t = cur->parent; t->parent != NULL; t = t->parent)
            {
                items.push_back(FPTransItem(t->name, basicCount));
            }
            trans.push_back(items);
        }
        vector<FPTransItem> items;
        int basicCount = cur->count;
        for (auto t = cur->parent; t->parent != NULL; t = t->parent)
        {
            items.push_back(FPTransItem(t->name, basicCount));
        }
        trans.push_back(items);
        prefix->push_back(newPrefix);
        FPGrowth *ftmp = new FPGrowth(minSupport);
        ftmp->minSupportCount = minSupportCount;
        ftmp->prefix = prefix;
        ftmp->initFromTrans(trans);
        ftmp->miningFPTree(result);
        delete ftmp;
        prefix->pop_back();
    }
}

#pragma - mark destructor method

FPGrowth::~FPGrowth(){
    if(root!=NULL){
        delete root;
    }
    if(prefix!=NULL&&prefix->size()==0){
        for(auto i:*prefix){
            if(i!=NULL){
                delete i;
            }
        }
    }
    for(auto i=headerTable.begin();i!=headerTable.end();i++){
        if((*i)!=NULL){
            delete (*i);
        }
    }
}