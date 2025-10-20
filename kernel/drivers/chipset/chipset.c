#include "kernel.h"
#include "stdtypes.h"
#include "util.h"
#include "chipset.h"
inline void __CPUID__(uint32_t *eax,uint32_t *ebx,uint32_t *ecx,uint32_t *edx, uint32_t number, uint32_t leaf) {
    asm volatile("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ebx), "=d"(*edx) : "a"(number), "c"(leaf));
}

void detect_cpu_info() {
    uint32_t a,b,c,d; // respectively eax, ebx, ecx, edx.

    // CPUID; LEAF= 0, SUBLEAF= 0
    __CPUID__(&a, &b, &c, &d, 0, 0);
    kernel_extended_info.cpu_features.max_cpuid_num = a;
    *(uint32_t*)&kernel_extended_info.cpu_info.vendor_string[0] = b;
    *(uint32_t*)&kernel_extended_info.cpu_info.vendor_string[4] = c;
    *(uint32_t*)&kernel_extended_info.cpu_info.vendor_string[8] = d;
    kernel_extended_info.cpu_info.vendor_string[12] = '\0';

    // CPUID; LEAF=1, SUBLEAF=0
    __CPUID__(&a, &b, &c, &d, 1, 0);
    kernel_extended_info.cpu_info.stepping_id = EXTRACT_BITS(a, 0, 4);
    kernel_extended_info.cpu_info.cpu_model = EXTRACT_BITS(a, 4, 4);
    kernel_extended_info.cpu_info.family_id = EXTRACT_BITS(a, 7, 4);
    kernel_extended_info.cpu_info.processor_type = EXTRACT_BITS(a, 11, 4);
    kernel_extended_info.cpu_info.extended_information.extended_model_id = EXTRACT_BITS(a, 15, 4);
    kernel_extended_info.cpu_info.extended_information.extended_family_id = EXTRACT_BITS(a, 19, 8);

    // TODO: enable feature detection and cache detection
}