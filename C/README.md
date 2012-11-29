Take a look at `connectfour.c` and `connectfour-strange.c`.
They only differ in line 336.

But they give - apart from many "abcdefghijklm" - different results:

connectfour:

```console
[...]abcdefghijklmabcdefghijklm########################Finish:
Maximum of 20000 reached
alreadyCounter: 64
```

connectfour-strange:

```console
########################Finish:
Maximum of 20000 reached
alreadyCounter: 65
```
