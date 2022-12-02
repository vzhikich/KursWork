#pragma once
#include "DataTypes.h"

class ErrorsCheck
{
private:
	DataType* Data;
	int IsExpression(int i, FILE* ef);
	int Balans(int nom, TypeOfLex ends, TypeOfLex ltStart, TypeOfLex ltFinish);
public:
	ErrorsCheck(DataType* Data) : Data(Data) {}
	int ErrorChecking();
	//constexpr bool IsOperation(TypeOfLex t);
};

