#include "extract.h"
#include "store.h"

int main()
{
  // make the extractor
  extractor state = make_extractor();
  
  // open the database connection
  open_storage();
  
  // hook it up to standard input, using the database inserter
  extract_from_file(state, stdin, &store_exchange_rate);
  
  // close the database connection
  close_storage();
}