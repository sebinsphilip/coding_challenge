/*

Author: Sebin Shaji Philip (sebin@kth.se)

compile:
    gcc traffic.c
    ./a.out 

SAMPLE INPUT and OUTPUT:
sebin@sebin:~/Desktop/thesis/infineon/code$ ./a.out 
Insert traffic pattern:R G Y R C R G Y R
ACCEPT


Notes: State diagram attached as 

* starting with 'X' code (is mentioned as valid input from STDIN) is inferred as valid from example 'X 8 S' - here my
  assumption is that '8' and 'S' are "Undefined codes". Although this behaviour can be easily changed by editing the
  statemachine array entries. This will result in "X X" valid .. i.e repeated 'X' is allowed as per this assumption!

* 'R G Y R P' is taken as valid sequence as per the given example, although there is a missing 'R' after 'P'.
   In normal cases (where end-of-string doesnt follow P or C) "R P R" and "R C R" is enforced.

* Turing-off the light after 'R', 'G' or 'Y' is considered valid. i.e "R X"/"G X"/"Y X" is valid 

*/


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEBUG 0 // Keep DEBUG as 0 to improve speed!
#define MAX_STRING_LENGTH 29
char input_string[100], *input_string_pointer;

typedef enum {
    X_State,
    R_State,
    C_State,
    P_State,
    G_State,
    Y_State,
    Terminate_State,
    Last_State
} eTrafficStates;

typedef enum {
    X_Parsed_Event,
    R_Parsed_Event,
    C_Parsed_Event,
    P_Parsed_Event,
    G_Parsed_Event,
    Y_Parsed_Event,
    UndefinedCode_Parsed_Event,
    EndOfString_Event,
    Last_Event
} eTrafficEvents;

typedef eTrafficStates (*pfEventHandler) (void);

typedef struct {
    eTrafficStates eTrafficStateMachine;
    eTrafficEvents eTrafficStateMachineEvents;
    pfEventHandler pfTrafficStateMachineEventHandler;
} sTrafficStateMachine;

/*  Event handler function definitions */
eTrafficStates X_ParsedHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    return X_State;
}

eTrafficStates R_ParsedHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    return R_State;
}

eTrafficStates C_ParsedHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    return C_State;
}

eTrafficStates P_ParsedHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    return P_State;
}

eTrafficStates G_ParsedHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    return G_State;
}

eTrafficStates Y_ParsedHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    return Y_State;
}

eTrafficStates ErrorTerminateHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    printf ("ERROR\n");
    return Terminate_State;
}

eTrafficStates AcceptTerminateHandler (void)
{
#if DEBUG
    printf ("+++ %s +++\n", __func__);
#endif
    printf ("ACCEPT\n");
    return Terminate_State;
}

sTrafficStateMachine asTrafficStateMachine [] =
{
    {X_State, R_Parsed_Event, R_ParsedHandler},
    {X_State, X_Parsed_Event, X_ParsedHandler},

    {R_State, G_Parsed_Event, G_ParsedHandler},
    {R_State, C_Parsed_Event, C_ParsedHandler},
    {R_State, P_Parsed_Event, P_ParsedHandler},
    {R_State, X_Parsed_Event, X_ParsedHandler},

    {G_State, Y_Parsed_Event, Y_ParsedHandler},
    {G_State, X_Parsed_Event, X_ParsedHandler},

    {Y_State, R_Parsed_Event, R_ParsedHandler},
    {Y_State, X_Parsed_Event, X_ParsedHandler},

    {C_State, R_Parsed_Event, R_ParsedHandler},

    {P_State, R_Parsed_Event, R_ParsedHandler},

    {Last_State , UndefinedCode_Parsed_Event, ErrorTerminateHandler},
    {Last_State , EndOfString_Event, AcceptTerminateHandler}

  
};
#define TRANSITION_COUNT (sizeof(asTrafficStateMachine)/sizeof(*asTrafficStateMachine))

eTrafficEvents read_next_letter ()
{
    eTrafficEvents event = UndefinedCode_Parsed_Event;
    if (input_string_pointer != NULL)
    {
        if ((*input_string_pointer == 'R') && ((*(input_string_pointer+1) == ' ') || (*(input_string_pointer+1) == '\0')))
        {
            event = R_Parsed_Event;
#if DEBUG
            printf ("R seen \n");
#endif

        }
        else if ((*input_string_pointer == 'G') && ((*(input_string_pointer+1) == ' ') || (*(input_string_pointer+1) == '\0')))
        {
            event = G_Parsed_Event;
#if DEBUG
            printf ("G seen \n");
#endif

        }
        else if ((*input_string_pointer == 'Y') && ((*(input_string_pointer+1) == ' ') || (*(input_string_pointer+1) == '\0')))
        {
            event = Y_Parsed_Event;
#if DEBUG
            printf ("Y seen \n");
#endif
            
        }
        else if ((*input_string_pointer == 'C') && ((*(input_string_pointer+1) == ' ') || (*(input_string_pointer+1) == '\0')))
        {
            event = C_Parsed_Event;
#if DEBUG
            printf ("C seen \n");
#endif
            
        }
        else if ((*input_string_pointer == 'P') && ((*(input_string_pointer+1) == ' ') || (*(input_string_pointer+1) == '\0')))
        {
            event = P_Parsed_Event;
#if DEBUG
            printf ("P seen \n");
#endif
            
        }
        else if ((*input_string_pointer == 'X') && ((*(input_string_pointer+1) == ' ') || (*(input_string_pointer+1) == '\0')))
        {
            event = X_Parsed_Event;
#if DEBUG
            printf ("X seen \n");
#endif
            
        }
        else
        {
            event = UndefinedCode_Parsed_Event;
#if DEBUG
            printf ("UndefinedCode_Parsed_Event seen \n");
#endif

        }

        if ((*(input_string_pointer+1) == '\0'))
        {
            input_string_pointer = NULL;
#if DEBUG
            printf ("End-of-String character detected\n");
#endif
        }
        else
        {
            input_string_pointer += 2;
        }
        

    }
    else
    {
        event = EndOfString_Event;
#if DEBUG
        printf ("EndOfString_Event seen \n");
#endif

    }

    return event;

}

int main () {
    uint32_t a,b,i, current_decimal, count=0;
    eTrafficStates eNextState = X_State;
    eTrafficEvents eNewEvent = UndefinedCode_Parsed_Event;

    printf ("Insert traffic pattern:");
    scanf ("%[^\n]", input_string);
    input_string_pointer = input_string;

#if DEBUG
    printf ("strlen():  %ld %s\n", strlen(input_string), input_string);
#endif

    if (strlen(input_string) > MAX_STRING_LENGTH)
    {
        //Too many codes
        (void) ErrorTerminateHandler ();
        return -1;
    }

    while (eNextState != Terminate_State)
    {
        eNewEvent = read_next_letter ();
#if DEBUG
        printf ("eNewEvent: %d, eNextState: %d\n", eNewEvent, eNextState);
        //printf ("TRANSITION_COUNT:%ld\n", TRANSITION_COUNT);
#endif
        for (i = 0; i < TRANSITION_COUNT; i++)
        {
            if ((eNextState == asTrafficStateMachine[i].eTrafficStateMachine) || (Last_State == asTrafficStateMachine[i].eTrafficStateMachine))
            {
#if DEBUG
                printf ("State: i: %d, [%d,%d]\n", i, eNextState, asTrafficStateMachine[i].eTrafficStateMachine);
#endif
                if (eNewEvent == asTrafficStateMachine[i].eTrafficStateMachineEvents)
                {
#if DEBUG
                    printf ("Event: [%d,%d]\n", eNewEvent, asTrafficStateMachine[i].eTrafficStateMachineEvents);
#endif
                    eNextState = (*asTrafficStateMachine[i].pfTrafficStateMachineEventHandler)();
                    break;

                }
            }

        }

        if (i >= TRANSITION_COUNT)
        {
            printf ("REJECT\n");
            eNextState = Terminate_State;

        }
        
    }

    return 0;
}