#ifdef NDEBUG
#undef NDEBUG
#endif

#include "phone_forward.h"
#include <assert.h>
#include <string.h>
#include <sys/resource.h>
#include <stdlib.h>

#define TEST_LEN 21900

#define MAX_LEN 23
#define REV_OFF

int main() {
  char num1[MAX_LEN + 1], num2[MAX_LEN + 1];
  PhoneForward *pf;
  PhoneNumbers *pnum;

  struct rlimit limit;
  getrlimit(RLIMIT_STACK, &limit);
  printf("Stack limit is %zu current and %zu max.\n", limit.rlim_cur, limit.rlim_max);

  pf = phfwdNew();

  int letter = 1;
  char *x = malloc(sizeof(char) * TEST_LEN);
  for (size_t index = 0; index <= TEST_LEN-2 ; index++) {
    if (letter == 10) {
      letter = 1;
    }

    x[index] = letter + '0';
    x[index + 1] = '\0';
    letter += 1;

    if (index % 1000 == 0) {
      printf("PERCENT %f AND INDEX is %zu\n", (float)index / TEST_LEN, index);
    }

    assert(phfwdAdd(pf, x, "123"));
  }
  
  free(x);
  phfwdDelete(pf);
  pf = phfwdNew();


  strcpy(num1, "123");
  strcpy(num2, "9");
  assert(phfwdAdd(pf, num1, num2) == true);
  memset(num1, 0, sizeof num1);
  memset(num2, 0, sizeof num2);

  pnum = phfwdGet(pf, "1234");
  assert(strcmp(phnumGet(pnum, 0), "94") == 0);
  assert(phnumGet(pnum, 1) == NULL);
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "12");
  assert(strcmp(phnumGet(pnum, 0), "12") == 0);
  phnumDelete(pnum);

  strcpy(num1, "123456");
  strcpy(num2, "777777");
  assert(phfwdAdd(pf, num1, num2) == true);

  pnum = phfwdGet(pf, "12345");
  assert(strcmp(phnumGet(pnum, 0), "945") == 0);
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "123456");
  assert(strcmp(phnumGet(pnum, 0), "777777") == 0);
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "997");
  assert(strcmp(phnumGet(pnum, 0), "997") == 0);
  phnumDelete(pnum);

  assert(phfwdAdd(pf, "431", "432") == true);
  assert(phfwdAdd(pf, "432", "433") == true);
  pnum = phfwdGet(pf, "431");
  assert(strcmp(phnumGet(pnum, 0), "432") == 0);
  phnumDelete(pnum);
  pnum = phfwdGet(pf, "432");
  assert(strcmp(phnumGet(pnum, 0), "433") == 0);
  phnumDelete(pnum);
#ifndef REV_OFF
  pnum = phfwdReverse(pf, "432");
  assert(strcmp(phnumGet(pnum, 0), "431") == 0);
  assert(strcmp(phnumGet(pnum, 1), "432") == 0);
  assert(phnumGet(pnum, 2) == NULL);
  phnumDelete(pnum);

  pnum = phfwdReverse(pf, "433");
  assert(strcmp(phnumGet(pnum, 0), "432") == 0);
  assert(strcmp(phnumGet(pnum, 1), "433") == 0);
  assert(phnumGet(pnum, 2) == NULL);
  phnumDelete(pnum);

  pnum = phfwdReverse(pf, "987654321");
  assert(strcmp(phnumGet(pnum, 0), "12387654321") == 0);
  assert(strcmp(phnumGet(pnum, 1), "987654321") == 0);
  assert(phnumGet(pnum, 2) == NULL);
  phnumDelete(pnum);
#endif
  phfwdRemove(pf, "12");

  pnum = phfwdGet(pf, "123456");
  assert(strcmp(phnumGet(pnum, 0), "123456") == 0);
  phnumDelete(pnum);
#ifndef REV_OFF
  pnum = phfwdReverse(pf, "987654321");
  assert(strcmp(phnumGet(pnum, 0), "987654321") == 0);
  assert(phnumGet(pnum, 1) == NULL);
  phnumDelete(pnum);
#endif
  assert(phfwdAdd(pf, "567", "0") == true);
  assert(phfwdAdd(pf, "5678", "08") == true);
#ifndef REV_OFF
  pnum = phfwdReverse(pf, "08");
  assert(strcmp(phnumGet(pnum, 0), "08") == 0);
  assert(strcmp(phnumGet(pnum, 1), "5678") == 0);
  assert(phnumGet(pnum, 2) == NULL);
  phnumDelete(pnum);
#endif
  assert(phfwdAdd(pf, "A", "1") == false);
  assert(phfwdAdd(pf, "1", "A") == false);

  phfwdRemove(pf, "");
  phfwdRemove(pf, NULL);

  pnum = phfwdGet(pf, "A");
  assert(phnumGet(pnum, 0) == NULL);
  phnumDelete(pnum);
#ifndef REV_OFF
  pnum = phfwdReverse(pf, "A");
  assert(phnumGet(pnum, 0) == NULL);
  phnumDelete(pnum);
#endif
  phfwdAdd(pf, "12", "123");
  pnum = phfwdGet(pf, "123");
  assert(strcmp(phnumGet(pnum, 0), "1233") == 0);
  phnumDelete(pnum);

  phfwdAdd(pf, "2", "4");
  phfwdAdd(pf, "23", "4");
#ifndef REV_OFF
  pnum = phfwdReverse(pf, "434");
  assert(strcmp(phnumGet(pnum, 0), "2334") == 0);
  assert(strcmp(phnumGet(pnum, 1), "234") == 0);
  assert(strcmp(phnumGet(pnum, 2), "434") == 0);
  assert(phnumGet(pnum, 3) == NULL);
  phnumDelete(pnum);
#endif
  phfwdDelete(pf);
  pnum = NULL;
  phnumDelete(pnum);
  pf = NULL;
  phfwdDelete(pf);

  pf = phfwdNew();
  phfwdAdd(pf, "1234", "76");
  pnum = phfwdGet(pf, "1234581");
  assert(strcmp(phnumGet(pnum, 0), "76581") == 0);
  phnumDelete(pnum);
  pnum = phfwdGet(pf, "7581");
  assert(strcmp(phnumGet(pnum, 0), "7581") == 0);
  phnumDelete(pnum);
#ifndef REV_OFF
  pnum = phfwdReverse(pf, "7581");
  assert(strcmp(phnumGet(pnum, 0), "7581") == 0);
  assert(phnumGet(pnum, 1) == NULL);
  phnumDelete(pnum);
#endif
  phfwdDelete(pf);
}