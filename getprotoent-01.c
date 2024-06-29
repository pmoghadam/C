// getprotoent-01.c
// Description: read entire /etc/protocols database with getprotoent()
// Compile: gcc -std=c99 -Wall -Wextra -Werror -pedantic -o getprotoent-01 getprotoent-01.c

#include <stdio.h>
#include <netdb.h>

int main()
{
    struct protoent *entry;
    while ((entry = getprotoent()) != NULL) {
        printf("%3d %-20s", entry->p_proto, entry->p_name);
        for (char **alias = entry->p_aliases; *alias != NULL; alias++)
            printf("%s ", *alias);
        printf("\n");
    }
    return 0;
}
