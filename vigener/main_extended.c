#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

void crypt(FILE *, wchar_t*, wchar_t*);
void hack (FILE*);

int main(const int argc, wchar_t** argv)
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
	//user guide
	if(argc < 3)
	{
		printf("./vigener -f source.txt -e/d -k key");
		exit(1);
	}

	//open source file
	FILE *source;
	source = fopen(argv[2], "r");

	if(source == NULL)
	{
		printf("Файл нельзя открыть\n");
		exit(1);
	}

	// Проверка на наличие ключа, т.к. от этого зависит будем ли мы просто шифровать/расшифровывать или взламывать текст
	if(argv[4] != NULL)
		crypt(source, argv[3], argv[5]);
	if(argv[4] == NULL && strcasecmp(argv[3], "-d") == 0)
		hack(source);

	fclose(source);
	return 0;
}

//Алгоритм шифровки и дешифровки
void crypt(FILE *source, wchar_t* mode, wchar_t* key)
{
	//to upper text
	for(int i = 0; i < strlen(key); i++)
			if((key[i] >= 97 && key[i] <= 122) || (key[i] >= 1072 && key[i] <= 1103))
				key[i] = (wchar_t)(key[i] - 32);
	int key_len = strlen(key);
	int N; //for eng
	int shift;

	//Определим язык, которым было зашифровано сообщение
	//То это русский язык, а значит N = 33
	wchar_t test_sym = key[0];
	if(test_sym > 1039 || test_sym < 1104)
	{
		N = 32;
		shift = 1040;
	}
	//Английский язык, а значит N = 26
	if(test_sym > 64 && test_sym < 91)
	{
		N = 26;
		shift = 65;
	}
	// Шифрование
	if(strcasecmp(mode, "-e") == 0)
	{
		FILE *encrypt;
		encrypt = fopen("encrypt.txt", "w");

		wchar_t m;
		int i = 0;
		while((m = getc(source)) != EOF)
		{
			// Превращение строчных букв в заглавные
			if((m >= 97 && m <= 122) || (m >= 1072 && m <= 1103))
				m -= 32;
			if((m >= 65 && m <= 90) || (m >= 1040 && m <= 1071))
			{
				fprintf(encrypt, "%c", shift + ( (m-shift) + (key[i % key_len] - shift) )%N);
				i++;
			} else
				fprintf(encrypt, "%c", m);
		}
		fclose(encrypt);
		printf("[+]Файл зашифрован!\n");
	}
	//decrypt
	if(strcasecmp(mode, "-d") == 0)
	{
		printf("%s\n", key);
		printf("[+]Дешифруем текст с ключом\n");
		FILE *decrypt;
		decrypt = fopen("decrypt.txt", "w");

		char c;
		int i = 0;
		while((c = getc(source)) != EOF)
		{
			if((m >= 97 && m <= 122) || (m >= 1072 && m <= 1103))
				c -= 32;
			if((m >= 65 && m <= 90) || (m >= 1040 && m <= 1071))
			{
				fprintf(decrypt, "%c", 65 + ( (c-65) + N - (key[i % key_len] - 65))%N );
				i++;
			}else
				fprintf(decrypt, "%c", c);
		}
		fclose(decrypt);
		printf("[+]Файл расшифрован!\n");
	}
}

// Нужно реализовать взлом шифра
void hack(FILE* decrypt)
{
	printf("дешифруем без ключа\n");
}
