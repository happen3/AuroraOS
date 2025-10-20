#pragma once
#include "stdtypes.h"
typedef struct {
    uint8_t max_cpuid_num;
    uint32_t is_rdrand_supported : 1;
    uint32_t is_sse3_supported : 1;
    uint32_t is_pclmulqdg_supported : 1; // Someone explain why that name
    uint32_t is_dtes64_supported : 1;
    uint32_t is_monitor_supported : 1;
    uint32_t is_dscpl_supported : 1;
    uint32_t is_vmx_supported : 1;
    uint32_t is_smx_supported : 1;
    uint32_t is_eist_supported : 1;
    uint32_t is_tm2_supported : 1;
    uint32_t is_ssse3_supported : 1;
    uint32_t is_cnxtid_supported : 1;
    uint32_t is_sdbg_supported : 1;
    uint32_t is_fma_supported : 1;
    uint32_t is_cmpxchg16b_supported : 1; // Really?!
    uint32_t is_xptr_supported : 1;
    uint32_t is_pdcm_supported : 1;
    uint32_t is_pcid_supported : 1;
    uint32_t is_dca_supported : 1;
    uint32_t is_sse41_supported : 1;
    uint32_t is_sse42_supported : 1;
    uint32_t is_x2apic_supported : 1;
    uint32_t is_movbe_supported : 1;
    uint32_t is_popcnt_supported : 1;
    uint32_t is_pscdeadline_supported : 1;
    uint32_t is_aes_supported : 1;
    uint32_t is_xsave_supported :1;
    uint32_t is_osxsave_supported : 1;
    uint32_t is_avx_supported : 1;
    uint32_t is_f16c_supported : 1;
} SUPPORTED_CPU_FEATURES;

typedef struct {
    uint8_t cache_type : 1;
    uint8_t cache_level : 1;
    boolean self_init_cache : 1;
    boolean associative_cache : 1;
    uint8_t line_partitions : 1;
    uint8_t cache_assoc_ways : 1;
    uint8_t number_of_sets : 1;
    uint8_t max_addressable_ids : 1;
    struct {
        uint16_t coherency_line_size;
        uint16_t physical_line_parts;
        uint16_t ways_of_asoc;
    } global_cache_info;
    uint32_t glb_number_of_sets;
    struct {
        boolean cache_inclusiveness : 1;
        boolean complex_cache_indexing : 1;
    } ex_cache_info;
    uint64_t cache_size;
} CPUINFO_CACHE;

typedef struct {
    uint8_t stepping_id;
    uint8_t cpu_model;
    char vendor_string[13]; // 12 + 1(\0)
    uint8_t family_id;
    boolean processor_type : 1;
    struct {
        uint8_t extended_model_id;
        uint8_t extended_family_id;
    } extended_information;
    struct {
        uint8_t brand_index;
        uint8_t cflush_size;
        uint8_t max_num_logical_processors;
        uint8_t apic_id;
    } cpu_spec;
    CPUINFO_CACHE cache_info[12];
} CPU_INFORMATION;

typedef struct {
    SUPPORTED_CPU_FEATURES cpu_features;
    CPU_INFORMATION cpu_info;
    struct {
        boolean sse : 1;
        boolean sse2 : 1;
        boolean mmx : 1;
        boolean pae : 1;
        boolean apic : 1;
        boolean acpi : 1;
    } legacy_features;
    boolean hypervisor_present;
} KERNEL_INFO;

extern KERNEL_INFO kernel_extended_info;