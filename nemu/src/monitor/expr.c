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

#define int_max 2147483647
#define int_max_str "2147483647\0"
#define int_min -2147483648
#define int_min_str "2147483648\0"

enum {
	NOTYPE = 256, 
	/* TODO: Add more token types */
	EQ = 255, AND = 254, OR = 253, 
	NEQ = 252, LEQ = 251, GEQ = 250, DEC = 249, 
	HEX = 248, REG = 247, LSHIFT = 246, RSHIFT = 245,
	DEREF = 244, NEG = 243, POS = 242, 
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
		if (L_cnt <= R_cnt)	// when the expr is not surrounded by a big pair of parentheses, L == R
			return false;
	}
	if (L_cnt - R_cnt == 1 && tokens[q].type == ')')
		return true;
	return false;
}


// transfer hex to uint32
uint64_t hex2uint(char* str, bool* success) {
	int str_len = strlen(str);
	uint64_t res = 0;
	*success = true;
	for(int i = 2; i < str_len; ++ i) {
		if (str[i] >= '0' && str[i] <= '9')
			res = res * 0x10 + str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			res = res * 0x10 + str[i] - 'a';
		else if (str[i] >= 'A' && str[i] <= 'F')
			res = res * 0x10 + str[i] - 'A';
		else {
			*success = false;
			return 0;
		}
		// overflow
		if (out_of_int_range(res)) {
			printf("Error! Exceeded hexadecimal number (int) range.\n");
			*success = false;
			return 0;
		}
	}
	return res;
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
		case DEREF: case '!': case NEG: case POS:
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

bool out_of_int_range(long long int res)
	return res>int_max || res<int_min;


long long int eval(int p, int q, bool *success) {
	*success = true;
#ifdef DEBUG
	for(int i=p; i<=q; ++i)
		printf("str[%d]=%s, type=%d\n", i, tokens[i].str, tokens[i].type);
	printf("\n");
#endif
	long long int res = 0;
	if (p > q) {
		*success = false;
		printf("Error! Bad expression.\n");
		return 0;
	}
	else if (p == q) {
		// single number
		if(tokens[p].type == DEC) {
			int dec_len = strlen(tokens[p].str);
			res = 0;
			for (int i=0; i<dec_len; ++i) {
				if(tokens[p].str[i]>='0' && tokens[p].str[i]<='9')
					res = res * 10 + tokens[p].str[i] - '0';
				if(out_of_int_range(res)) {
					printf("Error! Exceeded int number range.\n");
					*success = false;
					return 0;
				}
			}
			return res;
		}
		else if (tokens[p].type == HEX)
			return hex2uint(tokens[p].str, success); 
		else if (tokens[p].type == REG)
			return (uint64_t)reg2uint(tokens[p].str, success);
		printf("Error! Bad expression.\n");
		*success = false;
		return 0;
	}
	else if (tokens[p].type == '(' && check_parentheses(p, q)) {
		return eval(p+1, q-1, success);
	}
	else {
		long long int val1, val2;
		int op = dominant_op(p, q);
		if (op == p) {
			val2 = eval(op + 1, q, success);
			if(out_of_int_range(val2)) {
				printf("Error! Exceeded int range.\n");
				*success = false;
				return 0;
			}
			if (tokens[op].type == '!') {
				if(val2)
					return 0;
				return 1;
			}
			else if(tokens[op].type == DEREF)
				return vaddr_read(val2, SREG_DS, 4);
			else if(tokens[op].type == POS)
				return val2;
			else if(tokens[op].type == NEG) {
				res = -val2;
				if (out_of_int_range(res)) {
					printf("Error! Exceeded int range.\n");
					*success = false;
					return 0;
				}
				return res;	
			}
		}
		else if (op > p) {
			val1 = eval(p, op - 1, success);
			val2 = eval(op + 1, q, success);
			if(out_of_int_range(val1) || out_of_int_range(val2)) {
				printf("Error! Exceeded int range.\n");
				*success = false;
				return 0;
			}
			switch(tokens[op].type) {
				case '+':
					res = val1 + val2;
					if (out_of_int_range(res)) {
						printf("Error! Addition overflow.\n");
						*success = false;
						return 0;
					}
					return res;
				case '-':
					res = val1 - val2;
					if(out_of_int_range(res)) {
						printf("Error! Substraction overflow.\n");
						*success = false;
						return 0;
					}
					return res;
				case '*':
					res = val1 * val2;
					if(out_of_int_range(res)) {
						printf("Error! Multiplication overflow.\n");
						*success = false;
						return 0;
					}
					return res;
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
				case '>':  return val1 > val2;
				case '<':  return val1 < val2;
				case EQ:   return val1 == val2;
				case NEQ:  return val1 != val2;
				case LEQ:  return val1 <= val2;
				case GEQ:  return val1 >= val2;
				case LSHIFT:
					res = val1 << val2;
					if(out_of_int_range(res)) {
						printf("Error! Left-shift overflow.\n");
						*success = false;
						return 0;
					}
					return res;
				case RSHIFT:  return val1 >> val2;
				case AND:     
					      printf("val1=%lld, val2=%lld\n", (bool)val1, (bool)val2);
					      return val1 && val2;
				case OR:      return val1 || val2;
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


long long int expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	// printf("\nPlease implement expr at expr.c\n");
	// assert(0);
	for (int i = 0; i < nr_token; ++ i) {
		// deref
		if(tokens[i].type=='*' && (i==0 || (tokens[i-1].type!=DEC && tokens[i-1].type!=HEX && tokens[i-1].type!=REG && tokens[i-1].type!=')')))
			tokens[i].type = DEREF;
		// neg
		else if(tokens[i].type=='-' && (i==0 || (tokens[i-1].type!=DEC && tokens[i-1].type!=HEX && tokens[i-1].type!=REG && tokens[i-1].type!=')')))
			tokens[i].type = NEG;
		// pos
		else if(tokens[i].type=='+' && (i==1 || (tokens[i-1].type!=DEC && tokens[i-1].type!=HEX && tokens[i-1].type!=REG && tokens[i-1].type!=')')))
			tokens[i].type = POS;
	}
	return eval(0, nr_token-1, success);
}

