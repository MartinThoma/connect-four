Take a look at `connectfour.c` and `connectfour-strange.c`.
They only differ in line 362.

But they give - apart from many "abcdefghijklm" - different results:

connectfour:

```console
[...]abcdefghijklmabcdefghijklm########################Finish:
Maximum of 20000 reached
alreadyCounter: 1547
mirroredCounter: 0

```

connectfour-strange:

```console
########################Finish:
Maximum of 20000 reached
alreadyCounter: 1566
mirroredCounter: 0
```
