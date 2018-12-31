#include<stdio.h>
#include<stdlib.h>

#define append_to_list(A,B)\
    ((__builtin_types_compatible_p(typeof(B),int))?append_int(A,B):\
     (__builtin_types_compatible_p(typeof(B),char))?append_char(A,B):\
     (__builtin_types_compatible_p(typeof(B),float))?append_float(A,B):\
     (__builtin_types_compatible_p(typeof(B),long))?append_long(A,B):\
     (__builtin_types_compatible_p(typeof(B),double))?append_double(A,B):(void)0)

#define bucket_size 10000


#define add_to_set(A,B) (__builtin_types_compatible_p(typeof(B),int)?add_int(A,B):\
                        __builtin_types_compatible_p(typeof(B),float)?add_float(A,B):\
                        __builtin_types_compatible_p(typeof(B),char)?add_char(A,B):(void)0)

#define __hash_code(B) (__builtin_types_compatible_p(typeof(B),int)?__hash_code_int(B):\
                        __builtin_types_compatible_p(typeof(B),float)?__hash_code_float(B):\
                        __builtin_types_compatible_p(typeof(B),char)?__hash_code_char(B):(void)0)

#define hash_code(B) (__builtin_types_compatible_p(typeof(B),int)?__hash_code_int(B):\
                        __builtin_types_compatible_p(typeof(B),float)?__hash_code_float(B):\
                        __builtin_types_compatible_p(typeof(B),char)?__hash_code_char(B):(void)0)

int hash_val;

typedef struct SetHashTable{
    char types[bucket_size];
    void* keys[bucket_size];
}SetHashTable;

typedef struct List{
    //variables
    int size;
    int _actualsize;
    void** list;
    char* data_types;
}List;

void checkSize(List*);
void checkSize(List* self){
    if(self->size==self->_actualsize){
        self->_actualsize = self->size+10;
        self->list = realloc(self->list,self->_actualsize*sizeof(void*));
        self->data_types = realloc(self->data_types,self->_actualsize*sizeof(char));
   }
}

void append_int(List*,int);
void append_int(List* self, int a){
    checkSize(self);
    int* b = (int*)malloc(sizeof(int));
    *b = a;
    self->list[self->size] = b;
    self->data_types[self->size] = 'i';
    self->size++;
}

void append_float(List*,float);
void append_float(List* self, float a){
    checkSize(self);
    float* b = (float*)malloc(sizeof(float));
    *b = a;
    self->list[self->size] = b;
    self->data_types[self->size] = 'f';
    self->size++;
}

void append_long(List*,long);
void append_long(List* self, long a){
    checkSize(self);
    long* b = (long*)malloc(sizeof(long));
    *b = a;
    self->list[self->size] = b;
    self->data_types[self->size] = 'l';
    self->size++;
}

void append_double(List*,double);
void append_double(List* self, double a){
    checkSize(self);
    double* b = (double*)malloc(sizeof(double));
    *b = a;
    self->list[self->size] = b;
    self->data_types[self->size] = 'd';
    self->size++;
}

void append_char(List*,char);
void append_char(List* self,char a){
    checkSize(self);
    char* b = (char*)malloc(sizeof(char));
    *b = a;
    self->list[self->size] = b;
    self->data_types[self->size] = 'c';
    self->size++;
}

void append_others(List*,void*);
void append_others(List* self, void* b){
    checkSize(self);
    self->list[self->size] = b;
    self->data_types[self->size] = 'o';
    self->size++;
}

void append_voidstar(List*,void*);
void append_voidstar(List* self,void* b){
    checkSize(self);
    self->list[self->size] = b;
    self->data_types[self->size] = '*';
    self->size++;
}

void append_string(List*,char*);
void append_string(List* self, char* b){
    checkSize(self);
    self->list[self->size] = b;
    self->data_types[self->size] = 's';
    self->size++;
}

void append_list(List* , List* );
void append_list(List* self, List* to_append){
    checkSize(self);
    self->list[self->size] = to_append;
    self->data_types[self->size] = 'L';
    self->size++;
}

List* getNewList(int);
List* getNewList(int size){
    List* list = (List*)malloc(sizeof(List));
    list->_actualsize = size;
    list->size = 0;
    list->list = (void**)calloc(list->_actualsize,sizeof(void*));
    list->data_types = (char*)calloc(list->_actualsize,sizeof(char));
    return list;
}

void copy(List* ,List* );
void copy(List* self,List* to_copy){
    int i=0;
    List* l;
    for(;i<to_copy->size;i++){
        switch(to_copy->data_types[i]){
            case 'i':
                append_int(self, *(int*)to_copy->list[i]);
                break;
            case 'f':
                append_float(self, *(float*)to_copy->list[i]);
                break;
            case 'c':
                append_char(self, *(char*)to_copy->list[i]);
                break;
            case 's':
                append_string(self, (char*)to_copy->list[i]);
                break;
            case 'l':
                append_long(self, *(long*)to_copy->list[i]);
                break;
            case 'd':
                append_double(self, *(double*)to_copy->list[i]);
                break;
            case 'o':
                append_others(self, (void*)to_copy->list[i]);
                break;
            case 'L':
                l = getNewList(((List*)to_copy->list[i])->size);
                copy(l,((List*)to_copy->list[i]));
                append_list(self,l);
                break;
        }
    }
}

void transfer(List* , List* ,int ,int );
void transfer(List* self, List* to_trans,int start, int stop){
    int i;
    for(i=start;i<stop;i++){
        checkSize(self);
        self->list[self->size] = to_trans->list[i];
        to_trans->list[i] = to_trans->list[stop+i-start];
        self->data_types[self->size] = to_trans->data_types[i];
        to_trans->data_types[i] = to_trans->data_types[stop+i-start];
        self->size++;
    }
    int new_start = stop+i-start;
    int temp = new_start;
    while(temp<to_trans->size){
        to_trans->list[i] = to_trans->list[temp];
        to_trans->data_types[i] = to_trans->data_types[temp];
        temp++;
        i++;
    }
    to_trans->size = i;
}

SetHashTable* get_set(void);
SetHashTable* get_set(){
    SetHashTable* sethashtable = (SetHashTable*) calloc(1,sizeof(SetHashTable));
    return sethashtable;
}

int __hash_code_string(char*);
int __hash_code_string(char* cp)
{
    size_t hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char) *cp++;
    return ((int)hash) % bucket_size;
}

int __hash_code_int(int);
int __hash_code_int(int a){
    size_t hash = 5381;
    hash = 33 * hash ^ (unsigned int) a;
    hash_val = ((int)hash) % bucket_size;
    return ((int)hash) % bucket_size;
}

int __hash_code_char(char);
int __hash_code_char(char a){
    int c = a;
    hash_val = __hash_code_int(__hash_code_int(c));
    return hash_val;
}

int __hash_code_float(float);
int __hash_code_float(float a){
    int round = -1,j;
    size_t hash = 5381;
    while(a>0 && round<3){
        j = (int)a;
        hash = 33 * hash ^ j;
        a = (a-j)*10;
        round++;
    }
    hash_val = ((int)hash) % bucket_size;
    return ((int)hash)%bucket_size;
}

int add_int(SetHashTable* ,int);
int add_int(SetHashTable* set,int element){
    __hash_code(element);
    int hash_value = hash_val;
    if(!set->keys[hash_value]){
        int* key = (int*)malloc(sizeof(int));
        *key = element;
        set->keys[hash_value] = key;
        set->types[hash_value] = 'i';
        return 1;
    }
    return -1;
}

int add_float(SetHashTable* ,float);
int add_float(SetHashTable* set,float element){
    __hash_code(element);
    int hash_value = hash_val;
    if(!set->keys[hash_value]){
        float* key = (float*)malloc(sizeof(float));
        *key = element;
        set->keys[hash_value] = key;
        set->types[hash_value] = 'f';
        return 1;
    }
    return -1;
}

int add_char(SetHashTable* ,char );
int add_char(SetHashTable* set,char element){
    __hash_code(element);
    int hash_value = hash_val;
    if(!set->keys[hash_value]){
        char* key = (char*)malloc(sizeof(char));\
        *key = element;
        set->keys[hash_value] = key;
        set->types[hash_value] = 'c';
        return 1;
    }
    return -1;
}

int add_string(SetHashTable* , char*);
int add_string(SetHashTable* set, char* s){
    int hash_value = __hash_code_string(s);
    if(hash_value<0){
        hash_value = -1 * hash_value;
    }
    if(!set->keys[hash_value]){
        set->keys[hash_value] = s;
        set->types[hash_value] = 's';
        return 1;
    }
    return -1;
}

int add_set(SetHashTable* ,SetHashTable* );
int add_set(SetHashTable* set,SetHashTable* set1){
    int i=0;
    for(;i<bucket_size;i++){
        if(set1->keys[i]){
            if(!set->keys[i]){
                set->keys[i] = set1->keys[i];
                set->types[i] = set1->types[i];
            }
        }
    }
}

int add_list(SetHashTable* ,List* );
int add_list(SetHashTable* set,List* list){
    int i=0,j=0;
    for(;i<list->size;i++){
        switch(list->data_types[i]){
            case 'i':
                add_to_set(set, *(int*)list->list[i]);
                break;
            case 'f':
                add_to_set(set, *(float*)list->list[i]);
                break;
            case 'c':
                add_to_set(set, *(char*)list->list[i]);
                break;
            case 's':
                add_string(set, (char*)list->list[i]);
                break;
            case 'L':
                add_list(set, (List*)list->list[i]);
                break;
            case 'S':
                add_set(set, (SetHashTable*)list->list[i]);
                break;
            default:
                printf("got this in default -> %c",list->data_types[i]);\
                printf("\n");
                j++;
        }
    }
    if(j!=0)
        printf("Sets cannot store double and long variables\n");
}
