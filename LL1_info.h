#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<regex>
#include<utility>
using namespace std;

//文法结构
struct grammerStruct {
	string S;
	set<string>Vt;
	unordered_map<string, int>Vn;
	unordered_map<string, vector<string>>P;
};

int getBC(int pos, char line[]);

void scan(char lineToken[]);

void print();

void directLeftRecursion(string proLeft, vector<string>proRight);

void remove_left_recursion();

void remove_left_gene();