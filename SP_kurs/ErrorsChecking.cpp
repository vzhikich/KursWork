#include "ErrorsChecking.h"

int ErrorsCheck::IsExpression(int i, FILE* ef)
{
	int nom, error = 0;
	vector<int> ss;
	nom = i;
	if ((Data->LexTable[nom].type != LLBraket) && (Data->LexTable[nom].type != LIdentifier) && (Data->LexTable[nom].type != LNumber) && (Data->LexTable[nom].type != LNot))
	{
		fprintf(ef, "line %d: \tExpression must begin from identifier, number or '('!\n", Data->LexTable[nom].line);
		error++;
	}
	for (; (Data->LexTable[nom].type != LSeparator && (Data->LexTable[nom + 1].type != LStart)); nom++)
	{
		if (Data->LexTable[nom].type == LRBraket)
		{
			if (!(IsOperation(Data->LexTable[nom + 1].type)) && (Data->LexTable[nom + 1].type != LSeparator) && (Data->LexTable[nom + 1].type != LRBraket) && (Data->LexTable[nom + 1].type != LStart))
			{
				fprintf(ef, "line %d: \tAfter ')' must be ')', operation or ';'!\n", Data->LexTable[nom].line);
				error++;
			}
			if (ss.empty())
			{
				fprintf(ef, "line %d: \tToo much '('!\n", Data->LexTable[nom].line);
				error++;
			}
			else
			{
				ss.pop_back();
			}
		}
		if (Data->LexTable[nom].type == LLBraket)
		{
			if ((Data->LexTable[nom + 1].type != LIdentifier) && (Data->LexTable[nom + 1].type != LLBraket) &&
				(Data->LexTable[nom + 1].type != LNumber) && (Data->LexTable[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter '(' must be '(' or identifier!\n", Data->LexTable[nom].line);
				error++;
			}
			ss.push_back(nom);
		}
		if (IsOperation(Data->LexTable[nom].type) && Data->LexTable[nom].type != LNot)
		{
			if ((Data->LexTable[nom + 1].type != LIdentifier) && (Data->LexTable[nom + 1].type != LLBraket) && (Data->LexTable[nom + 1].type != LNumber) && (Data->LexTable[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter operation must be '(' or idetifier!\n", Data->LexTable[nom].line);
				error++;
			}
		}
		if ((Data->LexTable[nom].type == LIdentifier) || (Data->LexTable[nom].type == LNumber))
		{
			if (!(IsOperation(Data->LexTable[nom + 1].type)) && (Data->LexTable[nom + 1].type != LRBraket) && (Data->LexTable[nom + 1].type != LSeparator))
			{
				fprintf(ef, "line %d: \tAfter identifier must be ')' or ';' or operation!\n", Data->LexTable[nom].line);
				error++;
			}
		}
	}
	if (!(ss.empty()))
	{
		fprintf(ef, "line %d: \t')' expected!\n", Data->LexTable[nom].line);
		error++;
	}
	return error;
}

int ErrorsCheck::Balans(int nom, TypeOfLex ends, TypeOfLex ltStart, TypeOfLex ltFinish)
{
	vector<int> ss;
	int j = 0, i;
	i = nom;
	for (; Data->LexTable[i].type != ends; i++)
	{
		if (Data->LexTable[i].type == ltStart)
		{
			ss.push_back(i);
		}
		if (Data->LexTable[i].type == ltFinish)
		{
			if (ss.empty())
			{
				j = 1;	//  Too much ')';
				break;
			}
			else
			{
				ss.pop_back();
			}
		}
	}
	if (!(ss.empty()))
	{
		j = 2;	// ')' expected;
	}
	return j;
}

int ErrorsCheck::ErrorChecking()
{
	int label = 0;
	int i = 0, j = 1, temp = 0, ValNum = 0;
	FILE* ef;
	int while_num = 0, STARTBLOK_ENDBLOK_num = 0;//, r1, r2;
	int Err_num = 0;

	ef = fopen(std::string(Data->OutputFileName + "_errors.txt").c_str(), "w");
	fputs("List of bugs in the program\n", ef);
	fprintf(ef, "=================================================================================================\n");
	for (size_t i = 0; i < Data->LexTable.size(); i++)
	{
		if (Data->LexTable[i].type == LLComentar)
		{
			Data->LexTable.erase(Data->LexTable.begin() + i);
		}
	}
	//перевірка чи першим словом у програмі є program
	if (Data->LexTable[i++].type != LProgram)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \t'%s' expected! (program must begin from the keyword '%s')\n", Err_num, Data->LexTable[i].line, Data->LexName[LProgram].name.c_str(), Data->LexName[LProgram].name.c_str());
	}
	if (Data->EnableProgN)
	{
		if (Data->LexTable[i++].type != LProgramName)
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \tProgramName expected!\n", Err_num, Data->LexTable[i].line);
		}

		if (Data->LexTable[i++].type != LSeparator)
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \t';' expected!\n", Err_num, Data->LexTable[i].line);
		}
		//перевірка, чи другим словом в програмі є ім'я програми
	}
	if (!Data->PosLStart)
	{
		if (Data->LexTable[i++].type != LStart)
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \t'%s' expected!\n", Err_num, Data->LexTable[i].line, Data->LexName[LStart].name.c_str());
		}
	}

	if (Data->LexTable[i++].type != LData)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'%s' expected!\n", Data->LexTable[i].line, Data->LexName[LData].name.c_str());
	}
	switch (Balans(0, LEOF, LStart, LEnd))
	{
	case 1:
		Err_num++;
		fprintf(ef, "line %d: \tToo much '%s'!\n", Data->LexTable[j].line, Data->LexName[LEnd].name.c_str());
		break;
	case 2:
		Err_num++;
		fprintf(ef, "line %d: \t'%s' expected!\n", Data->LexTable[j].line, Data->LexName[LEnd].name.c_str());
		break;
	default:
		break;
	}
	

	//перевірка, чи  другим словом в програмі є var
	if (Data->LexTable[i].type != LVarType)
	{
		Err_num++;
		fprintf(ef, "%d line %d: \t'%s' expected!\n", Err_num, Data->LexTable[i].line, Data->LexName[LVarType].name.c_str());
	}
	if (Data->LexTable[i++].type == LVarType)
	{
		if (Data->LexTable[i].type != LIdentifier)  	//перевірка, чи після var йдуть ідентифікатори
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \tIdentifier expected!\n", Err_num, Data->LexTable[i].line);
		}
		else
		{
			do
			{
				if ((Data->LexTable[i].type == LIdentifier) && (Data->LexTable[i + 1].type == LComma))
				{
					Data->IdTable.push_back(Data->LexTable[i].name);
					for (size_t i = 0; i < Data->IdTable.size() - 1; i++)
					{
						if (Data->IdTable[i] == Data->IdTable[Data->IdTable.size() - 1])
						{
							Err_num++;
							fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data->LexTable[Data->IdTable.size() - 1].line);
						}
					}
					i = i + 2;
				}
			} while ((Data->LexTable[i].type == LIdentifier) && (Data->LexTable[i + 1].type == LComma));
			if ((Data->LexTable[i].type == LIdentifier) && (Data->LexTable[i + 1].type == LSeparator))
			{
				Data->IdTable.push_back(Data->LexTable[i].name);
				for (size_t i = 0; i < Data->IdTable.size() - 1; i++)
				{
					if (Data->IdTable[i] == Data->IdTable[Data->IdTable.size() - 1])
					{
						Err_num++;
						fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data->LexTable[Data->IdTable.size() - 1].line);
					}
				}
				i = i + 2;
				goto label1;
			}
			if (Data->LexTable[i].type != LSeparator)
			{
				if (Data->LexTable[i].type == LComma)
				{
					Err_num++;
					fprintf(ef, "line %d: \tToo much commas!\n", Data->LexTable[i].line);
				}
				else
				{
					Err_num++;
					fprintf(ef, "line %d: \t';' expected!\n", Data->LexTable[i].line);
				}
			}
			else
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much commas or identifier expected!\n", Data->LexTable[i].line);
			}
		}
		i++;
	label1:;
	}

	if (Data->LexTable[Data->LexTable.size() - 2].type != LEnd)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'%s' expected!\n", Data->LexTable[Data->LexTable.size() - 2].line, Data->LexName[LEnd].name.c_str());
	}
	if (Data->LexTable[Data->LexTable.size() - 1].type != LEOF)
	{
		Err_num++;
		fprintf(ef, "line %d: \tEnd of file expected!\n", Data->LexTable[Data->LexTable.size() - 1].line);
	}

	for (j = 0;; j++)
	{

		if (Data->LexTable[j].type == LUnknown)        //Пошук невідомих слів(не ідентифікаторів)
		{
			Err_num++;
			fprintf(ef, "line %d: \tUnknown identifier!(%s)\n", Data->LexTable[j].line, Data->LexTable[j].name.c_str());
		}
		if ((Data->LexTable[j].type == LIdentifier) && (j > i))
		{
			bool available = 0;
			for (size_t i = 0; i < Data->IdTable.size(); i++)
			{
				if (Data->IdTable[i] == Data->LexTable[j].name)
				{
					available = 1;
				}
			}
			if (!available)
			{
				Err_num++;
				fprintf(ef, "line %d: \tUnknown identifier!(%s)\n", Data->LexTable[j].line, Data->LexTable[j].name.c_str());
			}
		}
		if (Data->LexTable[j].type == LNewValue)
		{
			int buf;
			if (Data->LexTable[j - 1].type == LIdentifier)
			{
				buf = IsExpression((j + 1), ef);
			}
			else
			{
				buf = 1;
			}
			Err_num = Err_num + buf;
		}
		if (Data->LexTable[j].type == LPrint)
		{
			int buf, brak;
			if (Data->LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", Data->LexTable[j + 1].line);
			}
			buf = IsExpression((j + 1), ef);
			Err_num = Err_num + buf;
			brak = Balans(j, LSeparator, LLBraket, LRBraket);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much ')'!\n", Data->LexTable[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", Data->LexTable[j].line);
			}
		}
		if (Data->LexTable[j].type == LScan)
		{
			if (Data->LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", Data->LexTable[j + 1].line);
			}
			if (Data->LexTable[j + 2].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected!\n", Data->LexTable[j + 2].line);
			}
			if (Data->LexTable[j + 3].type != LRBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", Data->LexTable[j + 3].line);
			}
			if (Data->LexTable[j + 4].type != LSeparator)
			{
				Err_num++;
				fprintf(ef, "line %d: \t';' expected!\n", Data->LexTable[j + 4].line);
			}

		}
		if (Data->LexTable[j].type == LWHILE)				//перевірка WHILE
		{
			int buf;
			if (Data->LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected after 'WHILE'!\n", Data->LexTable[j + 1].line);
			}
			buf = IsExpression((j + 1), ef);
			int brak = Balans(i, LEOF, LWHILE, LDO);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much 'DO'!\n", Data->LexTable[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'DO' expected!\n", Data->LexTable[j].line);
			}
		}
		if (Data->LexTable[j].type == LDO)				//перевірка DO
		{
			if (Data->LexTable[j + 1].type != LStart)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'START' expected after 'DO'!\n", Data->LexTable[j + 1].line);
			}
		}

		if (Data->LexTable[j].type == LIf)				//перевірка If
		{
			if (Data->LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected after '%s'!\n", Data->LexTable[j + 1].line, Data->LexName[LIf].name.c_str());
			}
			Err_num += IsExpression((j + 2), ef);
		}
		if (Data->LexTable[j].type == LEOF) break;
	}
	if (Err_num == 0) fprintf(ef, "No errors found.");
	fclose(ef);
	return Err_num;
}
