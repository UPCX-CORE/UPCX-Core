.file	"switch_stack_linux.s"
.text
.globl	upcxvmoc_switch_stack
.type	upcxvmoc_switch_stack, @function
upcxvmoc_switch_stack:
   movq %rsp, -16(%rdi)
   leaq -16(%rdi), %rsp
   movq %rdx, %rdi
   callq *%rsi
   mov (%rsp), %rsp
   retq
.size	upcxvmoc_switch_stack, .-upcxvmoc_switch_stack
.section	.note.GNU-stack,"",@progbits
