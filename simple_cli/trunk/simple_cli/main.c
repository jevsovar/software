/// @file		main.c
/// @brief		This file is an example of Command line interface
///				implementation with three command functions.
///
/// @author		Jan Jevsovar   -   jan.jevsovar@gmail.com
///
/// @version	1.0.1
/// @date		27th June 2014
///
/// @copyright
/// This program is free software; you can redistribute it and/or
/// modify it under the terms of the GNU General Public License as
/// published by the Free Software Foundation; either version 2 of
/// the License, or (at your option) any later version. <br>
/// <br>This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
/// General Public License for more details at
/// http://www.gnu.org/copyleft/gpl.html

#include <stdio.h>
#include <string.h>
#include "simple_cli.h"

/// @brief User function 0. 
///	This command accepts two string parameters, converts them to integers and returns their sum.
void sum(char *response, size_t responseSize, char *par[2])
{
	// Get pointers to parameter strings.
	char *par1 = par[0];
	char *par2 = par[1];
	
	// Execute user function.
	int x = 0;
	int y = 0;
	
	sscanf(par1, "%d", &x);
	sscanf(par2, "%d", &y);

	int sum = x + y;
	
	// Write response.
	_snprintf(response, responseSize - 1, "sum = %d\r\n", sum);
}

/// @brief User function 1. Accepts two string parameters and prints them out directly.
void fun1(char *response, size_t responseSize, char *par[2])
{
	char *par1 = par[0];
	char *par2 = par[1];

	printf("fun1 : <%s><%s>\r\n", par1, par2);
}

/// @brief User function 2. Accepts two string parameters and prints them out directly.
void fun2(char *response, size_t responseSize, char *par[])
{
	char *parameter[4];
	for (int i = 0; i < 4; i++) parameter[i] = par[i];
	//for (int i = 0; i < 4; i++) parameter[i] = *(par + i);

	_snprintf(response, responseSize - 1, "fun2 : <%s><%s><%s><%s>\r\n", parameter[0], parameter[1], parameter[2], parameter[3]);
}

/// @brief Main example function.
int main(void)
{
	cli_t cli;

	CliInit(&cli);

	CliRegisterCommand(&cli, "fun1", fun1, 2);
	CliRegisterCommand(&cli, "fun2", fun2, 4);
	CliRegisterCommand(&cli, "sum",  sum,  2);

	// Command that triggers function fun1
	//char cmd[] = "fun1 one two";
	
	// Command that triggers function fun2
	char cmd[] = "fun2 three four five six";
	
	// Command that triggers function sum
	//char cmd[] = "sum 12 5";

	char *response = NULL;
	response = CliParseCommand(&cli, cmd);

	if (response != NULL)
	{
		printf("%s\r\n", response);
	}
	else
	{
		printf("No command found.\r\n");
	}

	getchar();
	return 0;
}
