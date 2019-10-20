#!/bin/bash

export DYLD_LIBRARY_PATH=/Users/kyazdani/Documents/malloc
export DYLD_INSERT_LIBRARIES=/Users/kyazdani/Documents/malloc/libft_malloc.so
export DYLD_FORCE_FLAT_NAMESPACE=1
$@
