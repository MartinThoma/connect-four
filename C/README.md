Related SO Questions
--------------------
 * [Why does printing a constant change the results of a single core / process application?](http://stackoverflow.com/q/13631776/562769)

Results
========
O0 vs. O3
---------
```console
Opitimization: O0
Maximum: 200000
alreadyCounter: 51915
mirroredCounter: 1

real	4m30.090s
user	4m6.811s
sys	0m0.416s
```

```console
Opitimization: O3
Probing: linear
Maximum: 200000
alreadyCounter: 51915
mirroredCounter: 1

real	0m57.472s
user	0m52.711s
sys	0m0.044s
```

Different MAXIMUM_SITUATIONS
----------------------------
```console
Probing: linear
Maximum of 200000 reached
alreadyCounter: 51915
mirroredCounter: 0

real	3m13.573s
user	3m12.592s
sys	0m0.660s
```

```console
Probing: quadratic
- (didn't finish after 15 minutes)
```
