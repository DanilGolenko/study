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
void writeDataToFile(const char* filename, OSRecord* records, int numRecords) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи.\n");
        return;
    }

    fwrite(records, sizeof(OSRecord), numRecords, file);
    fclose(file);
}

// Функция для чтения данных из файла
OSRecord* readDataFromFile(const char* filename, int* numRecords) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Ошибка при открытии файла для чтения.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    *numRecords = fileSize / sizeof(OSRecord);
    OSRecord* records = (OSRecord*)malloc(*numRecords * sizeof(OSRecord));
    if (records == NULL) {
        printf("Ошибка при выделении памяти для данных.\n");
        fclose(file);
        return NULL;
    }

    fread(records, sizeof(OSRecord), *numRecords, file);
    fclose(file);
    return records;
}

// Функция для сохранения результатов в файл
void saveDataToFile(const char* filename, OSRecord* records, int numRecords) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи.\n");
        return;
    }

    fwrite(records, sizeof(OSRecord), numRecords, file);
    fclose(file);
}


// Функция поиска записей по заданному полю и значению
void searchRecordsByField(OSRecord* records, int numRecords) {
    char searchField[MAX_STRING_LENGTH];
    char searchValue[MAX_STRING_LENGTH];

    printf("Введите поле для поиска: ");
    scanf("%s", searchField);
    printf("Введите значение для поля %s: ", searchField);
    scanf("%s", searchValue);

    int found = 0;
    for (int i = 0; i < numRecords; ++i) {
        if (strcmp(searchField, "Название") == 0 && strcmp(records[i].name, searchValue) == 0) {
            printf("Найдена запись с указанным названием:\n");
            printf("Название: %s\nГод выпуска: %d\nВерсия: %s\nЦена: %.2f\nРазработчик: %s\n",
                records[i].name, records[i].year, records[i].version, records[i].price, records[i].developer);
            found = 1;
        }
        
    }
    if (!found) {
        printf("Запись с указанным значением поля \"%s\" не найдена.\n", searchField);
    }
}

// Глобальная переменная для хранения поля, по которому производится сортировка
const char* g_sortByField;

// Функция сравнения для сортировки по указанному полю
int compareRecords(const void* a, const void* b) {
    const OSRecord* recordA = (const OSRecord*)a;
    const OSRecord* recordB = (const OSRecord*)b;

    if (strcmp(g_sortByField, "Название") == 0) {
        printf("Записи успешно отсортированы.");
        return strcmp(recordA->name, recordB->name);
    }
    else if (strcmp(g_sortByField, "Год выпуска") == 0) {
        printf("Записи успешно отсортированы.");
        return recordA->year - recordB->year;
    }
    else if (strcmp(g_sortByField, "Версия") == 0) {
        printf("Записи успешно отсортированы.");
        return strcmp(recordA->version, recordB->version);
    }
    else if (strcmp(g_sortByField, "Цена") == 0) {
        printf("Записи успешно отсортированы.");
        return (recordA->price > recordB->price) - (recordA->price < recordB->price);
    }
    else if (strcmp(g_sortByField, "Разработчик") == 0) {
        printf("Записи успешно отсортированы.");
        return strcmp(recordA->developer, recordB->developer);
    }
    else {
        printf("Ошибка: Неверное поле для сортировки.\n");
        return 0;
    }
}

// Функция для упорядочивания записей по указанному полю
void sortRecords(OSRecord* records, int numRecords, const char* sortByField) {
    g_sortByField = sortByField;
    qsort(records, numRecords, sizeof(OSRecord),
        (int (*)(const void*, const void*))compareRecords);
}

// Функция для добавления новой записи
void addRecord(OSRecord* records, int* numRecords) {
    if (*numRecords >= MAX_RECORDS) {
        printf("Ошибка: Достигнуто максимальное количество записей.\n");
        return;
    }
    printf("Введите название: ");
    if (scanf("%s", records[*numRecords].name) != 1) {
        printf("Ошибка при вводе названия.\n");
        return;
    }
    printf("Введите год выпуска: ");
    if (scanf("%d", &records[*numRecords].year) != 1 || records[*numRecords].year < 0) {
        printf("Ошибка: Год выпуска должен быть положительным целым числом.\n");
        return;
    }
    printf("Введите версию: ");
    if (scanf("%s", records[*numRecords].version) != 1) {
        printf("Ошибка при вводе версии.\n");
        return;
    }
    printf("Введите цену: ");
    if (scanf("%f", &records[*numRecords].price) != 1 || records[*numRecords].price < 0) {
        printf("Ошибка: Цена должна быть неотрицательным числом.\n");
        return;
    }
    printf("Введите разработчика: ");
    if (scanf("%s", records[*numRecords].developer) != 1) {
        printf("Ошибка при вводе разработчика.\n");
        return;
    }
    (*numRecords)++;
}

// Функция удаления записей
void deleteRecord(OSRecord* records, int* numRecords, int index) {
    if (index < 0 || index >= *numRecords) {
        printf("Не верно введён индекс.\n");
        return;
    }

    memmove(records + index, records + index + 1, (*numRecords - index - 1) * sizeof(OSRecord));

    (*numRecords)--;

    saveDataToFile("database.dat", records, *numRecords);
    printf("Запись удалена успешно.\n");
}

// Функция для изменения существующей записи
void modifyRecord(OSRecord* records, int numRecords, int index) {
    char choice[MAX_STRING_LENGTH];
    if (index < 0 || index >= numRecords) {
        printf("Ошибка: Недопустимый индекс записи.\n");
        return;
    }
    printf("Введите поле для изменения: ");
    scanf("%s", choice);
    if (strcmp(choice, "Название") == 0) {
        printf("Введите новое значение: ");
        scanf("%s", records[index].name);
        printf("Поле измененно.");
        return;
    }
    else if (strcmp(choice, "Год") == 0) {
        printf("Введите новое значение: ");
        scanf("%d", &records[index].year);
        printf("Поле измененно.");
        return;
    }
    else if (strcmp(choice, "Версия") == 0) {
        printf("Введите новое значение: ");
        scanf("%s", records[index].version);
        printf("Поле измененно.");
        return;
    }
    else if (strcmp(choice, "Цена") == 0) {
        printf("Введите новое значение: ");
        scanf("%f", &records[index].price);
        printf("Поле измененно.");
        return;
    }
    else if (strcmp(choice, "Разработчик") == 0) {
        printf("Введите новое значение: ");
        scanf("%s", records[index].developer);
        printf("Поле измененно.");
        return;
    }
    else {
        printf("Ошибка: Неверное поле для сортировки.\n");
        return;
    }
}


// Функция для вывода всех записей
void printAllRecords(OSRecord* records, int numRecords) {
    printf("Все записи в базе данных:\n");
    for (int i = 0; i < numRecords; ++i) {
        printf("%d. Название: %s, Год выпуска: %d, Версия: %s, Цена: %.2f, Разработчик: %s\n",
            i + 1, records[i].name, records[i].year, records[i].version, records[i].price, records[i].developer);
    }
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
        printf("Ошибка: Некорректный выбор.\n");
        return NULL;
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
            addRecord(records, &numRecords);
            break;
        case 2:
            printf("Введите индекс записи для удаления: ");
            scanf("%d", &choice);
            deleteRecord(records, &numRecords, choice - 1);
            break;
        case 3:
            printAllRecords(records, numRecords);
            break;
        case 4:
            printf("Введите индекс записи для изменения: ");
            scanf("%d", &choice);
            modifyRecord(records, numRecords, choice - 1);
            break;
        case 5:
            sortRecords(records, numRecords, selectSortField());
            break;
        case 6:
            searchRecordsByField(records, numRecords);
            break;
        case 7:
            saveDataToFile(filename, records, numRecords);
            printf("Данные сохранены в файл.\n");
            break;
        case 8:
            printf("Выход.\n");
            break;
        default:
            printf("Ошибка: Некорректный выбор.\n");
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