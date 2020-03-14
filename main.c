#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include<locale.h>

// Размер английского алфавита
#define N 26

// Статистическая вероятность языка
#define left_wall_fo_E 0.06
#define left_wall_fo_R 0.05
#define right_wall_fo_E 0.07
#define right_wall_fo_R 0.07

void crypt(FILE *, char*, char*);
void attack(FILE*);
int find_Key_lenth(FILE* file, char mod, char** kod);// mod={r,e}

int main(const int argc, char** argv)
{
	setlocale(LC_ALL, "Rus");
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
int toupp(int c)
{
	if (c >= 97 && c <= 122)
		return c - 32;

	if (c >= 224 && c <= 255)
		return c - 32;
	if (c ==184)
		return 168;
}

unsigned char alf(unsigned char c)
{
	c = toupp(c);
	if (c >= 65 && c <= 90)
	{
		return c - 65;
	}
	if (c >= 192 && c <= 197)
	{
		return c - 192;
	}
	if (c == 168)
	{
		return 6;
	}
	if (c >= 198 && c <= 223)
	{
		return c -191;
	}
	return c;
}


// Алгоритм шифровки и дешифровки
void crypt(FILE *source, char* mode, char* key)
{
	// Приводим текст к верхнему регистру
	int key_len = strlen(key);

	unsigned char mod = 0;
	int lenth_alf = 0;
	int otclonenie_table = 0;
	// Разбираемся с алфавитом(только заглавные)
	while (!mod && !feof(source))
	{
		if ((mod = toupp((unsigned char)fgetc(source))) <= 90 && mod >= 65)
			mod = 'e';
		else if ((mod) <= 192 + 33 && mod >= 192)
			mod = 'r';
		else
			mod = 0;
	}
	fseek(source, 0, SEEK_SET);
	if (mod == 'r')
	{
		lenth_alf = 33;
		otclonenie_table = 192;

	}
	if (mod == 'e')
	{
		lenth_alf = 26;
		otclonenie_table = 65;
	}
	if (mod == 0)
	{
		printf("Файл пуст\n");
		exit(0);
	}

	// Шифрование
	if (strcmp(mode, "-e") == 0)
	{
		FILE *encrypt;
		encrypt = fopen("encrypt.txt", "w");
		int m;
		int i = 0;



		while ((m = getc(source)) != EOF)
		{
			if (alf(m) == m)
			{
				fprintf(encrypt, "%c", m);
			}
			else
			{

				if (mod == 'e')
				{
					fprintf(encrypt, "%c", otclonenie_table + (alf(m) + alf(key[i % key_len])) % lenth_alf);
					i++;
				}
				if (mod == 'r')
				{
					if ((alf(m) + alf(key[i % key_len])) % lenth_alf == 6)
					{
						fprintf(encrypt, "Ё");

					}
					else
						if ((alf(m) + alf(key[i % key_len])) % lenth_alf < 6)
							fprintf(encrypt, "%c", otclonenie_table + (alf(m) + alf(key[i % key_len])) % lenth_alf);
						else
							fprintf(encrypt, "%c", otclonenie_table - 1 + (alf(m) + alf(key[i % key_len])) % lenth_alf);
					i++;
				}
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

		int m;
		int i = 0;



		while ((m = getc(source)) != EOF)
		{
			if (alf(m) == m)
			{
				fprintf(decrypt, "%c", m);
			}
			else
			{
				if (mod == 'e')
				{
					fprintf(decrypt, "%c", otclonenie_table + (alf(m) + lenth_alf - alf(key[i % key_len])) % lenth_alf);
					i++;
				}
				if (mod == 'r')
				{
					if ((alf(m) + lenth_alf - alf(key[i % key_len])) % lenth_alf == 6)
					{
						fprintf(decrypt, "Ё");
					}
					else
						if ((alf(m) + lenth_alf - alf(key[i % key_len])) % lenth_alf < 6)
							fprintf(decrypt, "%c", otclonenie_table + (alf(m) + lenth_alf - alf(key[i % key_len])) % lenth_alf);
						else
							fprintf(decrypt, "%c", otclonenie_table - 1 + (alf(m) + lenth_alf - alf(key[i % key_len])) % lenth_alf);
					i++;
				}
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
		if ((mod = toupp((unsigned char)fgetc(f))) <= 90 && mod >= 65)
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
	len_key =find_Key_lenth(f, mod, &kod);
	//printf("%i", len_key);
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
				amount_chars[alf((unsigned char)(kod)[j])][f]++;
				kol_vo[f]++;
			}
			else if ((unsigned char)(kod)[j] == 168)
			{
				amount_chars[6][f]++;
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
	int proverca_na_uo = 1;
	printf("[+]Выбери возможный ключ\n");
	for (int i = 0; i < length_alph; i++)
	{
		if (proverca_na_uo && i == 6 && mod=='r')
		{
			printf("[%2i] - %c", i, 168);
		}
		else if(!proverca_na_uo && mod == 'r')
			printf("[%2i] - %c", i, i-1 + otclonenie_table);
		else
			printf("[%2i] - %c", i, i + otclonenie_table);

		for (int j = 0; j < len_key - 1; j++)
		{
			if ((i + otklonenie_REZ[j]) % length_alph==6 && mod == 'r')
			{
				printf("%c", i, 168);
			}
			else if ((i + otklonenie_REZ[j]) % length_alph > 6 && mod == 'r')
				printf("%c", (i  + otklonenie_REZ[j]) % length_alph -1+ otclonenie_table);
			else
				printf("%c", (i + otklonenie_REZ[j]) % length_alph + otclonenie_table);
		}
		if (proverca_na_uo && i == 6 && mod == 'r')
		{
			proverca_na_uo = 0;
		}
		printf("\n");
	}

	int nomer_key = -1;
	printf("Введите вариант: ");
	scanf("%i", &nomer_key);

	char* key = (char*)malloc((len_key + 1) * sizeof(char));
	if(nomer_key==6 && mod == 'r')
		key[0] = 168;
	else
		if(nomer_key > 6 && mod == 'r')
		key[0] = nomer_key-1 + otclonenie_table;
		else
			key[0] = nomer_key + otclonenie_table;

	for (int i = 1; i < len_key; i++)
	{
		if ((nomer_key + otklonenie_REZ[i - 1]) % length_alph == 6 && mod == 'r')
			key[i] = 168;
		else
			if ((nomer_key + otklonenie_REZ[i - 1]) % length_alph > 6 && mod == 'r')
				key[i] = (nomer_key + otklonenie_REZ[i - 1]) % length_alph -1 + otclonenie_table;
			else
				key[i] = (nomer_key + otklonenie_REZ[i - 1]) % length_alph + otclonenie_table;
	}
	key[len_key] = 0;

	char mode[3] = { "-d" };

	fseek(f, 0, SEEK_SET);

	crypt(f, mode, key);
}

int find_Key_lenth(FILE* file, char mod, char** kod)// mod={r,e}
{
	//узнаём размер файла
	fseek(file, 0, SEEK_END);
	int lenth_kod = ftell(file);
	fseek(file, 0, SEEK_SET);
	//вводим его
	*kod = (char*)calloc((lenth_kod + 2) * sizeof(char), (lenth_kod + 2) * sizeof(char));

	fread(*kod, sizeof(char), lenth_kod, file);
	int smeshenie = 1;
	int proverka = 1;
	double MAX_index_covpadeniu = 0;
	int lenth_alf = 0;
	int otclonenie = 0;
	double index_covpadeniu = 0;
	int otclonenie_table = 0;
	int* kol_vo_simbols = 0;
	//разбераемся с алфавитом
	if (mod == 'r') {//русский
		otclonenie_table = 192;
		lenth_alf = 33;
	}
	else if (mod == 'e') {//англ
		otclonenie_table = 65;
		lenth_alf = 26;
	}
	else
		return 0;//в случае если,что-то пошло не так с алфавитом
	//масив для кол-ва символов
	kol_vo_simbols = (int*)malloc(lenth_alf * sizeof(int));

	//считам спецсимволы
	for (int i = 0; i + otclonenie < lenth_kod;)
	{
		if (!((unsigned char)(*kod)[i + otclonenie] >= otclonenie_table && (unsigned char)(*kod)[i + otclonenie] <= otclonenie_table + lenth_alf || (unsigned char)(*kod)[i + otclonenie] == 168))
		{
			//магия
			otclonenie++;
		}
		else
		{
			(*kod)[i] = (*kod)[i + otclonenie];
			i++;
		}
	}
	(*kod)[lenth_kod - otclonenie] = 0;
	lenth_kod = lenth_kod - otclonenie;
	//магия
	for (int i = 2; i < lenth_kod; i++)
	{
		for (int f = 0; f < i && f < 100; f++)
		{
			for (int q = 0; q < lenth_alf; q++)
			{
				kol_vo_simbols[q] = 0;
			}
			index_covpadeniu = 0;
			for (int j = f; j < lenth_kod; j = j + i)
			{
				if ((unsigned char)(*kod)[j] >= otclonenie_table && (unsigned char)(*kod)[j] < otclonenie_table + lenth_alf)
					kol_vo_simbols[(unsigned char)(*kod)[j] - otclonenie_table]++;
				else if ((unsigned char)(*kod)[j] == 168)
					kol_vo_simbols[lenth_alf - 1]++;
			}
			for (int j = 0; j < lenth_alf; j++)
			{
				index_covpadeniu = index_covpadeniu + kol_vo_simbols[j] * (kol_vo_simbols[j] - 1);
			}
			index_covpadeniu = index_covpadeniu / ((float)((lenth_kod) / i) *((lenth_kod) / i - 1));
			if ((index_covpadeniu < right_wall_fo_E  && index_covpadeniu > left_wall_fo_E  && mod == 'e') || ((/*index_covpadeniu < right_wall_fo_R  &&*/index_covpadeniu > left_wall_fo_R) && mod == 'r'))
			{
				if (index_covpadeniu > MAX_index_covpadeniu && (i % smeshenie) != 0 || proverka)
				{
					smeshenie = i;
					if (proverka)proverka--;
					MAX_index_covpadeniu = index_covpadeniu;
				}
				// Коэффицент, который позволяет отсеять "побочную" длину ключа
				if ((i % smeshenie) == 0)
					MAX_index_covpadeniu *= 1.06;//это фокус
				//можно вставить красивый вывод
			}
		}
	}
	free(kol_vo_simbols);
	//выводим значение
	return smeshenie;
}
