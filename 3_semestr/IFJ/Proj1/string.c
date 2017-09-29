/**
 * @author Marek Dolezel 
 * @date 2_10_2016
 * @file string.c 
 * @brief Implementation of dynamic string datatype. 
 * Copyright (c) 2016
 */

#include <stdlib.h>

#include "string.h"
#include "internal_codes.h"

/* TODO: Add functions as needed. Anyone can do it. Add prototype to string.h and implementation here */
String *string_new(void)
{
	String *s = (String *) malloc(sizeof(String));
	
	if (s == NULL)
		return NULL;
	
	s->string = (char *) malloc(STR_LEN);

	if (s->string == NULL) 
	{
		free(s);
		return NULL;
	}

	s->string[0] = '\0';
	s->length = 0;
	s->alloc_size = STR_LEN;
	s->nxt_realloc_size = 2 * STR_LEN;

	return s;
}

void string_add_char(String *s, char c) 
{
	if (((s->length) + 2) <= s->alloc_size) /* We need:  + 1 (for char c) + 1 (for \0) */
	{
		int length = s->length;
		s->string[length] = c;
		s->string[length+1] = '\0';

		s->length++;
	}
	else /* There is not enough space, we do realloc to nxt_realloc_size */
	{
		if ((s->string=(char*)realloc(s->string, s->nxt_realloc_size)) == NULL)
		{ /* Realloc has failed. */
			
		}
		else 
		{ /* Realloc went ok. */
			/* Update information for next realloc */
			s->alloc_size = s->nxt_realloc_size;
			s->nxt_realloc_size*=2;

			/* Finally add char we did realloc for. */
			string_add_char(s, c);
		}
			
	}
}

void string_del_char(String *s) {
	s->length--;
}

void string_init_str(String *s, char *sc)
{
	int n = 0;
	s->length = 0;

	while (sc[n] != '\0')
	{
		string_add_char(s, sc[n]);
		n++;
	}
}

void string_add_str(String *s, String *b)
{
	int n = 0;
	int len = b->length;

	while (n<len)
	{
		string_add_char(s, b->string[n]);
		n++;
	}
}

void string_del(String *s)
{
	free(s->string);
	free(s);
}

/* We don't use this function anywhere yet. */
int string_compare(String *a, String *b)
{
	if (a->length != b->length) /* If length differs - strings ain't the same! */
		return 0;
	
	int n = 0;
	while(a->string[n] == b->string[n] ) {
		if ( a->string[n] == '\0') /* Hurray! Strings are the same! */
		{
			return 1;
		}
		
		n++;
	}
	return 0; /* If we got out of while, then strings ain't the same :( */
}

/* Return fully qualified id */
String *str_fq(String *class, String *id)
{
	String *temp = string_new();

	string_add_str(temp, class);
	string_add_char(temp, '.');
	string_add_str(temp, id);

	return temp;

}
