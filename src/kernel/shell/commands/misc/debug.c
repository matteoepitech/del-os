/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/misc/debug
** File description:
** debug command source file
*/

#include <kernel/memory/early_allocator/early_alloc.h>
#include <kernel/scheduler/scheduler.h>
#include <kernel/fs/vfs/vfs_registry.h>
#include <kernel/fs/fd/fd_operations.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/fs/tmpfs/tmpfs.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/memory/vmm/vmm.h>
#include <kernel/memory/pmm/pmm.h>
#include <kernel/shell/shell.h>
#include <kernel/sys/syscall.h>
#include <kernel/sys/unistd.h>
#include <utils/misc/print.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

static void test_debug(void)
{
    __syscall__(KSYS_LOG, (int32_t) "Creating file called test.file", 0, 0, 0, 0);
    fd_t fd = __syscall__(KSYS_OPEN, (int32_t) "./test.file", KVFS_O_CREAT | KVFS_O_WRONLY, 0644, 0, 0);

    __syscall__(KSYS_LOG, (int32_t) "Creating directory proc/", 0, 0, 0, 0);
    __syscall__(KSYS_MKDIR, (int32_t) "/proc", 0, 0, 0, 0);
   
    __syscall__(KSYS_LOG_NB, fd, 0, 0, 0, 0);
    __syscall__(KSYS_LOG, (int32_t) "Putting \"Hello World!\" on test.file", 0, 0, 0, 0);
    __syscall__(KSYS_WRITE, fd, (int32_t) "Hello World!", sizeof("Hello World!"), 0, 0);

    __syscall__(KSYS_LOG, (int32_t) "Closing the file test.file", 0, 0, 0, 0);
    __syscall__(KSYS_CLOSE, fd, 0, 0, 0, 0);

    __syscall__(KSYS_WRITE, KSTDOUT_FILENO, (int32_t) "Hello Matthias", sizeof("Hello Matthias"), 0, 0);
    return;
}

/**
 * @brief Command for debug.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_debug(UNUSED uint32_t argc, UNUSED char *argv[])
{
    process_t *process = kprocess_create(test_debug);

    if (process == NULL || process->_main_thread == NULL) {
        KPRINTF_ERROR("debug: failed to create user process (out of memory or mapping error)");
        return KO_FALSE;
    }
    kscheduler_add_task(process->_main_thread);
    return KO_FALSE;
}
