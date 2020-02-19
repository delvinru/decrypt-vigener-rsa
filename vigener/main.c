#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Размер английского алфавита
#define N 26
//Хочу сделать определение для кириллицы, т.е. через #define определить N = 33 и дальше от этого плясать

void crypt  (FILE *, char*, char*);
void attack (FILE*);

int main(const int argc, char** argv)
{
	//user guide
	if(argc < 3)
	{
		printf("./vigener -f source.txt -e/d -k key\n");
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
	if(argv[4] == NULL && strcmp(argv[3], "-d") == 0)
		attack(source);

	fclose(source);
	return 0;
}

//Алгоритм шифровки и дешифровки
void crypt(FILE *source, char* mode, char* key)
{
	//to upper text
	int key_len = strlen(key);
	for(int i = 0; i < key_len; i++)
			if(key[i] >= 97 && key[i] <= 122)
				key[i] = (char)(key[i] - 32);
	// Шифрование
	if(strcmp(mode, "-e") == 0)
	{
		FILE *encrypt;
		encrypt = fopen("encrypt.txt", "w");
		char m;
		int i = 0;
		while((m = getc(source)) != EOF)
		{
			if(m >= 97 && m <= 122)
				m -= 32;
			if(m >= 65 && m <= 90)
			{
				fprintf(encrypt, "%c", 65 + ( (m-65) + (key[i % key_len] - 65) )%N);
				i++;
			} else
				fprintf(encrypt, "%c", m);
		}
		fclose(encrypt);
		printf("[+]Файл зашифрован!\n");
	}
	//decrypt
	if(strcmp(mode, "-d") == 0)
	{
		printf("%s\n", key);
		printf("[+]Дешифруем текст с ключом\n");
		FILE *decrypt;
		decrypt = fopen("decrypt.txt", "w");

		char c;
		int i = 0;
		while((c = getc(source)) != EOF)
		{
			if(c >= 97 && c <= 122)
				c -= 32;
			if(c >= 65 && c <= 90)
			{
				fprintf(decrypt, "%c", 65 + ( (c-65) + N - (key[i % key_len] - 65)) % N );
				i++;
			}else
				fprintf(decrypt, "%c", c);
		}
		fclose(decrypt);
		printf("[+]Файл расшифрован!\n");
	}
}

// Нужно реализовать взлом шифра
void attack(FILE* decrypt)
{
	printf("дешифруем без ключа\n");
}
