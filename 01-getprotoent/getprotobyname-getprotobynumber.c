// getprotobyname-getprotobynumber.c
// Description: Using getprotobyname() and getprotobynumber() to search in /etc/protocols database
// Compile: gcc -std=c99 -Wall -Wextra -Werror -pedantic -o getprotobyname-getprotobynumber getprotobyname-getprotobynumber.c

/* man 3 getprotoent

  The getprotobyname() function returns a protoent structure for the entry from the database that
  matches the protocol name name.  A connection is opened to the database if necessary.

  The getprotobynumber() function returns a protoent structure for the entry  from  the  database
  that matches the protocol number number.  A connection is opened to the database if necessary.

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

/* Output:
  6 tcp                 TCP
 17 udp                 UDP
*/


#include <stdio.h>
#include <netdb.h>

void show_protoentry(struct protoent *e)
{
    printf("%3d %-20s", e->p_proto, e->p_name);
    for (char **alias = e->p_aliases; *alias != NULL; alias++)
        printf("%s ", *alias);
    printf("\n");
}

int main()
{
    struct protoent *entry;

    // getprotobyname()
    char *name = "tcp";
    entry = getprotobyname(name);
    if (entry != NULL)
        show_protoentry(entry);
    else
        printf("Protocol \"%s\" not found\n", name);

    // getprotobynumber()
    int proto = 17;
    entry = getprotobynumber(proto);
    if (entry != NULL)
        show_protoentry(entry);
    else
        printf("Protocol number %d not found\n", proto);

    return 0;
}
