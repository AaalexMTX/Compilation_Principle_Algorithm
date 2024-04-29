#pragma once
#include<algorithm>
#include<regex>
#include<string>
#include<vector>
#include<set>
#include<unordered_map>
#include<unordered_set>

//�Ź�ͬʹ�õ�ȫ�ַ��� / ȫ�ֱ���

// ��extern ֻ���ڶ�����ļ�ʹ��
extern std::regex N;
extern std::regex T;							//����

extern char lineToken[50];						//��¼�ķ��ṹ(��¼һ��)
extern char expLineToken[50];					//��¼���ʽ�ṹ��һ�У�

//�ķ��ṹ
extern struct grammarStruct {
	std::string S;
	std::set<std::string>Vt;
	std::unordered_map<std::string, int>Vn;
	std::unordered_map<std::string, std::vector<std::string>>P;

	grammarStruct() {};
};

//�õ���������
std::string getFullChar(std::string candidate, int pos);

//����ָ��Ϊ ��һ���ǿ��ַ�
int getBC(int pos, char line[]);