#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<malloc.h>


//Размер английского алфавита
#define N 26
//Хочу сделать определение для кириллицы, т.е. через #define определить N = 33 и дальше от этого плясать

void crypt(FILE *, char*, char*);
void attack(FILE*);


#define left_wall_fo_E 0.06
#define left_wall_fo_R 0.05
#define right_wall_fo_E 0.07
#define right_wall_fo_R 0.07
int find_Key_lenth(FILE*, char mod, char** kod);


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

//Алгоритм шифровки и дешифровки
void crypt(FILE *source, char* mode, char* key)
{
	//to upper text
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
				fprintf(encrypt, "%c", m);
		}
		fclose(encrypt);
		printf("[+]Файл зашифрован!\n");
	}
	//decrypt
	if (strcmp(mode, "-d") == 0)
	{
		printf("%s\n", key);
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
				fprintf(decrypt, "%c", c);
		}
		fclose(decrypt);
		printf("[+]Файл расшифрован!\n");
	}
}

// Нужно реализовать взлом шифра
void attack(FILE* f)
{
	printf("дешифруем без ключа\n");
	unsigned char mod = 0;
	int lenht_alf = 0;
	int otclonenie_table = 0;

	//разбераемся с алфавитом(только заглавные)
	while (!mod && !feof(f))
	{
		if ((mod = (unsigned char)fgetc(f)) <= 90 && mod >= 65)mod = 'e';
		else 	if ((mod) <= 192 + 33 && mod >= 192)mod = 'r';
		else mod = 0;
	}
	fseek(f, 0, SEEK_SET);
	if (mod == 'r')
	{
		lenht_alf = 33;
		otclonenie_table = 192;

	}
	if (mod == 'e')
	{
		lenht_alf = 26;
		otclonenie_table = 65;
	}
	if (mod == 0)
	{
		printf("Файл пуст\n");
		return;
	}

	//найдем длину ключа
	int len_key = 0;
	char* kod = 0;
	len_key = find_Key_lenth(f, mod, &kod);
	int lenth_kod = strlen(kod);



	//масив для кол-ва символов
	int**kol_vo_simbols = 0;
	kol_vo_simbols = (int**)malloc(sizeof(int*)*lenht_alf);
	for (int i = 0; i < lenht_alf; i++)
	{
		kol_vo_simbols[i] = (int*)malloc(len_key * sizeof(int));
		for (int j = 0; j < len_key; j++)
		{
			kol_vo_simbols[i][j] = 0;
		}
	}
	//масив кол-ва знаков на каждую букву ключа
	int*kol_vo = 0;
	kol_vo = (int*)calloc(sizeof(int)*len_key, sizeof(int)*len_key);


	for (int f = 0; f < len_key; f++)
	{
		for (int j = f; j < lenth_kod; j = j + len_key)
		{
			if ((unsigned char)(kod)[j] >= otclonenie_table && (unsigned char)(kod)[j] < otclonenie_table + lenht_alf)
			{
				kol_vo_simbols[(unsigned char)(kod)[j] - otclonenie_table][f]++;
				kol_vo[f]++;
			}
			else if ((unsigned char)(kod)[j] == 168)
			{
				kol_vo_simbols[lenht_alf - 1][f]++;
				kol_vo[f]++;
			}
		}
	}
	//for (int f = 0; f < len_key; f++)
	//{
	//	for (int j = 0; j < lenht_alf; j++)
	//	{
	//		printf("%i ", kol_vo_simbols[j][f]);
	//	}
	//	printf("%i ", kol_vo[f]);
	//	printf("\n ");
	//}
		//масив для отклонений
	double**otklonenie = 0;
	otklonenie = (double**)malloc(sizeof(double*)*lenht_alf);
	for (int i = 0; i < lenht_alf; i++)
	{
		otklonenie[i] = (double*)malloc((len_key - 1) * sizeof(double));
		for (int j = 0; j < len_key - 1; j++)
		{
			otklonenie[i][j] = 0;
		}
	}
	//масив для peзультирующих отклонений
	int*otklonenie_REZ = 0;
	otklonenie_REZ = (int*)malloc(sizeof(int)*(len_key - 1));
	double MAX_otklonenie = 0;



	for (int j = 1; j < len_key; j++)
	{
		MAX_otklonenie = 0;
		for (int i = 0; i < lenht_alf; i++)
		{

			for (int q = 0; q < lenht_alf; q++)
			{
				otklonenie[i][j - 1] = otklonenie[i][j - 1] + (kol_vo_simbols[q][0] * kol_vo_simbols[(q + i) % lenht_alf][j]);
			}
			otklonenie[i][j - 1] = otklonenie[i][j - 1] / (kol_vo[0] * kol_vo[j]);
			//printf("%f ", otklonenie[i][j-1]);
			//if ((otklonenie[i][j - 1] < right_wall_fo_E  && otklonenie[i][j - 1] > left_wall_fo_E  && mod == 'e') || ((otklonenie[i][j - 1] < right_wall_fo_R  && otklonenie[i][j - 1] > left_wall_fo_R) && mod == 'r'))
			{
				if (otklonenie[i][j - 1] > MAX_otklonenie)
				{
					otklonenie_REZ[j - 1] = i;
					MAX_otklonenie = otklonenie[i][j - 1];
				}
			}
		}
		//printf("\n ");
	}
	//printf("%i ", otklonenie_REZ[0]);
	//printf("%i ", otklonenie_REZ[1]);
	//printf("%i ", otklonenie_REZ[2]);

	printf("Выбири возможный ключ\n");
	for (int i = 0; i < lenht_alf; i++)
	{
		printf("%i - %c", i, i + otclonenie_table);
		for (int j = 0; j < len_key - 1; j++)
		{
			printf("%c", (i + otklonenie_REZ[j]) % lenht_alf + otclonenie_table);
		}
		printf("\n");
	}
	int nomer_key = -1;

	scanf("%i", &nomer_key);

	char* key = (char*)malloc((len_key + 1) * sizeof(char));
	key[0] = nomer_key + otclonenie_table;
	for (int i = 1; i < len_key; i++)
	{
		key[i] = (nomer_key + otklonenie_REZ[i - 1]) % lenht_alf + otclonenie_table;
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
	for (int i = 0; i < lenth_kod; i++)
	{
		if (!((unsigned char)(*kod)[i] >= otclonenie_table && (unsigned char)(*kod)[i] <= otclonenie_table + lenth_alf || (unsigned char)(*kod)[i] == 168))otclonenie++;
	}

	//магия
	for (int i = 2; i < lenth_kod; i++)
	{
		for (int f = 0; f < i; f++)
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
				index_covpadeniu = index_covpadeniu + (kol_vo_simbols[j] * (kol_vo_simbols[j] - 1)) / ((float)((lenth_kod - otclonenie) / i) *((lenth_kod - otclonenie) / i - 1));

			}
			if ((index_covpadeniu < right_wall_fo_E  && index_covpadeniu > left_wall_fo_E  && mod == 'e') || ((index_covpadeniu < right_wall_fo_R  &&index_covpadeniu > left_wall_fo_R) && mod == 'r'))
			{
				if (index_covpadeniu > MAX_index_covpadeniu && (i % smeshenie) != 0 || proverka)
				{
					smeshenie = i;
					if (proverka)proverka--;
					MAX_index_covpadeniu = index_covpadeniu;
				}
				if ((i % smeshenie) == 0)MAX_index_covpadeniu *= 1.06;//это фокус
				//можно вставить красивый вывод
			}
		}
	}
	free(kol_vo_simbols);
	//выводим значение
	return smeshenie;
}

