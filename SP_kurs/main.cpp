#include "LexemAnalizator.h"
#include "ErrorsChecking.h"
#include "Generator.h"

FILE* InF, * OutF, * ErrorF;


int main(int argc, std::string* argv)
{
	//SetConsoleOutputCP(CP_UTF8);
	//setvbuf(stdout, nullptr, _IOFBF, 1000);
	DataType* Data = new DataType;
	Analizer A(Data);
	ErrorsCheck E(Data);
	Generator G(Data);
	printf("=========================================================================================================\n");
	printf("\t\t\t\tLasname = Flyk\tvariant = 22\n");
	printf("\t\t\t\tRANSLATOR (%s -> ASSEMBLER)\n", Data->ftype.c_str());
	printf("=========================================================================================================\n\n");
	printf("=========================================================================================================\n");
	// Перевірка кількості аргументів, які потрібно передати з командного рядка
	if (argc != 2)
	{
		printf("Input file name\n");
		std::cin >> Data->InputFileName;
	}
	else
	{
		//Отримання та формування імен вхідних і вихідних файлів
		Data->InputFileName = argv->c_str();
	}

	if ((InF = fopen(Data->InputFileName.c_str(), "r")) == 0)
	{
		printf("Error: Can not open file: %s\n", Data->InputFileName.c_str());
		system("pause");
		exit(1);
	}

	char temp[4] = { '\0' };

	Data->InputFileName.copy(temp, 3, Data->InputFileName.size() - 3);

	if (std::string(temp) != Data->ftype)
	{
		printf("Error: Invalid file type: %s\n", Data->InputFileName.c_str());
		system("pause");
		exit(1);
	}

	printf("Start translating file: %s\n", Data->InputFileName.c_str());
	int k = Data->InputFileName.size();
	while (k > 0)
	{
		if (Data->InputFileName[k] == '\\')
		{
			k++;
			break;
		}
		k--;
	}
	Data->OutputFileName = Data->InputFileName;
	Data->OutputFileName.erase(Data->InputFileName.size() - 4, Data->InputFileName.size());
	Data->OutputFileName.erase(0, k);

	printf("Output file: %s\n", std::string(Data->OutputFileName + ".asm").c_str());
	printf("=========================================================================================================\n\n");
	printf("=========================================================================================================\n");
	// Breaking into tokens and printing into file
	printf("Breaking into lexems are starting...\n");
	A.AnalisisLexems(InF);
	A.PrintLexemsInFile();
	printf("Breaking into lexems completed. There are %d lexems.\nReport file: lexems.txt\n", Data->LexTable.size());
	printf("=========================================================================================================\n\n");
	printf("=========================================================================================================\n");

	printf("Error checking are starting...\n");
	Data->numberErrors = E.ErrorChecking();
	printf("Error checking is complete. There is(are) %d errors.\nReport file: errors.txt\n", Data->numberErrors);
	printf("=========================================================================================================\n\n");
	printf("=========================================================================================================\n");
	// If there are no errors, go to a translation stage
	if (Data->numberErrors != 0)
	{
		printf("Translation can not be continued. Errors were found. Please correct the errors and try again.\n");
	}
	else
	{
		if ((OutF = fopen(std::string(Data->OutputFileName + ".asm").c_str(), "w")) == 0)
		{
			printf("Error: Can not create file: %s\n", std::string(Data->OutputFileName + ".asm").c_str());
			system("pause");
			exit(1);
		}
		printf("Code generation is starting...\n");
		G.GenerateCode(OutF);
		printf("Code generation is finish.\n");
		fclose(OutF);
		printf("=========================================================================================================\n\n");
		printf("=========================================================================================================\n");
		if (directory_exists("masm32"))
		{
			system(std::string("masm32\\bin\\ml /c /coff " + Data->OutputFileName + ".asm").c_str());
			system(std::string("masm32\\bin\\Link /SUBSYSTEM:WINDOWS " + Data->OutputFileName + ".obj").c_str());
			//system(std::string("del " + std::string(Data->OutputFileName) + ".obj").c_str());
		}
		else
		{
			printf("WARNING!\n");
			printf("Can't compile asm file, because masm32 doesn't exist.\n");
		}

		printf("=========================================================================================================\n\n");
		printf("=========================================================================================================\n");
		printf("Done!\n");
	}
	system("pause");
	return 0;
}