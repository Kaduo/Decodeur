/*******************************************************************************
Nom ......... : trace.h
Role ........ : Macro de débogage
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef TRACE
#define TRACE

#ifdef DEBUG
#define trace(fmt, ...) do { fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define trace(fmt, ...)  do {} while (0)
#endif

#endif
