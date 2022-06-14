SYSWRITE = 4
STDOUT = 1
SYSEXIT = 1
EXIT_SUCCESS = 0

liczba1: .long 0x10304008, 0x701100FF, 0x45100020, 0x08570030
liczba1_len = .-liczba1
ilosc1 = liczba1_len / 4	#podzielenie *tablicy* przez 4 - czyli dlugosc longa

liczba2: .long 0xF040500C, 0x00220026, 0x321000CB, 0x04520031
liczba2_len = .-liczba2
ilosc2 = liczba2_len / 4

.section .bss
	przen: .space 4			#rezerwacja 1 bitu dla przeniesienia
	wynik: .space liczba1_len	#rezerwacja dlugosci liczby dla wyniku

.section .text
.global _start
_start:
	clc	#czyszczenie przeniesienia
	pushf	#dodanie przeniesienia na stos
	movl $ilosc1, %ebx	#index petli
	movl $ilosc1, %esi	#index 1 liczby
	movl $ilosc2, %edi	#index 2 liczby

dodawanie:
	popf	#pobranie przeniesienia ze stosu

	#zmniejszanie wszystkich indexow
	dec %ebx
	dec %esi
	dec %edi

	movl liczba1(, %esi, 4), %eax	#przeniesienie n-tych 4 bitow pierwszej liczby do rejestru
	adcl liczba2(, %edi, 4), %eax	#dodanie do rejestru n-tych 4 bitow drugiej liczby z przeniesieniem

	pushf				#dodanie flagi przeniesienia z dodawania na stos
	movl %eax, wynik(, %ebx, 4)	#przeniesienie wyniku dodawania na n-te 4 bity zarezerwowanego miejsca

	cmp $0, %ebx	#jezeli skonczy sie petla obsluz przeniesienie
	jz obsluga_przen

	jmp dodawanie	#powrot na poczatek petli

obsluga_przen:
	popf			#pobranie przeniesienia ze stosu
	movl $0, przen		#wyzerowanie zmiennej od przeniesienia
	jc dodanie_przen	#sprawdzenie czy pobrane przeniesienie jest 1 i dodanie go jezeli tak
	jmp koniec		#skonczenie programu

dodanie_przen:
	movl $1, przen

koniec:
	#zakonczenie
	mov $SYSEXIT, %eax
	mov $EXIT_SUCCESS, %ebx
	int $0x80
