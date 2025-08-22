#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stu {
    int id;
    char name[20];
} s;

FILE *fp1, *fp2;
int idno, op, found = 0;
char sname[20], ch;

void newstu();
void enquiry();
void update();
void delete_stu();
void report();

void line() {
    puts("---------------------------------------------------------------");
}

int main() {
    while (1) {
        line();
        puts("\t\t STUDENT ENROLLMENT SYSTEM");
        line();
        puts("\t\t M E N U");
        line();
        puts("\t 1. New student");
        puts("\t 2. Enquiry");
        puts("\t 3. Report");
        puts("\t 4. Update");
        puts("\t 5. Delete");
        puts("\t 6. Exit");
        line();
        printf("\t Enter your option: ");
        scanf("%d", &op);

        switch (op) {
            case 1: newstu(); break;
            case 2: enquiry(); break;
            case 3: report(); break;
            case 4: update(); break;
            case 5: delete_stu(); break;
            case 6: exit(0);
            default: puts("Invalid Option Selected");
        }
    }
    return 0;
}

void newstu() {
    fp1 = fopen("6pmc.dat", "ab");
    if (!fp1) {
        puts("Error opening file!");
        return;
    }

    while (1) {
        printf("Enter student id: ");
        scanf("%d", &s.id);
        printf("Enter student name: ");
        scanf("%s", s.name);

        fwrite(&s, sizeof(s), 1, fp1);
        printf("%s record created successfully\n", s.name);

        printf("Do you want to continue [y/n]? ");
        scanf(" %c", &ch);  // notice space before %c
        if (ch == 'n' || ch == 'N')
            break;
    }
    fclose(fp1);
}

void enquiry() {
    found = 0;
    fp1 = fopen("6pmc.dat", "rb");
    if (!fp1) {
        puts("File not found");
        return;
    }

    line();
    puts("\t 1. Id wise");
    puts("\t 2. Name wise");
    puts("\t 3. Back to menu");
    line();
    printf("Enter your option: ");
    scanf("%d", &op);

    if (op == 1) {
        printf("Enter student id to search: ");
        scanf("%d", &idno);

        while (fread(&s, sizeof(s), 1, fp1)) {
            if (idno == s.id) {
                found = 1;
                puts("Id\tName");
                line();
                printf("%d\t%s\n", s.id, s.name);
                break;
            }
        }
    } else if (op == 2) {
        printf("Enter student name to search: ");
        scanf("%s", sname);

        while (fread(&s, sizeof(s), 1, fp1)) {
            if (strcasecmp(sname, s.name) == 0) {
                found = 1;
                puts("Id\tName");
                line();
                printf("%d\t%s\n", s.id, s.name);
                break;
            }
        }
    }

    if (!found)
        puts("Student not found");

    fclose(fp1);
}

void report() {
    fp1 = fopen("6pmc.dat", "rb");
    if (!fp1) {
        puts("File not found");
        return;
    }

    line();
    puts("Id\tName");
    line();

    while (fread(&s, sizeof(s), 1, fp1)) {
        printf("%d\t%s\n", s.id, s.name);
    }

    fclose(fp1);
}

void update() {
    found = 0;
    fp1 = fopen("6pmc.dat", "rb+");
    if (!fp1) {
        puts("File not found");
        return;
    }

    printf("Enter student id to update: ");
    scanf("%d", &idno);

    while (fread(&s, sizeof(s), 1, fp1)) {
        if (idno == s.id) {
            found = 1;
            puts("Id\tName");
            line();
            printf("%d\t%s\n", s.id, s.name);

            printf("Enter new id and name: ");
            scanf("%d %s", &s.id, s.name);

            fseek(fp1, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp1);
            printf("%s record updated successfully\n", s.name);
            break;
        }
    }

    if (!found)
        puts("Student not found");

    fclose(fp1);
}

void delete_stu() {
    found = 0;
    fp1 = fopen("6pmc.dat", "rb");
    fp2 = fopen("temp.dat", "wb");
    if (!fp1 || !fp2) {
        puts("File error!");
        return;
    }

    printf("Enter student id to delete: ");
    scanf("%d", &idno);

    while (fread(&s, sizeof(s), 1, fp1)) {
        if (idno == s.id) {
            puts("Id\tName");
            line();
            printf("%d\t%s\n", s.id, s.name);
            found = 1;
        } else {
            fwrite(&s, sizeof(s), 1, fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);

    if (found) {
        printf("Are you sure to delete? [y/n]: ");
        scanf(" %c", &ch);
        if (ch == 'y' || ch == 'Y') {
            remove("6pmc.dat");
            rename("temp.dat", "6pmc.dat");
            puts("One record deleted successfully");
        } else {
            puts("Operation cancelled by the user");
        }
    } else {
        puts("Student not found");
    }
}
