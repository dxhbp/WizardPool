#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct SorcererPool{
    char *pool;     // 起始位置
    size_t capacity;// 总容量
    size_t used;    // 已用容量
    struct SorcererPool  *next;  // 下一内存池的指针
}SorcererPool;

/// @brief 新建内存池
/// @param size 大小,不包括管理头
/// @return 置于管理头起始位置,失败返回`NULL`
SorcererPool* sorcerer_create(size_t size){
    SorcererPool *p = (SorcererPool*)malloc(size+sizeof(SorcererPool));
    if(p!=NULL){
        p->capacity = size;
        p->used = 0;
        p->next =NULL;
        p->pool = (char*)(p+1);
    }
    return p;
}

/// @brief 分配内存,返回指针
/// @param poolhead 内存头
/// @param i 大小
/// @return 分配的内存的头部指针
void* sorcerer_alloc(SorcererPool *poolhead,size_t i){
    
    if(poolhead==NULL){
        printf("传入空指针\n");
        return  NULL;
    }

    if(!poolhead){
        printf("未知\n");
        return NULL;
    }
    
    if(poolhead->capacity-0x10 < i){
        printf("申请空间超过块大小\n");
        return NULL;
    }
    
    if((poolhead->capacity-poolhead->used) < i+(0x10)){
        if(poolhead->next==NULL){
            SorcererPool *p = sorcerer_create(poolhead->capacity);
            if(p==NULL){
                printf("分配空间失败\n");
                return NULL;
            }
            poolhead->next = p;
            return sorcerer_alloc(p,i);
        }
        SorcererPool *p;
        for(p=poolhead;p->next!=NULL;p=p->next){}
        return sorcerer_alloc(p,i);
    }
    
    void* ptr = poolhead->pool + poolhead->used;
    if(((uintptr_t)ptr&0xF)!=0){
        //printf("内存对齐启动\n");
        void* new_ptr = (void*)(((uintptr_t)ptr + (uintptr_t)0xF) & (~0xF));
        poolhead->used +=(size_t)((uintptr_t)new_ptr - (uintptr_t)ptr);
        ptr = new_ptr;
    }
    
    poolhead->used += i;
    return ptr;
}

/// @brief 释放内存池
/// @param head 头指针
void sorcerer_destroy(SorcererPool *head){
    SorcererPool *next = NULL;
    for(SorcererPool *p=head;p!=NULL;p=next){
        next = p->next;
        free(p);
    }
}


/// @brief 获取池的剩余空间
/// @param pool 池指针
/// @return 剩余字节数
size_t sorcerer_remaining(SorcererPool *pool){
    if(pool == NULL) return 0;
    return pool->capacity-pool->used;
}

