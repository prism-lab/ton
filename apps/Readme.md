# This directory contains applications only.

# List of apps:

## adjust-block
Adjusts block loaded from a .boc file by incrementing `vert_seqno` by _vs-incr_ (1 by default).

_//TODO: Добавить ссылку на ton.tex/2.1.17.-Correcting-invalid-shardchain-blocks. Но я не уверен, что оно реально сюда._

## create-state
Creates initial state for a TON blockchain, using configuration defined by Fift-language source files.

## dump-block
Dumps specified blockchain block or state from `boc-file`, or runs some tests.







---

# Contrib guideline:
If your application consists of only 1 file - put it  in this folder and use existing `CMakeLists.txt`.
Otherwise create a subfolder for it and use `add_subdirectory()` option in the existing `CMakeLists.txt`.
