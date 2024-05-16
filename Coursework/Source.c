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
void writeDataToFile(const char* filename, OSRecord* records, int numRecords) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������.\n");
        return;
    }

    fwrite(records, sizeof(OSRecord), numRecords, file);
    fclose(file);
}

// ������� ��� ������ ������ �� �����
OSRecord* readDataFromFile(const char* filename, int* numRecords) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    *numRecords = fileSize / sizeof(OSRecord);
    OSRecord* records = (OSRecord*)malloc(*numRecords * sizeof(OSRecord));
    if (records == NULL) {
        printf("������ ��� ��������� ������ ��� ������.\n");
        fclose(file);
        return NULL;
    }

    fread(records, sizeof(OSRecord), *numRecords, file);
    fclose(file);
    return records;
}

// ������� ��� ���������� ����������� � ����
void saveDataToFile(const char* filename, OSRecord* records, int numRecords) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������.\n");
        return;
    }

    fwrite(records, sizeof(OSRecord), numRecords, file);
    fclose(file);
}


// ������� ������ ������� �� ��������� ���� � ��������
void searchRecordsByField(OSRecord* records, int numRecords) {
    char searchField[MAX_STRING_LENGTH];
    char searchValue[MAX_STRING_LENGTH];

    printf("������� ���� ��� ������: ");
    scanf("%s", searchField);
    printf("������� �������� ��� ���� %s: ", searchField);
    scanf("%s", searchValue);

    int found = 0;
    for (int i = 0; i < numRecords; ++i) {
        if (strcmp(searchField, "��������") == 0 && strcmp(records[i].name, searchValue) == 0) {
            printf("������� ������ � ��������� ���������:\n");
            printf("��������: %s\n��� �������: %d\n������: %s\n����: %.2f\n�����������: %s\n",
                records[i].name, records[i].year, records[i].version, records[i].price, records[i].developer);
            found = 1;
        }
        
    }
    if (!found) {
        printf("������ � ��������� ��������� ���� \"%s\" �� �������.\n", searchField);
    }
}

// ���������� ���������� ��� �������� ����, �� �������� ������������ ����������
const char* g_sortByField;

// ������� ��������� ��� ���������� �� ���������� ����
int compareRecords(const void* a, const void* b) {
    const OSRecord* recordA = (const OSRecord*)a;
    const OSRecord* recordB = (const OSRecord*)b;

    if (strcmp(g_sortByField, "��������") == 0) {
        printf("������ ������� �������������.");
        return strcmp(recordA->name, recordB->name);
    }
    else if (strcmp(g_sortByField, "��� �������") == 0) {
        printf("������ ������� �������������.");
        return recordA->year - recordB->year;
    }
    else if (strcmp(g_sortByField, "������") == 0) {
        printf("������ ������� �������������.");
        return strcmp(recordA->version, recordB->version);
    }
    else if (strcmp(g_sortByField, "����") == 0) {
        printf("������ ������� �������������.");
        return (recordA->price > recordB->price) - (recordA->price < recordB->price);
    }
    else if (strcmp(g_sortByField, "�����������") == 0) {
        printf("������ ������� �������������.");
        return strcmp(recordA->developer, recordB->developer);
    }
    else {
        printf("������: �������� ���� ��� ����������.\n");
        return 0;
    }
}

// ������� ��� �������������� ������� �� ���������� ����
void sortRecords(OSRecord* records, int numRecords, const char* sortByField) {
    g_sortByField = sortByField;
    qsort(records, numRecords, sizeof(OSRecord),
        (int (*)(const void*, const void*))compareRecords);
}

// ������� ��� ���������� ����� ������
void addRecord(OSRecord* records, int* numRecords) {
    if (*numRecords >= MAX_RECORDS) {
        printf("������: ���������� ������������ ���������� �������.\n");
        return;
    }
    printf("������� ��������: ");
    if (scanf("%s", records[*numRecords].name) != 1) {
        printf("������ ��� ����� ��������.\n");
        return;
    }
    printf("������� ��� �������: ");
    if (scanf("%d", &records[*numRecords].year) != 1 || records[*numRecords].year < 0) {
        printf("������: ��� ������� ������ ���� ������������� ����� ������.\n");
        return;
    }
    printf("������� ������: ");
    if (scanf("%s", records[*numRecords].version) != 1) {
        printf("������ ��� ����� ������.\n");
        return;
    }
    printf("������� ����: ");
    if (scanf("%f", &records[*numRecords].price) != 1 || records[*numRecords].price < 0) {
        printf("������: ���� ������ ���� ��������������� ������.\n");
        return;
    }
    printf("������� ������������: ");
    if (scanf("%s", records[*numRecords].developer) != 1) {
        printf("������ ��� ����� ������������.\n");
        return;
    }
    (*numRecords)++;
}

// ������� �������� �������
void deleteRecord(OSRecord* records, int* numRecords, int index) {
    if (index < 0 || index >= *numRecords) {
        printf("�� ����� ����� ������.\n");
        return;
    }

    memmove(records + index, records + index + 1, (*numRecords - index - 1) * sizeof(OSRecord));

    (*numRecords)--;

    saveDataToFile("database.dat", records, *numRecords);
    printf("������ ������� �������.\n");
}

// ������� ��� ��������� ������������ ������
void modifyRecord(OSRecord* records, int numRecords, int index) {
    char choice[MAX_STRING_LENGTH];
    if (index < 0 || index >= numRecords) {
        printf("������: ������������ ������ ������.\n");
        return;
    }
    printf("������� ���� ��� ���������: ");
    scanf("%s", choice);
    if (strcmp(choice, "��������") == 0) {
        printf("������� ����� ��������: ");
        scanf("%s", records[index].name);
        printf("���� ���������.");
        return;
    }
    else if (strcmp(choice, "���") == 0) {
        printf("������� ����� ��������: ");
        scanf("%d", &records[index].year);
        printf("���� ���������.");
        return;
    }
    else if (strcmp(choice, "������") == 0) {
        printf("������� ����� ��������: ");
        scanf("%s", records[index].version);
        printf("���� ���������.");
        return;
    }
    else if (strcmp(choice, "����") == 0) {
        printf("������� ����� ��������: ");
        scanf("%f", &records[index].price);
        printf("���� ���������.");
        return;
    }
    else if (strcmp(choice, "�����������") == 0) {
        printf("������� ����� ��������: ");
        scanf("%s", records[index].developer);
        printf("���� ���������.");
        return;
    }
    else {
        printf("������: �������� ���� ��� ����������.\n");
        return;
    }
}


// ������� ��� ������ ���� �������
void printAllRecords(OSRecord* records, int numRecords) {
    printf("��� ������ � ���� ������:\n");
    for (int i = 0; i < numRecords; ++i) {
        printf("%d. ��������: %s, ��� �������: %d, ������: %s, ����: %.2f, �����������: %s\n",
            i + 1, records[i].name, records[i].year, records[i].version, records[i].price, records[i].developer);
    }
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
        printf("������: ������������ �����.\n");
        return NULL;
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
            addRecord(records, &numRecords);
            break;
        case 2:
            printf("������� ������ ������ ��� ��������: ");
            scanf("%d", &choice);
            deleteRecord(records, &numRecords, choice - 1);
            break;
        case 3:
            printAllRecords(records, numRecords);
            break;
        case 4:
            printf("������� ������ ������ ��� ���������: ");
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
            printf("������ ��������� � ����.\n");
            break;
        case 8:
            printf("�����.\n");
            break;
        default:
            printf("������: ������������ �����.\n");
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