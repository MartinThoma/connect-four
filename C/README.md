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

Probing
-------
```console
Opitimization: O3
Probing: linear
Maximum: 200000
  alreadyCounter: 51915
  mirroredCounter: 1
  hashMiss: 1,690,100,514 (approx 1.6 * 10^9)

real	0m52.056s
user	0m51.723s
sys	0m0.176s
```

```console
Opitimization: O3
Probing: quadratic
Maximum: 200000
  alreadyCounter: 49920
  mirroredCounter: 1
  hashMiss: 258511701
  Status: 191740 of 200000
real	36m6.886s
user	35m43.874s
sys	0m3.992s
```

Array Size
----------
```console
Opitimization: O3
Probing: linear
Maximum: 2000000
  alreadyCounter: 526262
  mirroredCounter: 41
  hashMiss: 3,036,219,459 (approx 3.0 * 10^9)

real	63m52.766s
user	59m56.237s
sys	0m8.621s
```


```console
Opitimization: O3
Probing: linear
Maximum: 20,000,000
  alreadyCounter: 4,009,244
  mirroredCounter: 202
  hashMiss: 126,038,750
Status: 15,243,706 of 20,000,000

Shutting down

real	125m5.717s
user	111m35.798s
sys	0m35.854s
```
