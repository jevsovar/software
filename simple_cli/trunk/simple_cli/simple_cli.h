/// @defgroup	SIMPLE_CLI Simple CLI
/// @{
/// @brief		Simple Command line interface tool.
/// @details	This module is used for parsing command strings (eg. from UART).

/// @file		simple_cli.h
/// @brief		Simple CLI header file
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

#ifndef SIMPLE_CLI_
#define SIMPLE_CLI_

#include <stdint.h>

#ifdef _cplusplus
extern "C"
{
#endif

/// Maximum number of commands that can be registered in one CLI instance.
#define MAX_NR_OF_CMDS			   10
/// Longest command name allowed to be registered.
#define MAX_LENGTH_OF_CMD_NAME	   20
/// Length of response buffer.
#define MAX_LENGTH_OF_RESPONSE	   100
/// Maximum number of parameters of registered functions.
#define MAX_NR_OF_PARAMETERS	   4

/// Main CLI structure definition.
typedef struct
{
	/// String array for names of registered commands. 
	char cmdName[MAX_NR_OF_CMDS][MAX_LENGTH_OF_CMD_NAME];
	/// Array of pointers to the functions of registered commands.
	void(*cmdFunction[MAX_NR_OF_CMDS])(char *response, size_t responseSize, char **par);
	/// Array of number of parameters for each registered function.
	uint8_t nrOfParameters[MAX_NR_OF_CMDS];

	/// Storage for parsed parameters.
	char *parameters[MAX_NR_OF_PARAMETERS];

	/// Counter for keeping track how many commands have been registered already.
	uint16_t nrOfCmdsRegistered;

	/// Response storage buffer.
	char response[MAX_LENGTH_OF_RESPONSE];
} cli_t;

/// @brief		Initialization function for CLI.
/// @param		cli : pointer to instantiated CLI
void CliInit(cli_t *cli);

/// @brief		Registers selected function to CLI command definitions.
/// @param		cli : pointer to chosen CLI instance
/// @param		name : registered function name
/// @param		function : pointer to registered function
/// @param		nrOfPar : number of parameters of registered function
void CliRegisterCommand(cli_t *cli, char *name, void(*function)(char *response, size_t responseSize, char **par), uint8_t nrOfPar);

/// @brief		Checks if passed command matches any registered command and executes that command function.
/// @param		cli : pointer to chosen CLI instance
/// @param		cmd	: command string with all function parameters
/// @returns	pointer to matched function response string if command was found, else returns null pointer
char* CliParseCommand(cli_t *cli, char *cmd);

#ifdef _cplusplus
}
#endif

#endif

/// @}
