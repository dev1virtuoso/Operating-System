/* SPDX-License-Identifier: GPL-2.0+ */

#define CONFIG_DEBUG_MAPLE_TREE
#define CONFIG_MAPLE_SEARCH
#define MAPLE_32BIT (MAPLE_NODE_SLOTS > 31)
#include "shared.h"
#include <stdlib.h>
#include <time.h>
#include "linux/init.h"
