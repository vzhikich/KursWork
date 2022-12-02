#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <io.h>

#define IsOperation(k) (k < 12)		//�������� �� ��������	

using namespace std;

enum TypeOfLex
{
	LAdd,			//���������
	LSub,			//��������
	LMul,			//��������
	LDiv,			//������
	LMod,			//������ � �������
	LEqu,			//����
	LNotEqu,		//�� ����
	LGreate,		//�����
	LLess,			//������
	LNot,			//��
	LAnd,			//�
	LOr,			//���

	LBegProg,		//�������� ����� ��������
	LProgram,		//PROGRAM
	LProgramName,	//����� ��������
	LStart,			//BEGIN
	LData,			//VAR
	LVarType,		//INT16
	LIdentifier,	//��������������
	LEnd,			//BEGIN

	LScan,			//���
	LPrint,			//����

	LIf,			//�������� if
	LElse,			//�������� else
	
	
	LWHILE,			//����
	LDO,

	LNewValue,		//���� ���������

	LLBraket,		//��� �����
	LRBraket,		//����� �����
	LNumber,		//�����	
	LSeparator,		//������ � �����
	LComma,			//����
	LEOF,			//����� �����
	LFComentar,		//������� ���������
	LSComentar,		//����� ���������
	LLComentar,		//���� ��������
	LRComentar,		//������ ��������
	LUnknown		//�������� ������������
};

struct LexAndName				//��������� ������������� � ���� ������
{
	TypeOfLex type;							
	std::string name;
};

//DATA
typedef struct Lexem			//��������� �������
{		
	string name;				//����� �������
	int value;					//��������
	int line;					//�����
	TypeOfLex type;				//��� 
}Lexema;

typedef struct L
{
	vector<Lexema> LexTable;	//������� ������

	vector<string> IdTable;		//������� ��������������
	string ftype = "f22";		//����� ����
	string TName = "xx";		//�������� ��������������
	vector<LexAndName> LexName =//������� ������������� � ���� ������
	{
		{LAdd,"+"},
		{LSub,"-"},
		{LMul,"*" },
		{LDiv,"DIV"},
		{LMod,"MOD"},
		{LEqu,"=="},
		{LNotEqu,"<>"},
		{LGreate,">>"},
		{LLess,"<<"},
		{LNot,"!!"},
		{LAnd,"&&"},
		{LOr,"||"},

		{LBegProg,""},
		{LProgram,"PROGRAM"},
		{LProgramName,""},
		{LStart,"BEGIN"},
		{LData,"VAR"},
		{LVarType,"INT16"},
		{LIdentifier,""},
		{LEnd,"END"},

		{LScan,"SCAN"},
		{LPrint,"PRINT"},

		{LIf,"IF"},
		{LElse,"ELSE"},

		{LWHILE,"WHILE"},
		{LDO,"DO"},

		{LNewValue,"->"},

		{LLBraket,"("},
		{LRBraket,")"},
		{LNumber,""},
		{LSeparator,";"},
		{LComma,","},
		{LFComentar,"#"},
		{LSComentar,"*"},
		{LLComentar,"#*"},
		{LRComentar,"*#"},
		{LEOF,"EndFile"}
	};

	bool IsPresentInput,					//������ ���������� ���������
		 IsPresentOutput,
		 IsPresentMod,
		 IsPresentAnd,
		 IsPresentOr,
		 IsPresentNot,
		 IsPresentEqu,
		 IsPresentGreate,
		 IsPresentLess,
		 IsPresentDiv;

	bool EnableProgN = 0, PosLStart = 0;	

	int numberErrors;			//������� �������

	std::string InputFileName;	//����� �������� �����
	std::string OutputFileName; //����� ��������� �����
}DataType;

bool directory_exists(const std::string& directory);  //������� �������� �������� �������� 