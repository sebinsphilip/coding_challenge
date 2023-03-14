/*

gcc gate.c -lm
./a.out


//SAMPLE INPUT and OUTPUT

sebin@sebin:~/Desktop/thesis/infineon/code$ gcc gate.c -lm
sebin@sebin:~/Desktop/thesis/infineon/code$ ./a.out 
Insert boolean function(s):
3
4 abcedf
4 dcebfa
3 ABabaAcebc
1
15
2, 0, 1, 2


sebin@sebin:~/Desktop/thesis/infineon/code$ ./a.out 
Insert boolean function(s):
3
2 abBA
3 abAced
2 abABdcCD
1, 3
4
2, 2


*/


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define DEBUG 0 // Keep DEBUG as 0 to improve speed!
#define NUMBER_OF_LETTER_IN_ENGLISH 26

typedef enum
{
    or_op,
    and_op
} operation;

typedef struct
{
    int *input1;
    int *input2;
    operation op;
    uint input1_negation_bit : 1;
    uint input2_negation_bit : 1;
    uint output_used_bit : 1;
    int number_of_input_combinations_yielding_true;
} bool_gate;

bool_gate *output_array_metadata[10];
int input_output_array[NUMBER_OF_LETTER_IN_ENGLISH];

void assign_array_index(char *temp, int i, int k, int input_order, int n_input)
{
    if (NULL != temp)
    {
        output_array_metadata[i][k].output_used_bit = 0;
        output_array_metadata[i][k].number_of_input_combinations_yielding_true = 0;

        if (*temp < 'a')
        {
            //input is negation, i.e [A-Z]
            if (1 == input_order)
            {
                output_array_metadata[i][k].input1_negation_bit = 1;
                output_array_metadata[i][k].input1 = &input_output_array[(uint)(*temp - 65)];
            }
            else
            {
                output_array_metadata[i][k].input2_negation_bit = 1;
                output_array_metadata[i][k].input2 = &input_output_array[(uint)(*temp - 65)];
            }
            if (n_input <= (uint)(*temp - 65))
            {
                //a potential output (previous) is being used as input to another gate, mark it!
                output_array_metadata[i][(uint)(*temp - 65) - n_input].output_used_bit = 1;
            }
#if DEBUG
            printf("[%c:%d X_index:%u]", *temp, input_order, (uint)(*temp - 65));
#endif
            
        }
        else
        {
            //input is normal input, i.e [a-z]
            if (1 == input_order)
            {
                output_array_metadata[i][k].input1_negation_bit = 0;
                output_array_metadata[i][k].input1 = &input_output_array[(uint)(*temp - 97)];
            }
            else
            {
                output_array_metadata[i][k].input2_negation_bit = 0;
                output_array_metadata[i][k].input2 = &input_output_array[(uint)(*temp - 97)];

            }
            if (n_input <= (uint)(*temp - 97))
            {
                //a potential output (previous) is being used as input to another gate, mark it!
                output_array_metadata[i][(uint)(*temp - 97) - n_input].output_used_bit = 1;
            }
#if DEBUG
            printf("[%c:%d x_index:%u]", *temp, input_order, (uint)(*temp - 97));
#endif
            
        }
    }
}

void calculate_boolean_true (int n_input, int n_output, bool_gate* function_metadata)
{
    int i,j, a,b,first_flag;
    int limit = pow(2, n_input);

    if (NULL != function_metadata)
    {
        for (i = 0; i < limit; i++)
        {
            for (j = 0; j < n_input; j++)
            {
                //assign bit pattern to input fields
                input_output_array[j] = (i >> (n_input - j - 1)) & 1;
#if DEBUG
                printf ("%d", input_output_array[j]);
#endif
            }

            for (j = 0; j < n_output; j++)
            {
                a = *(function_metadata[j].input1);
                b = *(function_metadata[j].input2);
                if (1 == function_metadata[j].input1_negation_bit)
                {
                    a = !a;
                }
                if (1 == function_metadata[j].input2_negation_bit)
                {
                    b = !b;
                }

                if (and_op == function_metadata[j].op)
                {
                    //do AND operation
                    input_output_array[n_input + j] = a & b;
                }
                else
                {
                    //do OR operation
                    input_output_array[n_input + j] = a | b;
                }
                if (1 == input_output_array[n_input + j])
                {
                    function_metadata[j].number_of_input_combinations_yielding_true += 1;
                }
#if DEBUG
                printf ("%d", input_output_array[n_input + j]);
#endif

            }
#if DEBUG
            printf ("\n");
#endif
        }
        first_flag = 0;
        for (j = 0; j < n_output; j++)
        {
            
            if (0 == function_metadata[j].output_used_bit)
            {
                if (first_flag)
                {
                    printf (",");
                }
                printf ("%d", function_metadata[j].number_of_input_combinations_yielding_true);
                first_flag = 1;
            }
            
        }

    }

}

int main()
{
    int number_of_functions, i, j, k, boolean_inputs[10];
    char gate_string[10][25], *temp = NULL;

    printf("Insert boolean function(s):\n");
    scanf("%d", &number_of_functions);

    for (i = 0; i < number_of_functions; i++)
    {
        scanf("%d %[^\n]", &boolean_inputs[i], gate_string[i]);

        output_array_metadata[i] = (bool_gate *)malloc(sizeof(bool_gate) * (strlen(gate_string[i]) / 2));

        j = k = 0;
        temp = gate_string[i];
        while (NULL != temp)
        {
            if (temp[j] != '\0')
            {
#if DEBUG
                printf(" %c|%c ", *temp, *(temp + 1));
#endif
                if (isalpha(*temp) && isalpha(*(temp + 1)))
                {
                    if (tolower ((unsigned char)temp[0]) <= tolower ((unsigned char)temp[1]))
                    {
                        //AND
                        output_array_metadata[i][k].op = and_op;
                        assign_array_index(temp, i, k, 1, boolean_inputs[i]);
                        assign_array_index(temp+1, i, k, 2, boolean_inputs[i]);
                    }
                    else
                    {
                        //OR
                        output_array_metadata[i][k].op = or_op;
                        assign_array_index(temp+1, i, k, 1, boolean_inputs[i]);
                        assign_array_index(temp, i, k, 2, boolean_inputs[i]);
                    }
                }
                else
                {
                    printf ("Invalid input detected!\n");
                    return -1;
                }
#if DEBUG
                printf ("\n");
#endif
                temp += 2;
                k++;
            }
            else
            {
                break;
            }
        }
#if DEBUG
        printf("\n");
#endif
    }


    for (i = 0; i < number_of_functions; i++)
    {
#if DEBUG
        printf ("\n=============== %d %s =================\n", boolean_inputs[i], gate_string[i]);
#endif
        calculate_boolean_true (boolean_inputs[i], (strlen(gate_string[i]) / 2), output_array_metadata[i]);
        printf ("\n");
        free (output_array_metadata[i]);
    }


    return 0;
}