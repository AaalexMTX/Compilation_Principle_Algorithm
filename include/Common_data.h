#pragma once
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

//�ķ��ṹ
struct grammarStruct {
	std::string S;
	std::set<std::string>Vt;
	std::unordered_map<std::string, int>Vn;
	std::unordered_map<std::string, std::vector<std::string>>P;

	//grammarStruct() {};
	//grammarStruct(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
	//	: S(s), Vt(vt), Vn(vn), P(p) {}
};

//�õ���������
std::string getFullChar(std::string candidate, int pos);

//����ָ��Ϊ ��һ���ǿ��ַ�
int getBC(int pos, char line[]);