#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

// Simple minimal test framework
static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_EQUAL(expected, actual) do { \
    if ((expected) != (actual)) { \
        printf("FAILED: %s:%d: Expected 0x%02lX, got 0x%02lX\n", __FILE__, __LINE__, (unsigned long)(expected), (unsigned long)(actual)); \
        tests_failed++; \
        return; \
    } \
} while(0)

#define RUN_TEST(test) do { \
    printf("Running %s... ", #test); \
    Reset_Registers(); \
    test(); \
    if (tests_failed == 0) printf("PASSED\n"); \
    else printf("FAILED\n"); \
    tests_run++; \
} while(0)

// Helper prototype from registers.c
void Reset_Registers(void);

#endif // TEST_FRAMEWORK_H