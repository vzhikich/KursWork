
#pragma once
#include "DataTypes.h"

class Analizator
{
private:
	DataType* Data;
	Lexema GetLexem(FILE* f);
public:
	Analizator(DataType* Data) : Data(Data) {}
	int AnalisofLexems(FILE* FIn); // analisis number of tokens
	void PrintLexems();	// print all tokens in file
};

