/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#include <object.h>
#include <machine.h>
#include <arch/model/statedata.h>
#include <arch/kernel/vspace.h>
#include <arch/kernel/thread.h>
#include <arch/linker.h>

void
Arch_switchToThread(tcb_t* tcb)
{
    word_t base;

    /* set PD */
    setVMRoot(tcb);

    /* update the GDT_TLS entry with the thread's TLS_BASE address */
    base = getRegister(tcb, TLS_BASE);
    x86_write_fs_base(base);


    /* update the GDT_IPCBUF entry with the thread's IPC buffer address */
    base = tcb->tcbIPCBuffer;
    x86_write_gs_base(base);

}

BOOT_CODE void
Arch_configureIdleThread(tcb_t* tcb)
{
    setRegister(tcb, FLAGS, FLAGS_USER_DEFAULT);
    setRegister(tcb, NextIP, (word_t)idleThreadStart);
    setRegister(tcb, CS, SEL_CS_0);
    setRegister(tcb, DS, SEL_DS_0);
    setRegister(tcb, ES, SEL_DS_0);
    setRegister(tcb, FS, SEL_DS_0);
    setRegister(tcb, GS, SEL_DS_0);
    setRegister(tcb, SS, SEL_DS_0);
}

void
Arch_switchToIdleThread(void)
{
    /* Force the idle thread to run on kernel page table */
    setVMRoot(NODE_STATE(ksIdleThread));
}

void CONST
Arch_activateIdleThread(tcb_t* tcb)
{
    /* Don't need to do anything */
}
