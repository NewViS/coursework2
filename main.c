#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define maxlen 128
#define int_len 11
#if (defined(linux) || defined(MACH))
#define CLEAR system("clear")
#else
#define CLEAR system("cls")
#endif /* OS_TYPE */

/* define structure */
/* subelement */
struct note2{
    char *trans;

    struct note2 *next;
};

/* library element */
struct note1{
    char *car, *transmission;
    int year, power, dim[3];
    float eng, cons;

    struct note1 *next;
};

/* define structured type */
typedef struct note1 note;
typedef struct note2 note2;

/* read line from fie */
char *read_line(FILE* is, int max_len){
    char c;
    int i;
    char *s;

    s = malloc(sizeof(char)*max_len);
    if(s!=NULL) {
        i = 0;
        fflush(stdin);
        while (((c = fgetc(is)) != '\n') && (i < max_len - 1) && (c != EOF)) {
            s[i] = c;
            i++;
        }
        s[i] = '\0';
    }else puts("M Allocation error in read_line");

    return s;
}

/* read int from file */
int read_int(FILE* is){
    char *s;
    int res;

    res = 0;
    s = read_line(is, int_len);
    if(s != NULL) {
        res = atoi(s);
        free(s);
    }
    return res;
}

/* string splitter */
char **split(char *str, int *len, char sep){
    char **arr;
    int i,m,*count,j,k;

    for(i=0,m=1;i<*len;i++){
        if(str[i]==sep) m++;
    }

    count = malloc(m*sizeof(int));
    arr=(char**)malloc(m*sizeof(char*));

    if(count != NULL && arr != NULL) {
        for(i=0;i<m;i++)    count[i]=0;

        for (i = 0, j = 1, k=0; i < *len; i++, j++) {
            if (str[i] == sep){
                count[k]=j;
                j = 0;
                k++;
            }
            else if(i==*len-1){
                count[k]=j+1;
            }
        }

        for (i=0, k=0; i < m; i++,k++) {
            arr[i]=malloc(count[i]*sizeof(char));
            if(arr[i]!=NULL){
                for(j=0;j<count[i]-1;j++,k++){
                    arr[i][j]=str[k];
                }
                arr[i][j]='\0';
            }else{
                i=m;
                puts("No memory allocated!");
            }
        }
        *len = m;
    }else puts("No memory allocated!");
    free(count);
    /*printf("%s %s %s %s %s %s %s %s %s\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8]);*/
    return arr;
}

/* print help menu to console */
void print_menu(){
    puts("Choose operation:\n0 - help\n1 - add card(s)\n2 - edit card\n"
         "3 - delete card\n4 - print list\n"
         "5 - search by param\n6 - sort by param\n7 - export file\n8 - exit");
}

/* counting lines in file */
int count_lines(FILE *file){
    int num;
    num=0;
    while (!feof(file))
    {
        if (getc(file) == '\n')
            num++;
    }
    num++;
    rewind(file);
    return num;
}

/* clear array of string */
void clearStringArray(char **str, int n)
{
    int i;

    for(i=0;i<n;i++)
    {
        free(str[i]);

    }
    free(str);


}

/* check if char already in list */
note2* in_list(note2 *head, char *chr){
    note2 *tmp;
    tmp = head;
    if(tmp!=NULL){
        while(tmp->next != NULL){
            if(!strcmp(chr, tmp->trans)) return tmp;
            tmp = tmp->next;
        }
        if(!strcmp(chr, tmp->trans)) return tmp;
    }
    return NULL;
}

/* add subelement */
void push2(note2 **head, char *chr){
    note2 *tmp;
    if(!in_list(*head, chr)){
        tmp = (note2*) malloc(sizeof(note2));
        tmp->trans = malloc((strlen(chr) + 1) * sizeof(char));
        if(tmp !=NULL && tmp->trans != NULL) {
            strcpy(tmp->trans, chr);
            tmp->next = (*head);
            (*head) = tmp;
        }
        else puts("No memory allocated in push2!");
    }
}

/* add element to list start */
void push(note **head, char **s2, note2 **hd, int kol_s2) {
    note *tmp;
    note2 *tran;
    tmp = (note*) malloc(sizeof(note));
    if (tmp != NULL && kol_s2 == 9) {
        push2(hd, s2[4]);
        tran = in_list(*hd, s2[4]);
        tmp->car = malloc((strlen(s2[0]) + 1) * sizeof(char));
        if (tmp->car != NULL) {
            strcpy(tmp->car, s2[0]);
            tmp->transmission = tran->trans;
            tmp->dim[0] = atoi(s2[6]);
            tmp->dim[1] = atoi(s2[7]);
            tmp->dim[2] = atoi(s2[8]);
            tmp->year = atoi(s2[1]);
            tmp->power = atoi(s2[2]);
            tmp->eng = atof(s2[3]);
            tmp->cons = atof(s2[5]);

            tmp->next = (*head);
            (*head) = tmp;
        } else {
            puts("Out if memory adding element!");
        }
    }else puts("Push element error!");
}

/* print structure header */
void print_header(){
    printf("  ID |%25s | %4s |POWER| ENG |%10s|%5s | %10s\n","NAME","YEAR", "TRANSMISSION", "L/100", "DIMENSIONS");
    printf("+----+--------------------------+------+-----+-----+------------+------+---------------+\n");
}

/* print structure */
void struct_out(const note *nt, int num){
    printf("%4d |%25s | %4d | %3d | %0.1f | %10s |%5.1f | %d-%d-%d\n",
           num, nt->car, nt->year, nt->power, nt->eng, nt->transmission, nt->cons, nt->dim[0], nt->dim[1], nt->dim[2]);
}

/* number of elements in lib */
int liblen(note *head){
    int k;
    for(k=0; head; k++) head = head -> next;
    return k;
}

/* print list */
void printLinkedList(const note *head) {
    int i;
    i = 1;
    print_header();
    while (head) {
        struct_out(head, i);
        head = head->next;
        i++;
    }
    printf("\n");
}

/* get element */
note* getNth(note* head, int n) {
    int counter;
    counter = 0;
    while (counter < n && head != NULL) {
        head = head->next;
        counter++;
    }
    return head;
}

/* delete element */
void pop(note **head) {
    note* prev = NULL;
    if (head == NULL) {
        exit(-1);
    }
    prev = (*head);
    (*head) = (*head)->next;
    free(prev);
}

/* delete element by index */
void deleteNth(note **head, int n) {
    note *prev, *elm;
    if (n == 0) {
        pop(head);
    } else {
        prev = getNth(*head, n-1);
        elm  = prev->next;

        prev->next = elm->next;
        free(elm);
    }
}

/* edit element */
void lib_edit(note *head, int id, int cat, char *val, note2 **hd){
    note2 *tran;
    head = getNth(head, id-1);
    switch (cat) {
        /* car name */
        case 1:{
            head->car = malloc((strlen(val)+1)*sizeof(char));
            if(head->car!=NULL) strcpy(head->car, val);
            else puts("No memory allocated for car name");
            break;
        }
        /* prod year */
        case 2:{
            head->year = atoi(val);
            break;
        }
        /* power */
        case 3:{
            head->power = atoi(val);
            break;
        }
        /* value */
        case 4:{
            head->eng = atof(val);
            break;
        }
        /* trans */
        case 5:{
            push2(hd, val);
            tran = in_list(*hd, val);
            head->transmission = tran->trans;
            break;
        }
        /* f cons */
        case 6:{
            head->cons=atof(val);
            break;
        }
        /* DIM */
        case 7:{
            head->dim[0] = atoi(val);
            break;
        }
        case 8:{
            head->dim[1] = atoi(val);
            break;
        }
        case 9:{
            head->dim[2] = atoi(val);
            break;
        }
        default:{
            puts("Wrong field");
        }
    }
}

/* delete linked list */
void deleteList(note **head) {
    note* prev;
    prev = NULL;
    while ((*head)->next) {
        prev = (*head);
        (*head) = (*head)->next;
        if(prev->car!=NULL) free(prev->car);
        if(prev != NULL)free(prev);
    }
    if(*head!=NULL)free(*head);
}

/* delete linked list */
void deleteList2(note2 **head) {
    note2* prev;
    prev = NULL;
    while ((*head)->next) {
        prev = (*head);
        (*head) = (*head)->next;
        if(prev->trans!=NULL) free(prev->trans);
        if(prev != NULL)free(prev);
    }
    if(*head!=NULL)free(*head);
}

/* is string consist substring */
int is_consist(char* string, char* substr){
    char **substring;
    int k, j, i, num_substr;
    num_substr = strlen(substr);
    substring = split(substr, &num_substr, ' ');
    for(i=0; i<num_substr; i++) {
        k=0;
        for (j = 0; string[j]; j++) {
            if (tolower(string[j]) == tolower(substring[i][k])) {
                if(!k) {    /* is it word start */
                    if (!j) {
                        k++;
                    } else if (!isalnum(string[j - 1])) {
                        k++;
                    }
                } else{
                    k++;
                }
                if (!substring[i][k]){  /*is it word end*/
                    if(j==strlen(string)-1){
                        return 1;
                    }else if(!isalnum(string[j+1])){
                        return 1;
                    }
                }
            }else k=0;
        }
    }
    return 0;
}

/* find element by field */
void find(note *nt, int field, char *val, char *range){
    int param, k;
    param = 0x100 >> (field-1);
    k=0;

    while (nt){
        if((is_consist(nt->car, val)*(0x100)
        + (abs(nt->year-atoi(val))<=atoi(range))*(0x80)
        + (abs(nt->power-atoi(val))<=atoi(range))*(0x40)
        + (fabs(nt->eng-atof(val))<=atof(range))*(0x20)
        + (is_consist(nt->transmission, val))*(0x10)
        + (fabs(nt->cons-atof(val))<=atof(range))*(0x8)
        + (abs(nt->dim[0]-atoi(val))<=atoi(range))*(0x4)
        + (abs(nt->dim[1]-atoi(val))<=atoi(range))*(0x2)
        + (abs(nt->dim[2]-atoi(val))<=atoi(range)))&param){
            if(!k) print_header();
            k++;
            struct_out(nt,k);
        }
        nt = nt->next;
    }
    if(!k) puts("No such elements");
}

/* sort element by field */
/* merge lists */
void merge(note *a, note *b, note **c, int field) {
    note tmp;
    int param, bool;
    param = 0x100 >> (field-1);
    *c = NULL;
    if (a == NULL) {
        *c = b;
        return;
    }
    if (b == NULL) {
        *c = a;
        return;
    }

    bool = ((strcmp(a->car, b->car)<0)*(0x100) + (a->year > b->year)*(0x80)
     + (a->power > b->power)*(0x40)
     + (a->eng > b->eng)*(0x20)
     + (strcmp(a->transmission, b->transmission)<0)*(0x10)
     + (a->cons > b->cons)*(0x8)
     + (a->dim[0] > b->dim[0])*(0x4)
     + (a->dim[1] > b->dim[1])*(0x2)
     + (a->dim[2] > b->dim[2]))&param;

    if (bool) {
        *c = a;
        a = a->next;
    } else {
        *c = b;
        b = b->next;
    }
    tmp.next = *c;
    while (a && b) {
        bool = ((strcmp(a->car, b->car)<0)*(0x100) + (a->year > b->year)*(0x80)
                + (a->power > b->power)*(0x40)
                + (a->eng > b->eng)*(0x20)
                + (strcmp(a->transmission, b->transmission)<0)*(0x10)
                + (a->cons > b->cons)*(0x8)
                + (a->dim[0] > b->dim[0])*(0x4)
                + (a->dim[1] > b->dim[1])*(0x2)
                + (a->dim[2] > b->dim[2]))&param;
        if (bool) {
            (*c)->next = a;
            a = a->next;
        } else {
            (*c)->next = b;
            b = b->next;
        }
        (*c) = (*c)->next;
    }
    if (a) {
        while (a) {
            (*c)->next = a;
            (*c) = (*c)->next;
            a = a->next;
        }
    }
    if (b) {
        while (b) {
            (*c)->next = b;
            (*c) = (*c)->next;
            b = b->next;
        }
    }
    *c = tmp.next;
}

/* split list */
void split_list(note *src, note **low, note **high) {
    note *fast = NULL;
    note *slow = NULL;

    if (src == NULL || src->next == NULL) {
        (*low) = src;
        (*high) = NULL;
        return;
    }

    slow = src;
    fast = src->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            fast = fast->next;
            slow = slow->next;
        }
    }

    (*low) = src;
    (*high) = slow->next;
    slow->next = NULL;
}

/* merge list sorting */
void mergeSort(note **head, int field) {
    note *low  = NULL;
    note *high = NULL;
    if ((*head == NULL) || ((*head)->next == NULL)) {
        return;
    }
    split_list(*head, &low, &high);
    mergeSort(&low, field);
    mergeSort(&high, field);
    merge(low, high, head, field);
}

/* export lib to file */
void fileExport(FILE *df, note *head){
    while(head){
        fprintf(df, "%s;%d;%d;%f;%s;%f;%d;%d;%d\n", head->car, head->year, head->power, head->eng, head->transmission, head->cons, head->dim[0], head->dim[1], head->dim[2]);
        head = head->next;
    }
}

int main() {
    note *lib;
    note2 *trans;
    char *filename, sep, *s, **s2;
    FILE *df;
    int menu, submenu, s_len, lines, i, count, id;

    menu = 0;

    lib = NULL;
    trans = NULL;
    filename = NULL;
    s = NULL;
    s2 = NULL;

    while(menu != 8) {
        /*menu = read_int(stdin);*/
        switch (menu) {
            /* help */
            case 0: {
                print_menu();
                break;
            }

            /* add card */
            case 1:{
                puts("Do you want to add elements from .csv or type manually?\n0 - file\n1 - console");
                submenu = read_int(stdin);
                switch (submenu) {
                    /* file input*/
                    case 0:{
                        puts("Enter filename:");
                        filename = read_line(stdin, maxlen);
                        df = fopen(filename, "r");
                        if (df != NULL){
                            lines = count_lines(df);

                            sep = ';';
                            for (i = 0, count = 0; i < lines; i++) {
                                s = read_line(df, maxlen);
                                s_len = strlen(s);
                                s2 = split(s, &s_len, sep);

                                if (s2 != NULL) {
                                    push(&lib, s2, &trans, s_len);
                                    count++;
                                    clearStringArray(s2, s_len);
                                } else {
                                    i = lines;
                                    puts("Out of memory!");
                                }
                            }
                            fclose(df);
                            puts("File input success!");

                        }else perror("Data error!");
                        break;
                    }

                    /* console input */
                    case 1:{
                        s2 = malloc(9*sizeof(char*));
                        if(s2!=NULL) {
                            puts("Enter car name:");
                            s2[0] = read_line(stdin, maxlen);
                            puts("Enter production year:");
                            s2[1] = read_line(stdin, maxlen);
                            puts("Enter engine power:");
                            s2[2] = read_line(stdin, maxlen);
                            puts("Enter engine volume:");
                            s2[3] = read_line(stdin, maxlen);
                            puts("Enter transmission type:");
                            s2[4] = read_line(stdin, maxlen);
                            puts("Enter fuel consumption /100km:");
                            s2[5] = read_line(stdin, maxlen);
                            puts("Enter length:");
                            s2[6] = read_line(stdin, maxlen);
                            puts("Enter wide:");
                            s2[7] = read_line(stdin, maxlen);
                            puts("Enter height:");
                            s2[8] = read_line(stdin, maxlen);

                            push(&lib, s2, &trans, 9);
                            clearStringArray(s2, 8);
                        }else puts("Console push memory error!");
                        break;
                    }

                    default: puts("Wrong number");

                }
                break;
            }

            /* edit card */
            case 2:{
                puts("Enter card ID you want to edit:");
                id = read_int(stdin);
                if(id>=1 && id<= liblen(lib)){
                    puts("What do you want to edit?\n 1 - car name\n 2 - production year\n 3 - engine power"
                         "\n 4 - engine value\n 5 - transmission type\n 6 - fuel consumption"
                         "\n 7 - length\n 8 - width\n 9 - height");
                    submenu = read_int(stdin);
                    puts("Enter new value:");
                    s = read_line(stdin, maxlen);
                    lib_edit(lib, id, submenu, s, &trans);
                    puts("Edited");
                }else puts("ID is out of range");
                break;
            }

            /* delete card */
            case 3:{
                puts("Enter ID of element you want to delete:");
                id = read_int(stdin);
                if(id>=1 && id<= liblen(lib)) {
                    deleteNth(&lib, id-1);
                    puts("Element deleted");
                }else puts("ID is out of range");
                break;
            }

            /* print cards */
            case 4:{
                if(lib != NULL) {
                    printLinkedList(lib);
                }
                break;
            }

            /* search by param */
            case 5:{
                puts("What do you want to find?\n 1 - car name\n 2 - production year\n 3 - engine power"
                     "\n 4 - engine value\n 5 - transmission type\n 6 - fuel consumption"
                     "\n 7 - length\n 8 - width\n 9 - height");
                submenu = read_int(stdin);
                if(submenu>=1 && submenu <=9) {
                    puts("Enter search value");
                    s = read_line(stdin, maxlen);
                    if ((submenu >= 2 && submenu < 5) || (submenu >= 6)) {
                        puts("Enter search range:");
                        filename = read_line(stdin, maxlen);
                    } else filename = NULL;
                    find(lib, submenu, s, filename);
                }else puts("Wrong search field");
                break;
            }

            /* sort by param */
            case 6:{
                puts("What do you want to sort?\n 1 - car name\n 2 - production year\n 3 - engine power"
                     "\n 4 - engine value\n 5 - transmission type\n 6 - fuel consumption"
                     "\n 7 - length\n 8 - width\n 9 - height");
                submenu = read_int(stdin);
                if(submenu>=1 && submenu <=9) {
                    mergeSort(&lib, submenu);
                    puts("Sorted");
                }else puts("Wrong sort field");
                break;
            }

            /* export */
            case 7:{
                puts("Enter filename:");
                filename = read_line(stdin, maxlen);
                df = fopen(filename, "w");
                if (df != NULL){
                    fileExport(df, lib);
                    fclose(df);
                    puts("File export success!");

                }else perror("Data error!");
                break;
            }

            default: {
                if (menu > 8) puts("Enter 0 for help");
                break;
            }
        }
        printf("Enter operation: ");
        menu = read_int(stdin);
        CLEAR;
        //print_menu();
    }

    if(lib != NULL) deleteList(&lib);
    if(trans != NULL) deleteList2(&trans);
    if(filename!=NULL) free(filename);
    if(s != NULL) free(s);
    if(s2 !=NULL) free(s2);
    CLEAR;

    return 0;
}