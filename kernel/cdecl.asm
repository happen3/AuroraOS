; =========================================================== ;
; CDECL helper, provides helper functions for CDecl functions ;
; =========================================================== ;

; takes BX: number of arguments to clear
; returns AX: final count of arguments to clear (4bytes/arg)
; this operation is **unsafe**, it doesn't restore the stack.
cdecl_clear:
    mov ax, 4
    mul bx
    ret
