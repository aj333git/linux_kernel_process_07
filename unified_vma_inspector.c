// unified_vma_inspector.c (Linux 5.15, versioned)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/pid.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel Mastery");
MODULE_DESCRIPTION("Unified Process VMA + Thread + Stack + Heap Inspector");
MODULE_VERSION("1.1");  // default version; can be updated per build

static int target_pid = 1; // default PID=1
module_param(target_pid, int, 0644);
MODULE_PARM_DESC(target_pid, "Target Process PID");

// --- Inspect function ---
static void inspect_process(struct task_struct *task)
{
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct task_struct *thread;

    if (!task) return;

    printk(KERN_INFO "=== Inspecting process: %s [PID=%d] ===\n", task->comm, task->pid);

    mm = task->mm;
    if (!mm) {
        printk(KERN_INFO "No memory info for kernel thread or exited process.\n");
        return;
    }

    printk(KERN_INFO "--- VM Areas ---\n");
    for (vma = mm->mmap; vma; vma = vma->vm_next) {
        printk(KERN_INFO "VMA: 0x%lx - 0x%lx, Flags=%lx",
               vma->vm_start, vma->vm_end, vma->vm_flags);
        if (vma->vm_file)
            printk(KERN_CONT " (Mapped File: %s)", vma->vm_file->f_path.dentry->d_name.name);
        else
            printk(KERN_CONT " (Anonymous Mapping)");
        printk(KERN_CONT "\n");
    }

    printk(KERN_INFO "--- Threads & Stacks ---\n");
    do_each_thread(task, thread) {
        printk(KERN_INFO "Thread: %s [TID=%d], Kernel stack ptr: %px\n",
               thread->comm, thread->pid, thread->stack);
    } while_each_thread(task, thread);

    printk(KERN_INFO "--- Heap Info ---\n");
    printk(KERN_INFO "Program break (end of heap): 0x%lx\n", mm->brk);
}

// --- Init and Exit (versioned) ---
static int __init unified_vma_inspector_1_1_init(void)
{
    struct pid *pid_struct;
    struct task_struct *task;

    printk(KERN_INFO "Unified VMA Inspector v1.1 module loaded.\n");

    pid_struct = find_get_pid(target_pid);
    if (!pid_struct) {
        printk(KERN_ERR "Cannot find PID %d\n", target_pid);
        return -ESRCH;
    }

    task = pid_task(pid_struct, PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "Cannot find task_struct for PID %d\n", target_pid);
        return -ESRCH;
    }

    inspect_process(task);
    return 0;
}

static void __exit unified_vma_inspector_1_1_exit(void)
{
    printk(KERN_INFO "Unified VMA Inspector v1.1 module unloaded.\n");
}

module_init(unified_vma_inspector_1_1_init);
module_exit(unified_vma_inspector_1_1_exit);

