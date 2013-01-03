/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\signal.h
    @brief A way to set handlers for exceptional conditions (also known as signals)
*/
#ifndef INCLUDE_LIB_SIGNAL_H
#define INCLUDE_LIB_SIGNAL_H

/*
 * The actual signal values. Using other values with signal
 * produces a SIG_ERR return value.
 */
/** @brief Signal: Interactive attention (press CTRL-C , ...) */
#define	SIGINT		2
/** @brief Signal: Illegal instruction */
#define	SIGILL		4
/** @brief Signal: Floating point error */
#define	SIGFPE		8
/** @brief Signal: Segmentation violation */
#define	SIGSEGV		11
/** @brief Signal: Termination request */
#define	SIGTERM		15
/** @brief Signal: Control-break */
#define SIGBREAK	21
/** @brief Signal: Abnormal termination (abort) */
#define	SIGABRT		22

/** @brief Maximum signal number+1 */
#define NSIG 23

/*
 * A pointer to a signal handler function. A signal handler takes a
 * single int, which is the signal it handles.
 */
//typedef	void (*__p_sig_fn_t)(int);

/*
 * These are special values of signal handler pointers which are
 * used to send a signal to the default handler (SIG_DFL), ignore
 * the signal (SIG_IGN), indicate an error return (SIG_ERR),
 * get an error (SIG_SGE), or acknowledge (SIG_ACK).
 */
//#define	SIG_DFL	((__p_sig_fn_t) 0)
//#define	SIG_IGN	((__p_sig_fn_t) 1)
//#define	SIG_ERR ((__p_sig_fn_t) -1)
//#define SIG_SGE ((__p_sig_fn_t) 3)
//#define SIG_ACK ((__p_sig_fn_t) 4)

/*
 * Call signal to set the signal handler for signal sig to the
 * function pointed to by handler. Returns a pointer to the
 * previous handler, or SIG_ERR if an error occurs. Initially
 * unhandled signals defined above will return SIG_DFL.
 */
//__p_sig_fn_t signal(int, __p_sig_fn_t);


#endif
