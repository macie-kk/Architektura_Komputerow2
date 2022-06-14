SYSEXIT = 1
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1
SYSREAD = 3
STDIN = 0
.global _start

.data
	buf: .ascii "     "
	buf_len = . - buf

.text
	msg: .ascii "Write text (5): \n"
	msg_len = . - msg

dobrze: .ascii "Takie same "
dobrze_len = . - dobrze

blad: .ascii "Nie takie same"
blad_len = . - blad

test: .ascii "slowo"
test_len = . - test

_start:
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $test, %ecx
mov $test_len, %edx
int $0x80

mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $msg, %ecx
mov $msg_len, %edx
int $0x80

mov $SYSREAD, %eax
mov $STDIN, %ebx
mov $buf, %ecx
mov $buf_len, %edx
int $0x80

mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $buf, %ecx
mov $buf_len, %edx
int $0x80

xorl %edi, %edi
movl $buf, %edi
xorl %ecx, %ecx
movl $test, %ecx

cmpl %edi, %ecx
jne if_false

if_true:
	mov $SYSWRITE, %eax
	mov $STDOUT, %ebx
	mov $dobrze, %ecx
	mov $dobrze_len, %edx
	int $0x80
	jmp dalej

if_false:
	mov $SYSWRITE, %eax
	mov $STDOUT, %ebx
	mov $blad, %ecx
	mov $blad_len, %edx
	int $0x80

dalej:
	mov $SYSEXIT, %eax
	mov $EXIT_SUCCESS, %ebx
	int $0x80
