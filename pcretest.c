#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

#include "store.h"

char* PATTERN = "<tr><td.*><\\/td><td.*><p.*>(\\w\\w\\w) "
                "([^<]+)<\\/p><\\/td><td.*><p.*>(\\d+\\.\\d+)"
                "<\\/p><\\/td><td.*><p.*>(\\d+\\.\\d+)<\\/p><\\/td>"
                "<td.*><\\/td><\\/tr>";

char* target1 = "<tr><td  width=\"8\"></td><td  height=\"26\"><p style=\"font-family:Arial, Helvetica, sans-serif;font-size:11px;color:#6a6d70;\">CAD Canada Dollars</p></td><td  align =\"right\"><p style=\"font-family:Arial, Helvetica, sans-serif;font-size:11px;color:#6a6d70;\">1.0443936547</p></td><td  align =\"right\"><p style=\"font-family:Arial, Helvetica, sans-serif;font-size:11px;color:#6a6d70;\">0.9574933700</p></td><td  width=\"8\"></td></tr>";
char* target2 = "<tr><td bgcolor=\"#f4f4f4\" width=\"8\"></td><td bgcolor=\"#f4f4f4\" height=\"26\"><p style=\"font-family:Arial, Helvetica, sans-serif;font-size:11px;color:#6a6d70;\">XOF CFA Francs BCEAO</p></td><td bgcolor=\"#f4f4f4\" align =\"right\"><p style=\"font-family:Arial, Helvetica, sans-serif;font-size:11px;color:#6a6d70;\">0.0022006473</p></td><td bgcolor=\"#f4f4f4\" align =\"right\"><p style=\"font-family:Arial, Helvetica, sans-serif;font-size:11px;color:#6a6d70;\">454.4117519979</p></td><td bgcolor=\"#f4f4f4\" width=\"8\"></td></tr>";

typedef struct pcre_setup_s
{
  pcre* regexp;
  pcre_extra* extra;
  const char* errptr;
  int erroffset;
} pcre_setup;

pcre_setup* setup_extraction()
{
  // allocate the memory
  pcre_setup* setup = malloc(sizeof(pcre_setup));
  
  // prepare the structures
  setup->regexp = pcre_compile(PATTERN, 0, &setup->errptr, &setup->erroffset, NULL);
  setup->extra = pcre_study(setup->regexp, 0, &setup->errptr);  
  
  return setup;
}

int main()
{
  int ovector[20];
  
  pcre_setup* setup = setup_extraction();
  int result = pcre_exec(setup->regexp, setup->extra, target1, strlen(target1), 0, 0, ovector, 20);
  const char *symbol, *description, *to, *from;
  pcre_get_substring(target1, ovector, result, 1, &symbol);
  pcre_get_substring(target1, ovector, result, 2, &description);
  pcre_get_substring(target1, ovector, result, 3, &to);
  pcre_get_substring(target1, ovector, result, 4, &from);

  // convert strings to doubles
  float conversion_to = atof(to);
  float conversion_from = atof(from);

  printf("Conversion: %s (%s) : %f (%f)\n", symbol, description, conversion_to, conversion_from);
  open_storage();
  write_currency(symbol, conversion_to, conversion_from);
  close_storage();
  
  pcre_free_substring(symbol);
  pcre_free_substring(description);
  pcre_free_substring(to);
  pcre_free_substring(from);
  
  return 0;
}
