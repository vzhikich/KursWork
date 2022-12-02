#pragma once
#include "DataTypes.h"

class Generator
{
private:
	DataType* Data;
	vector<int> bufExprPostfixForm;	//Буфер для виразу в постфіксній формі
	vector<int> opStack, ifStack, loopStack, startStack;
	string type = "dw", typeL = "dword";
	void BeginASMFile(FILE* fout);
	void BeginCodeSegm(FILE* fout);

	void CheckPresent();

	void PrintData(FILE* f);

	void PrintMOD(FILE* f);
	void PrintAND(FILE* f);
	void PrintOR(FILE* f);
	void PrintNOT(FILE* f);
	void PrintEQ(FILE* f);
	void PrintGE(FILE* f);
	void PrintLE(FILE* f);
	void PrintCheck(FILE* f);

	void PrintScan(FILE* f);
	void PrintOutput(FILE* f);

	void PrintEnding(FILE* f);

	bool Priotiry(TypeOfLex t, int  s);

	int ConvertToPostfixForm(int i);

	void GenASMCode(string str, FILE* f);

	void PrintCode(FILE* f);
public:
	Generator(DataType* Data) : Data(Data) {}
	void GenerateCode(FILE* f);
};

#pragma once
