// getprotobynumber.c
// Description: Using getprotobynumber() to search in /etc/protocols database
// Compile: gcc -std=c99 -Wall -Wextra -Werror -pedantic -o getprotobynumber getprotobynumber.c

/* Output:
17 udp UDP
*/

#include <stdio.h>
#include <netdb.h>

int main()
{
    int proto = 17;

    struct protoent *entry;
    entry = getprotobynumber(proto);
    if (entry != NULL) {
        printf("%d %s ", entry->p_proto, entry->p_name);
        for (char **alias = entry->p_aliases; *alias != NULL; alias++)
            printf("%s ", *alias);
        printf("\n");
    } else {
        printf("Protocol number %d not found\n", proto);
    }

    return 0;
}
