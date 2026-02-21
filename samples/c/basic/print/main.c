#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * In C you mostly use printf to print formatted strings to stdout.
 * A newline character '\n' is added to not continue printing on the same line.
 *
 * Format specifiers:
 *   %d    Signed decimal integer (int).
 *   %i    Signed decimal integer (int).
 *   %u    Unsigned decimal integer (unsigned int).
 *   %f    Floating-point number (double).
 *   %e    Scientific notation (double).
 *   %g    Shortest representation of %f or %e (double).
 *   %c    Single character (int).
 *   %s    Null-terminated string (char *).
 *   %p    Pointer (void *).
 *   %x    Unsigned hexadecimal (lowercase).
 *   %X    Unsigned hexadecimal (uppercase).
 *   %%    Prints a literal '%' character.
 */
int main(int argc, char** argv)
{
	printf("\n==============================\n");
	printf("\tsample: print\n");
	printf("==============================\n\n");

	int integer = 50;
	printf("integer: %d\n", integer);

	double double_a = 40;
	printf("double_a: %f\n", double_a);
	printf("\n"); /* Just to make the output more readable :) */


	/* %.Nf where N is the number of digits after the decimal point. */
	printf("variations of pi using different control decimals:\n");
	printf(" %f\n", 3.1415926535);
	printf(" %.2f\n", 3.1415926535);
	printf(" %.8f\n", 3.1415926535);
	printf("\n"); /* Just to make the output more readable :) */


	/* Strings. */
	const char* name = "lukas";
	printf("name: %s\n", name);

	char name_buf[32];
	sprintf(name_buf, "name: %s", "lukas");
	printf("name_buf: %s\n", name_buf);

	memset(name_buf, 0, sizeof(name_buf)); /* Clear the buffer before using it again. */
	snprintf(name_buf, sizeof(name_buf), "name: %s", "ludvig");
	printf("name_buf: %s\n", name_buf);
	printf("\n"); /* Just to make the output more readable :) */


	/* Pointers. */
	void* void_ptr = malloc(10);
	printf("void_ptr: %p (memory address)\n", void_ptr);
	free(void_ptr);

	return 0;
}
