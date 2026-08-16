// include/sorcerer_pool.h
#ifndef SORCERER_POOL_H
#define SORCERER_POOL_H

#include <stddef.h>   
#include <stdint.h>   

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 巫师内存池结构
 * 
 * 每个池包含数据区和管理头，通过 next 指针形成链表
 */
typedef struct SorcererPool {
    char *pool;              // 数据区起始位置
    size_t capacity;         // 池容量（字节）
    size_t used;             // 已使用（字节）
    struct SorcererPool *next; // 下一个池
} SorcererPool;

/**
 * @brief 创建一个新的内存池
 * 
 * @param size 每个池的数据区大小（不含管理头）
 * @return 成功返回池指针，失败返回 NULL
 */
SorcererPool* sorcerer_create(size_t size);

/**
 * @brief 从池中分配内存
 * 
 * @param pool 池指针
 * @param size 要分配的大小（字节）
 * @return 成功返回内存指针，失败返回 NULL
 */
void* sorcerer_alloc(SorcererPool *pool, size_t size);

/**
 * @brief 释放整个内存池链
 * 
 * @param pool 池指针（头节点）
 */
void sorcerer_destroy(SorcererPool *pool);

/**
 * @brief 获取池的剩余空间
 * 
 * @param pool 池指针
 * @return 剩余字节数
 */
size_t sorcerer_remaining(SorcererPool *pool);

#ifdef __cplusplus
}
#endif

#endif // SORCERER_POOL_H