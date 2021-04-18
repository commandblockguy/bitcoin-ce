# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= MINER
ICON ?= icon.png
DESCRIPTION ?= "Bitcoin Miner"
COMPRESSED ?= NO
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
