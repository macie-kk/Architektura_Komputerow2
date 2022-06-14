SYSEXIT = 1

.section .data
	dodatnia: .float 1.69
	ujemna: .float -6.2
	zero: .float 0

.section .text
.global _start
_start:
	FINIT	# inicjowanie jednostki FPU

	# +nieskonczonosc: dzielenie liczby dodatniej przez 0
	FLDS dodatnia
	FDIV zero

	# -nieskonczonosc: dzielenie liczby ujemnej przez 0
	FLDS ujemna
	FDIV zero

	# +zero: mnozenie zera przez liczbe dodatnia
	FLDS zero
	FMUL dodatnia

	# -zero: mnozenie zera przez liczbe ujemna
	FLDS zero
	FMUL ujemna

	# NaN: pierwiastek z liczby ujemnej
	FLDS ujemna
	FSQRT

exit:
	MOV $SYSEXIT, %eax
	mov $0, %ebx
	int $0x80
