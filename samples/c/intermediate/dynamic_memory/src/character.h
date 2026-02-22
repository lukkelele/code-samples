#pragma once

#include <stdint.h>

typedef enum {
	CHARACTER_TYPE_PLAYER,
	CHARACTER_TYPE_NPC,
} character_type_t;

typedef struct {
	character_type_t type;
	char* name;
	uint8_t age;
} character_t;

/**
 * @brief Create a new character instance.
 */
character_t* character_create(character_type_t type, const char* name, uint8_t age);

/**
 * @brief Destroy a character instance.
 */
void character_destroy(character_t* character);

/**
 * @brief Print character info.
 */
void character_print(character_t* character);

/**
 * @brief Get character type as a string.
 */
const char* character_type_to_str(character_type_t type);
