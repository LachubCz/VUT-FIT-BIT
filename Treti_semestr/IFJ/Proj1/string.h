/**
 * @author Marek Dolezel
 * @date 10_02_2016
 * @file string.h
 * @brief String submodule
 * Copyright (c) 2016
 */

#ifndef STRING_H /* This definition has to correspond to file name */
#define STRING_H /* . */

/**
* @brief data structure for string abstraction
*/
typedef struct string_s {
	char *string; /**< string */
	int length;  /**< length of string */
	size_t alloc_size; /**< allocated size */
	size_t nxt_realloc_size; /**< Size to be allocated after next realloc */
} String;

/**
  * @brief Create new string
  * @return pointer to String datastructure
  */
String *string_new(void);

/**
  * @brief Add new char to string
  * @param s pointer to String
  * @param c char
  */
void string_add_char(String *s, char c);

/**
  * @brief Delete String
  * @param s pointer to String
  */
void string_del_char(String *s);

/**
  * @brief Add new char* to string
  * @param s pointer to String
  * @param sc pointer to char
  */
void string_init_str(String *s, char *sc);

/**
  * @brief Add new string to string
  * @param s pointer to String
  * @param b pointer to String
  */
void string_add_str(String *s, String *b);

/**
  * @brief Delete String
  * @param s pointer to String
  */
void string_del(String *s);

/**
  * @brief Compare two strings
  * @param a pointer to String
  * @param b pointer to String
  */
int string_compare(String *a, String *b);

/**
  * @brief return fully qualified identifier
  * @param class pointer to String
  * @param id    pointer to String
  */
String *str_fq(String *class, String *id);


#endif
