#include "character.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void character_destroy(character_t* character)
{
	if (character == NULL) {
		return;
	}

	printf("Destroying character: %s\n", character->name ? character->name : "NULL");
	free(character->name);
	free(character);
}

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
