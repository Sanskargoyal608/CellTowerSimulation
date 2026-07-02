// syscall.S – macOS arm64 (Apple Silicon) version
// Implements syscall3 and syscall6 using the macOS arm64 syscall ABI.
//
// macOS arm64 syscall convention:
//   x16 = syscall number
//   x0–x5 = arguments 1–6
//   svc #0x80 triggers the syscall
//
// C prototype: long syscall3(long number, long arg1, long arg2, long arg3)
//   number → x0,  arg1 → x1,  arg2 → x2,  arg3 → x3
// We move x0 → x16 (syscall number), then shift args down.

.text
.global _syscall3
.global _syscall6

// long syscall3(long number, long arg1, long arg2, long arg3)
_syscall3:
    mov  x16, x0          // syscall number
    mov  x0,  x1          // arg1
    mov  x1,  x2          // arg2
    mov  x2,  x3          // arg3
    svc  #0x80
    ret

// long syscall6(long number, long a1, long a2, long a3, long a4, long a5, long a6)
// arm64 passes first 8 args in x0–x7
_syscall6:
    mov  x16, x0          // syscall number
    mov  x0,  x1          // arg1
    mov  x1,  x2          // arg2
    mov  x2,  x3          // arg3
    mov  x3,  x4          // arg4
    mov  x4,  x5          // arg5
    mov  x5,  x6          // arg6
    svc  #0x80
    ret
