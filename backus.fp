-- Some programs from Backus' paper [CACM, Aug 78, vol 21, #8]
--

-- Inner product (5.2)
{IP '+@&*@trans}
IP:<<1,2,3>,<6,5,4>>

-- Examples of applications (11.2.2)
+:<1,2>
tl:<A,B,C>
1:<A,B,C>
2:<A,B,C>

-- last (11.2.5)
{last (null@tl->1;last@tl)}
last:<1,2>

-- Factorial (11.3.1)
{eq0 eq@[id,%0]}
{sub1 -@[id,%1]}
{fac (eq0->%1;*@[id,fac@sub1])}
fac:2

-- Matrix multiply (11.3.3)
{MM &&IP @ &distl @ distr @ [1,trans@2]}

--   1 2 3   1 0 1    6 0  4
--   4 5 6 * 1 0 0 = 15 0 10
--   7 8 9   1 0 1   24 0 16

MM:<<<1,2,3>,<4,5,6>,<7,8,9>>, <<1,0,1>,<1,0,0>,<1,0,1>>>
