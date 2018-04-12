/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the ��Vision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

extern int  SendChar(int ch); // �����ⲿ��������main�ļ��ж���

extern int  GetKey(void);

struct __FILE {

  int handle;                 // Add whatever you need here 

};

FILE __stdout;

FILE __stdin;

int fputc(int ch, FILE *f) {

  return (SendChar(ch));

}

int fgetc(FILE *f) {

  return (SendChar(GetKey()));

}

void _ttywrch(int ch) {

 SendChar (ch);

}

int ferror(FILE *f) {                            // Your implementation of ferror

  return EOF;

}

void _sys_exit(int return_code) {

label:  goto label;           // endless loop

}
