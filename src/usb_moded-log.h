/**
 * @file usb_moded-log.h
 *
 * Copyright (c) 2010 Nokia Corporation. All rights reserved.
 * Copyright (c) 2016 - 2020 Jolla Ltd.
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * @author Philippe De Swert <philippe.de-swert@nokia.com>
 * @author Simo Piiroinen <simo.piiroinen@jollamobile.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the Lesser GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef  USB_MODED_LOG_H_
# define USB_MODED_LOG_H_

# include <stdbool.h>
# include <stdarg.h>
# include <syslog.h>

/* Logging functionality */

/* ========================================================================= *
 * Constants
 * ========================================================================= */

# define LOG_ENABLE_DEBUG      01
# define LOG_ENABLE_TIMESTAMPS 01
# define LOG_ENABLE_LEVELTAGS  01
# define LOG_ENABLE_CONTEXT    0

enum
{
    LOG_TO_STDERR, // log to stderr
    LOG_TO_SYSLOG, // log to syslog
};

enum
{
    LOG_MIN_LEVEL = LOG_CRIT,
    LOG_MAX_LEVEL = LOG_DEBUG,
};

/* ========================================================================= *
 * CONTEXT STACK
 * ========================================================================= */

# if LOG_ENABLE_CONTEXT
const char *context_enter(const char *func);
void        context_leave(void *aptr);

#  define LOG_REGISTER_CONTEXT\
     __attribute__((cleanup(context_leave))) const char *qqq =\
         context_enter(__func__)
#else
# define LOG_REGISTER_CONTEXT\
     do{}while(0)
#endif

/* ========================================================================= *
 * Prototypes
 * ========================================================================= */

/* ------------------------------------------------------------------------- *
 * LOG
 * ------------------------------------------------------------------------- */

void        log_emit_va     (const char *file, const char *func, int line, int lev, const char *fmt, va_list va);
void        log_emit_real   (const char *file, const char *func, int line, int lev, const char *fmt, ...) __attribute__((format(printf, 5, 6)));
void        log_debugf      (const char *fmt, ...);
int         log_get_level   (void);
void        log_set_level   (int lev);
bool        log_p           (int lev);
int         log_get_type    (void);
void        log_set_type    (int type);
const char *log_get_name    (void);
void        log_set_name    (const char *name);
void        log_set_lineinfo(bool lineinfo);
bool        log_get_lineinfo(void);
void        log_init        (void);

/* ========================================================================= *
 * Macros
 * ========================================================================= */

# define log_emit(LEV, FMT, ARGS...) do {\
    if( log_p(LEV) ) {\
        log_emit_real(__FILE__,__FUNCTION__,__LINE__, LEV, FMT, ##ARGS);\
    }\
} while(0)

# define log_crit(    FMT, ARGS...)   log_emit(LOG_CRIT,    FMT, ##ARGS)
# define log_err(     FMT, ARGS...)   log_emit(LOG_ERR,     FMT, ##ARGS)
# define log_warning( FMT, ARGS...)   log_emit(LOG_WARNING, FMT, ##ARGS)

# if LOG_ENABLE_DEBUG
#  define log_notice( FMT, ARGS...)   log_emit(LOG_NOTICE,  FMT, ##ARGS)
#  define log_info(   FMT, ARGS...)   log_emit(LOG_INFO,    FMT, ##ARGS)
#  define log_debug(  FMT, ARGS...)   log_emit(LOG_DEBUG,   FMT, ##ARGS)
# else
#  define log_notice( FMT, ARGS...)   do{}while(0)
#  define log_info(   FMT, ARGS...)   do{}while(0)
#  define log_debug(  FMT, ARGS...)   do{}while(0)

#  define log_debugf( FMT, ARGS...)   do{}while(0)
# endif

#endif /* USB_MODED_LOG_H_ */
