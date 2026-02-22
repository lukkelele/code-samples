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
 * @brief Allocate memory for a character instance.
 * Variation of character_create without arguments to the instance.
 * @retval 0 Memory allocated for character instance.
 * @retval EINVAL Invalid argument.
 * @retval ENOMEM Failed to allocate memory for instance.
 */
int character_alloc(character_t** character);

/**
 * @brief Destroy a character instance by freeing the allocated memory.
 */
void character_destroy(character_t* character);

/**
 * @brief Destroy a character instance by freeing the allocated memory
 * and setting the pointer to NULL if successful.
 * Variation of character_destroy.
 */
void character_destroy2(character_t** character);

/**
 * @brief Set character name.
 * @param character Instance to modify.
 * @param name Null-terminated string containing the new name.
 * @retval 0 Name set.
 * @retval EINVAL Invalid argument(s).
 * @retval ENOMEM Failed to allocate memory for string.
 */
int character_set_name(character_t* character, const char* name);

/**
 * @brief Print character info.
 */
void character_print(character_t* character);

/**
 * @brief Get character type as a string.
 */
const char* character_type_to_str(character_type_t type);
