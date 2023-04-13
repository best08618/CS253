### Expected Output
Expected word frequency results of pride-and-prejudice.txt look like below
```
mr  -  786
elizabeth  -  635
very  -  488
darcy  -  418
such  -  395
mrs  -  343
much  -  329
more  -  327
bennet  -  323
bingley  -  306
jane  -  295
miss  -  283
one  -  275
know  -  239
before  -  229
herself  -  227
though  -  226
well  -  224
never  -  220
sister  -  218
soon  -  216
think  -  211
now  -  209
time  -  203
good  -  201
```

## Week1 
Run following commands and it will return (print) most frequent 25 terms. 

```
cd week_1
g++ fq_week1.cpp -o fq
./fq ../pride-and-prejudice.txt
```

## Week2
### Exercise 4.1 
monolith style 
```
cd week_2
g++ Four.cpp -o four
./four ../pride-and-prejudice.txt 
```

### Exercise 5.1 
cookbook style
```
cd week_2
g++ Five.cpp -o five
./five ../pride-and-prejudice.txt 
```
### Exercise 6.1 
Pipeline Style
```
cd week_2
gcc Six.c -o six 
./six ../pride-and-prejudice.txt 
```

### Exercise 7.1 
Code golf Style
```
cd week_2
g++ Seven.cpp -o seven
./seven ../pride-and-prejudice.txt 
```