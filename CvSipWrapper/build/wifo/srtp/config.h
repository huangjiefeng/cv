#ifndef CONFIG_H
#define CONFIG_H

#define inline
#define asm(tok) __asm { tok }

/* if we're on a big endian machine, we need to define this */

#ifdef __BIG_ENDIAN__
#define WORDS_BIGENDIAN      1
#else
#define WORDS_BIGENDIAN      0
#endif

/* check for <stdint.h> or <machine/types.h>              */
/* #undef HAVE_STDINT_H */
/* #undef HAVE_MACHINE_TYPES_H */
/* #undef HAVE_SYS_INT_TYPES_H */

/* check for microsoft integer definitions (e.g., cygwin) */
#if defined(__CYGWIN__)
#define HAVE_MS_TYPES 1
#endif

/* if we don't have uio.h, we'll need to define struct iovec */
/* #undef HAVE_SYS_UIO_H */

/* <unistd.h> is used by some test/ apps                  */
/* #undef HAVE_UNISTD_H */

/* check for stdlib.h - we use it for xalloc() and free() */
#define HAVE_STDLIB_H 1

/* test apps should use inet_aton(), if it's available */
/* #undef HAVE_INET_ATON */

/* check if we have syslog functions                      */
/* #undef HAVE_SYSLOG_H */

/* check to see if the user has requested the use of syslog */
/* #undef USE_SYSLOG */

/* #undef ERR_REPORTING_STDOUT */

/* #undef ERR_REPORTING_SYSLOG */

/* define ERR_REPORTING_FILE to have messages sent to file */
#define ERR_REPORTING_FILE "srtp.log"

/*
 * set ENABLE_DEBUGGING to 1 to compile in dynamic debugging system,
 * set it to 0 to not compile in dynamic debugging (for a slight
 * performance improvement)
 */
#define ENABLE_DEBUGGING 1

/* if we're going to use GDOI, define SRTP_GDOI to 1      */
/* #undef SRTP_GDOI */

/*
 * CPU_type is defined as 1 if the host processor is of that type.
 * Note that more than one type can be defined at once; this is so
 * that special instructions and other optimizations can be handled
 * independently.
 *
 * CPU_RISC     RISC machines (assume slow byte access)
 * CPU_CISC     CISC machines (e.g. Intel)
 * CPU_ALTIVEC  Motorola's SIMD instruction set
 *
 */
#ifdef __BIG_ENDIAN__ /* PPC processor */
#define CPU_RISC     1
#define CPU_CISC     0
#define CPU_ALTIVEC  1
#else
#define CPU_RISC     0
#define CPU_CISC     1
#define CPU_ALTIVEC  0
#endif

/*
 * if /dev/random is available, then DEV_RANDOM == 1
 *
 * /dev/random is a (true) random number generator which is
 * implemented in many modern operating systems
 */
/* #undef DEV_RANDOM */

/* whether to use ismacryp code */
/* #undef GENERIC_AESICM */

#endif /* CONFIG_H */




