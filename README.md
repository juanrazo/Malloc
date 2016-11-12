# os-malloc

Juan Razo

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
the paths the code I added. 

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
  checking from 0x8f23110, size=     104, allocated=0...#####<--------------- p2 freed<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
  checking from 0x8f23000, size=     320, allocated=1...			<--------------- p1 resized<br />
  checking from 0x8f23150, size=      40, allocated=0...<br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size= 1047896, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 8f23008 	p3: 8f23190										<--------------- p1 at same address and incresed size<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x8f23000, size=     376, allocated=0...   <---------------- p1 resized & deallocate <br />
  checking from 0x8f23188, size=     256, allocated=1...<br />
  checking from 0x8f23298, size=     600, allocated=1...   <---------------- p1 copied to different address<br />
  checking from 0x8f23500, size= 1047280, allocated=0...<br />
 mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 8f232a0 	p3: 8f23190                          <----------- moved p1 after p3 and copied<br />
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

#### For the testing of next fit I used 6 pointers:
#### p1 = 254, p2 = 100, p3 = 254, p4 = 100, p5 = 104776 and p6 = 80
####

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
p1: 92cc008 	p2: 92cc118 	p3: 92cc190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=0...<br />
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
  checking from 0x92cc298, size=     104, allocated=1...<br />
  checking from 0x92cc310, size= 1047776, allocated=0...<br />
 mcheck: numBlocks=5, amtAllocated=0k, amtFree=0k, arenaSize=1023k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p2:%8 	p3: 92cc118 	p4: 92cc190<br />
---------------------------Addresses----------------------------<br />
<br />
  checking from 0x92cc000, size=     256, allocated=1...<br />
  checking from 0x92cc110, size=     104, allocated=0...<br />
  checking from 0x92cc188, size=     256, allocated=1...<br />
  checking from 0x92cc298, size=     104, allocated=1...<br />
  checking from 0x92cc310, size= 1047776, allocated=1...<br />
 mcheck: numBlocks=5, amtAllocated=1023k, amtFree=0k, arenaSize=0k<br />
<br />
<br />
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p3: 92cc190 	p4: 92cc2a0 	p5: 92cc318<br />
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
---------------------------Addresses----------------------------<br />
p1: 92cc008 	p3: 92cc190 	p4: 92cc2a0 	p5: 92cc318 	p6: 92cc118<br />
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


