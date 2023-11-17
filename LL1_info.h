#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<unordered_map>
#include<regex>
#include<unordered_map>
#include<utility>
using namespace std;

//文法结构
struct grammerStruct {
	set<string>Vt;
	set<pair<string, int>>Vn;
	string S;
	unordered_map<string, vector<string>>P;
};

int getBC(int pos, char line[]);

void scan(char *lineToken);

void print();

void directLeftRecursion(string proLeft, vector<string>proRight);

void remove_left_recursion();

void remove_left_gene();