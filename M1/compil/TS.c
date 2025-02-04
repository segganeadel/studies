#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TS.h"

#define MAX_SCOPE_SIZE 100

typedef struct Scope
{
    Identifier *identifiers;
    int next_identifier_index;

    struct Scope *prev;
} Scope;

Scope *current_scope = NULL;

int is_declared_in_current_scope(char *name)
{
    Scope *scope = current_scope;
    int i;
    for (i = 0; i < scope->next_identifier_index; i++)
        if (strcmp(scope->identifiers[i].name, name) == 0)
            return 1;

    return 0;
}

int is_declared(char *name)
{
    Identifier *idf = get(name);

    if (idf != NULL)
        return 1;

    return 0;
}

Identifier *get(char *name)
{
    Scope *scope = current_scope;

    while (scope != NULL)
    {
        int i;
        for (i = 0; i < scope->next_identifier_index; i++)
            if (strcmp(scope->identifiers[i].name, name) == 0)
                return &scope->identifiers[i];

        scope = scope->prev;
    }

    return NULL;
}

void create_new_scope()
{
    Scope *scope = malloc(sizeof(Scope));

    scope->identifiers = malloc(sizeof(Identifier) * MAX_SCOPE_SIZE);
    scope->next_identifier_index = 0;
    scope->prev = current_scope;

    current_scope = scope;
}

void destroy_most_inner_scope()
{
    Scope *scope = current_scope;

    current_scope = scope->prev;

    free(scope->identifiers);
    free(scope);
}

void insert(char *name, char *type)
{
    Identifier *identifiers = current_scope->identifiers;
    int next_identifier_index = current_scope->next_identifier_index;

    identifiers[next_identifier_index].name = name;
    identifiers[next_identifier_index].type = type;
    identifiers[next_identifier_index].is_array = 0;
    identifiers[next_identifier_index].array_size = 0;

    current_scope->next_identifier_index++;
}

void insert_array(char *name, char *type, int array_size)
{
    Identifier *identifiers = current_scope->identifiers;
    int next_identifier_index = current_scope->next_identifier_index;

    identifiers[next_identifier_index].name = name;
    identifiers[next_identifier_index].type = type;
    identifiers[next_identifier_index].is_array = 1;
    identifiers[next_identifier_index].array_size = array_size;

    current_scope->next_identifier_index++;
}

void display()
{
    Scope *scope = current_scope;
    printf("\n/***************Table des symboles IDF******************/\n");
    printf("____________________________________________________________________\n");
    printf("\t| Nom_Entite | Type_Entite |\n");
    printf("____________________________________________________________________\n");
    while (scope != NULL)
    {
        int i;
        for (i = 0; i < scope->next_identifier_index; i++)
            if (scope->identifiers[i].is_array)
                printf("Name: %s, Type: %s[], Array Size: %d\n", scope->identifiers[i].name, scope->identifiers[i].type, scope->identifiers[i].array_size);
            else
                printf("Name: %s, Type: %s\n", scope->identifiers[i].name, scope->identifiers[i].type);

        scope = scope->prev;
    }
}
