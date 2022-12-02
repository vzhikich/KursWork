#include "LexemAnalizator.h"
#define AorN(ch) ((isalpha(ch) || isdigit(ch)))
string backup;
int line = 1, bln = 1;

Lexema Analizator::GetLexem(FILE* f)
{
	char ch = 0;
	Lexema res;
	std::string b;
	res.value = 0;

	if (backup.size() == 0)
	{
	next:;
		ch = getc(f);
		if ((ch == '\n') || (ch == '\t') || (ch == ' '))
		{
			if (ch == '\n')
			{
				bln++;
				line++;
			}
			goto next;
		}
		while ((ch != '\n') && (ch != '\t') && (ch != ' '))
		{
			if (ch == EOF)
			{
				if (b.empty())
				{
					res.type = LEOF;
					res.value = 0;
					res.line = line;
					return res;
				}
				ungetc(ch, f);
				break;
			}
			b.push_back(ch);
			if (AorN(b[0]) != AorN(ch))
			{
				ungetc(ch, f);
				b.pop_back();
				break;
			}
			ch = getc(f);
		}
		if (ch == '\n')
		{
			line++;
		}
		backup = b;
	}
	else
	{
		b = backup;
	}
	while (b.size() != 0)
	{
		for (size_t i = 0; i < Data->LexName.size(); i++)
		{
			string temp = b;
			if (Data->LexName[i].name.size() <= b.size() && Data->LexName[i].name.size() > 0)
			{
				temp.erase(Data->LexName[i].name.size(), b.size());
				if (temp == Data->LexName[i].name)
				{
					res.name = Data->LexName[i].name;
					res.type = Data->LexName[i].type;
					res.line = bln;
					backup.erase(0, temp.size());
					return res;
				}
			}
		}
		if (!AorN(b[0]))
		{
			res.name.push_back(b[0]);
			res.type = LUnknown;
			res.line = bln;
			backup.erase(0, 1);
			return res;
		}
		else
		{
			break;
		}
	}
	res.name = b;
	res.type = LIdentifier;
	res.line = bln;
	backup.erase(0, b.size());
	return res;
}

int Analizator::AnalisofLexems(FILE* FIn)
{
	std::sort(Data->LexName.begin(), Data->LexName.end(), []
	(const LexAndName& first, const LexAndName& second) {return first.name.size() > second.name.size(); });

	do
	{
		Data->LexTable.push_back(GetLexem(FIn));
		if (backup.size() == 0)
		{
			bln = line;
		}
	} while (Data->LexTable[Data->LexTable.size() - 1].type != LEOF);
	std::sort(Data->LexName.begin(), Data->LexName.end(), []
	(const LexAndName& first, const LexAndName& second) {return first.type < second.type; });
	for (size_t i = 0; i < Data->LexTable.size(); i++)
	{
		if (Data->LexTable[i].type == LProgram && Data->LexTable[i+1].type == LIdentifier)
		{
			Data->LexTable[i+1].type = LProgramName;
		}
	
		if (Data->LexTable[i].type == LIdentifier)
		{
			size_t j = 0;
		
			if (Data->LexTable[i].name.size() <= Data->TName.size())
			{
				for (; j < Data->LexTable[i].name.size(); j++)
				{
					if ((islower(Data->LexTable[i].name[j]) != islower(Data->TName[j])) && !isdigit(Data->LexTable[i].name[j]))
					{
						Data->LexTable[i].type = LUnknown;
						break;
					}
				}
				
			}
			else
			{
				Data->LexTable[i].type = LUnknown;
			}
		}
		if (Data->LexTable[i].type == LFComentar && Data->LexTable[i + 1].type == LSComentar) {
			Data->LexTable[i].name += Data->LexTable[i + 1].name;
			Data->LexTable.erase(Data->LexTable.begin() + i + 1);
			int j = i;
			for (; j < Data->LexTable.size(); j++) {
				if (Data->LexTable[j].type == LSComentar && Data->LexTable[j + 1].type == LFComentar) {
					Data->LexTable[j].name += Data->LexTable[j + 1].name;
					Data->LexTable.erase(Data->LexTable.begin() + j + 1);
				}
			}
		}
		if (Data->LexTable[i].type == LLComentar)
		{
			if (Data->LexName[LRComentar].name.size() != 0)
			{
				do
				{
					Data->LexTable[i].name += Data->LexTable[i + 1].name;
					if (Data->LexName[LLComentar].name != Data->LexName[LRComentar].name && Data->LexTable[i + 1].type == LRComentar)
					{
						break;
					}
					else if (Data->LexName[LLComentar].name == Data->LexName[LRComentar].name && Data->LexTable[i + 1].type == LLComentar)
					{
						break;
					}
					Data->LexTable.erase(Data->LexTable.begin() + i + 1);

				} while (true);
				Data->LexTable.erase(Data->LexTable.begin() + i + 1);
			}
			else
			{
				while (Data->LexTable[i].line == Data->LexTable[i + 1].line)
				{
					Data->LexTable[i].name += Data->LexTable[i + 1].name;
					Data->LexTable.erase(Data->LexTable.begin() + i + 1);
				}
			}
		}
	}
	for (size_t i = 0; i + 2 < Data->LexTable.size(); i++)
	{
		if (Data->LexTable[i].type == LProgram && i != 0 && Data->LexName[LBegProg].name.size() > 0)
		{
			if (Data->LexTable[i - 1].type == LBegProg)
			{
				Data->LexTable[i - 1].name += Data->LexTable[i].name;
				Data->LexTable[i - 1].type = LProgram;
				Data->LexTable.erase(Data->LexTable.begin() + i);
				i--;
			}
			else
			{
				Data->LexTable[i].type = LUnknown;
			}
		}
		else if (Data->LexTable[i].type == LProgram && Data->LexName[LBegProg].name.size() > 0)
		{
			Data->LexTable[i].type = LUnknown;
		}
		if (Data->EnableProgN && Data->LexTable[i - 1].type == LProgram && Data->LexTable[i].type == LIdentifier)
		{
			Data->LexTable[i].type = LProgramName;
		}
		Data->LexTable[i].value = atoi(Data->LexTable[i].name.c_str());
		if (Data->LexTable[i].name[0] == '0')
		{
			Data->LexTable[i].type = LNumber;
		}
		if (Data->LexTable[i].value != 0)
		{
			Data->LexTable[i].type = LNumber;
			if (!(Data->LexTable[i - 2].type == LIdentifier || Data->LexTable[i - 2].type == LNumber) && Data->LexTable[i - 1].type == LSub)
			{
				Data->LexTable[i].name = "-" + Data->LexTable[i].name;
				Data->LexTable[i].value *= -1;
				Data->LexTable.erase(Data->LexTable.begin() + i - 1);
				i--;
			}
		}
	}
	for (int i = 0; i < Data->LexTable.size(); ++i) {
		if (Data->LexTable[i].type == LProgramName) {
			Data->EnableProgN = true;
		}
	}


	return Data->LexTable.size();
}

void Analizator::PrintLexems()
{
	FILE* outTokensFiles;
	string type;
	outTokensFiles = fopen(std::string(Data->OutputFileName + "_lexems.txt").c_str(), "w");
	fputs("\t\tToken table!\n", outTokensFiles);
	fprintf(outTokensFiles, "=======================================================\n");
	fprintf(outTokensFiles, "%s\t%12s%20s%10s%5s\n", "#", "NAME", "TYPE", "VALUE", "LINE");
	fprintf(outTokensFiles, "=======================================================\n");
	for (size_t i = 0; i < Data->LexTable.size(); ++i)
	{
		switch (Data->LexTable[i].type)
		{
		case LProgram: type = "program"; break;
		case LProgramName: type = "program name"; break;
		case LStart: type = "start"; break;
		case LData: type = "variable"; break;
		case LVarType: type = "var type"; break;
		case LIdentifier: type = "identifier"; break;
		case LEnd: type = "end"; break;
		case LScan: type = "input"; break;
		case LPrint: type = "output"; break;
		case LIf: type = "if"; break;
		case LElse: type = "else"; break;
		case LWHILE: type = "while"; break;
		case LDO: type = "do"; break;
		case LNewValue: type = "new value"; break;
		case LAdd: type = "add"; break;
		case LSub: type = "sub"; break;
		case LMul: type = "mul"; break;
		case LDiv: type = "div"; break;
		case LMod: type = "mod"; break;
		case LEqu: type = "equ"; break;
		case LNotEqu: type = "not equ"; break;
		case LGreate: type = "greate"; break;
		case LLess: type = "less"; break;
		case LNot: type = "not"; break;
		case LAnd: type = "and"; break;
		case LOr: type = "or"; break;
		case LLBraket: type = "left braket"; break;
		case LRBraket: type = "right braket"; break;
		case LNumber: type = "number"; break;
		case LSeparator: type = "separator"; break;
		case LComma: type = "comma"; break;
		case LEOF: type = "end of file"; break;
		case LLComentar: type = "comentar"; break;
		case LUnknown: type = "unknown"; break;
		}
		fprintf(outTokensFiles, "%d\t%12s%20s%10d%5d\n", i + 1, Data->LexTable[i].name.c_str(), type.c_str(), Data->LexTable[i].value, Data->LexTable[i].line);
	}
	fclose(outTokensFiles);
}
