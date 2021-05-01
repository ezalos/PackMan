#include "head.h"
#define SPACES 1

extern int depth;

int debug_level = DEBUG;

void	logging_set_level(int level)
{
    debug_level = level;
}

void	logging(const char* fmt, ...)
{
    va_list args;

    if (debug_level >= 1)
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
            printf("\t");
            k++;
        }
        j++;
    }
}

void	logging_recursive(const char* fmt, ...)
{
    va_list args;

    if (debug_level >= 1)
    {
        print_spaces(depth);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void	debug(const char* fmt, ...)
{
    va_list args;

    if (debug_level >= 2)
    {
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void	debug_recursive(const char* fmt, ...)
{
    va_list args;

    if (debug_level >= 2)
    {
        print_spaces(depth);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void	log_btc_name(t_btc *inst)
{
	logging(btc_to_str(inst));
}

char	*btc_to_str(t_btc *inst)
{
	if (inst->type == BTC_CALL_JMP)
		return ("BTC_CALL_JMP");
	else if (inst->type == BTC_DEF_FIND_ABS_VADDR)
		return ("BTC_DEF_FIND_ABS_VADDR");
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