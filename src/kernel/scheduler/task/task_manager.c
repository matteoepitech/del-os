/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/task/task_manager
** File description:
** Task manager source file
*/

#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/task.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/sys/gdt.h>

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
    task->_ctx._eip = (uint32_t) entry;
    task->_ctx._cs = KGDT_CODE_SELECTOR;
    task->_ctx._eflags = 0x202; /* IF=1 */
    task->_ctx._ds = KGDT_DATA_SELECTOR;
    task->_ctx._es = KGDT_DATA_SELECTOR;
    task->_ctx._fs = KGDT_DATA_SELECTOR;
    task->_ctx._gs = KGDT_DATA_SELECTOR;
    stack_top = (uint32_t)(task->_stack_ptr) + KERNEL_PROCESS_STACK_SIZE;
    task->_ctx._esp = stack_top;
    return task;
}
