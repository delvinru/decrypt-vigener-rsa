#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// Размер английского алфавита
#define N 26

// Статистическая вероятность языка
#define left_wall_fo_E 0.06
#define left_wall_fo_R 0.05
#define right_wall_fo_E 0.07
#define right_wall_fo_R 0.07

void crypt(FILE *, char*, char*);
void attack(FILE*);
int  find_key_length(FILE*, char mod, char** kod);

int main(const int argc, char** argv)
{
	//user guide
	if (argc < 3)
	{
		printf("./vigener -f source.txt -e/d -k key\n");
		exit(1);
	}

	//open source file
	FILE *source;
	source = fopen(argv[2], "r");

	if (source == NULL)
	{
		printf("Файл нельзя открыть\n");
		exit(1);
	}

	// Проверка на наличие ключа, т.к. от этого зависит будем ли мы просто шифровать/расшифровывать или взламывать текст
	if (argv[4] != NULL)
		crypt(source, argv[3], argv[5]);
	if (argv[4] == NULL && strcmp(argv[3], "-d") == 0)
		attack(source);

	fclose(source);
	return 0;
}

// Алгоритм шифровки и дешифровки
void crypt(FILE *source, char* mode, char* key)
{
	// Приводим текст к верхнему регистру
	int key_len = strlen(key);
	for (int i = 0; i < key_len; i++)
		if (key[i] >= 97 && key[i] <= 122)
			key[i] = (char)(key[i] - 32);
	// Шифрование
	if (strcmp(mode, "-e") == 0)
	{
		FILE *encrypt;
		encrypt = fopen("encrypt.txt", "w");
		char m;
		int i = 0;
		while ((m = getc(source)) != EOF)
		{
			if (m >= 97 && m <= 122)
				m -= 32;
			if (m >= 65 && m <= 90)
			{
				fprintf(encrypt, "%c", 65 + ((m - 65) + (key[i % key_len] - 65)) % N);
				i++;
			}
			else
			{
				fprintf(encrypt, "%c", m);
                // Нужно убрать этот элемент, т.к. он нарушает принцип работы шифрования Виженера. Пока оставим, но я категорически советую убрать его.
				i++;
			}
		}

		fclose(encrypt);
		printf("[+]Файл зашифрован! Смотрите файл encrypt.txt\n");
	}
	// Расшифровка
	if (strcmp(mode, "-d") == 0)
	{
		printf("[+]Дешифруем текст с ключом\n");
		FILE *decrypt;
		decrypt = fopen("decrypt.txt", "w");

		char c;
		int i = 0;
		while ((c = getc(source)) != EOF)
		{
			if (c >= 97 && c <= 122)
				c -= 32;
			if (c >= 65 && c <= 90)
			{
				fprintf(decrypt, "%c", 65 + ((c - 65) + N - (key[i % key_len] - 65)) % N);
				i++;
			}
			else
			{
				fprintf(decrypt, "%c", c);
                // Аналогично коду при шифровке, т.к. это i++ не верно.
				i++;
			}
		}

		fclose(decrypt);
		printf("[+]Файл расшифрован! Смотрите файл decrypt.txt\n");
	}
}

void attack(FILE* f)
{
	printf("[+]Дешифруем без ключа\n");
	unsigned char mod = 0;
	int length_alph = 0;
	int otclonenie_table = 0;

	// Разбираемся с алфавитом(только заглавные)
	while (!mod && !feof(f))
	{
		if ((mod = (unsigned char)fgetc(f)) <= 90 && mod >= 65)
            mod = 'e';
		else if ((mod) <= 192 + 33 && mod >= 192)
            mod = 'r';
		else
            mod = 0;
	}
	fseek(f, 0, SEEK_SET);
	if (mod == 'r')
	{
		length_alph = 33;
		otclonenie_table = 192;

	}
	if (mod == 'e')
	{
		length_alph = 26;
		otclonenie_table = 65;
	}
	if (mod == 0)
	{
		printf("Файл пуст\n");
	    exit(0);	
	}

	//найдем длину ключа
	int len_key = 0;
	char* kod = 0;
	len_key = find_key_length(f, mod, &kod);
	int length_kod = strlen(kod);

	//массив для количества символов
	int** amount_chars = 0;
	amount_chars = (int**)malloc(sizeof(int*)*length_alph);
	for (int i = 0; i < length_alph; i++)
	{
		amount_chars[i] = (int*)malloc(len_key * sizeof(int));
		for (int j = 0; j < len_key; j++)
			amount_chars[i][j] = 0;
	}
	//масив кол-ва знаков на каждую букву ключа
	int* kol_vo = 0;
	kol_vo = (int*)calloc(sizeof(int)*len_key, sizeof(int)*len_key);

	for (int f = 0; f < len_key; f++)
	{
		for (int j = f; j < length_kod; j = j + len_key)
		{
			if ((unsigned char)(kod)[j] >= otclonenie_table && (unsigned char)(kod)[j] < otclonenie_table + length_alph)
			{
				amount_chars[(unsigned char)(kod)[j] - otclonenie_table][f]++;
				kol_vo[f]++;
			}
			else if ((unsigned char)(kod)[j] == 168)
			{
				amount_chars[length_alph - 1][f]++;
				kol_vo[f]++;
			}
		}
	}

	//масив для отклонений
	double** otklonenie = 0;
	otklonenie = (double**)malloc(sizeof(double*)*length_alph);
	for (int i = 0; i < length_alph; i++)
	{
		otklonenie[i] = (double*)malloc((len_key - 1) * sizeof(double));
		for (int j = 0; j < len_key - 1; j++)
			otklonenie[i][j] = 0;
	}

	//масив для peзультирующих отклонений
	int* otklonenie_REZ = 0;
	otklonenie_REZ = (int*)malloc(sizeof(int)*(len_key - 1));
	double MAX_otklonenie = 0;

	for (int j = 1; j < len_key; j++)
	{
		MAX_otklonenie = 0;
		for (int i = 0; i < length_alph; i++)
		{

			for (int q = 0; q < length_alph; q++)
				otklonenie[i][j - 1] = otklonenie[i][j - 1] + (amount_chars[q][0] * amount_chars[(q + i) % length_alph][j]);
			otklonenie[i][j - 1] = otklonenie[i][j - 1] / (kol_vo[0] * kol_vo[j]);
			{
				if (otklonenie[i][j - 1] > MAX_otklonenie)
				{
					otklonenie_REZ[j - 1] = i;
					MAX_otklonenie = otklonenie[i][j - 1];
				}
			}
		}
	}

	printf("[+]Выбери возможный ключ\n");
	for (int i = 0; i < length_alph; i++)
	{
		printf("[%2i] - %c", i, i + otclonenie_table);
		for (int j = 0; j < len_key - 1; j++)
			printf("%c", (i + otklonenie_REZ[j]) % length_alph + otclonenie_table);
		printf("\n");
	}

	int nomer_key = -1;
    printf("Введите вариант: ");
	scanf("%i", &nomer_key);

	char* key = (char*)malloc((len_key + 1) * sizeof(char));
	key[0] = nomer_key + otclonenie_table;
	for (int i = 1; i < len_key; i++)
		key[i] = (nomer_key + otklonenie_REZ[i - 1]) % length_alph + otclonenie_table;
	key[len_key] = 0;

	char mode[3] = { "-d" };

	fseek(f, 0, SEEK_SET);

	crypt(f, mode, key);
}

int find_key_length(FILE* file, char mod, char** kod)// mod={r,e}
{
	//узнаём размер файла
	fseek(file, 0, SEEK_END);
	int length_kod = ftell(file);
	fseek(file, 0, SEEK_SET);
	//вводим его
	*kod = (char*)calloc((length_kod + 2) * sizeof(char), (length_kod + 2) * sizeof(char));

	fread(*kod, sizeof(char), length_kod, file);
	int smeshenie = 1;
	int proverka = 1;
	double MAX_index_covpadeniu = 0;
	int length_alph = 0;
	int otclonenie = 0;
	double index_covpadeniu = 0;
	int otclonenie_table = 0;
	int* amount_chars = 0;

	//разбираемся с алфавитом
	if (mod == 'r')
    {
        //русский
		otclonenie_table = 192;
		length_alph = 33;
	}
	else if (mod == 'e')
    {
        //англ
		otclonenie_table = 65;
		length_alph = 26;
	}
    //в случае если,что-то пошло не так с алфавитом
	else
		return 0;
	//масив для кол-ва символов
	amount_chars = (int*)malloc(length_alph * sizeof(int));

	//считам спецсимволы
	for (int i = 0; i < length_kod; i++)
	{
		if (!((unsigned char)(*kod)[i] >= otclonenie_table && (unsigned char)(*kod)[i] <= otclonenie_table + length_alph || (unsigned char)(*kod)[i] == 168))otclonenie++;
	}

	//магия
	for (int i = 2; i < length_kod; i++)
	{
		for (int f = 0; f < i; f++)
		{
			for (int q = 0; q < length_alph; q++)
				amount_chars[q] = 0;

			index_covpadeniu = 0;
			for (int j = f; j < length_kod; j = j + i)
			{
				if ( (unsigned char)(*kod)[j] >= otclonenie_table && (unsigned char)(*kod)[j] < otclonenie_table + length_alph )
					amount_chars[(unsigned char)(*kod)[j] - otclonenie_table]++;
				else if ((unsigned char)(*kod)[j] == 168)
					amount_chars[length_alph - 1]++;
			}
			for (int j = 0; j < length_alph; j++)
				index_covpadeniu = index_covpadeniu + (amount_chars[j] * (amount_chars[j] - 1)) / ((float)((length_kod - otclonenie) / i) *((length_kod - otclonenie) / i - 1));

			if ((index_covpadeniu < right_wall_fo_E  && index_covpadeniu > left_wall_fo_E  && mod == 'e') || ((index_covpadeniu < right_wall_fo_R  &&index_covpadeniu > left_wall_fo_R) && mod == 'r'))
			{
				if (index_covpadeniu > MAX_index_covpadeniu && (i % smeshenie) != 0 || proverka)
				{
					smeshenie = i;
					if (proverka)
                        proverka--;
					MAX_index_covpadeniu = index_covpadeniu;
				}
                // Коэффицент, который позволяет отсеять "побочную" длину ключа
				if ((i % smeshenie) == 0)
                    MAX_index_covpadeniu *= 1.06;
				//можно вставить красивый вывод
			}
		}
	}
	free(amount_chars);
	// Выводим значение
	return smeshenie;
}
