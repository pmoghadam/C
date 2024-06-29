// getprotobyname.c
// Description: Using getprotobyname() to search in /etc/protocols database
// Compile: gcc -std=c99 -Wall -Wextra -Werror -pedantic -o getprotobyname getprotobyname.c

/* Output:

6 tcp TCP

*/

#include <stdio.h>
#include <netdb.h>

int main()
{
    char *name = "tcp";

    struct protoent *entry = getprotobyname(name);
    if (entry != NULL) {
        printf("%d %s ", entry->p_proto, entry->p_name);
        for (char **alias = entry->p_aliases; *alias != NULL; alias++)
            printf("%s ", *alias);
        printf("\n");
    } else {
        printf("Protocol \"%s\" not found\n", name);
    }

    return 0;
}
