/*******************************************************************************
* NAME: Stack Calculator                                                       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  16.12.18                                                      *
*******************************************************************************/

#ifndef __STACK_CALCULATOR_H__
#define __STACK_CALCULATOR_H__

typedef struct stack_calculator_s stack_calculator_t;

stack_calculator_t *StackCalculatorCreate(size_t number_of_inputs);
/*******************************************************************************
* Creates stack calculator                                                     *
*                                                                              *
* Parameters:                                                                  *
*  number_of_inputs - max number of inputs to be added to stack                *
*                                                                              *
* Returns:                                                                     *
*  pointer to stack calculator                                                 *
*  NULL if memory allocation failed                                            *
*******************************************************************************/

void StackCalculatorDestroy(stack_calculator_t *sc);
/*******************************************************************************
* Deletes stack calculator                                                     *
*                                                                              *
* Parameters:                                                                  *
*  sc - pointer to stack calculator, not NULL.                                 *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/

void StackCalculatorAddOperator(stack_calculator_t *sc, char operator);
/*******************************************************************************
* Adds operator to stack calculator                                            *
*                                                                              *
* Parameters:                                                                  *
*  sc - pointer to stack calculator, not NULL.                                 *
*  operator - mathematical operator (char)                                      *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/

void StackCalculatorAddOperand(stack_calculator_t *sc, double operand);
/*******************************************************************************
* Adds operator to stack calculator                                            *
*                                                                              *
* Parameters:                                                                  *
*  sc - pointer to stack calculator, not NULL.                                 *
*  operand - number (double)                                                   *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/

double StackCalculatorReturnResult();
/*******************************************************************************
* Returns result of stack calculator                                           *
*                                                                              *
* Parameters:                                                                  *
*  none                                                                        *
*                                                                              *
* Returns:                                                                     *
*  result of stack calculator                                                  *
*******************************************************************************/

#endif
