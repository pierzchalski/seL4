--
-- Copyright 2014, General Dynamics C4 Systems
--
-- This software may be distributed and modified according to the terms of
-- the GNU General Public License version 2. Note that NO WARRANTY is provided.
-- See "LICENSE_GPLv2.txt" for details.
--
-- @TAG(GD_GPL)
--

#include <mode/object/structures.bf>


base 32

block x86_pat_msr {
    padding     5
    field pa7   3
    padding     5
    field pa6   3
    padding     5
    field pa5   3
    padding     5
    field pa4   3
    padding     5
    field pa3   3
    padding     5
    field pa2   3
    padding     5
    field pa1   3
    padding     5
    field pa0   3
}

-- Local APIC

block apic_base_msr {
    field_high  base_addr           20
    field       enabled             1
    padding                         2
    field       is_bsp              1
    padding                         8
}

block apic_version {
    padding                         8
    field       max_lvt_entry       8
    padding                         8
    field       version             8
}

block apic_svr {
    padding                         22
    field       focus_processor_chk 1
    field       enabled             1
    field       spurious_vector     8
}

block apic_lvt {
    padding                         13
    field       timer_mode          2
    field       masked              1
    field       trigger_mode        1
    field       remote_irr          1
    field       pin_polarity        1
    field       delivery_status     1
    padding                         1
    field       delivery_mode       3
    field       vector              8
}

block apic_icr1 {
    padding                         12
    field       dest_shorthand      2
    padding                         2
    field       trigger_mode        1
    field       level               1
    padding                         1
    field       delivery_status     1
    field       dest_mode           1
    field       delivery_mode       3
    field       vector              8
}

block apic_icr2 {
    field       dest                8
    padding                         24
}

-- x86-specific IRQ state structure

block irq_ioapic {
    field   irqType         4
    field   id              5
    field   pin             5
    field   level           1
    field   polarity_low    1
    field   masked          1
    padding                 15
    padding                 32
}

block irq_msi {
    field   irqType     4
    field   bus         8
    field   dev         5
    field   func        3
    padding             12

    field   handle      32
}

block irq_free {
    field   irqType     4
    padding             28
    padding             32
}

block irq_reserved {
    field   irqType     4
    padding             28
    padding             32
}

tagged_union x86_irq_state irqType {
    tag irq_free        0
    tag irq_ioapic      1
    tag irq_msi         2
    tag irq_reserved    3
}

-- CPUID bitfields. Same on 32 and 64 bit.

block cpuid_001h_eax {
    padding                 4
    field extended_family   8
    field extended_model    4
    padding                 2
    field type              2
    field family            4
    field model             4
    field stepping          4
}

block cpuid_001h_ebx {
    padding                 24
    field brand             8
}

block vmx_basic_msr {
    padding                 8
    field true_msrs         1
    field in_out_exit_info  1
    field memory_type       4
    field monitor_smm_int   1
    field physical_address_limit 1
    padding                 3
    field vmxon_size        13
    padding                 1
    field vmcs_revision     31
}

block feature_control_msr {
    padding                 16
    field senter            1
    field senter_functions  7
    padding                 5
    field vmx_outside_smx   1
    field vmx_in_smx        1
    field lock              1
}

block vmx_ept_vpid_cap_msr {
    padding                         20
    field invvpid_single_context_ng 1
    field invvpid_all_context       1
    field invvpid_single_context    1
    field invvpid_single_address    1
    padding                         7
    field invvpid                   1
    padding                         5
    field invept_all_context        1
    field invept_single_context     1
    padding                         3
    field ept_flags                 1
    field invept                    1
    padding                         2
    field ept_1g                    1
    field ept_2m                    1
    padding                         1
    field ept_wb                    1
    padding                         5
    field ept_uc                    1
    padding                         1
    field ept_depth_4               1
    padding                         5
    field ept_exec_only             1
}

-- This is the layout of the data exit qualification register
-- when the exit reason (as read from the data exit reason)
-- register is 'control register'
block vmx_data_exit_qualification_control_regster {
    field data          16
    padding             4
    field reg           4
    padding             1
    field msw_type      1
    field access_type   2
    field cr            4
}
