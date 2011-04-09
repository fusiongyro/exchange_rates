#include <stdio.h>

void process_lines(FILE* fp)
{
  char line[1024];
  int i = 1;
  while (fgets(line, 1024, fp))
    printf("%d: %s\n", i++, line);
}

int main()
{
  FILE* fp = fopen("ucc-xe.txt", "r");
  process_lines(fp);
  fclose(fp);
  return 0;
}