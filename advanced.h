#include<stdio.h>
#include<stdlib.h>

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
    self->data_types[self->size] = 'd';
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
    self->data_types[self->size] = 'D';
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

void append_voidstar(List*,void**);
void append_voidstar(List* self,void** b){
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
    list->list = calloc(list->_actualsize,sizeof(void*));
    list->data_types = (char*)calloc(list->_actualsize,sizeof(char));
    return list;
}

void copy(List* ,List* );
void copy(List* self,List* to_copy){
    int i=0;
    List* l;
    for(;i<to_copy->size;i++){
        switch(to_copy->data_types[i]){
            case 'd':
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
            case 'D':
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
