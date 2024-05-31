#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_STRING_LENGTH 100
#define MAX_RECORDS 100

// ��������� ��� �������� ������� � ���� ������
typedef struct {
    char name[MAX_STRING_LENGTH];
    int year;
    char version[MAX_STRING_LENGTH];
    float price;
    char developer[MAX_STRING_LENGTH];
} OSRecord;

// ������� ��� ������ ������ � ����
int writeDataToFile(const char* filename, OSRecord* records, int numRecords) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return 1;  // ������ ��� �������� ����� ��� ������
    }

    fwrite(records, sizeof(OSRecord), numRecords, file);
    fclose(file);
    return 0;  // �����
}

// ������� ��� ������ ������ �� �����
OSRecord* readDataFromFile(const char* filename, int* numRecords) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;  // ������ ��� �������� ����� ��� ������
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    *numRecords = fileSize / sizeof(OSRecord);
    OSRecord* records = (OSRecord*)malloc(*numRecords * sizeof(OSRecord));
    if (records == NULL) {
        fclose(file);
        return NULL;  // ������ ��� ��������� ������ ��� ������
    }

    fread(records, sizeof(OSRecord), *numRecords, file);
    fclose(file);
    return records;
}


// ������� ������ ������� �� ��������� ���� � ��������
OSRecord* searchRecordsByField(OSRecord* records, int numRecords, const char* searchField, const char* searchValue, int* foundCount) {
    OSRecord* foundRecords = (OSRecord*)malloc(numRecords * sizeof(OSRecord));
    *foundCount = 0;

    for (int i = 0; i < numRecords; ++i) {
        if ((strcmp(searchField, "��������") == 0 && strcmp(records[i].name, searchValue) == 0) ||
            (strcmp(searchField, "���") == 0 && records[i].year == atoi(searchValue)) ||
            (strcmp(searchField, "������") == 0 && strcmp(records[i].version, searchValue) == 0) ||
            (strcmp(searchField, "����") == 0 && records[i].price == atof(searchValue)) ||
            (strcmp(searchField, "�����������") == 0 && strcmp(records[i].developer, searchValue) == 0)) {
            foundRecords[*foundCount] = records[i];
            (*foundCount)++;
        }
    }

    return foundRecords;
}

// ���������� ���������� ��� �������� ����, �� �������� ������������ ����������
const char* g_sortByField;

// ������� ��������� ��� ���������� �� ���������� ����
int compareRecords(const void* a, const void* b) {
    const OSRecord* recordA = (const OSRecord*)a;
    const OSRecord* recordB = (const OSRecord*)b;

    if (strcmp(g_sortByField, "��������") == 0) {
        return strcmp(recordA->name, recordB->name);
    }
    else if (strcmp(g_sortByField, "��� �������") == 0) {
        return recordA->year - recordB->year;
    }
    else if (strcmp(g_sortByField, "������") == 0) {
        return strcmp(recordA->version, recordB->version);
    }
    else if (strcmp(g_sortByField, "����") == 0) {
        return (recordA->price > recordB->price) - (recordA->price < recordB->price);
    }
    else if (strcmp(g_sortByField, "�����������") == 0) {
        return strcmp(recordA->developer, recordB->developer);
    }
    else {
        return 0;
    }
}

// ������� ��� �������������� ������� �� ���������� ����
void sortRecords(OSRecord* records, int numRecords, const char* sortByField) {
    g_sortByField = sortByField;
    qsort(records, numRecords, sizeof(OSRecord), compareRecords);
}

// ������� ��� ���������� ����� ������
int addRecord(OSRecord* records, int* numRecords) {
    if (*numRecords >= MAX_RECORDS) {
        return 1;  // ���������� ������������ ���������� �������
    }
    printf("������� ��������: ");
    if (scanf("%s", records[*numRecords].name) != 1) {
        return 1;  // ������ ��� ����� ��������
    }
    printf("������� ��� �������: ");
    if (scanf("%d", &records[*numRecords].year) != 1 || records[*numRecords].year < 0) {
        return 1;  // ������ ��� ����� ���� �������
    }
    printf("������� ������: ");
    if (scanf("%s", records[*numRecords].version) != 1) {
        return 1;  // ������ ��� ����� ������
    }
    printf("������� ����: ");
    if (scanf("%f", &records[*numRecords].price) != 1 || records[*numRecords].price < 0) {
        return 1;  // ������ ��� ����� ����
    }
    printf("������� ������������: ");
    if (scanf("%s", records[*numRecords].developer) != 1) {
        return 1;  // ������ ��� ����� ������������
    }
    (*numRecords)++;
    return 0;  // �����
}

// ������� �������� �������
int deleteRecord(OSRecord* records, int* numRecords, int index) {
    if (index < 0 || index >= *numRecords) {
        return 1;  // ������������ ������
    }

    memmove(records + index, records + index + 1, (*numRecords - index - 1) * sizeof(OSRecord));
    (*numRecords)--;

    return writeDataToFile("database.dat", records, *numRecords);
}

// ������� ��� ��������� ������������ ������
int modifyRecord(OSRecord* records, int numRecords, int index, const char* field, const char* newValue) {
    if (index < 0 || index >= numRecords) {
        return 1;  // ������������ ������
    }

    if (strcmp(field, "��������") == 0) {
        strcpy(records[index].name, newValue);
    }
    else if (strcmp(field, "���") == 0) {
        records[index].year = atoi(newValue);
    }
    else if (strcmp(field, "������") == 0) {
        strcpy(records[index].version, newValue);
    }
    else if (strcmp(field, "����") == 0) {
        records[index].price = atof(newValue);
    }
    else if (strcmp(field, "�����������") == 0) {
        strcpy(records[index].developer, newValue);
    }
    else {
        return 1;  // ������������ ����
    }

    return 0;  // �����
}


// ������� ��� ������ ���� ��� ����������
const char* selectSortField() {
    int choice;
    printf("\n�������� ���� ��� ����������:\n");
    printf("1. ��������\n");
    printf("2. ��� �������\n");
    printf("3. ������\n");
    printf("4. ����\n");
    printf("5. �����������\n");
    printf("��� �����: ");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        return "��������";
    case 2:
        return "��� �������";
    case 3:
        return "������";
    case 4:
        return "����";
    case 5:
        return "�����������";
    default:
        return NULL;  // ������������ �����
    }
}

// ������� ����
void menu(OSRecord* records, int numRecords) {
    int choice;
    const char* filename = "database.dat";

    do {
        printf("\n����:\n");
        printf("1. �������� ������\n");
        printf("2. ������� ������\n");
        printf("3. ������� ��� ������\n");
        printf("4. �������� ������\n");
        printf("5. ����������� ������\n");
        printf("6. ����� ������\n");
        printf("7. ��������� ������ � ����\n");
        printf("8. �����\n");
        printf("�������� ��������: ");
        scanf("%d", &choice);


        switch (choice) {
        case 1:
            if (addRecord(records, &numRecords) != 0) {
                printf("������ ��� ���������� ������.\n");
            }
            break;
        case 2:
            printf("������� ������ ������ ��� ��������: ");
            scanf("%d", &choice);
            if (deleteRecord(records, &numRecords, choice - 1) != 0) {
                printf("������ ��� �������� ������.\n");
            }
            break;
        case 3:
            printf("��� ������ � ���� ������:\n");
            for (int i = 0; i < numRecords; ++i) {
                printf("%d. ��������: %s, ��� �������: %d, ������: %s, ����: %.2f, �����������: %s\n",
                    i + 1, records[i].name, records[i].year, records[i].version, records[i].price, records[i].developer);
            }
            break;
        case 4:
            printf("������� ������ ������ ��� ���������: ");
            scanf("%d", &choice);
            int index = choice - 1;
            char field[MAX_STRING_LENGTH], newValue[MAX_STRING_LENGTH];
            printf("������� ���� ��� ���������: ");
            scanf("%s", field);
            printf("������� ����� ��������: ");
            scanf("%s", newValue);
            if (modifyRecord(records, numRecords, index, field, newValue) != 0) {
                printf("������ ��� ��������� ������.\n");
            }
            break;
        case 5:
            const char* sortByField = selectSortField();
            if (sortByField != NULL) {
                sortRecords(records, numRecords, sortByField);
                printf("������ ������� �������������.\n");
            }
            else {
                printf("������������ ����� ���� ��� ����������.\n");
            }
            break;
        case 6: {
            char searchField[MAX_STRING_LENGTH], searchValue[MAX_STRING_LENGTH];
            int foundCount;
            printf("������� ���� ��� ������: ");
            scanf("%s", searchField);
            printf("������� �������� ��� ���� %s: ", searchField);
            scanf("%s", searchValue);
            OSRecord* foundRecords = searchRecordsByField(records, numRecords, searchField, searchValue, &foundCount);
            if (foundCount > 0) {
                printf("������� ������:\n");
                for (int i = 0; i < foundCount; ++i) {
                    printf("��������: %s, ��� �������: %d, ������: %s, ����: %.2f, �����������: %s\n",
                        foundRecords[i].name, foundRecords[i].year, foundRecords[i].version, foundRecords[i].price, foundRecords[i].developer);
                }
            }
            else {
                printf("������ � ��������� ��������� ���� \"%s\" �� �������.\n", searchField);
            }
            free(foundRecords);
            break;
        }
        case 7:
            if (writeDataToFile(filename, records, numRecords) == 0) {
                printf("������ ��������� � ����.\n");
            }
            else {
                printf("������ ��� ���������� ������ � ����.\n");
            }
            break;
        case 8:
            printf("�����.\n");
            break;
        default:
            printf("������������ �����.\n");
        }
    } while (choice != 8);
}

// �������� ������� ���������
int main() {
    // setlocale(LC_CTYPE, "RUS");
    system("chcp 1251");
    system("cls");
    OSRecord* records = NULL;
    int numRecords = 0;

    // �������� ������ �� �����
    const char* filename = "database.dat";
    records = readDataFromFile(filename, &numRecords);
    if (records == NULL) {
        printf("������ ��� �������� ������ �� �����.\n");
        return 1;
    }

    // �������������� � �������������
    menu(records, numRecords);
    // ������������ ������
    free(records);

    return 0;
}