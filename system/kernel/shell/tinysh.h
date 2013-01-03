/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\shell\tinysh.h
    @brief Internal shell. Command handler
*/
#ifndef SYSTEM_KERNEL_SHELL_TINYSH_H
#define SYSTEM_KERNEL_SHELL_TINYSH_H
/*
 * tinysh.h
 *
 * Header for minimal portable shell
 *
 * Copyright (C) 2001 Michel Gutierrez <mig@nerim.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef BUFFER_SIZE
/** @brief Shell Buffer size */
#define BUFFER_SIZE 256
#endif
#ifndef HISTORY_DEPTH
/** @brief Maximum number of commands to store in memory */
#define HISTORY_DEPTH 16
#endif
#ifndef MAX_ARGS
/** @brief Maximum number of command arguments */
#define MAX_ARGS 16
#endif
#ifndef PROMPT_SIZE
/** @brief Size of prompt */
#define PROMPT_SIZE 64
#endif
#ifndef TOPCHAR
/** @brief Top Char */
#define TOPCHAR '\\'
#endif

/** @brief Structure of a shell command: argc, argv */
typedef void (*tinysh_fnt_t)(int argc, char **argv);

/** @brief Structure of a command in tinysh */
typedef struct tinysh_cmd_t
{
    /** @brief Parent command. 0 if top level command */
    struct tinysh_cmd_t * parent;
    /** @brief Command input name. Can not be 0 */
    char * name;
    /** @brief Command help string, can be 0 */
    char * help;
    /** @brief Command usage string, can be 0 */
    char * usage;
    /** @brief Function to launch on cmd, can be 0 */
    tinysh_fnt_t function;
    /** @brief Current argument when function called */
    void * arg;
    /** @brief Must be set to 0 at init */
    struct tinysh_cmd_t * next;
    /** @brief Must be set to 0 at init */
    struct tinysh_cmd_t * child;
} tinysh_cmd_t;

/** @brief Just for help */
typedef tinysh_cmd_t SHELLCMD;

/** @brief Current colour of prompt */
extern int PROMPT_COLOR;

#ifdef __cplusplus
extern "C" {
#endif

/** @fn void tinysh_char_in(unsigned long c)
    @brief Envia uma nova tecla para ser intrepertado pelo tinysh
    @param c Caracter a interpretar
    @ingroup sys_kernel_shell 
*/
void tinysh_char_in(unsigned long c);
/** @fn void tinysh_add_command(tinysh_cmd_t *cmd)
    @brief Adiciona um novo comando á lista de comandos disponíveis
    @param cmd Comando a adicionar
    @ingroup sys_kernel_shell 
*/
void tinysh_add_command(tinysh_cmd_t *cmd);
/** @fn void tinysh_set_prompt(char * str)
    @brief Define o prompt da shell (a nova linha que é imprimida a cada enter)
    @param str Novo prompt
    @ingroup sys_kernel_shell 
*/
void tinysh_set_prompt(char * str);
/** @fn void* tinysh_get_arg()
    @brief Devolve pointer para os argumentos que o comando a ser executado está a usar
    @return Pointer para lista de args
    @ingroup sys_kernel_shell 
*/
void* tinysh_get_arg();
/** @fn unsigned long tinysh_atoxi(char * s)
    @brief Converte uma string para número escalar (suporta decimal ou hexadecimal)
    @param s String a converter
    @return Escalar da string
    @ingroup sys_kernel_shell 
*/
unsigned long tinysh_atoxi(char * s);

#ifdef __cplusplus
}
#endif

#endif
