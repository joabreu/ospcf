/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\time.h
    @brief Time and Date functions
*/
#ifndef INCLUDE_LIB_TIME_H
#define INCLUDE_LIB_TIME_H

/** @fn signed long int maketime(unsigned long int year, unsigned long int month, unsigned long int day, unsigned long int hour, unsigned long int minute, unsigned long int second)
    @brief Retorna o número de segundos passados desde 1970
    @ingroup libs_libc_time 
    @param year Ano Actual
    @param month Mês
    @param day Dia
    @param hour Hora
    @param minute Minuto
    @param second Segundos
    @return Número de segundos
*/
signed long int maketime(unsigned long int year, unsigned long int month,
                         unsigned long int day, unsigned long int hour,
                         unsigned long int minute, unsigned long int second);

#endif
