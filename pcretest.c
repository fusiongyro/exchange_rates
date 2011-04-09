#include <stdio.h>
#include <pcre.h>

int main()
{
  printf("Using PCRE version %s\n", pcre_version());
  return 0;
}
