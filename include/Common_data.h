#pragma once
#include<regex>
#include<string>
#include<set>
#include<unordered_map>
#include<unordered_set>

//�Ź�ͬʹ�õ�ȫ�ַ��� / ȫ�ֱ���

// ��extern ֻ���ڶ�����ļ�ʹ��
extern std::regex N;
extern std::regex T;							//����

//�õ���������
std::string getFullChar(std::string candidate, int pos);

//����ָ��Ϊ ��һ���ǿ��ַ�
int getBC(int pos, char line[]);

