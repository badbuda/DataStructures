/*******************************************************************************
*Description: Stack Calculator Implementation using final state machine					                                       *
*Created BY Bareket Sar Shalom                                                 *
*Date: 17.12.18                                                                *
*******************************************************************************/
#include <stdio.h>/*******************printf, sizeof***************************/
#include <string.h>/*******************printf, sizeof***************************/
#include <strings.h>/*******************printf, sizeof***************************/
#include <stdlib.h>/*******************printf, sizeof***************************/
#include "calculator.h"
#include "../include/stack.h"
#include "../include/enum_rd58.h"
#include "../include/function_pointers.h"

#define NUMBER_OF_STATES 4
#define ASCII 256
/******************************************************************************/
bool CmpFunction (char a, char b);
/******************************************************************************/
typedef struct stack_calculator_s stack_calculator_t;

enum operator {MIN_PREC, OPEN, CLOSE, ADD = 43, SUB = 45, DUP = 42, DIV = 42, NUM, INVALID_INPUT};
typedef enum {GET_NUMBER, GET_OPERATOR, ERROR, END}states;
typedef void (*next_state_t)(void *num, stack_calculator_t *calculator, status *status_function, char **expression);
typedef double (*lut_precedence_t)(double first, double sec);
typedef int (*lut_precedence2_t)();
/******************************************************************************/
lut_precedence_t lut_precedence[ASCII] = {0};
lut_precedence2_t lut_precedence2[ASCII] = {0};

next_state_t next_state[NUMBER_OF_STATES][ASCII] = {{0},{0}};

struct stack_calculator_s
{
	stack_t *stack_operand;
	stack_t *stack_operator;
	int state;
};
/******************************************************************************/
void Evaluate (void *operator1, stack_calculator_t *calculator)
{
	char operator = *(char*)operator1;
	double pop_first = 0, pop_sec = 0,result = 0;
	char stack_operator = *(char*)StackPeek(calculator->stack_operator);
	if (CmpFunction(stack_operator , operator))
	{
		StackPush(calculator->stack_operator, &operator);
	}
	else
	{
		pop_first = *(double*)StackPeek(calculator->stack_operand);
		StackPop(calculator->stack_operand);
		pop_sec = *(double*)StackPeek(calculator->stack_operand);
		StackPop(calculator->stack_operand);
		result = lut_precedence[*(char*)StackPeek(calculator->stack_operator)]
																											(pop_sec, pop_first);
		printf("%lf\n", result);

		StackPush(calculator->stack_operand, &result);
	}
}
/******************************************************************************/
void InsertNumber (void *num, stack_calculator_t *calculator,
																			status *status_function, char **expression)
{
	double i = 0;
	int j;

	(void)status_function;
	(void)expression;
	j = *(double*)num;
	StackPush(calculator->stack_operand, num);

	i = *(double*)StackPeek(calculator->stack_operand);

}
/******************************************************************************/
void FSMGetNumber (void *num, stack_calculator_t *calculator,
																			status *status_function, char **expression)
{
	double i = 0, num1 = 0;
	(void)status_function;

	num1 = strtod(*expression, expression);
	InsertNumber(&num1, calculator, status_function, expression);
	i = *(double*)StackPeek(calculator->stack_operand);
	calculator->state = GET_OPERATOR;
}

/******************************************************************************/
void FSMGetOperator (void *num, stack_calculator_t *calculator,
	 																		status *status_function, char **expression)
{
	(void)num;
	(void)calculator;
	(void)status_function;

	Evaluate(*expression, calculator);
	++(*expression);

	calculator->state = GET_NUMBER;
}
/******************************************************************************/
double InsertAdd (double first, double sec)
{
	return (first + sec);
}
/******************************************************************************/
double InsertDiv (double first, double sec)
{
	return (first / sec);
}
/******************************************************************************/
double InsertSub (double first, double sec)
{
	return (first - sec);
}
/******************************************************************************/
double InsertDup (double first, double sec)
{
	return (first * sec);
}
/******************************************************************************/
void ErrorFunction (void *num, stack_calculator_t *calculator, status *status_function, char **expression)
{
	(void)num;
	(void)calculator;
	(void)expression;

	*status_function = ACTION_FAIL;
}

void EndFunction (void *num, stack_calculator_t *calculator, status *status_function, char **expression)
{
	char operator = '$';
	(void)num;
	(void)calculator;
	(void)expression;
	Evaluate (&operator, calculator);
	*status_function = STOP_TASK;
}
/******************************************************************************/
stack_calculator_t *CalculatorCreate(char *expression)
{
	stack_calculator_t* stack_calculator;
	char begin = '$';

	stack_calculator = (stack_calculator_t *)malloc(sizeof(stack_calculator_t));

	if (NULL == stack_calculator)
	{
		return (NULL);
	}

	stack_calculator->stack_operand = StackCreate(strlen(expression), sizeof(double));
	stack_calculator->stack_operator = StackCreate(strlen(expression), sizeof(char));

	StackPush(stack_calculator->stack_operator, &begin);

	stack_calculator->state = GET_NUMBER;

	return (stack_calculator);
}
/******************************************************************************/
void LutStates()
{
	int i, j;

	for (i = 0; i < NUMBER_OF_STATES; ++i)
	{
		for (j = 0; j < 256; ++j)
		{
			next_state[NUMBER_OF_STATES][i] = ErrorFunction;
		}
	}

	for (i = '0'; i <= '9'; ++i)
	{
		next_state[GET_NUMBER][i] = FSMGetNumber;
	}

	next_state[GET_NUMBER][DUP] = ErrorFunction;
	next_state[GET_OPERATOR][DUP] = FSMGetOperator;

	next_state[GET_NUMBER][DIV] = ErrorFunction;
	next_state[GET_OPERATOR][DIV] = FSMGetOperator;

	next_state[GET_NUMBER][ADD] = ErrorFunction;
	next_state[GET_OPERATOR][ADD] = FSMGetOperator;

	next_state[GET_NUMBER][SUB] = ErrorFunction;
	next_state[GET_OPERATOR][SUB] = FSMGetOperator;

	next_state[GET_NUMBER][INVALID_INPUT] = ErrorFunction;

	next_state[GET_OPERATOR]['\0'] = EndFunction;
}
int AddPrecedence()
{
	return (42);
}
int SubPrecedence()
{
	return (42);
}
int DupPrecedence()
{
	return (45);
}
int DivPrecedence()
{
	return (45);
}
/******************************************************************************/
void Precedence()
{
	lut_precedence['+'] = InsertAdd;
	lut_precedence['-'] = InsertSub;
	lut_precedence['/'] = InsertDiv;
	lut_precedence['*'] = InsertDup;
}

void Precedence2()
{
	lut_precedence2['+'] = AddPrecedence;
	lut_precedence2['-'] = SubPrecedence;
	lut_precedence2['/'] = DivPrecedence;
	lut_precedence2['*'] = DupPrecedence;
}
/******************************************************************************/
bool CmpFunction (char a, char b)
{
	if ((int)lut_precedence2[a] < (int)lut_precedence2[b])
	{
		return (TRUE);
	}

	return (FALSE);
}
/******************************************************************************/
double CalculateExpression(char *expression, status *stat)
{
	stack_calculator_t *calculator = CalculatorCreate(expression);
	LutStates();
	Precedence();
	Precedence2();
	calculator->state = GET_NUMBER;
	while (*stat != ACTION_FAIL && *stat != STOP_TASK)
	{
		int num = 0;
		int i ,j;
		j = (int)*expression;
		i = calculator->state;
		next_state[calculator->state][(int)*expression](&num, calculator,
																									stat, &expression);
	}

	return (*(double*)StackPeek(calculator->stack_operand));
}
/******************************************************************************/
