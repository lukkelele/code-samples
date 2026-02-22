#include "character.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARACTER_CREATE_USE_MALLOC 0

character_t* character_create(character_type_t type, const char* name, uint8_t age)
{
	character_t* character = malloc(sizeof(character_t));
	if (character == NULL) {
		printf("Failed to alloc %lu bytes to create character instance\n", sizeof(character_t));
		return NULL;
	}

	character->name = malloc(strlen(name) + 1);
	if (character->name == NULL) {
		printf("Failed to alloc %lu bytes for character name\n", strlen(name) + 1);
		free(character);
		return NULL;
	}

	character->type = type;
	strcpy(character->name, name);
	character->age = age;
	return character;
}

int character_alloc(character_t** character)
{
	if (character == NULL) {
		return EINVAL;
	}
	if (*character != NULL) {
		return EINVAL;
	}

#if CHARACTER_CREATE_USE_MALLOC
	character_t* instance = malloc(sizeof(*instance));
#else
	/**
	 * calloc allocates memory for an array of objects (thus 1 in this case)
	 * and initializes all bytes in the allocated storage to zero.
	 * @link: https://cppreference.com/w/c/memory/calloc.html
	 */
	character_t* instance = calloc(1, sizeof(*instance));
#endif
	if (instance == NULL) {
		return ENOMEM;
	}

	*character = instance;
	return 0;
}

void character_destroy(character_t* character)
{
	if (character == NULL) {
		return;
	}

	printf("[character_destroy] Destroying character: %s\n",
		   character->name ? character->name : "NULL");
	free(character->name);
	free(character);
}

void character_destroy2(character_t** character)
{
	if ((character == NULL) || (*character == NULL)) {
		return;
	}

	printf("[character_destroy2] Destroying character: %s\n",
		   (*character)->name ? (*character)->name : "NO_NAME");
	free(*character);
	*character = NULL;
}

int character_set_name(character_t* character, const char* name)
{
	if ((character == NULL) || (name == NULL)) {
		return EINVAL;
	}

	/**
	 * One extra byte is allocated because of storage for '\0'.
	 * @link: https://en.wikipedia.org/wiki/Null_character
	 * @link: https://en.wikipedia.org/wiki/Null-terminated_string
	 */
	const size_t len = strlen(name) + 1;
	char* new_name = malloc(len);
	if (new_name == NULL) {
		return ENOMEM;
	}

	/* No need to use strcpy as we already have the length. */
	memcpy(new_name, name, len);

	/* Free existing memory, if any has been allocated before. */
	if (character->name) {
		free(character->name);
	}
	character->name = new_name;

	return 0;
}

/**
 * The following is another version of the character_set_name function.
 * Its implementation is worse in various ways, see the comments for why that is.
 */
#if 0
int character_set_name_BAD_IMPLEMENTATION(character_t* character, const char* name)
{
	if ((character == NULL) || (name == NULL)) {
		return EINVAL;
	}

	/**
	 * BAD: Memory for name is freed BEFORE the new name has been allocated.
	 * If the allocation fails, the old name is lost. This is bad design.
	 */
	if (character->name) {
		free(character->name);
	}

	/* BAD: Potential NULL dereference if malloc fails (character->name). */
	character->name = malloc(strlen(name) + 1);
	strcpy(character->name, name);
	return 0;
}
#endif

void character_print(character_t* character)
{
	if (character == NULL) {
		return;
	}

	printf("[Character] Type: %s | Name: %s | Age: %u\n", character_type_to_str(character->type),
		   character->name ? character->name : "Unknown", character->age);
}

const char* character_type_to_str(const character_type_t type)
{
	switch (type) {
		case CHARACTER_TYPE_PLAYER: return "PLAYER";
		case CHARACTER_TYPE_NPC: return "NPC";
		default: return "UNKNOWN";
	}
}
