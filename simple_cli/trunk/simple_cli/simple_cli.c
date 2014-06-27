/// @ingroup	SIMPLE_CLI Simple CLI
/// @{

/// @file		simple_cli.c
/// @brief		Simple CLI source code file
/// @author		Jan Jevsovar   -   jan.jevsovar@gmail.com
///
/// @version	1.0.0
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
#include <stdint.h>
#include <assert.h>
#include "simple_cli.h"

static uint8_t stringLongerThan(char *string, uint16_t maxLength)
{
	uint8_t isLonger = 1;
	for (uint16_t ch = 0; ch < maxLength; ch++)
	{
		if (string[ch] == '\0')
		{
			isLonger = 0;
			break;
		}
	}
	return isLonger;
}

static uint8_t cliParametersSplit(cli_t *cli, char *cmd, char **name, char **par)
{
	// Split cmd string to name and parameters.
	*name = strtok(cmd, " ");
	uint8_t nrOfParRead = 0;
	// Check for NR_OF_PARAMETERS + 1 to see also if there is too many parameters
	for (int parCnt = 0; parCnt < MAX_NR_OF_PARAMETERS + 1; parCnt++)
	{
		char *p = NULL;
		p = strtok(NULL, " ");

		if (p != NULL)
		{
			if (parCnt < MAX_NR_OF_PARAMETERS) cli->parameters[parCnt] = p;
			nrOfParRead++;
		}
		else
		{
			break;
		}
	}
	return nrOfParRead;
}

static uint8_t cliCommandCheck(cli_t *cli, char *name, uint8_t nrOfParRead)
{
	uint8_t cmdFound = 0;

	for (uint16_t cmdIndex = 0; cmdIndex < cli->nrOfCmdsRegistered; cmdIndex++)
	{
		assert(!stringLongerThan(cli->cmdName[cmdIndex], MAX_LENGTH_OF_CMD_NAME));

		// First, number of parameters must match.
		if (nrOfParRead == cli->nrOfParameters[cmdIndex])
		{
			// Check if cmd matches any registered command name.
			if (!strncmp(cli->cmdName[cmdIndex], name, sizeof(cli->cmdName[cmdIndex])))
			{
				// Clear response buffer.
				cli->response[0] = '\0';
				// Execute matching command.
				cli->cmdFunction[cmdIndex](cli->response, sizeof(cli->response), &cli->parameters[0]);
				cmdFound = 1;
				break;
			}
		}
	}

	return cmdFound;
}

void CliInit(cli_t *cli)
{
	memset(cli, 0, sizeof(cli_t));
}

void CliRegisterCommand(cli_t *cli, char *name, void(*function)(char *response, size_t responseSize, char **par), uint8_t nrOfPar)
{
	// Check if number of parameters suits.
	assert(nrOfPar <= MAX_NR_OF_PARAMETERS);

	// Check if CLI definition space is full.
	assert(cli->nrOfCmdsRegistered < MAX_NR_OF_CMDS);

	// Check length of command name.
	assert(!stringLongerThan(name, MAX_LENGTH_OF_CMD_NAME));

	// Check if function pointer points anywhere.
	assert(function != NULL);

	// Register number of parameters.
	cli->nrOfParameters[cli->nrOfCmdsRegistered] = nrOfPar;

	// Register command name.
	strncpy(cli->cmdName[cli->nrOfCmdsRegistered], name, MAX_LENGTH_OF_CMD_NAME - 1);

	// Register command function.
	cli->cmdFunction[cli->nrOfCmdsRegistered] = function;

	cli->nrOfCmdsRegistered++;
}

char* CliParseCommand(cli_t *cli, char *cmd)
{
	char *name = 0;
	char *par[MAX_NR_OF_PARAMETERS];

	uint8_t nrOfParRead = 0;
	uint8_t cmdFound = 0;

	// Parse out command name and parameters,
	nrOfParRead = cliParametersSplit(cli, cmd, &name, par);
	// then check if command matches any registered command.
	cmdFound = cliCommandCheck(cli, name, nrOfParRead);

	// Check if registered function has overflown response buffer.
	assert(!stringLongerThan(cli->response, MAX_LENGTH_OF_RESPONSE));

	if (cmdFound) return cli->response;
	else return NULL;
}

/// @}
