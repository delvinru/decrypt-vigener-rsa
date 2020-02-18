#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main()
{
  // wchar_t wch[256];
  setlocale(LC_ALL, "ru_RU.UTF-8");
  // wprintf(L"Введите русские символы: ");
  // wscanf(L"%s", wch);
  // wprintf(L"\nСтрока: %s\n", wch);
  // wprintf(L"Цифра:%c %d", wch[0], wch[0]);
  // wchar_t c;
  // wprintf(L"enter: ");
  // wscanf(L"%lc", &c);
  // wprintf(L"el: %ld", c);
  for(int i = 1020; i < 1110; i++)
    wprintf(L"El: %5lc Num: %ld\n", i , i);
  wprintf(L"******\n");
  for(int i = 65; i < 128; i++)
    wprintf(L"El: %5lc Num: %ld\n", i, i);
  return 0;
}
