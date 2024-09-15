#ifndef _u_common_h
#define _u_common_h

#include <stdint.h>
#include <stddef.h>

#define SECTION(x) __attribute__((section(x)))
#define NAKED      __attribute__((naked))
#define PACKED     __attribute__((packed))

#define NO_INIT SECTION(".noinit")
#define EEPROM  SECTION(".eeprom")

#define SECTION_FN(x) NAKED SECTION(x)

#define INIT1 SECTION_FN(".init1")
#define INIT3 SECTION_FN(".init3")
#define INIT5 SECTION_FN(".init5")
#define INIT7 SECTION_FN(".init7")
#define INIT8 SECTION_FN(".init8")

#define FINIT9 SECTION_FN(".finit9")
#define FINIT8 SECTION_FN(".finit8")
#define FINIT7 SECTION_FN(".finit7")
#define FINIT5 SECTION_FN(".finit5")
#define FINIT4 SECTION_FN(".finit4")
#define FINIT3 SECTION_FN(".finit3")
#define FINIT2 SECTION_FN(".finit2")
#define FINIT1 SECTION_FN(".finit1")

#define max(a, b) ({       \
    typeof(a) __a = a;     \
    typeof(b) __b = b;     \
    __a > __b ? __a : __b; \
})

#define min(a, b) ({       \
    typeof(a) __a = a;     \
    typeof(b) __b = b;     \
    __a < __b ? __a : __b; \
})

#define clamp(val, mi, ma) min(max(mi, val), ma)

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef float    f32;

typedef size_t usize;

#endif