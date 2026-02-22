#include <assert.h>
#include <stdio.h>

#include "character.h"

int main(int argc, char** argv)
{
	printf("\n==============================\n");
	printf("  sample: dynamic_memory\n");
	printf("==============================\n\n");

	printf("Creating player 1\n");
	character_t* player1 = character_create(CHARACTER_TYPE_PLAYER, "ludvig", 32);
	assert(player1 && "player1 is NULL");

	character_print(player1);

	/* Clean up resources by freeing memory. */
	character_destroy(player1);

	return 0;
}
