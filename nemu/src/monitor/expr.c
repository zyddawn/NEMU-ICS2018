#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

#define DEBUG

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
	
	{" +", NOTYPE},				// white space
	{"\\+", '+'},
	{"==", EQ},
	{"\\-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"%", '%'},
	{"&&", AND},
	{"\\|\\|", OR},
	{"!=", NEQ},
	{"!", '!'},
	{"<=", LEQ},
	{">=", GEQ},
	{"<<", LSHIFT},
	{">>", RSHIFT},
	{">", '>'},
	{"<", '<'},
	{"\\0x[0-9a-fA-F]+", HEX},
	{"\\$[a-z]+", REG},
	{"[0-9]+", DEC},
	{"\\(", '('},
	{"\\)", ')'},

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
				// printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
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
				int clean_token_len = strlen(tokens[nr_token].str);
				while (clean_token_len --)
					tokens[nr_token].str[clean_token_len] = '\0';
				
				switch(rules[i].token_type) {
					case DEC: case HEX: case REG: case EQ: 
					case AND: case OR: case NEQ: case LEQ: 
					case GEQ: case LSHIFT: case RSHIFT: 
					case '+': case '-': case '*': case '/': 
					case '!': case '>': case '<': case '%':
					case '(': case ')':
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].type = rules[i].token_type;
						nr_token ++;
						break;
					case NOTYPE:
					default: break;
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
	if(tokens[p].type != '(') {
		printf("Error! No need to call check_parentheses in this position.\n");
		return false;
	}
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
			*success = false;
			return 0;
		}
	}
	return (uint32_t)res;
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

int assign_priority(int op) {
	switch(op) {
		case AND: case OR:
			return 1;
		case NEQ: case EQ:
			return 2;
		case LEQ: case GEQ: case '>': case '<':
			return 3;
		case LSHIFT: case RSHIFT: 
			return 4;
		case '+': case '-':
			return 5;
		case '%': case '*': case '/':
			return 6;
		case DEREF: case '!':
			return 7;
		case ')': case '(': case DEC: case REG: case HEX:
			return -1;
		default:
			return 100;
	}
}

int dominant_op(int p, int q) {
	int cnt_parentheses = 0,
	    min_prior = 10, min_index = -1;
	for (int i = p; i <= q; ++ i) {
		if (tokens[i].type == '(')
			++ cnt_parentheses;
		else if (tokens[i].type == ')')
			-- cnt_parentheses;
		if(cnt_parentheses < 0) {
			printf("Parentheses not matched!\n");
			return -1;
		}
		int cur_prior = assign_priority(tokens[i].type);
		if (cnt_parentheses == 0 && cur_prior > 0 && min_prior >= cur_prior) {
			min_prior = cur_prior;
			min_index = i;
		}
	}
	if (cnt_parentheses != 0) {
		printf("Parentheses not matched!\n");
		return -1;
	}
	if(min_index == -1)
		printf("Dominant operator not found!\n");
	return min_index;
}


uint32_t eval(int p, int q, bool *success) {
	*success = true;
	for(int i=p; i<=q; ++i)
		printf("%s ", tokens[i].str);
	printf("\n");
	if (p > q) {
		*success = false;
		printf("Error! Bad expression.\n");
		return 0;
	}
	else if (p == q) {
		// single number
		if(tokens[p].type == DEC) {
			if(strlen(tokens[p].str) > strlen("4294967296\0") || (strlen(tokens[p].str) == strlen("4294967296\0") && strcmp(tokens[p].str, "4294967296\0") >= 0)) {
				printf("Error! Too large decimal number.\n");
				*success = false;
				return 0;
			}
			// printf("number = %s, nemu outcome = %u\n", tokens[p].str, (uint32_t)atoi(tokens[p].str));
			return (uint32_t)atoi(tokens[p].str);
		}
		else if (tokens[p].type == HEX)
			return hex2uint(tokens[p].str, success);
		else if (tokens[p].type == REG)
			return reg2uint(tokens[p].str, success);
		printf("Error! Bad expression.\n");
		*success = false;
		return 0;
	}
	else if (tokens[p].type == '(' && check_parentheses(p, q)) {
		printf("p = %d, q = %d | p+1 = %d, q-1 = %d\n");
		return eval(p+1, q-1, success);
	}
	else {
		uint32_t val1, val2;
		int op = dominant_op(p, q);
		if (op == p) {
			val2 = eval(op + 1, q, success);
			if (tokens[op].type == '!') {
				if(val2 > 0)
					return 0;
				return 1;
			}
			else if(tokens[op].type == DEREF)
				return vaddr_read(val2, SREG_DS, 4);
		}
		else if (op > p) {
			val1 = eval(p, op - 1, success);
			val2 = eval(op + 1, q, success);
			uint64_t res;
			switch(tokens[op].type) {
				case '+':
					res = (uint64_t)val1 + (uint64_t)val2;
					if (res >= 4294967296) {
						printf("Error! Addition overflow.\n");
						*success = false;
						return 0;
					}
					return val1 + val2;
				case '-':
					if(val1 < val2) {
						printf("Error! Negative result for unsigned int type.\n");
						*success = false;
						return 0;
					}
					return val1 - val2;
				case '*':
					res = (uint64_t)val1 * (uint64_t)val2;
					if(res > 4294967296) {
						printf("Error! Multiplication overflow.\n");
						*success = false;
						return 0;
					}
					return val1 * val2;
				case '/':
					if (val2 == 0) {
						printf("Error! Division by zero.\n");
						*success = false;
						return 0;
					}
					return val1 / val2;
				case '%':
					if (val2 != 0)
						return val1 % val2;
					else {
						printf("Error! Mod zero error.\n");
						*success = false;
						return 0;
					}
				case '>':
					return val1 > val2;
				case '<':
					return val1 < val2;
				case EQ:
					return val1 == val2;
				case NEQ:
					return val1 != val2;
				case LEQ:
					return val1 <= val2;
				case GEQ:
					return val1 >= val2;
				case LSHIFT:
					res = (uint64_t)val1 << (uint64_t)val2;
					if(res >= 4294967296) {
						printf("Error! Left-shift overflow.\n");
						*success = false;
						return 0;
					}
					return val1 << val2;
				case RSHIFT:
					return val1 >> val2;
				case AND:
					if(val1 > 0 && val2 > 0)
						return 1;
					return 0;
				case OR:
					if(val1 > 0 || val2 > 0)
						return 1;
					return 0;
				default:
					*success = false;
					return 0;
			}
		}
		else {
			*success = false;
			return 0;
		}
	}
	return 0;
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	// printf("\nPlease implement expr at expr.c\n");
	// assert(0);
	for (int i = 0; i < nr_token; ++ i) {
		if(tokens[i].type=='*' && (i==0 || (tokens[i-1].type!=DEC && tokens[i-1].type!=HEX && tokens[i-1].type!=REG && tokens[i-1].type!='(' && tokens[i-1].type!=')')))
			tokens[i].type = DEREF;
	}
	return eval(0, nr_token-1, success);
}

