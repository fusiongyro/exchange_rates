#include <string.h>
#include "extract.h"

// NOTE: we must handle exchange rates with commas in them too!
char* PATTERN = "<tr><td.*><\\/td><td.*><p.*>(\\w\\w\\w) "
                "[^<]+<\\/p><\\/td><td.*><p.*>(\\d*,?\\d+\\.\\d+)"
                "<\\/p><\\/td><td.*><p.*>(\\d*,?\\d+\\.\\d+)<\\/p><\\/td>"
                "<td.*><\\/td><\\/tr>";

struct extractor_s
{
  pcre* regexp;
  pcre_extra* extra;
  const char* errptr;
  int erroffset;
};

extractor make_extractor()
{
  // allocate the memory
  extractor setup = malloc(sizeof(struct extractor_s));
  
  // prepare the structures
  setup->regexp = pcre_compile(PATTERN, 0, &setup->errptr, &setup->erroffset, NULL);
  setup->extra = pcre_study(setup->regexp, 0, &setup->errptr);  
  
  return setup;
}

void remove_comma(char *string)
{
  char* comma = index(string, ',');
  
  // no comma: no problem
  if (comma == NULL)
    return;
    
  // otherwise, we need to overwrite the comma with the rest of the string
  strcpy(comma, comma+1);
}

void extract_from_string(extractor state, char* line, exchange_rate_cb callback)
{
  int ovector[20];
  int result = pcre_exec(state->regexp, state->extra, line, strlen(line), 0, 0, ovector, 20);
  
  if (result <= 0)
    return;
  
  // extract the groups
  const char *c_symbol, *c_to, *c_from;
  pcre_get_substring(line, ovector, result, 1, &c_symbol);
  pcre_get_substring(line, ovector, result, 2, &c_to);
  pcre_get_substring(line, ovector, result, 3, &c_from);

  // mutable copies
  char *symbol = strdup(c_symbol);
  char *to     = strdup(c_to);
  char *from   = strdup(c_from);

  // remove commas
  remove_comma(to);     remove_comma(from);

  // convert to floats
  float conversion_to   = atof(to);
  float conversion_from = atof(from);
  
  // invoke the callback
  (*callback)(symbol, conversion_to, conversion_from);
  
  // free the storage
  free(symbol); free(to); free(from);
  pcre_free_substring(c_symbol);
  pcre_free_substring(c_to);
  pcre_free_substring(c_from);
}

void skip_to_top_85(FILE* fp)
{
  int erroffset;
  const char* errptr;
  pcre* regexp = pcre_compile("Top 85", 0, &errptr, &erroffset, NULL);
  
  char line[1024];
  while (fgets(line, 1024, fp) && (pcre_exec(regexp, NULL, line, strlen(line), 0, 0, NULL, 0) < 0));
}

void extract_from_file(extractor state, FILE* fp, exchange_rate_cb callback)
{
  char line[1024];
  
  // step 1: skip to the top 85
  skip_to_top_85(fp);
  
  // step 2: process each line of the file
  while (fgets(line, 1024, fp))
    extract_from_string(state, line, callback);
}

// this is a sample callback
void print_exchange_rate(const char *symbol, double conversion_to, double conversion_from)
{
  printf("Conversion: %s : %f <-> %f\n", symbol, conversion_to, conversion_from);
}
