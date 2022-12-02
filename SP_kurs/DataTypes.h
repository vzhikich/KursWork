#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <io.h>

#define IsOperation(k) (k < 12)		//перев≥рка на операц≥ю	

using namespace std;

enum TypeOfLex
{
	LAdd,			//додаванн€
	LSub,			//в≥дн≥манн€
	LMul,			//множенн€
	LDiv,			//д≥ленн€
	LMod,			//д≥ленн€ з остачою
	LEqu,			//р≥вно
	LNotEqu,		//не р≥вно
	LGreate,		//б≥льше
	LLess,			//меньше
	LNot,			//не
	LAnd,			//≥
	LOr,			//або

	LBegProg,		//на€вн≥сть ≥менн≥ програми
	LProgram,		//PROGRAM
	LProgramName,	//назва програми
	LStart,			//BEGIN
	LData,			//VAR
	LVarType,		//INT16
	LIdentifier,	//≥дентиф≥катори
	LEnd,			//BEGIN

	LScan,			//вв≥д
	LPrint,			//вив≥д

	LIf,			//оператор if
	LElse,			//оператор else
	
	
	LWHILE,			//цикл
	LDO,

	LNewValue,		//знак присвоЇнн€

	LLBraket,		//л≥ва дужка
	LRBraket,		//права дужка
	LNumber,		//цифра	
	LSeparator,		//крапка з комою
	LComma,			//кома
	LEOF,			//к≥нець файлу
	LFComentar,		//початок коментарю
	LSComentar,		//к≥нець коментарю
	LLComentar,		//л≥вий коментар
	LRComentar,		//правий коментар
	LUnknown		//нев≥домий специф≥катор
};

struct LexAndName				//структура специф≥катор≥в ≥ назв лексем
{
	TypeOfLex type;							
	std::string name;
};

//DATA
typedef struct Lexem			//структура лексеми
{		
	string name;				//назва лексеми
	int value;					//значенн€
	int line;					//р€док
	TypeOfLex type;				//тип 
}Lexema;

typedef struct L
{
	vector<Lexema> LexTable;	//таблиц€ лексем

	vector<string> IdTable;		//таблиц€ ≥дентиф≥катор≥в
	string ftype = "f22";		//назва мови
	string TName = "xx";		//прототип ≥дентиф≥катору
	vector<LexAndName> LexName =//таблиц€ специф≥катор≥в ≥ назв лексем
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

	bool IsPresentInput,					//ќзнаки присутност≥ оператор≥в
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

	int numberErrors;			//к≥льк≥сть помилок

	std::string InputFileName;	//назва вх≥дного файлу
	std::string OutputFileName; //назва вих≥дного файлу
}DataType;

bool directory_exists(const std::string& directory);  //функц≥€ перев≥рки на€вност≥ директор≥њ 