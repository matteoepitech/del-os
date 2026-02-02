/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/task/task_manager
** File description:
** Task manager source file
*/

#include "kernel/interruption/interruption.h"
#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/scheduler/task.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/sys/syscall.h>
#include <kernel/sys/gdt.h>
#include <utils/asm/hlt.h>

/**
 * @brief Create a new task and return it.
 *
 * @param void (*entry)(void)    A function pointer to the handler of the task
 *
 * @return The new task created.
 */
task_t *
ktask_create(void (*entry)(void))
{
    uint32_t stack_top = 0;
    task_t *task = NULL;

    if (entry == NULL) {
        return NULL;
    }
    task = kcalloc(sizeof(task_t));
    if (task == NULL) {
        return NULL;
    }
    task->_state = KTASK_IDLE;
    task->_stack_ptr = kmalloc(KERNEL_PROCESS_STACK_SIZE);
    task->_stack_size = KERNEL_PROCESS_STACK_SIZE;
    task->_next = NULL;
    kmemset(&task->_ctx, 0, sizeof(isr_registers_t));
    stack_top = (uint32_t)task->_stack_ptr + KERNEL_PROCESS_STACK_SIZE;
    stack_top -= sizeof(uint32_t);
    *(uint32_t *) stack_top = (uint32_t) entry;
    stack_top -= sizeof(uint32_t);
    *(uint32_t *) stack_top = 0;
    task->_ctx._eip = (uint32_t) ktask_stubs;
    task->_ctx._esp = stack_top;
    task->_ctx._cs = KGDT_CODE_SELECTOR;
    task->_ctx._eflags = 0x202; /* IF=1 */
    task->_ctx._ds = KGDT_DATA_SELECTOR;
    task->_ctx._es = KGDT_DATA_SELECTOR;
    task->_ctx._fs = KGDT_DATA_SELECTOR;
    task->_ctx._gs = KGDT_DATA_SELECTOR;
    return task;
}

/**
 * @brief Remove the current task from the list of tasks.
 */
void
ktask_exit(void)
{
    if (ktask_current == NULL) {
        ktask_yield();
    }
    ktask_current->_state = KTASK_ZOMBIE;
    kinterruption_extern_start();
    ktask_yield();
    __builtin_unreachable();
}

/**
 * @brief Yield the task and wait while doing an hlt for the CPU. DO NOT HARD HALT HERE.
 */
void
ktask_yield(void)
{
    for (;; make_hlt());
}

/**
 * @brief ONLY use this function to create an entry.
 *        Doing your entry and if your entry is returning, doing an ktask_exit().
 *
 * @param *(entry)(void)     The entry of the function
 */
__attribute__((noreturn)) void
ktask_stubs(void (*entry(void)))
{
    entry();
    __syscall__(KSYS_EXIT, 0, 0, 0, 0, 0);
    __builtin_unreachable();
}
