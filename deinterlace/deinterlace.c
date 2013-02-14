#include <stdio.h>
#include <stdlib.h>
#include <popt.h>

char*		szInputName = 0;
char*		szUName = 0;
char*		szVName = 0;

const struct poptOption	optArray[] = {
	{	NULL,	'i',	POPT_ARG_STRING,	&szInputName, 	0, "Input File", "Filename", },
	{	NULL,	'u',	POPT_ARG_STRING,	&szUName,		0, "U Output File", "Filename", },
	{	NULL,	'v',	POPT_ARG_STRING,	&szVName,		0, "V Output File", "Filename", },
	
	POPT_AUTOHELP
	POPT_TABLEEND
};

int parse_args(int argc, const char* argv[]) {
	poptContext	popt;
	char rc;
	int result = 0;
	
	popt = poptGetContext("deinterlace", argc, argv, optArray, 0);
	
	while ((rc = poptGetNextOpt(popt)) > 0) {
		switch (rc) {
		default:
			break;
		}
	}
	
	poptFreeContext(popt);

	if ((szInputName != 0) &&
		(szUName != 0) &&
		(szVName != 0))
	{
		result = 1;
	}
	
	return result;
}

int main(int argc, const char* argv[]) {
	int result = 0;
	
	if (parse_args(argc, argv) == 1) {
		FILE	*fpInput = 0, *fpU = 0, *fpV = 0;
		char	u, v;
		
		fpInput = fopen(szInputName, "r");
		fpU		= fopen(szUName,     "w");
		fpV     = fopen(szVName,     "w");
		
		if ((fpInput == 0L) ||
			(fpU == 0L) ||
			(fpV == 0L))
		{
			printf("Must specify all files -i, -u, & -v\n");
			return -1;
		}
		
		while (!feof(fpInput)) {
			u = (char)fgetc(fpInput);
			v = (char)fgetc(fpInput);
			fputc((int)u, fpU);
			fputc((int)v, fpV);
		}
		
		fclose(fpV);
		fclose(fpU);
		fclose(fpInput);
	} else {
		printf("Must specify all files -i, -u, & -v\n");
		return -1;
	}
	
	return result;
}
