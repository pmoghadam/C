// getprotoent.c
// Description: read entire /etc/protocols database with getprotoent()
// Compile: gcc -std=c99 -Wall -Wextra -Werror -pedantic -o getprotoent getprotoent.c

/* 
       man 3 getprotoent

       The  getprotoent() function reads the next entry from the protocols database (see protocols(5))
       and returns a protoent structure containing the broken-out fields from the entry.  A connection
       is opened to the database if necessary.

       The protoent structure is defined in <netdb.h> as follows:

           struct protoent {
               char  *p_name;       // official protocol name
               char **p_aliases;    // alias list
               int    p_proto;      // protocol number
           }

       The members of the protoent structure are:

       p_name The official name of the protocol.

       p_aliases
              A NULL-terminated list of alternative names for the protocol.

       p_proto
              The protocol number.

*/


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
