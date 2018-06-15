#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, 
	/* TODO: Add more token types */
	EQ = 255, AND = 254, OR = 253, 
	NEQ = 252, LEQ = 251, GEQ = 250, DEC = 249, 
	HEX = 248, REG = 247, LSHIFT = 246, RSHIFT = 245,
	DEREF = 244, 
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	
	{" +",	NOTYPE},				// white space
	{"\\+", '+'},
	{"==", EQ},
	{"-", '-'},
	{"*", '*'},
	{"/", '/'},
	{"%", '%'},
	{"&&", AND},
	{"||", OR},
	{"!=", NEQ},
	{"!", '!'},
	{"<=", LEQ},
	{">=", GEQ},
	{"<<", LSHIFT},
	{">>", RSHIFT},
	{">", '>'},
	{"<", '<'},
	{"0x[0-9a-fA-F]+", HEX},
	{"$[a-z]+", REG},
	{"[0-9]+", DEC},
	{"(", '('},
	{")", ')'},

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				// token length limit
				if (substr_len > 32) {
					printf("Error! Token length limit exceeded.\n");
					return false;
				}

				// clean
				int prev_token_len = strlen(tokens[nr_token].str);
				while (prev_token_len --)
					tokens[nr_token].str[preb_token_len] = '\0';
				
				switch(rules[i].token_type) {
					case DEC: 
					case HEX:
					case REG:
					case EQ:
					case AND:
					case OR:
					case NEQ:
					case LEQ:
					case GEQ:
					case LSHIFT:
					case RSHIFT:
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].type = rules[i].token_type;
						nr_token ++;
						break;
					case NOTYPE:
					default:
						 break;
				}

				break;
			}
		}
		
		if(nr_token > 32) {
			printf("Error! Expression memory limit exceeded.\n");
			return false;
		}


		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

// check if parentheses match
bool check_parentheses(int p, int q) {
	uint32_t L_cnt = 0,
		 R_cnt = 0;
	for(int i = p; i < q; ++ i) {
		if (tokens[i].type == '(')
			++ L_cnt;
		else if (tokens[i].type == ')')
			++ R_cnt;
		if (L_cnt <= R_cnt)
			return false;
	}
	if (L_cnt - R_cnt == 1 && tokens[q].type == ')')
		return true;
	return false;
}


// transfer hex to uint32
uint32_t hex2uint(char* str, bool* success) {
	int str_len = strlen(str);
	uint64_t res = 0;

	*success = true;
	for(int i = 2; i < str_len; ++ i) {
		if (str[i] >= '0' && str[i] <= 9)
			res = res * 0x10 + str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			res = res * 0x10 + str[i] - 'a';
		else if (str[i] >= 'A' && str[i] <= 'F')
			res = res * 0x10 + str[i] - 'A';
		else {
			*success = false;
			return 0;
		}
		// uint32_t overflow
		if (res >= 0x100000000) {
			printf("Error! Too large hexadecimal number.\n");
			*success = false
			return 0
		}
	}
	return (uint32_t)(res & 0xFFFFFFFF);
}

// transfer register to uint32
uint32_t reg2uint(char* str, bool* success) {
	*success = true;
	if (strcmp(str, "$eax") == 0)
		return cpu.eax;
	else if (strcmp(str, "$ecx") == 0)
		return cpu.ecx;
	else if (strcmp(str, "$edx") == 0)
		return cpu.edx;
	else if (strcmp(str, "$ebx") == 0)
		return cpu.ebx;
	else if (strcmp(str, "$esp") == 0)
		return cpu.esp;
	else if (strcmp(str, "$ebp") == 0)
		return cpu.ebp;
	else if (strcmp(str, "$esi") == 0)
		return cpu.esi;
	else if (strcmp(str, "$edi") == 0)
		return cpu.edi;
	else if (strcmp(str, "$eip") == 0)
		return cpu.eip;
	else {
		printf("No register matched.\n");
		*success = false; 
		return 0;
	}
}







uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	printf("\nPlease implement expr at expr.c\n");
	assert(0);

	return 0;
}

