#include <stdio.h>

bool notGate(bool input)
{
    return !input;
}

bool andGate(bool input1, bool input2)
{
    return input1 && input2;
}

bool orGate(bool input1, bool input2)
{
    return input1 || input2;
}

int main()
{
    bool inputA = true;
    bool inputB = true;

    bool not_r1 = notGate(inputA);
    bool not_r2 = notGate(inputB);

    bool and_r1 = andGate(inputA, not_r2);
    bool and_r2 = andGate(not_r1, inputB);
    bool or_r = orGate(and_r1, and_r2);

    printf("output: %s\n", or_r ? "true" : "false");

    return 0;
}