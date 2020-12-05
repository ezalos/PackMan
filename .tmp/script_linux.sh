#!/bin/sh

export LD_PRELOAD="./libft_malloc.so"

export HUGETLB_MORECORE=yes

$@
