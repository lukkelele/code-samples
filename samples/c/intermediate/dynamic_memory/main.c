#include <assert.h>
#include <stdio.h>

#include "character.h"

int main(int argc, char** argv)
{
	printf("\n==============================\n");
	printf("  sample: dynamic_memory\n");
	printf("==============================\n\n");

	/************************************************
	 * Character 1
	 ************************************************/
	printf("Creating character 1\n");
	character_t* character1 = character_create(CHARACTER_TYPE_PLAYER, "ludvig", 32);
	assert(character1 && "character1 is NULL");
	character_print(character1);

	character_set_name(character1, "ullis");
	character_print(character1);

	printf("\n"); /* Just making the output a bit more readable. */

	/************************************************
	 * Character 2
	 *
	 * This variation allocates memory to the
	 * instance by taking a pointer to the pointer.
	 ************************************************/
	printf("Creating character 2\n");
	character_t* character2 = NULL;
	character_alloc(&character2);
	assert(character2 && "character2 is NULL");
	character2->age = 26;
	character_print(character2);

	character_set_name(character2, "Lulle");
	character2->name[0] = 'b';
	character_print(character2);

	/**
	 * In C, string literals have static storage duration and are typically stored
	 * in read-only memory (.rodata).
	 * When assigning a string literal to a `char*`, the pointer refers to memory
	 * that must NOT be modified and must NOT be freed.
	 * Attempting to modify the contents (e.g name[0] = 'L') results in
	 * undefined behavior and will usually cause a segmentation fault.
	 *
	 * The correct approach is:
	 *  - Use `const char*` if the string is immutable and not owned.
	 *  - Allocate and copy (malloc/strdup) if the string must be modified or freed.
	 *
	 * Change the 0 to 1 in the '#if 0' line below to run that code.
	 */
#if 0
	character2->name = "lukas";
	character2->name[0] = 'L'; /** DANGEROUS !!! */
	character_print(character2);
#endif

	/* Clean up resources by freeing memory. */
	character_destroy(character1);

	/**
	 * This variation of freeing the character takes a pointer to the character pointer.
	 * By doing this, we can set the 'character2' variable to NULL inside the function
	 * which is helpful to prevent dangling pointers.
	 */
	character_destroy2(&character2);
	assert((character2 == NULL) && "character2 is expected to be NULL");

	return 0;
}
