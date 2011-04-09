#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

#include "store.h"

char* PATTERN = "<tr><td.*><\\/td><td.*><p.*>(\\w\\w\\w) "
                "([^<]+)<\\/p><\\/td><td.*><p.*>(\\d+\\.\\d+)"
                "<\\/p><\\/td><td.*><p.*>(\\d+\\.\\d+)<\\/p><\\/td>"
                "<td.*><\\/td><\\/tr>";

typedef struct extraction_state_s
{
  pcre* regexp;
  pcre_extra* extra;
  const char* errptr;
  int erroffset;
} extraction_state;

extraction_state* setup_extraction()
{
  // allocate the memory
  extraction_state* setup = malloc(sizeof(extraction_state));
  
  // prepare the structures
  setup->regexp = pcre_compile(PATTERN, 0, &setup->errptr, &setup->erroffset, NULL);
  setup->extra = pcre_study(setup->regexp, 0, &setup->errptr);  
  
  return setup;
}

typedef void (*exchange_rate_callback)(const char*, const char*, double, double);

void extract_from_string(extraction_state* extractor, char* line, exchange_rate_callback callback)
{
  printf("examining line: %s\n", line);
  int ovector[20];
  int result = pcre_exec(extractor->regexp, extractor->extra, line, strlen(line), 0, 0, ovector, 20);
  
  if (result < 0)
    return;
  
  if (result == 0)
    printf("SQUAK!\n");
  
  // extract the groups
  const char *symbol, *description, *to, *from;
  pcre_get_substring(line, ovector, result, 1, &symbol);
  pcre_get_substring(line, ovector, result, 2, &description);
  pcre_get_substring(line, ovector, result, 3, &to);
  pcre_get_substring(line, ovector, result, 4, &from);

  // convert strings to doubles
  float conversion_to = atof(to);
  float conversion_from = atof(from);
  
  // call the callback
  (*callback)(symbol, description, conversion_to, conversion_from);
  
  // free the storage
  pcre_free_substring(symbol);
  pcre_free_substring(description);
  pcre_free_substring(to);
  pcre_free_substring(from);
}

void extract_from_file(extraction_state* extractor, FILE* fp, exchange_rate_callback callback)
{
  char line[1024];
  
  // step 1: process each line of the file
  while (fgets(line, 1024, fp))
    extract_from_string(extractor, line, callback);
}

void print_exchange_rate(const char *symbol, const char *description, double conversion_to, double conversion_from)
{
  printf("Conversion: %s (%s) : %f (%f)\n", symbol, description, conversion_to, conversion_from);
}

int main()
{
  extraction_state* setup = setup_extraction();
  extract_from_file(setup, stdin, &print_exchange_rate);
  
  return 0;
}
