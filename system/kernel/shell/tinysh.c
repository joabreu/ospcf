/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\shell\tinysh.c
    @brief Internal shell. Command handler
*/
/*
 * tinysh.c
 *
 * Minimal portable shell
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
#include <string.h>
#include <kernel\console.h>
#include <drivers\char\keyboard.h>
#include "tinysh.h"

int PROMPT_COLOR = 15;  // Branco

/** @fn static void help_fnt( int argc, char **argv )
    @brief Callback for help command
    @ingroup sys_kernel_shell 
    @param argc Arguments count
    @param argv Arguments list
    @internal
*/
static void help_fnt(int argc, char **argv);

/** @brief Shell Command structure: Help command 
    @internal
*/
static tinysh_cmd_t help_cmd={ 0, "help", "Mostra a ajuda", 0, help_fnt, 0, 0, 0 };

/** @brief History of commands
    @internal
*/
static char input_buffers[HISTORY_DEPTH][BUFFER_SIZE+1]={ };
/** @brief Trash buffer
    @internal
*/
static char trash_buffer[BUFFER_SIZE+1]={0};
/** @brief Current input_buffers index
    @internal
*/
static int cur_buf_index=0;
/** @brief Current context buffer (prompt)
    @internal
*/
static char context_buffer[BUFFER_SIZE+1]={0};
/** @brief Current context_buffer index
    @internal
*/
static int cur_context=0;
/** @brief Current line index
    @internal
*/
static int cur_index=0;
/** @brief Print input keys as we enter it. Echo commands
    @internal
*/
static int echo=1;
/** @brief Shell prompt
    @internal
*/
static char prompt[PROMPT_SIZE+1]="> ";
/** @brief Root Command, first added
    @internal
*/
static tinysh_cmd_t * root_cmd=&help_cmd;
/** @brief Current command context
    @internal
*/
static tinysh_cmd_t * cur_cmd_ctx=0;
/** @brief Current command argument
    @internal
*/
static void * tinysh_arg=0;

static void help_fnt( int argc, char **argv )
{
    kprintf("?            Display help on given or available commands\n");
    kprintf("<TAB>        Auto-completion\n");
    kprintf("<RETURN>     Execute command line\n");
    kprintf("<UP>         Recall previous input line\n");
    kprintf("<DOWN>       Recall next input line\n");
    kprintf("<any>        Treat as input character\n");
}

enum { NULLMATCH,FULLMATCH,PARTMATCH,UNMATCH,MATCH,AMBIG };

/** @fn int strstart(char *s1, char *s2)
    @brief Verify if the non-spaced part of s2 is included at the begining of s1

    Return FULLMATCH if s2 equal to s1, PARTMATCH if s1 starts with s2 but there are remaining chars in s1, UNMATCH if s1 does not start with s2
    @ingroup sys_kernel_shell 
    @param s1 string
    @param s2 String
    @return FULLMATCH,PARTMATCH and UNMATCH
*/
int strstart(char *s1, char *s2)
{
    while(*s1 && *s1==*s2) { s1++; s2++; }

    if(*s2==' ' || *s2==0)
    {
         if(*s1==0)
              return FULLMATCH; // full match
         else
              return PARTMATCH; // partial match
    }
    else
         return UNMATCH;     // no match
}

/** @fn static int parse_command(tinysh_cmd_t **_cmd, char **_str)
    @brief Check command at given livel with input string
    @ingroup sys_kernel_shell 
    @param _cmd First command at this level, return matched cmd
    @param _str Point to first unprocessed input, return next unprocessed
    @return NULLMATCH, MATCH, AMBIG, UNMATCH command
    @internal
*/
static int parse_command(tinysh_cmd_t **_cmd, char **_str)
{
    char *str=*_str;
    tinysh_cmd_t *cmd;
    //int matched_len=0;
    tinysh_cmd_t *matched_cmd=0;

    // first eliminate first blanks
    while(*str==' ') str++;
    if(!*str)
    {
         *_str=str;
         return NULLMATCH; // end of input
    }
  
    // first pass: count matches
    for(cmd=*_cmd;cmd;cmd=cmd->next)
    {
         int ret=strstart((char *)cmd->name,str);
         if(ret==FULLMATCH)
         {
              // found full match
              while(*str && *str!=' ') str++; 
              while(*str==' ') str++;
              *_str=str;
              *_cmd=cmd;
              return MATCH;
         }
         else if (ret==PARTMATCH)
         {
              if(matched_cmd)
              {
                   *_cmd=matched_cmd;
                   return AMBIG;
              }
              else
                   matched_cmd=cmd;
         }
         else 
         {
              // UNMATCH
         }
    }
    
    if(matched_cmd)
    {
         // Disable this and you will have to write the full parth of cmd
         while(*str && *str!=' ') str++; 
         while(*str==' ') str++;
         *_cmd=matched_cmd;
         *_str=str;
         return MATCH;
         //return UNMATCH;
    }
    else
         return UNMATCH;
}

/** @fn static void do_context(tinysh_cmd_t *cmd, char *str)
    @brief Create a context from current input line
    @ingroup sys_kernel_shell 
    @param cmd Command
    @param str Input line
    @internal
*/
static void do_context(tinysh_cmd_t *cmd, char *str)
{
    while(*str) 
         context_buffer[cur_context++]=*str++;
    context_buffer[cur_context]=0;
    cur_cmd_ctx=cmd;
}

/** @fn static void exec_command(tinysh_cmd_t *cmd, char *str)
    @brief Execute the given command by calling callback with appropiate arguments
    @ingroup sys_kernel_shell 
    @param cmd Command
    @param str Input line
    @internal
*/
static void exec_command(tinysh_cmd_t *cmd, char *str)
{
    char *argv[MAX_ARGS];
    int argc=0;
    int i;

    // copy command line to preserve it for history
    for(i=0;i<BUFFER_SIZE;i++)
         trash_buffer[i]=str[i];
    str=trash_buffer;

    // cut into arguments
    argv[argc++]=cmd->name;
    while(*str && argc<MAX_ARGS)
    {
         while(*str==' ') str++;
         if(*str==0)
              break;
         argv[argc++]=str;
         while(*str!=' ' && *str) str++;
         if(!*str) break;
         *str++=0;
    }
    // call command function if present
    if(cmd->function)
    {
         tinysh_arg=cmd->arg;
         cmd->function(argc,&argv[0]);
    }
}
/** @fn static int exec_command_line(tinysh_cmd_t *cmd, char *_str)
    @brief Try to execute the current command line
    @ingroup sys_kernel_shell 
    @param cmd Command
    @param _str Input line
    @return Zero
    @internal
*/
static int exec_command_line(tinysh_cmd_t *cmd, char *_str)
{
    char *str=_str;

    while(true)
    {
         int ret;
         ret=parse_command(&cmd,&str);
         if(ret==MATCH) // found unique match
         {
              if(cmd)
              {
                   if(!cmd->child) // no sub-command, execute
                   {
                        exec_command(cmd,str);
                        return 0;
                   }
                   else
                   {
                        if(*str==0) // no more input, this is a context
                        {
                             do_context(cmd,_str);
                             return 0;
                        }
                        else // process next command word
                             cmd=cmd->child;
                   }
              } 
              else // cmd == 0
                   return 0;
         }
         else if(ret==AMBIG)
         {
              kprintf("Ambiguidade: '%s'\n", str);
              return 0;
         }
         else if(ret==UNMATCH) // UNMATCH
         {
              kprintf("Comando nao reconhecido: '%s'\n", str);
              return 0;
         }
         else // NULLMATCH
              return 0;
    }
}
/** @fn static void display_child_help(tinysh_cmd_t *cmd)
    @brief Display help for list of commands
    @ingroup sys_kernel_shell 
    @param cmd Command
    @internal
*/
static void display_child_help(tinysh_cmd_t *cmd)
{
    tinysh_cmd_t *cm;
    int len=0;

    kprintf("\n");
    for(cm=cmd;cm;cm=cm->next)
         if(len<strlen(cm->name))
              len=strlen(cm->name);
    for(cm=cmd;cm;cm=cm->next)
         if(cm->help)
         {
              int i;
              kprintf(cm->name);
              for(i=strlen(cm->name);i<len+2;i++)
                   kprintf(" ");
              kprintf(cm->help);
              kprintf("\n");
         }
}
/** @fn static int help_command_line(tinysh_cmd_t *cmd, char *_str)
    @brief Try to display help for current command line
    @ingroup sys_kernel_shell 
    @param cmd Command
    @param _str Input line
    @return Zero
    @internal
*/
static int help_command_line(tinysh_cmd_t *cmd, char *_str)
{
    char *str=_str;

    while(true)
    {
         int ret;
         ret=parse_command(&cmd,&str);
         if(ret==MATCH && *str==0) // found unique match or empty line
         {
              //tinysh_cmd_t *cm;
              // int len=0;
              if(cmd->child) // display sub-commands help
              {
                   display_child_help(cmd->child);
                   return 0;
              }
              else  // no sub-command, show single help
              {
                   if(*(str-1)!=' ')
                        kprintf(" ");
                   if(cmd->usage)
                        kprintf(cmd->usage);
                   kprintf(": ");
                   if(cmd->help)
                        kprintf(cmd->help);
                   else
                        kprintf("Ajuda nao disponivel\n");
                   kprintf("\n");
              }
              return 0;
         }
         else if(ret==MATCH && *str)  // continue processing the line
         {
              cmd=cmd->child;
         }
         else if(ret==AMBIG)
         {
              kprintf("\nAmbiguidade: '%s'\n", str);
              return 0;
         }
         else if(ret==UNMATCH)
         {
              kprintf("\nComando nao reconhecido: '%s'\n", str);
              return 0;
         }
         else // NULLMATCH
         {
              if(cur_cmd_ctx)
                   display_child_help(cur_cmd_ctx->child);
              else
                   display_child_help(root_cmd);
              return 0;
         }
    }
}

/** @fn static int complete_command_line(tinysh_cmd_t *cmd, char *_str)
    @brief Try to complete current command line
    @ingroup sys_kernel_shell 
    @param cmd Command
    @param _str Input line
    @return TRUE or FALSE, for matched command or not
    @internal
*/
static int complete_command_line(tinysh_cmd_t *cmd, char *_str)
{
    char *str=_str;

    while(true)
    {
         int ret;
         int common_len=BUFFER_SIZE;
         int _str_len;
         int i;
         char *__str=str;

         // tinysh_cmd_t *_cmd=cmd;
         ret=parse_command(&cmd,&str);
         for(_str_len=0;__str[_str_len]&&__str[_str_len]!=' ';_str_len++);
              if(ret==MATCH && *str)
              {
                   cmd=cmd->child;
              }
              else if(ret==AMBIG || ret==MATCH || ret==NULLMATCH)
              {
                   tinysh_cmd_t *cm;
                   tinysh_cmd_t *matched_cmd=0;
                   int nb_match=0;
              
                   for(cm=cmd;cm;cm=cm->next)
                   {
                        int r=strstart(cm->name,__str);
                        if(r==FULLMATCH)
                        {
                             for(i=_str_len;cmd->name[i];i++)
                                  tinysh_char_in(cmd->name[i]);
                             if(*(str-1)!=' ')
                                  tinysh_char_in(' ');
                             if(!cmd->child)
                             {
                                  if(cmd->usage)
                                  {
                                       kprintf("%s\n", cmd->usage);
                                       return 1;
                                  }
                                  else
                                       return 0;
                             }
                             else
                             {
                                  cmd=cmd->child;
                                  break;
                             }
                        }
                        else if(r==PARTMATCH)
                        {
                             nb_match++;
                             if(!matched_cmd)
                             {
                                  matched_cmd=cm;
                                  common_len=strlen(cm->name);
                             }
                             else
                             {
                                  for(i=_str_len;cm->name[i] && i<common_len &&
                                       cm->name[i]==matched_cmd->name[i];i++);
                                  if(i<common_len)
                                       common_len=i;
                             }
                        }
                   }
                   if(cm)
                        continue;
                   if(matched_cmd)
                   {
                        if(_str_len==common_len)
                        {
                             kprintf("\n");
                             for(cm=cmd;cm;cm=cm->next)
                             {
                                  int r=strstart(cm->name,__str);
                                  if(r==FULLMATCH || r==PARTMATCH)
                                  {
                                       kprintf("%s\n", cm->name);
                                  }
                             }
                             return 1;
                        }
                        else
                        {
                             for(i=_str_len;i<common_len;i++)
                                  tinysh_char_in(matched_cmd->name[i]);
                             if(nb_match==1)
                                  tinysh_char_in(' ');
                        }
                   }
                   return 0;
              }
              else // UNMATCH
                   return 0;
    }
}
/** @fn static void start_of_line()
    @brief Start a new line: prints the prompt (context buffer)
    @ingroup sys_kernel_shell 
    @internal
*/
static void start_of_line()
{
	// display start of new line
	int color = ConsoleRetColour();
    ConsoleSetColour(PROMPT_COLOR);
    kprintf(prompt);
	if(cur_context)
    	 kprintf("%s> ", context_buffer);
    cur_index = 0;
    ConsoleSetColour(color);
}

// character input 
void tinysh_char_in(unsigned long c)
{
    char * line=input_buffers[cur_buf_index];

    if(c == 0)
         return;
    if(c=='\n' || c=='\r') // validate command
    {
         tinysh_cmd_t *cmd;
         // first, echo the newline
         if(echo)
         {
              int color = ConsoleRetColour();
              ConsoleSetColour(PROMPT_COLOR);
              ConsolePutch(c);
              ConsoleSetColour(color);
         }
         while(*line && *line==' ') line++;
         if(*line) // not empty line
         {
              cmd=cur_cmd_ctx?cur_cmd_ctx->child:root_cmd;              
              exec_command_line(cmd,line);
              cur_buf_index=(cur_buf_index+1)%HISTORY_DEPTH;
              cur_index=0;
              input_buffers[cur_buf_index][0]=0;
         }
         start_of_line();
    }
    else if(c==TOPCHAR) // return to top level
    {
         if(echo)
         {
              int color = ConsoleRetColour();
              ConsoleSetColour(PROMPT_COLOR);
              ConsolePutch(c);
              ConsoleSetColour(color);
         }
         cur_context=0;
         cur_cmd_ctx=0;
    }
    else if(c==8 || c==127) // backspace
    {
         if(cur_index>0)
         {
              kprintf("\b \b");
              cur_index--;
              line[cur_index]=0;
         }
    }
    else if(c==(KEY_UPARROW | KB_SPECIAL)) // Up arrow: back in history
    {
         int prevline=(cur_buf_index+HISTORY_DEPTH-1)%HISTORY_DEPTH;
         if(input_buffers[prevline][0])
         {
              line=input_buffers[prevline];
              // fill the rest of the line with spaces
              while(cur_index-->strlen(line))
                   kprintf("\b \b");
              kprintf("\r");
              start_of_line();
              
              int color = ConsoleRetColour();
              ConsoleSetColour(PROMPT_COLOR);
              
              kprintf("%s", line);
              ConsoleSetColour(color);
              
              cur_index=strlen(line);
              cur_buf_index=prevline;
        }
    }
    else if(c==(KEY_DOWNARROW | KB_SPECIAL)) // down: next in history
    {
        int nextline=(cur_buf_index+1)%HISTORY_DEPTH;
        if(input_buffers[nextline][0])
        {
              line=input_buffers[nextline];
              // fill the rest of the line with spaces
              while(cur_index-->strlen(line))
                   kprintf("\b \b");
              kprintf("\r");
              start_of_line();
              
              int color = ConsoleRetColour();
              ConsoleSetColour(PROMPT_COLOR);
              
              kprintf("%s", line);
              ConsoleSetColour(color);
              
              cur_index=strlen(line);
              cur_buf_index=nextline;
        }
    }
    else if(c=='?') // display help
    {
        tinysh_cmd_t *cmd;
        cmd=cur_cmd_ctx?cur_cmd_ctx->child:root_cmd;
        help_command_line(cmd,line);
        start_of_line();
        kprintf("%s", line);
        cur_index=strlen(line);
    }
    else if(c==9 || c=='!') // TAB: autocompletion
    {
        tinysh_cmd_t *cmd;
        cmd=cur_cmd_ctx?cur_cmd_ctx->child:root_cmd;
        if(complete_command_line(cmd,line))
        {
              start_of_line();
              int color = ConsoleRetColour();
              ConsoleSetColour(PROMPT_COLOR);
              kprintf("%s", line);
              ConsoleSetColour(color);
        }
        cur_index=strlen(line);
    }      
    else // any input character
    {
         // Prevent special keys to be inputed
         if(!(c & KB_SPECIAL))
         {
        
              if(cur_index<BUFFER_SIZE)
              {
                   if(echo)
                   {
                        int color = ConsoleRetColour();
                        ConsoleSetColour(PROMPT_COLOR);
                        ConsolePutch(c);
                        ConsoleSetColour(color);
                   }
                   line[cur_index++]=c;
                   line[cur_index]=0;
              }
        
         }
    }
}
// add a new command
void tinysh_add_command(tinysh_cmd_t *cmd)
{
    tinysh_cmd_t *cm;

    if(cmd->parent)
    {
         cm=cmd->parent->child;
         if(!cm)
         {
              cmd->parent->child=cmd;
         }
         else
         {
              while(cm->next)
          	       cm=cm->next;
              cm->next=cmd;
         }
    }
    else if(!root_cmd)
    {
         root_cmd=cmd;
    }
    else
    {
         cm=root_cmd;
         while(cm->next) cm=cm->next;
         cm->next=cmd;      
    }
}
// modify shell prompt
void tinysh_set_prompt(char * str)
{
    int i;
    for(i=0;str[i] && i<PROMPT_SIZE;i++)
         prompt[i]=str[i];
    prompt[i]=0;
    // force prompt display by generating empty command
    tinysh_char_in('\r');
}
// return current command argument
void* tinysh_get_arg()
{
    return tinysh_arg;
}
// string to decimal/hexadecimal conversion
unsigned long tinysh_atoxi(char * s)
{
    int ishex=0;
    unsigned long res=0;

    if(*s==0) return 0;

    if(*s=='0' && *(s+1)=='x')
    {
         ishex=1;
         s+=2;
    }

    while(*s)
    {
         if(ishex)
	          res*=16;
         else
	          res*=10;

         if(*s>='0' && *s<='9')
	          res+=*s-'0';
         else if(ishex && *s>='a' && *s<='f')
	          res+=*s+10-'a';
         else if(ishex && *s>='A' && *s<='F')
	          res+=*s+10-'A';
         else
	         break;
         s++;
    }
    return res;
}
