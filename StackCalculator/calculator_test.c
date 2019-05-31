/*******************************************************************************
*Description: Stack Calculator Test 					       *
*Created BY Bareket Sar Shalom                                                 *
*Date: 17.12.18                                                                *
*******************************************************************************/
#include <stdio.h>/*******************printf, sizeof***************************/
#include <string.h>/*******************printf, sizeof**************************/
#include <strings.h>/*******************printf, sizeof*************************/
#include "../include/test.h"
#include "../include/stack.h"
#include "calculator.h"
#include "../include/enum_rd58.h"
#include "../include/function_pointers.h"

typedef struct stack_calculator_s stack_calculator_t;
struct stack_calculator_s
{
	stack_t *stack_operand;
	stack_t *stack_operator;
	int state;
};
#define EMPTY 0
/******************************************************************************/
TestResult CalculatorCreateTest();

/******************************************************************************/
int main()
{
	RUN_TEST(CalculatorCreateTest);
	return (TEST_PASS);
}
/******************************************************************************/
TestResult CalculatorCreateTest()
{
	status stat = SUCCESS;
	char *expression = "5+2";
	char *expression2 = "2+8+3-5";
	printf("%f", CalculateExpression(expression2, &stat));

	return (TEST_PASS);
}
