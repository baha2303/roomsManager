#include <stdlib.h>
#include <string.h>
#include "SystemRoom.h"

#define ASSERT(test_number, test_condition)  \
   if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
   else printf("\nTEST %s OK", test_number)


int main(int argc, char **argv)
{

 SystemRoom sys = systemCreate("Tech","C:\\Users\\baha2\\Desktop\\HarelTests\\rooms");
 systemDisplay(sys,stdout);
    return 0;
}
