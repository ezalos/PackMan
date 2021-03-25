#include <stdio.h>
#include <stdarg.h>
#include "head.h"
#define DEBUG 1
#define SPACES 2

int debug_level = DEBUG;

void	logging_set_level(int level)
{
    debug_level = level;
}

void	logging(const char* fmt, ...)
{
    va_list args;

    if (debug_level == 1)
    {
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void	print_spaces(int nb)
{
    int j;
    int k;

    j = 0;
    while (j < nb)
    {
        k = 0;
        while (k < SPACES)
        {
            printf(" ");
            k++;
        }
        j++;
    }
}

void	logging_recursive(int recursive_depth, const char* fmt, ...)
{
    va_list args;

    if (debug_level == 1)
    {
        print_spaces(recursive_depth);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void	log_btc_name(t_btc *inst)
{
	if (inst->type == BTC_CALL_JMP)
		logging("BTC_CALL_JMP\n");
	else if (inst->type == BTC_CALL_MPROTECT)
		logging("BTC_CALL_MPROTECT\n");
	else if (inst->type == BTC_CALL_CYPHER)
		logging("BTC_CALL_CYPHER\n");
	else if (inst->type == BTC_DEF_WRITE)
		logging("BTC_DEF_WRITE\n");
	else if (inst->type == BTC_DEF_CYPHER)
		logging("BTC_DEF_CYPHER\n");
	else if (inst->type == BTC_DEF_INIT_PERM)
		logging("BTC_DEF_INIT_PERM\n");
	else if (inst->type == BTC_DEF_KEY_SCHED)
		logging("BTC_DEF_KEY_SCHED\n");
	else if (inst->type == BTC_DEF_BEGIN)
		logging("BTC_DEF_BEGIN\n");
	else if (inst->type == BTC_DEF_CYPHER_PREPARE)
		logging("BTC_DEF_CYPHER_PREPARE\n");
	else if (inst->type == BTC_DEF_END)
		logging("BTC_DEF_END\n");
	else
		dprintf(2, "Error: Unknown btc type\n");
}

char	*btc_to_str(t_btc *inst)
{
	if (inst->type == BTC_CALL_JMP)
		return("BTC_CALL_JMP");
	else if (inst->type == BTC_CALL_MPROTECT)
		return("BTC_CALL_MPROTECT");
	else if (inst->type == BTC_CALL_CYPHER)
		return("BTC_CALL_CYPHER");
	else if (inst->type == BTC_DEF_WRITE)
		return("BTC_DEF_WRITE");
	else if (inst->type == BTC_DEF_CYPHER)
		return("BTC_DEF_CYPHER");
	else if (inst->type == BTC_DEF_INIT_PERM)
		return("BTC_DEF_INIT_PERM");
	else if (inst->type == BTC_DEF_KEY_SCHED)
		return("BTC_DEF_KEY_SCHED");
	else if (inst->type == BTC_DEF_BEGIN)
		return("BTC_DEF_BEGIN");
	else if (inst->type == BTC_DEF_CYPHER_PREPARE)
		return("BTC_DEF_CYPHER_PREPARE");
	else if (inst->type == BTC_DEF_END)
		return("BTC_DEF_END");
	else
		dprintf(2, "Error: Unknown btc type\n");
	return ("UNIDENTIFIED T_BTC TYPE");
}