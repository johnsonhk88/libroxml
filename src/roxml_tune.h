/** \file roxml_tune.h
 *  \brief header for libroxml.so
 *         
 * This is the header file used to tune libroxml.
 * It contains the default options taht should fit
 * all standard systems.
 * Change this file only if you *really* know what you are doing.
 * \author blunderer <blunderer@blunderer.org>
 * \date 11 May 2012
 *
 * Copyright (C) 2012 blunderer
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * The author added a static linking exception, see License.txt.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/** \page roxml_tune tuning libroxml
 *
 * Being dedicated to embedded systems, libroxml include the ability to tune its behaviour.
 * files \file roxml_tune.h, \file roxml_tiny.h and \file site.in are used for that purpose.
 * 
 * \section tune <roxml_tune>
 * This file describe all options that can be used. Each consist of a #define CONFIG_XML_xxx set to
 * either 0 or 1. This file describes default values and should not be modified but to had options. 
 *
 * The configure mecanism will be used to select which files needs to be compiled in. The roxml_stub 
 * will provide an empty stub API when function is disabled to make sure the library interface stays
 * consistent.
 *
 * \section tune_end </roxml_tune>
 *
 */

#ifndef ROXML_TUNE_H
#define ROXML_TUNE_H

#ifdef ROXML_DEF_H
#error "roxml_defines.h must be included after roxml_tune.h to be tuned."
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*************** Define the default behaviour **********************/

/* Is the target library thread safe or not
 * This option should be activated.
 * If unsure, set to 1
 */
#ifndef CONFIG_XML_THREAD_SAFE
#if(CONFIG_XML_HEAP_BUFFERS==1)
#error "The CONFIG_XML_THREAD_SAFE option is not compatible with CONFIG_XML_HEAP_BUFFERS. Disable one of those."
#endif
#define CONFIG_XML_THREAD_SAFE	1
#endif /* CONFIG_XML_THREAD_SAFE */

/* Is the target library uses stack for internal
 * buffers or heap. Stack require some RAM while heap
 * can cut off performances.
 * This option should not be activated.
 * If unsure, set to 0
 */
#ifndef CONFIG_XML_HEAP_BUFFERS
#define CONFIG_XML_HEAP_BUFFERS	0
#endif /* CONFIG_XML_HEAP_BUFFERS */

/* Define the size of internal buffer.
 * For very small systems, large internal buffers
 * can cause the systeml the behave strangely.
 * This option should not be activated.
 * If unsure, set to 0
 */
#ifndef CONFIG_XML_SMALL_BUFFER
#define CONFIG_XML_SMALL_BUFFER	0
#endif /* CONFIG_XML_SMALL_BUFFER */

/* Limit the size of input XML libroxml can handle
 * to 64kb instead of 4Gb.
 * Setting this option will reduce the size of RAM loaded tree
 * This option should not be activated.
 * If unsure, set to 0
 */
#ifndef CONFIG_XML_SMALL_INPUT_FILE
#define CONFIG_XML_SMALL_INPUT_FILE	0
#endif /* CONFIG_XML_SMALL_INPUT_FILE */

/*************** Implement HEAP/STACK BUFFERS **********************/
#if(CONFIG_XML_HEAP_BUFFERS==1)
	#define ROXML_GET_BASE_BUFFER(name) char *name = malloc(ROXML_BASE_LEN)
	#define ROXML_PUT_BASE_BUFFER(name) free(name)
#else /* CONFIG_XML_HEAP_BUFFERS==0 */
	#define ROXML_GET_BASE_BUFFER(name) char name[ROXML_BASE_LEN]
	#define ROXML_PUT_BASE_BUFFER(name)
#endif /* CONFIG_XML_HEAP_BUFFERS */

/**************** Implement THREAD SAFETY **************************/
#if(CONFIG_XML_THREAD_SAFE==0)
	#define pthread_t			char
	#define pthread_mutex_t			char

	#define pthread_self()			0
	#define pthread_mutex_init(a, b)
	#define pthread_mutex_lock(a)		*(a) = 0
	#define pthread_mutex_unlock(a)		*(a) = 0
	#define pthread_mutex_destroy(a)
#else /* CONFIG_XML_THREAD_SAFE==1 */
	#if defined(_WIN32)
		#include "roxml_win32_native.h"
	#else
		#include <pthread.h>
	#endif
#endif /* CONFIG_XML_THREAD_SAFE */

#endif /* ROXML_TUNE_H */
