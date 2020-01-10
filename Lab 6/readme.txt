Brief After-Action Report:

1. Issues:

One error that I encountered was when I was trying to create the pthreads and had an
issue with passing the 4th argument into pthread_create. For the 4th arg, passed in
'(void*) it' but I receieved the error saying:
    warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
which resulted in a segmentation fault when I ran 'make clean check'. I then tried to
pass in just the address of the int using '&it' but this resulted in the error:
    make: *** [check] Error 1
I finally resolved this issue by creating an array of integers and passing in the 
address of the array element as the 4th argument.

Another issue that I ran into was the following errors stating:
    error: subscripted value is neither array nor pointer nor vector
                 pixel_arr[px][py][it] = scaled_color[it];
    error: subscripted value is neither array nor pointer nor vector
       pixel_arr[x][y][0], pixel_arr[x][y][1], pixel_arr[x][y][2]);
At first, I didn't understand what this error meant as I had declared pixel_arr as an
array, but the error was saying that it wasn't. After comparing the declaration of
pixel_arr with the initialization of the elements in it, I realized that I had declared
the array improperly as the array must have its size declared at compile time. I resolved
this by declaring pixel_arr[width][height][3].


2. Results:

----- OUTPUT OF UNMODIFIED (BENCHMARK) VERSION -----
time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m35.955s
user	0m35.950s
sys	0m0.000s

----- OUTPUT OF MODIFIED VERSION -----
time ./srt 1-test.ppm >1-test.ppm.tmp
real	0m35.811s
user	0m35.801s
sys	0m0.003s

time ./srt 2-test.ppm >2-test.ppm.tmp
real	0m18.053s
user	0m35.905s
sys	0m0.002s

time ./srt 4-test.ppm >4-test.ppm.tmp
real	0m12.586s
user	0m43.068s
sys	0m0.005s

time ./srt 8-test.ppm >8-test.ppm.tmp
real	0m9.050s
user	0m35.924s
sys	0m0.008s

Based on the following results, my implementation of SRT improved the performance by
about 4x the original implementation as the real time was 1/4th of the original time.
As the number of threads increased by a factor of 2, the real time was decreasing by
1/2, 1/3, and 1/4 of its previous time. For example, with 1 thread, the real time was
about 36 seconds and with 2 threads, the real time was about 18 seconds i.e the real time
was halved (1/2). For 4 threads, the real time decreased about 1/3 of the real time for
2 threads (1/3 of 18s = 6s and 18s - 6s = 12s). Similarly for 8 threads, the real time
decreased by 1/4th of the real time for 4 threads (1/4 of 12s = 3s and 12s - 3s = 9s).
 
