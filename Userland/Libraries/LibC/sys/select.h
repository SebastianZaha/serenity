/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// Includes essentially mandated by POSIX:
// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_select.h.html
#include <signal.h>
#include <time.h>

#include <fd_set.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/time.h>
#include <sys/types.h>

__BEGIN_DECLS

int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);
int pselect(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timespec* timeout, sigset_t const* sigmask);

__END_DECLS
