// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
extern "C" int DlinaStroki(char *s);extern "C" char *
CopyStr(char *s1, char *s2, int L);
#include <string.h>

int _tmain()
{
	const char * str = "mama";
	int k = DlinaStroki((char *) str);
	
	char lsat[] = "aaaaaaaa";
	char *am = CopyStr((char *)str, (char *)lsat, k);

	am = CopyStr((char *)(lsat +2), (char *)lsat, k);
    return 0;
}

