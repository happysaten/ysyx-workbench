/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
    int NO;
    struct watchpoint *next;

    char expr[256]; // 存储监视的表达式
    word_t val;     // 上一次表达式的值

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL,
          *free_ =
              NULL; // head指向使用中的监视点链表，free_指向空闲的监视点链表

void init_wp_pool() {
    int i;
    for (i = 0; i < NR_WP; i++) {
        wp_pool[i].NO = i;
        wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    }

    head = NULL;
    free_ = wp_pool;
}

// 从空闲链表获取一个监视点结构
static void new_wp() {
    if (free_ == NULL) {
        // 没有空闲监视点可用
        printf("Error: No free watchpoint available!\n");
        assert(0);
        return;
    }

    // 从空闲链表中取出一个节点
    WP *wp = free_;
    free_ = free_->next;

    // 将节点添加到使用中的链表头部
    wp->next = head;
    head = wp;

    return;
}

// 释放监视点结构回到空闲链表
static void free_wp(WP *wp) {
    if (wp == NULL)
        return;

    // 如果是头节点
    if (head == wp) {
        head = wp->next;
    } else {
        // 查找wp的前一个节点
        WP *prev = head;
        while (prev && prev->next != wp) {
            prev = prev->next;
        }

        if (prev) {
            prev->next = wp->next;
        } else {
            // 未找到该监视点
            return;
        }
    }

    // 清空监视点信息
    memset(wp->expr, 0, sizeof(wp->expr));
    wp->val = 0;

    // 添加到空闲链表头部
    wp->next = free_;
    free_ = wp;
}

// 设置监视点
void set_watchpoint(char *e) {
    bool success = true;
    word_t val = expr(e, &success);
    if (!success) {
        printf("Invalid expression for watchpoint\n");
        return;
    }

    new_wp();
    WP *wp = head;
    if (wp) {
        strncpy(wp->expr, e, sizeof(wp->expr) - 1);
        wp->expr[sizeof(wp->expr) - 1] = '\0'; // 确保字符串终止
        wp->val = val;
        printf("Watchpoint %d: %s\n", wp->NO, wp->expr);
    }
    return;
}

// 删除监视点
bool delete_watchpoint(int num) {
    WP *wp = head;
    while (wp) {
        if (wp->NO == num) {
            free_wp(wp);
            return true;
        }
        wp = wp->next;
    }
    return false;
}

// 打印所有监视点
void print_watchpoints() {
    if (head == NULL) {
        printf("No watchpoints.\n");
        return;
    }
    printf("Num\tWhat\tValue\n");
    WP *wp = head;
    while (wp) {
        printf("%d\t%s\t%u\n", wp->NO, wp->expr, wp->val);
        wp = wp->next;
    }
}

// 检查所有监视点
bool check_watchpoints() {
    bool changed = false;
    WP *wp = head;

    while (wp) {
        bool success = true;
        word_t new_val = expr(wp->expr, &success);

        if (success && new_val != wp->val) {
            printf("Watchpoint %d: %s\n", wp->NO, wp->expr);
            printf("Old value = %u\n", wp->val);
            printf("New value = %u\n", new_val);
            wp->val = new_val;
            changed = true;
        }

        wp = wp->next;
    }

    return changed;
}
