/* Copyright (c) 2008-2009, 2011, 2013 The Linux Foundation.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LINUX_REMOTE_SPINLOCK_H
#define __LINUX_REMOTE_SPINLOCK_H

#include <linux/spinlock.h>
#include <linux/mutex.h>

#include <asm/remote_spinlock.h>

typedef struct {
	spinlock_t local;
	_remote_spinlock_t remote;
} remote_spinlock_t;

#define remote_spin_lock_init(lock, id) \
	({ \
		spin_lock_init(&((lock)->local)); \
		_remote_spin_lock_init(id, &((lock)->remote)); \
	})
#define remote_spin_lock(lock) \
	do { \
		spin_lock(&((lock)->local)); \
		_remote_spin_lock(&((lock)->remote)); \
	} while (0)
#define remote_spin_unlock(lock) \
	do { \
		_remote_spin_unlock(&((lock)->remote)); \
		spin_unlock(&((lock)->local)); \
	} while (0)
#define remote_spin_lock_irqsave(lock, flags) \
	do { \
		spin_lock_irqsave(&((lock)->local), flags); \
		_remote_spin_lock(&((lock)->remote)); \
	} while (0)
#define remote_spin_unlock_irqrestore(lock, flags) \
	do { \
		_remote_spin_unlock(&((lock)->remote)); \
		spin_unlock_irqrestore(&((lock)->local), flags); \
	} while (0)
#define remote_spin_trylock(lock) \
	({ \
		spin_trylock(&((lock)->local)) \
		? _remote_spin_trylock(&((lock)->remote)) \
			? 1 \
			: ({ spin_unlock(&((lock)->local)); 0; }) \
		: 0; \
	})
#define remote_spin_trylock_irqsave(lock, flags) \
	({ \
		spin_trylock_irqsave(&((lock)->local), flags) \
		? _remote_spin_trylock(&((lock)->remote)) \
			? 1 \
			: ({ spin_unlock_irqrestore(&((lock)->local), flags); \
				0; }) \
		: 0; \
	})

#define remote_spin_release(lock, pid) \
	_remote_spin_release(&((lock)->remote), pid)

#define remote_spin_release_all(pid) \
	_remote_spin_release_all(pid)

#define remote_spin_owner(lock) \
	_remote_spin_owner(&((lock)->remote))

typedef struct {
	struct mutex local;
	_remote_mutex_t remote;
} remote_mutex_t;

#define remote_mutex_init(lock, id) \
	({ \
		mutex_init(&((lock)->local)); \
		_remote_mutex_init(id, &((lock)->remote)); \
	})
#define remote_mutex_lock(lock) \
	do { \
		mutex_lock(&((lock)->local)); \
		_remote_mutex_lock(&((lock)->remote)); \
	} while (0)
#define remote_mutex_trylock(lock) \
	({ \
		mutex_trylock(&((lock)->local)) \
		? _remote_mutex_trylock(&((lock)->remote)) \
			? 1 \
			: ({mutex_unlock(&((lock)->local)); 0; }) \
		: 0; \
	})
#define remote_mutex_unlock(lock) \
	do { \
		_remote_mutex_unlock(&((lock)->remote)); \
		mutex_unlock(&((lock)->local)); \
	} while (0)

#endif
