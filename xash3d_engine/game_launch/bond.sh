#! /bin/sh

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
LD_LIBRARY_PATH="$SCRIPT_DIR" $SCRIPT_DIR/bond $*
