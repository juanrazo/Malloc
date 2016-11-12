# os-malloc

###Juan Razo

# Section 1 Introduction
I started this lab by reading the file myAllocator.c and getting a basic understanding of next-fit from a power point at 
https://courses.cs.washington.edu/courses/cse351/10sp/lectures/15-memallocation.pdf. I manually computed align8 in binary to 
get a good understanding of how it worked. After good understanding of firstFitAllocRegion I was able to implement
nextFitAclloc by adding a global variable to store the last preFix (nextFit) and search from the nextFit when called the next 
time. If nextFit reaches the end then go back to the begining and look once more before increasing the arena. When running
a test on nextFit I found that growArena never increases the arena, even when the growingDisabled is not set. For resizeRegion 
I get the size difference and align8(size difference) to evaluate the successor of the old region. If the size difference plus
the prefix and suffix fit in the adjacent region then set r->allocated to 0 so I can coalscePrev() both regions and set the 
new region as allocated. This will grow the old region with an adjacent regioin, ofcourse it will grow to the size passed in 
the parameter increase. If the size difference does not fit in the adjacent block then call firstFitAllocRegion and copy the 
old region into the new one, just as resize did before. To test my functions I tried to do Path testing. I tried to cover all
the paths to the code I added. 

# Section 2 Testing resizeRegion()
#### To test resize I create a file testResize.c and created used 3 pointers. For p1 I allocate 254, p2 = 100 and p3 = 254. I then free p2 and resize p1 to 300. This means that resize should increase the size to the adjecent block which was where p2 was at. Below we can see the addresses being used whe using resize:

[student@localhost os-malloc-lab-juanrazo]$ ./testResize  <br />
 mcheck: numBlocks=0, amtAllocated=0k, amtFree=0k, arenaSize=0k <br />
  checking from 0x8f23000, size=     256, allocated=1... <br />
  checking from 0x8f23110, size= 1048288, allocated=0...<br />
 mcheck: numBlocks=2, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x8f23000, size=     256, allocated=1...<br />
  checking from 0x8f23110, size=     104, allocated=1...<br />
  checking from 0x8f23188, size= 1048168, allocated=0...<br />
 mcheck: numBlocks=3, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 8f23008 	p2: 8f23118 	p3: 8f23190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x8f23000, size=     256, allocated=1...<br />
  checking from 0x8f23110, size=     104, allocated=1...<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x8f23000, size=     256, allocated=1...<br />
  checking from 0x8f23110, size=     104, allocated=0... __<--------------- p2 freed__<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x8f23000, size=     320, allocated=1...		__<--------------- p1 resized__<br />
  checking from 0x8f23150, size=      40, allocated=0...<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 8f23008 	p3: 8f23190										__<--------------- p1 at same address and incresed size__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x8f23000, size=     376, allocated=0... __<---------------- p1 resized & deallocate__<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size=     600, allocated=1... __<---------------- p1 copied to different address__<br />
  checking from 0x8f23500, size= 1047280, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 8f232a0 	p3: 8f23190                        __<----------- moved p1 after p3 and copied__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x8f23000, size=     376, allocated=0...<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size=     600, allocated=1...<br />
  checking from 0x8f23500, size= 1047280, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 8f232a0 	p3: 8f23190<br />
---------------------------Addresses----------------------------<br />
<br />
<br />
---------------------------BEGIN TO FREE----------------------------<br />
<br />
  checking from 0x8f23000, size=     648, allocated=0...<br />
  checking from 0x8f23298, size=     600, allocated=1...<br />
  checking from 0x8f23500, size= 1047280, allocated=0...<br />
 mcheck: numBlocks=3, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x8f23000, size= 1048560, allocated=0...<br />
 mcheck: numBlocks=1, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
10000 firstFitAllocRegion(4) required 0.371000 seconds<br />
[student@localhost os-malloc-lab-juanrazo]$ <br />

# Section 3 Testing nextFit
#### For the testing of next fit I used 6 pointers:
#### p1 = 254, p2 = 100, p3 = 254, p4 = 100, p5 = 104776 and p6 = 80
#### I begin by allocating p1, p2, and p2. Then free p2 and allocate p4 to see if the addressed used is after p3. This would indicate that nextFit will not begin to look at the begining of arena and will instead begin to look at p3. As shown below we can see the address is after p3. Passing the test where nextFit begins to look at the previous address. For the next test I allocate p5 which alloacates the rest of the arena. Now on the next allocate which is p6 it should loop back to the begining of the arena and becuase p2 was freed at after it was allocated, there should be a free space to fit p6. We can see that p6 gets allocated where p2 was at. This means nestFit looped around and found space for p6 and takes care of the rest of my code. This should cover just about all paths except for calling nextFit once more and increasing the arena. I will include what happens when I call nextFit on my next section. Basically growArena returns 0 when I was calling it.

<br />
[student@localhost os-malloc-lab-juanrazo]$ ./testNextFit <br />
 mcheck: numBlocks=0, amtAllocated=0k, amtFree=0k, arenaSize=0k<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size= 1048288, allocated=0...<br />
 mcheck: numBlocks=2, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=1...<br />
  checking from 0x92cc188, size= 1048168, allocated=0...<br />
 mcheck: numBlocks=3, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=1...<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p2: 92cc118 	p3: 92cc190 __<----------------- allocate p1, p2, p3__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=0... __<----------------- free p2__<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p2: 92cc118 	p3: 92cc190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=0...<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size=     104, allocated=1... __<--------------- allocate p4__<br />
  checking from 0x92cc310, size= 1047776, allocated=0...<br />
 mcheck: numBlocks=5, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p2:%8 	p3: 92cc118 	p4: 92cc190    __<--------------- allocate p4__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=0...<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size=     104, allocated=1...<br />
  checking from 0x92cc310, size= 1047776, allocated=1... __<--------------- allocate p5__<br />
 mcheck: numBlocks=5, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p3: 92cc190 	p4: 92cc2a0 	p5: 92cc318 __<--------------- allocate p5__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=      80, allocated=1... __<--------------- allocate p6__<br />
  checking from 0x92cc170, size=       8, allocated=0...<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size=     104, allocated=1...<br />
  checking from 0x92cc310, size= 1047776, allocated=1...<br />
 mcheck: numBlocks=6, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p3: 92cc190 	p4: 92cc2a0 	p5: 92cc318 	p6: 92cc118 __<--------------- allocate p6__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=      80, allocated=1...<br />
  checking from 0x92cc170, size=       8, allocated=0...<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size=     104, allocated=1...<br />
  checking from 0x92cc310, size= 1047776, allocated=1...<br />
 mcheck: numBlocks=6, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
<br />
---------------------------BEGIN TO FREE----------------------------<br />
<br />
  checking from 0x92cc000, size= 1048560, allocated=0...<br />
 mcheck: numBlocks=1, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
10000 nextFitAllocRegion(4) required 0.369000 seconds<br />
[student@localhost os-malloc-lab-juanrazo]$ <br />


# Section 4 growArena bug
#### This is what happens when I use growArena. Is the same test as Section 3 with the exception that p6 is now 800 and is more than what is free on the arena. I was expecting for the arena to grow but insead I get an address of 0.
#### For the testing of next fit I used 6 pointers:
#### p1 = 254, p2 = 100, p3 = 254, p4 = 100, p5 = 104776 and p6 = 800

[student@localhost os-malloc-lab-juanrazo]$ ./testNextFit <br />
 mcheck: numBlocks=0, amtAllocated=0k, amtFree=0k, arenaSize=0k<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size= 1048288, allocated=0...<br />
 mcheck: numBlocks=2, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=1...<br />
  checking from 0x9a81188, size= 1048168, allocated=0...<br />
 mcheck: numBlocks=3, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=1...<br />
  checking from 0x9a81188, size=     256, allocated=1...<br />
  checking from 0x9a81298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 9a81008 	p2: 9a81118 	p3: 9a81190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=0...<br />
  checking from 0x9a81188, size=     256, allocated=1...<br />
  checking from 0x9a81298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 9a81008 	p2: 9a81118 	p3: 9a81190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=0...<br />
  checking from 0x9a81188, size=     256, allocated=1...<br />
  checking from 0x9a81298, size=     104, allocated=1...<br />
  checking from 0x9a81310, size= 1047776, allocated=0...<br />
 mcheck: numBlocks=5, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 9a81008 	p2:%8 	p3: 9a81118 	p4: 9a81190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=0...<br />
  checking from 0x9a81188, size=     256, allocated=1...<br />
  checking from 0x9a81298, size=     104, allocated=1...<br />
  checking from 0x9a81310, size= 1047776, allocated=1...<br />
 mcheck: numBlocks=5, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 9a81008 	p3: 9a81190 	p4: 9a812a0 	p5: 9a81318<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=0...<br />
  checking from 0x9a81188, size=     256, allocated=1...<br />
  checking from 0x9a81298, size=     104, allocated=1...<br />
  checking from 0x9a81310, size= 1047776, allocated=1...<br />
 mcheck: numBlocks=5, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 9a81008 	p3: 9a81190 	p4: 9a812a0 	p5: 9a81318 	p6:       0 __<--------------- allocate p6__<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x9a81000, size=     256, allocated=1...<br />
  checking from 0x9a81110, size=     104, allocated=0...<br />
  checking from 0x9a81188, size=     256, allocated=1...<br />
  checking from 0x9a81298, size=     104, allocated=1...<br />
  checking from 0x9a81310, size= 1047776, allocated=1...<br />
 mcheck: numBlocks=5, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
<br />
---------------------------BEGIN TO FREE----------------------------<br />
<br />
  checking from 0x9a81000, size= 1048560, allocated=0...<br />
 mcheck: numBlocks=1, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
10000 nextFitAllocRegion(4) required 0.364000 seconds<br />
[student@localhost os-malloc-lab-juanrazo]$ <br />
