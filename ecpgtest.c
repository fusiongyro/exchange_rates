#include <stdio.h>
#include "store.h"

int main()
{
  open_storage();
  write_currency("USD", 1.0, 1.0);
  close_storage();
  return 0;
}
