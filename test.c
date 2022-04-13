#include <stdio.h>

#define COLLECTIONS_CANARY_PROTECT
#define COLLECTIONS_HASH_PROTECT
// #define COLLECTIONS_DUMPING

const char* const RESET ="\033[0m";
const char* const RED ="\033[1;31m";
const char* const GREEN = "\033[1;32m";
const char* const YELLOW = "\033[1;33m";
const char* const BLUE = "\033[1;34m";

#define TEST(FUNC) \
    if (!FUNC()) printf("%s[FAILED] %s%s\n", RED, #FUNC, RESET); \
    else printf("%s[PASSED] %s%s\n", GREEN, #FUNC, RESET);

typedef struct {
    char* str;
    int i;
    double d;
} SampleStruct;

#define STACK_TYPE SampleStruct
#include "lib/stack.h"
#undef STACK_TYPE

#define STACK_TYPE int
#include "lib/stack.h"
#undef STACK_TYPE

typedef struct _TortureChamber
{
    char executionerLeft[1];
    Stack_SampleStruct victim;
    char executionerRight[1];
} TortureChamber;

void PrintSampleStruct(SampleStruct d) { printf("%s\n\t\t\t%d\n\t\t\t%lg", d.str, d.i, d.d); }
void PrintInt(int i) { printf("%d", i); }

int TortureChamberTest()
{
    TortureChamber chamber = {0};
    StackCtor(SampleStruct, &chamber.victim, 5, PrintSampleStruct);

    SampleStruct test = {"test", 1, 1.58};
    StackPush(SampleStruct, &chamber.victim, test);

    chamber.executionerLeft[9] = 0;
    chamber.executionerRight[-9] = 48;

    CErrorSet error = StackVerify(SampleStruct, &chamber.victim);

    return (error & (1 << C_STRUCT_LEFT_CANARY_FAULT)) &&
           (error & (1 << C_STRUCT_RIGHT_CANARY_FAULT));
}

int ArcherTest()
{
    Stack_SampleStruct aim = {0};
    StackCtor(SampleStruct, &aim, 5, PrintSampleStruct);

    SampleStruct test = {"test", 1, 1.58};
    StackPush(SampleStruct, &aim, test);

    SampleStruct archer = {"I'm strong archer", 666, 666.666};
    aim.data[0] = archer;
    CErrorSet error = StackVerify(SampleStruct, &aim);
    
    return (error & (1 << C_DATA_HASH_FAULT));
}

int DataAssassinationAttemptTest()
{
    Stack_int prey = {0};
    StackCtor(int, &prey, 5, PrintInt);

    prey.data[-1] = 0xDEAD;
    prey.data[prey.capacity + 1] = 0xDEAD;

    CErrorSet error = StackVerify(int, &prey);

    return (error & (1 << C_DATA_LEFT_CANARY_FAULT)) &&
           (error & (1 << C_DATA_RIGHT_CANARY_FAULT));
}

int TwoConstructsTest()
{
    Stack_int stack = {0};
    StackCtor(int, &stack, 5, PrintInt);
    CErrorSet error = StackCtor(int, &stack, 5, PrintInt);

    return (error & (1 << C_ALREADY_CONSTRUCTED));
}

int TwoDestructorsTest()
{
    Stack_int stack = {0};
    StackCtor(int, &stack, 5, PrintInt);
    StackDtor(int, &stack);
    CErrorSet error = StackDtor(int, &stack);

    return (error & (1 << C_ALREADY_DELETED));
}

int NullPointerReferencesTest()
{
    Stack_int stack = {0};
    StackCtor(int, &stack, 5, PrintInt);
    stack.data = NULL;

    CErrorSet error = StackPush(int, &stack, 5);
    error |= StackPush(int, NULL, 5);

    return (error & (1 << C_NULL_REFERENCE)) &&
           (error & (1 << C_NULL_REFERENCE_DATA));
}

int OutOfMemoryTest()
{
    Stack_int stack = {0};
    CErrorSet error = StackCtor(int, &stack, (size_t)(-1) / 8, PrintInt);

    return (error & (1 << C_OUT_OF_MEMORY));
}

int StackValuesSpoilTest()
{
    Stack_int stack = {0};
    StackCtor(int, &stack, 10, PrintInt);
    stack.size = stack.capacity + 10;
    CErrorSet error = StackVerify(int, &stack);

    return (error & (1 << C_BAD_VALUES));
}

int StackFunctionalityTest()
{
    Stack_int stack = {0};
    StackCtor(int, &stack, 1, PrintInt);
    CErrorSet error = C_OK;

    error |= StackPush(int, &stack, 4);
    error |= StackPush(int, &stack, 5);
    error |= StackPush(int, &stack, 6);

    int temp = 0;

    error |= StackPop(int, &stack, &temp);
    if (temp != 6) {
        return 0;
    }

    error |= StackDtor(int, &stack);

    return (error == C_OK);
}

int SizeTest()
{
    Stack_int stack = {0};
    StackCtor(int, &stack, 1, PrintInt);

    StackPush(int, &stack, 4);
    StackPush(int, &stack, 5);
    StackPush(int, &stack, 6);

    size_t size;
    StackSize(int, &stack, &size);

    return (size == 3);
}

int main()
{
    TEST(TortureChamberTest)
    TEST(ArcherTest)
    TEST(DataAssassinationAttemptTest)
    TEST(TwoConstructsTest)
    TEST(TwoDestructorsTest)
    TEST(NullPointerReferencesTest)
    TEST(OutOfMemoryTest)
    TEST(StackValuesSpoilTest)
    TEST(StackFunctionalityTest)
    TEST(SizeTest)

    return 0;
}
