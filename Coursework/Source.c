#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_STRING_LENGTH 100
#define MAX_RECORDS 100

// Структура для описания записей в базе данных
typedef struct {
    char name[MAX_STRING_LENGTH];
    int year;
    char version[MAX_STRING_LENGTH];
    float price;
    char developer[MAX_STRING_LENGTH];
} OSRecord;

// Функция для записи данных в файл
int writeDataToFile(const char* filename, OSRecord* records, int numRecords) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return 1;  // Ошибка при открытии файла для записи
    }

    fwrite(records, sizeof(OSRecord), numRecords, file);
    fclose(file);
    return 0;  // Успех
}

// Функция для чтения данных из файла
OSRecord* readDataFromFile(const char* filename, int* numRecords) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;  // Ошибка при открытии файла для чтения
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    *numRecords = fileSize / sizeof(OSRecord);
    OSRecord* records = (OSRecord*)malloc(*numRecords * sizeof(OSRecord));
    if (records == NULL) {
        fclose(file);
        return NULL;  // Ошибка при выделении памяти для данных
    }

    fread(records, sizeof(OSRecord), *numRecords, file);
    fclose(file);
    return records;
}


// Функция поиска записей по заданному полю и значению
OSRecord* searchRecordsByField(OSRecord* records, int numRecords, const char* searchField, const char* searchValue, int* foundCount) {
    OSRecord* foundRecords = (OSRecord*)malloc(numRecords * sizeof(OSRecord));
    *foundCount = 0;

    for (int i = 0; i < numRecords; ++i) {
        if ((strcmp(searchField, "Название") == 0 && strcmp(records[i].name, searchValue) == 0) ||
            (strcmp(searchField, "Год") == 0 && records[i].year == atoi(searchValue)) ||
            (strcmp(searchField, "Версия") == 0 && strcmp(records[i].version, searchValue) == 0) ||
            (strcmp(searchField, "Цена") == 0 && records[i].price == atof(searchValue)) ||
            (strcmp(searchField, "Разработчик") == 0 && strcmp(records[i].developer, searchValue) == 0)) {
            foundRecords[*foundCount] = records[i];
            (*foundCount)++;
        }
    }

    return foundRecords;
}

// Глобальная переменная для хранения поля, по которому производится сортировка
const char* g_sortByField;

// Функция сравнения для сортировки по указанному полю
int compareRecords(const void* a, const void* b) {
    const OSRecord* recordA = (const OSRecord*)a;
    const OSRecord* recordB = (const OSRecord*)b;

    if (strcmp(g_sortByField, "Название") == 0) {
        return strcmp(recordA->name, recordB->name);
    }
    else if (strcmp(g_sortByField, "Год выпуска") == 0) {
        return recordA->year - recordB->year;
    }
    else if (strcmp(g_sortByField, "Версия") == 0) {
        return strcmp(recordA->version, recordB->version);
    }
    else if (strcmp(g_sortByField, "Цена") == 0) {
        return (recordA->price > recordB->price) - (recordA->price < recordB->price);
    }
    else if (strcmp(g_sortByField, "Разработчик") == 0) {
        return strcmp(recordA->developer, recordB->developer);
    }
    else {
        return 0;
    }
}

// Функция для упорядочивания записей по указанному полю
void sortRecords(OSRecord* records, int numRecords, const char* sortByField) {
    g_sortByField = sortByField;
    qsort(records, numRecords, sizeof(OSRecord), compareRecords);
}

// Функция для добавления новой записи
int addRecord(OSRecord* records, int* numRecords) {
    if (*numRecords >= MAX_RECORDS) {
        return 1;  // Достигнуто максимальное количество записей
    }
    printf("Введите название: ");
    if (scanf("%s", records[*numRecords].name) != 1) {
        return 1;  // Ошибка при вводе названия
    }
    printf("Введите год выпуска: ");
    if (scanf("%d", &records[*numRecords].year) != 1 || records[*numRecords].year < 0) {
        return 1;  // Ошибка при вводе года выпуска
    }
    printf("Введите версию: ");
    if (scanf("%s", records[*numRecords].version) != 1) {
        return 1;  // Ошибка при вводе версии
    }
    printf("Введите цену: ");
    if (scanf("%f", &records[*numRecords].price) != 1 || records[*numRecords].price < 0) {
        return 1;  // Ошибка при вводе цены
    }
    printf("Введите разработчика: ");
    if (scanf("%s", records[*numRecords].developer) != 1) {
        return 1;  // Ошибка при вводе разработчика
    }
    (*numRecords)++;
    return 0;  // Успех
}

// Функция удаления записей
int deleteRecord(OSRecord* records, int* numRecords, int index) {
    if (index < 0 || index >= *numRecords) {
        return 1;  // Недопустимый индекс
    }

    memmove(records + index, records + index + 1, (*numRecords - index - 1) * sizeof(OSRecord));
    (*numRecords)--;

    return writeDataToFile("database.dat", records, *numRecords);
}

// Функция для изменения существующей записи
int modifyRecord(OSRecord* records, int numRecords, int index, const char* field, const char* newValue) {
    if (index < 0 || index >= numRecords) {
        return 1;  // Недопустимый индекс
    }

    if (strcmp(field, "Название") == 0) {
        strcpy(records[index].name, newValue);
    }
    else if (strcmp(field, "Год") == 0) {
        records[index].year = atoi(newValue);
    }
    else if (strcmp(field, "Версия") == 0) {
        strcpy(records[index].version, newValue);
    }
    else if (strcmp(field, "Цена") == 0) {
        records[index].price = atof(newValue);
    }
    else if (strcmp(field, "Разработчик") == 0) {
        strcpy(records[index].developer, newValue);
    }
    else {
        return 1;  // Недопустимое поле
    }

    return 0;  // Успех
}


// Функция для выбора поля для сортировки
const char* selectSortField() {
    int choice;
    printf("\nВыберите поле для сортировки:\n");
    printf("1. Название\n");
    printf("2. Год выпуска\n");
    printf("3. Версия\n");
    printf("4. Цена\n");
    printf("5. Разработчик\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        return "Название";
    case 2:
        return "Год выпуска";
    case 3:
        return "Версия";
    case 4:
        return "Цена";
    case 5:
        return "Разработчик";
    default:
        return NULL;  // Некорректный выбор
    }
}

// Функция меню
void menu(OSRecord* records, int numRecords) {
    int choice;
    const char* filename = "database.dat";

    do {
        printf("\nМеню:\n");
        printf("1. Добавить запись\n");
        printf("2. Удалить запись\n");
        printf("3. Вывести все записи\n");
        printf("4. Изменить запись\n");
        printf("5. Сортировать записи\n");
        printf("6. Поиск записи\n");
        printf("7. Сохранить данные в файл\n");
        printf("8. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);


        switch (choice) {
        case 1:
            if (addRecord(records, &numRecords) != 0) {
                printf("Ошибка при добавлении записи.\n");
            }
            break;
        case 2:
            printf("Введите индекс записи для удаления: ");
            scanf("%d", &choice);
            if (deleteRecord(records, &numRecords, choice - 1) != 0) {
                printf("Ошибка при удалении записи.\n");
            }
            break;
        case 3:
            printf("Все записи в базе данных:\n");
            for (int i = 0; i < numRecords; ++i) {
                printf("%d. Название: %s, Год выпуска: %d, Версия: %s, Цена: %.2f, Разработчик: %s\n",
                    i + 1, records[i].name, records[i].year, records[i].version, records[i].price, records[i].developer);
            }
            break;
        case 4:
            printf("Введите индекс записи для изменения: ");
            scanf("%d", &choice);
            int index = choice - 1;
            char field[MAX_STRING_LENGTH], newValue[MAX_STRING_LENGTH];
            printf("Введите поле для изменения: ");
            scanf("%s", field);
            printf("Введите новое значение: ");
            scanf("%s", newValue);
            if (modifyRecord(records, numRecords, index, field, newValue) != 0) {
                printf("Ошибка при изменении записи.\n");
            }
            break;
        case 5:
            const char* sortByField = selectSortField();
            if (sortByField != NULL) {
                sortRecords(records, numRecords, sortByField);
                printf("Записи успешно отсортированы.\n");
            }
            else {
                printf("Некорректный выбор поля для сортировки.\n");
            }
            break;
        case 6: {
            char searchField[MAX_STRING_LENGTH], searchValue[MAX_STRING_LENGTH];
            int foundCount;
            printf("Введите поле для поиска: ");
            scanf("%s", searchField);
            printf("Введите значение для поля %s: ", searchField);
            scanf("%s", searchValue);
            OSRecord* foundRecords = searchRecordsByField(records, numRecords, searchField, searchValue, &foundCount);
            if (foundCount > 0) {
                printf("Найдены записи:\n");
                for (int i = 0; i < foundCount; ++i) {
                    printf("Название: %s, Год выпуска: %d, Версия: %s, Цена: %.2f, Разработчик: %s\n",
                        foundRecords[i].name, foundRecords[i].year, foundRecords[i].version, foundRecords[i].price, foundRecords[i].developer);
                }
            }
            else {
                printf("Запись с указанным значением поля \"%s\" не найдена.\n", searchField);
            }
            free(foundRecords);
            break;
        }
        case 7:
            if (writeDataToFile(filename, records, numRecords) == 0) {
                printf("Данные сохранены в файл.\n");
            }
            else {
                printf("Ошибка при сохранении данных в файл.\n");
            }
            break;
        case 8:
            printf("Выход.\n");
            break;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (choice != 8);
}

// Основная функция программы
int main() {
    // setlocale(LC_CTYPE, "RUS");
    system("chcp 1251");
    system("cls");
    OSRecord* records = NULL;
    int numRecords = 0;

    // Загрузка данных из файла
    const char* filename = "database.dat";
    records = readDataFromFile(filename, &numRecords);
    if (records == NULL) {
        printf("Ошибка при загрузке данных из файла.\n");
        return 1;
    }

    // Взаимодействие с пользователем
    menu(records, numRecords);
    // Освобождение памяти
    free(records);

    return 0;
}