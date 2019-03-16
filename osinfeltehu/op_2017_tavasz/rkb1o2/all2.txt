module examples1
import StdEnv 

// Start = 4+5 // 9
// Start = 42 // 42
// Start = 3+10*2 // 23
// Start = sqrt 3.0 // 1.73...

double x = x + x
quadruple x = double (double x)

// Start = double 2
// Start = quadruple 2

// factorial n = prod [1 .. n]
// Start = factorial 5

// two cases 
abs1 x
| x<0 = ~x
| otherwise = x

// Start = abs1 -4 // 4

// otherwise can be omitted 
abs2 x
| x<0 = ~x
= x

// Start = abs2 4 // 4

// more then two guards or cases
signof x
| x>0 = 1
| x==0 = 0
| x<0 = -1
 
// Start =  signof -8 // -1

factor :: Int -> Int
factor n
| n==0 = 1
| n>0 = n * factor (n-1)

// Start = factor 5

// function compositions
odd x = not (isEven x)

// Start = odd 23 // True

twiceof :: (a -> a) a -> a
twiceof f x = f (f x)

// Start = twiceof inc 0

Twice :: (t->t) -> (t->t)
Twice f = f o f

// Start = Twice inc 2 // 4

// Exercises

// 1. Check if an integer is even - in two ways. To divide integer use /, for remainder use rem
even1 :: Int -> Bool
even1 x 
| (x / 2) * 2 == x = True
= False

// Start = even1 6 // True

even2 :: Int -> Bool
even2 x
| x rem 2 == 0 = True
= False

// Start = even2 5 // False


// 2. Write a function that takes two arguments, say n and x, and computes their power.
power :: Int Int -> Int
power x n
| n==0 = 1
| n>0 = x * power x (n-1)

// Start = power 2 5 // 32


// 3. use 2. to construct a function that squares its argument

sq :: Int -> Int
sq x = power x 2

// Start = sq 4 // 16


// 4. Define the function isum :: Int -> Int which adds the digits of its argument.
isum :: Int -> Int 
isum x 
| x == 0 = 0
= (x rem 10) + isum (x / 10)

// Start = isum 123 // 6


// 5. Use the function isum to check whether a number can be divided by 9.
div9 :: Int -> Bool 
div9 x 
| (isum x) rem 9 == 0 = True
= False

// Start = div9 18 // True


// 6. Define a function maxi with two arguments that delivers the maximum of the two.
maxi :: Int Int -> Int 
maxi x y
| x > y = x
= y

// Start = maxi 45 3 // 45


// 7. Define a function mini that has two arguments that delivers the minimum of the two.
mini :: Int Int -> Int 
mini x y
| x < y = x
= y

//Start = mini 4 6 // 4







module examples2

import StdEnv

l1 :: [Int]
l1 = [1, 2, 3, 4, 5]
l2 :: [Bool]
l2 = [True, False, True]
l3 :: [Real->Real]
l3 = [sin, cos, sin]
l4 :: [[Int]]
l4 = [[1, 2, 3], [8, 9]]
l5 :: [a]
l5 = []
l6 :: [Int]
l6 = [1..10]
l7 :: [Int]
l7 = [1..]

// Start = l4

/* [1..10]     
   [1,2..10]   
   [1,0.. -10] 
   [1.. -10]   
   [1..0]      
   [1..1]      
   [1,3..4]  
   [1..]       
   [1,3..]     
   [100,80..]  */

// Start = [1,2..10]

// Start = hd [1, 2, 3, 4, 5]       
// Start = tl [1, 2, 3, 4, 5]      
// Start = drop 2 [1, 2, 3, 4, 5]   
// Start = take 2 [1, 2, 3, 4, 5]   
// Start = [1, 2, 3] ++ [6, 7]      
// Start = reverse [1, 2, 3]        
// Start = length [1, 2, 3, 4]      
// Start = last [1, 2, 3]           
// Start = init [1, 2, 3]           
// Start = isMember 2 [1, 2, 3]     
// Start = isMember 5 [1, 2, 3]     
// Start = flatten [[1,2], [3, 4, 5], [6, 7]]  

// Start = take 2 []                
// Start = drop 5 [1,2,3]           
// Start = take 2 [1 .. 10]         
// Start = drop ([1..5]!!2) [1..5]  

// Start = reverse [1,3..10]           
// Start = reverse [5,4 .. -5]         
// Start = isMember 0 []               
// Start = isMember -1 [1..10]         
// Start = isMember ([1..5]!!1) [1..5] 

// some list patterns
triplesum :: [Int] -> Int
triplesum [x, y, z] = x + y + z

// Start = triplesum [1,2,4] // 7  [1,2,3,4] error
head :: [Int] -> Int
head [x : y] = x

// Start = head [1..5] // 1

tail :: [Int] -> [Int]
tail [x : y] = y

// Start = tail [1..5] // [2,3,4,5]

// omitting values
f :: Int Int -> Int
f _ x = x

// Start = f 4 5 // 5

// patterns with list constructor
g :: [Int] -> Int
g [x, y : z] =  x + y

// Start = g [1, 2, 3, 4, 5] // 3

// patterns + recursively applied functions
lastof :: [Int] -> Int
lastof [x] = x
lastof [x : y] = lastof y

// Start = lastof [1..10] // 10

// recursive functions on lists
sum1 :: [Int] -> Int
sum1 x
| x == [] = 0
| otherwise = hd x + sum1 (tl x)

// Start = sum1 [1..5] // 15 

sum2 :: [Int] -> Int
sum2 [] = 0
sum2 [first : rest] = first + sum2 rest

// Start = sum2 [1..5] // 15 

// recursive function with any element pattern
length1 :: [Int] -> Int
length1 [] = 0
length1 [_ : rest]= 1 + length1 rest

// Start = length1 [1..10] // 10

// Exercises

// 1. Evaluate the following expressions:

//Start = (take 3 [1..10]) ++ (drop 3 [1..10])

//Start = length (flatten [[1,2], [3], [4, 5, 6, 7], [8, 9]])

//Start = isMember (length [1..5]) [7..10]

//Start = [1..5] ++ [0] ++ reverse [1..5]

// 2. Generate lists 
// from -10 to 10
// Start = [ -10 .. 10 ]
// even numbers from 1 to 100
// Start = [2, 4 .. 100]
// every 10th element from 0 to 100
// Start = [0, 10 ..100]

// 3. Operations with lists: write functions for the followings
// keep the head of every sublist e.g. [[1, 2, 3], [3, 4], [5, 7, 8, 9]] -> [1, 3, 5]
heads :: [[Int]] -> [Int]
heads [] = []
heads [x : xs] = [ hd x : heads xs]

// Start = heads [[1, 2, 3], [3, 4], [5, 7, 8, 9]]

// 4. Keep the tails of a list in 2 versions 
// e.g. [[1, 2, 3], [3, 4], [5, 7, 8, 9]] -> [[2, 3], [4], [7, 8, 9]] 
tails :: [[Int]] -> [[Int]]
tails [] = []
tails [x : xs] = [ tl x : tails xs]

// Start = tails [[1, 2, 3], [3, 4], [5, 7, 8, 9]]

tailsd :: [[Int]] -> [[Int]]
tailsd [] = []
tailsd [x : xs] = [ (drop 1 x) : tailsd xs]

// Start = tailsd [[1, 2, 3], [3, 4], [5, 7, 8, 9]]

// 5. Rewrite flatten with ++
lc :: [[Int]] -> [Int]
lc [] = []
lc [x : xs] = x ++ lc xs

// Start = lc [[1, 2, 3], [3, 4], [5, 7, 8, 9]]

// 6. Test if a list is symmetrical
sim :: [Int] -> Bool
sim x 
| x == reverse x = True
= False

// Start = sim [1, 2, 1]

// 7. Keep the first 2 and the last 2 elements of a list
droptake2 :: [Int] -> [Int]
droptake2 x 
| length x < 4 = []
= (take 2 x) ++ (drop ((length x)-2) x)

// Start = droptake2 [1, 2, 3, 4, 5, 6, 7, 8, 9]
// Start = droptake2 [1, 2, 3]
// Start = droptake2 [1]
//Start = droptake2 [1, 2, 3, 4]




module examples3

import StdEnv

// 1. Compute the product of the elements of a list
product :: [Int] -> Int
product [] = 1
product [h:t] = h * product t

Start = product [1..5] // 120

// 2. delete the elements equal to 5
not_five :: [Int] -> [Int]
not_five  []  =  []
not_five  [5:t]  = not_five t 
not_five  [h:t]  = [h : not_five t] 

// Start = not_five [5,4,5,4,3]  // [4,4,3]

// 3. Delete an element n from a list
del :: Int [Int] -> [Int]
del _  []   =  []
del n  [h:t]   
|  n==h     = del n t 
del n [h:t] = [h : del n t]

// Start = del 5 [1, 5, 6, 7, 5, 8, 5] // [1, 6, 7, 8]


// 4. write a funtion with the patterns depending on the parameter:
// if the param is [] then is equal to 20, if is a two element list starting with 4 then is 30
// if is a two element list ending with 5 then is 40, in all other cases is 50, 
// the order of the patterns is important
gp :: [Int] -> Int
gp [] = 20
gp [4,_] = 30
gp [_,5] = 40
gp _ = 50 

// Start = gp [4, 6] // 30


// 5. write a funtion which returns true if a is divisible by b
div_by :: Int Int -> Bool
div_by a b = a rem b == 0

// Start = div_by 16 4      // True


// 6. write a funtion which returns true if a is divisible by b or vice versa
div_any :: Int Int -> Bool
div_any a b = div_by a b || div_by b a

// Start = div_any 4 16     // True


// 7. sumsq n returns 1*1 + 2*2 + ... + n*n - with a pattern for 0
sumsq :: Int -> Int
sumsq 0         = 0
sumsq n 
| n > 0 = sumsq (n-1) + n*n

// Start = sumsq 3 // 14


// version 2. - without pattern for 0
sums :: Int -> Int
sums n 
| n > 0 = sumsq (n-1) + n*n
= 0

// Start = sums 3 // 14
  

// 8. check if a number is palindrom e.g.12321
p :: Int -> [Int]
p x = digits x []

digits :: Int [Int] -> [Int]
digits 0 l = l
digits x l = digits (x/10) [x rem 10 : l]

pali :: Int -> Bool
pali x = y == reverse y
where 
    y = p x

// Start = pali 12321 // True
 

// 9. Computes the nth Fibonacci number - try more versions
fib :: Int -> Int
fib 0 = 1
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)

// Start = fib 5 // 8

fib1 :: Int -> (Int, Int)
fib1 0 = (1,1)
fib1 1 = (1,1)
fib1 n = (b,a+b)
where
	(a,b) = fib1 (n-1)

// Start = fib1 8 // (21, 34)

fib2 :: Int Int Int -> Int
fib2 a b 0 = a
fib2 a b c = fib2 b (a+b) (c-1)

// Start = fib2 1 1 10 // 89

fib3 :: Int -> Int
fib3 n = fibAux n 1 1

fibAux 0 a b         = a
fibAux i a b | i > 0 = fibAux (i-1) b (a+b)

// Start = fib3 8 // 34


// 10. exists x xs checks whether x exists as an element in the list xs (or is ||, and is &&)
exists :: Int [Int] -> Bool
exists x []     = False
exists x [y:ys] = x == y || exists x ys

// Start = exists 3 [1, 2, 1, 1, 2, 3, 2, 1, 3] // True

// 11. write the function duplicates which checks if there are duplicates in the list xs
duplicates :: [Int] -> Bool
duplicates []     = False
duplicates [x:xs] = exists x xs || duplicates xs

// Start = duplicates [1, 2, 1, 1, 2, 3, 2, 1, 3] // True


// 12. remove x xs removes x from the list xs
remove :: Int [Int] -> [Int]
remove x []                 = []
remove x [y:ys] 
| x == y    = remove x ys
| otherwise = [y : remove x ys]

// Start = remove 3 [1, 2, 1, 1, 2, 3, 2, 1, 3] // [1, 2, 1, 1, 2, 2, 1] 

// 13. removeDuplicates l returns the list l with all duplicate elements removed
removeDuplicates :: [Int] -> [Int]
removeDuplicates []     = []
removeDuplicates [x:xs] = [x : removeDuplicates (remove x xs)]

// Start = removeDuplicates [1, 2, 1, 2, 3, 1, 2, 4, 2, 3] // [1, 2, 3, 4]


// 14. filter the elements that are satisfying a condition
filter` :: (Int -> Bool) [Int] -> [Int]
filter`  _  []    =  []
filter`  p  [h:t]    
| p h             = [h : filter` p t]
filter`  p  [h:t] = filter` p  t  

// Start = filter` ((<>) 5) [1, 5, 6, 7, 5, 8, 5]  // [1, 6, 7, 8]






module examples4

import StdEnv

// Start = filter isEven [1..10] // [2,4,6,8,10]

odd x = not (isEven x)
// Start = odd 23 // True

// Start = filter (not o isEven) [1..100] // [1,3,5,..,99]

// Start = takeWhile isEven [2,4,6,7,8,9] // [2, 4, 6]

// Start = dropWhile isEven [2,4,6,7,8,9] // [7, 8, 9]

plus x y = x + y

successor :: (Int -> Int)
successor = plus 1

// Start = successor 4 // 5

succ = (+) 1

//Start =  succ 5 // 6

// the function adding 5 to something
// Start = map (plus 5) [1,2,3] // [6,7,8]


// Start = until ((<)10) ((+)2) 0 // 12

// iteration of a function

// Start = iterate inc 1 // infinite list [1..]

// Start = map inc [1, 2, 3]        // [2, 3, 4]

// Start = map double [1, 2, 3]     // [2, 4, 6]

// lambda expressions
// Start = map (\x = x*x+2*x+1) [1..10] // [4,9,16,25,36,49,64,81,100,121]

// Start = foldr (+) 10 [1, 2, 3]   // 16

product [] = 1
product [x:xs] = x * product xs

product1 = foldr (*) 1

// Start = product1 [1, 2, 3] // 6

and1 [] = True
and1 [x:xs] = x && and xs

and2 = foldr (&&) True

// Start = and2 [True, True, False] // False

sum1 = foldr (+) 0

// Start = sum1 [1, 2, 3] // 6


///////////////
//Exercises
// 1. 1. Map a list of functions to a value. E.g. mapfun [f,g,h] x = [f x, g x, h x]
f1 :: Int -> Int
f1 x = x + 1
f2 :: Int -> Int
f2 x = x + 2
f3 :: Int -> Int
f3 x = x + 3

mapfun :: [Int -> Int] Int -> [Int]
mapfun [] x = []
mapfun [f : fs] x = [f x : mapfun fs x]

Start = mapfun [inc, inc, inc] 3 // [4, 4, 4]
//Start = mapfun [f1, f2, f3] 1 


// 2. compute n! factorial using foldr
f :: Int -> Int
f n = foldr (*) 1 [1..n]

//Start = f 5 // 120


// 3. rewrite flatten using foldr 
// (for the following list [[1,2], [3, 4, 5], [6, 7]] => [1,2,3,4,5,6,7])
flat :: [[Int]] -> [Int]
flat x = foldr (++) [] x

//Start = flat [[1,2], [3, 4, 5], [6, 7]] // [1,2,3,4,5,6,7]


// 4. using map and foldr compute how many elements are altogether in the following list 
// [[1,2], [3, 4, 5], [6, 7]] => 7
g :: [[Int]] -> Int
g x = foldr (+) 0 (map length x)

//Start = g [[1,2], [3, 4, 5], [6, 7]] // 7


// 5. using map extract only the first elements of the sublists in 
// [[1,2], [3, 4, 5], [6, 7]] => [1,3,6]
firsts :: [[Int]] -> [Int]
firsts x = map hd x

//Start = firsts [[1,2], [3, 4, 5], [6, 7]] // [1,3,6]


// 6. compute the squares of the elements of a list using map
// [1, 2, 3] -> [1, 4, 9]
sq :: Int -> Int
sq x = x*x

sqrs :: [Int] -> [Int]
sqrs y = map sq y 

//Start = sqrs [1, 2, 3] // [1, 4, 9]


// 7. same as 6. with lambda expression 
sqrs_lambda :: [Int] -> [Int]
sqrs_lambda y = map (\x = x*x) y

//Start = sqrs_lambda [1, 2, 3] // [1, 4, 9]



module examples5
import StdEnv

// Exercises

// 1. write a recursive function that computes n at power k (n, k positive numbers)
power :: Int Int -> Int
power n 0 = 1
power n k = n * power n (k-1)

//Start = power 2 5 // 32


// 2. add 3 to every element of a list
f1 :: [Int] -> [Int]
f1 list = map ((+) 3) list

//Start = f1 [1,5,3,1,6]  // [4,8,6,4,9]  


// 3. compute the double of the positive elements of a list [1, 2, -2, 3, -4] -> [2, 4, 6]
// hints: first filter it then use map 
double :: Int -> Int
double x = 2*x

f2 :: [Int] -> [Int]
f2 list = map double (filter ((<)0) list)

//Start = f2 [1, 2, -2, 3, -4] // [2, 4, 6]


// 4. write a function that keeps the integers of a list up to the first 0 encounterred 
// and then divides by 2 every element [1, 2, -2, 3, 0, -4] -> [0, 1, -1, 1]
// hints: use takeWhile then map
div2 :: Int -> Int
div2 x = x/2

tofirts0 :: [Int] -> [Int]
tofirts0 x = takeWhile ((<>)0) x

//Start = tofirts0 [1, 2, -2, 3, 0, -4, 0, 1, 2, 3, 0] 

f3 :: [Int] -> [Int]
f3 list = map div2 (tofirts0 list)

//Start = f3 [1, 2, -2, 3, 0, -4] // [0, 1, -1, 1]

// Solution version 2: writing directly the function in map

f3` :: [Int] -> [Int]
f3` list = map div2 (takeWhile ((<>)0) list)

//Start = f3` [1, 2, -2, 3, 0, -4] // [0, 1, -1, 1]


// 5. write a function for the square of every element of a list and sublists
// [[1,2],[3,4,5,6],[7,8]]  -> [[1,4],[9,16,25,36],[49,64]]  
// hint: map in map
sq :: Int -> Int
sq x = x*x

f4 :: [[Int]] -> [[Int]]
f4 lists = map (map sq) lists

//Start = f4 [[1,2],[3,4,5,6],[7,8]] // [[1,4],[9,16,25,36],[49,64]]


// 6. Replicate n>0 times the element of a list e.g. n=3 [3..6] ->
// [[3,3,3],[4,4,4],[5,5,5],[6,6,6]]
replicate :: Int Int -> [Int]
replicate 0 x = []
replicate n x = [x : replicate (n-1) x]

//Start = replicate 5 10

f5 :: Int [Int] -> [[Int]]
f5 n list = map (replicate n) list  

//Start = f5 3 [3..6]


// 7. insert 0 at the beginning of each sublist
// [[1,2], [3,4,5], [6,7]] -> [[0,1,2], [0,3,4,5], [0,6,7]]
f6 :: [[Int]] -> [[Int]]
f6 lists = map ((++) [0]) lists

//Start =  f6 [[1,2], [3,4,5], [6,7]]


// 8. filter the element smaller then n, e.g. n=3 [1,5,3,2,1,6,4,3,2,1] -> [1,2,1,2,1]
f7 :: Int [Int] -> [Int]
f7 n list = filter ((>)n) list
 
//Start = f7 3 [1,5,3,2,1,6,4,3,2,1] 


// 9. using notempty eliminate the empty lists: 
// [[1,2,3],[],[3,4,5],[2,2],[],[],[]] -> [[1,2,3], [3,4,5], [2,2]]
notempty :: [Int] -> Bool
notempty x = not (x == [])

f8 :: [[Int]] -> [[Int]]
f8 lists = filter notempty lists

//Start = f8 [[1,2,3],[],[3,4,5],[2,2],[],[],[]]


// 10. compute the sum of the sublist using foldr [[1,2,3], [3,4,5], [2,2]] -> [6,12,4]
f9 :: [[Int]] -> [Int]
f9 lists = map (foldr (+) 0) lists

//Start = f9 [[1,2,3], [3,4,5], [2,2]]


// 11. (bonus point) rewrite map using foldr

//Start = mymap inc [1..10]




module examples6

import StdEnv

// Types
//(1,'f')          :: (Int, Char)
//("world",True,2) :: (String, Bool, Int)
//([1,2],sqrt)     :: ([Int], Real->Real)
//(1,(2,3))        :: (Int, (Int,Int))

// Start = (1,'f')
// Start = ("world",True,2)
// Start :: ([Int], Real->Real)
// Start = ([1,2],sqrt)
// Start = (1,(2,3))

fst1 :: (a,b) -> a
fst1 (x,y) = x

// Start = fst1 (10, "world") // 10

snd1 :: (a,b) -> b
snd1 (x,y) = y

// Start = snd1 (1,(2,3))    // (2,3)

splitAt1 :: Int [a] -> ([a],[a])
splitAt1 n xs = (take n xs, drop n xs)

// Start = splitAt1 3 ['hello'] // (['h','e','l'],['l','o'])

search :: a [(a,b)] -> b | == a
search s [(x,y):ts]
| x == s = y
| otherwise = search s ts

// Start = search 3 [(1,1), (2,4), (3,9), (3,7)] // 9

zip1 :: [a] [b] -> [(a,b)]
zip1 [] ys = []
zip1 xs [] = []
zip1 [x : xs] [y : ys] = [(x , y) : zip1 xs ys]

// Start = zip1 [1,2,3] ['abc'] // [(1,'a'),(2,'b'),(3,'c')]

myzip :: [a] [b] -> [(a,b)]
myzip as bs = [(a , b) \\ a <- as & b <- bs]

// Start = myzip [1,2,3] [10, 20, 30, 40, 50] // [(1,10),(2,20),(3,30)]


// Start = [x * x \\ x <- [1..10]] // [1,4,9,16,25,36,49,64,81,100]

// Start = map (\x = x * x) [1..10]


// Start = [(x,y) \\ x <- [1..2], y <- [4..6]]  // [(1,4),(1,5),(1,6),(2,4),(2,5),(2,6)]

// Start = [(x,y) \\ x <- [1..2] & y <- [4..6]] // [(1,4),(2,5)]

// Start = [(x,y) \\ x <- [1..5], y <- [1..x] | isEven x]  // [(2,1),(2,2),(4,1),(4,2),(4,3),(4,4)]

qsort :: [a] -> [a] | Ord a
qsort [] = []
qsort [h : xs] = qsort [x \\ x <- xs | x <  h] ++ [h] ++
                 qsort [x \\ x <- xs | x >= h]

// Start = qsort [2,1,5,3,6,9,0,1] // [0,1,1,2,3,5,6,9]

/////////////////////////

// 1. generate a list with 10 times of 13 : [13,13,13,13,13,13,13,13,13,13]
l1 :: [Int]
l1 = [13 \\ x<-[1..10]]

Start = l1


// 2. generate the following list [(1,1),(1,2),(2,1),(2,2)]
l2 :: [(Int, Int)]
l2 = [(x,y) \\ x<-[1..2], y<-[1..2]]

//Start = l2


// 3. generate the following list [(1,3),(1,2),(1,1),(2,3),(2,2),(2,1),(3,3),(3,2),(3,1)]
l3 :: [(Int, Int)]
l3 = [(x,y) \\ x<-[1..3], y<-[3,2,1]]

//Start = l3


// 4. generate the list [(1,5),(2,6),(3,7),(4,8),(5,9),(6,10)]
l4 :: [(Int, Int)]
l4 = [(x,y) \\ x<-[1..10] & y<-[5..10]]

//Start = l4


// 5. generate the list [1,2,2,3,3,3,4,4,4,4,...,10,..,10]
l5 :: [Int]
l5 = [y \\ y<-[1..10], x<-[1..y]]

//Start = l5


// 6. generate the list [[1],[2,2],[3,3,3],[4,4,4,4],...,[10,..,10]]
l6 :: [[Int]]
l6 = [[y \\ x<-[1..y]] \\ y<-[1..10]]

//Start = l6
  
  
// 7. generate 6 pythagoras numbers : [(3,4,5),(6,8,10),(5,12,13),(9,12,15),(8,15,17),(12,16,20)]
l7 :: [(Int, Int, Int)]
l7 = take 6 [(a,b,c) \\ c<-[1..], b<-[1..c], a<-[1..b] | a*a + b*b == c*c]

//Start = l7




module examples7
import StdEnv

// 1. Generate 100 even numbers using list comprehensions
l1 :: [Int]
l1 = [x*2 \\ x<-[1..100]]

//Start = l1

// 2. Generate the following list [4, 16, 36, 64, 100, 144, 196, 256, 324, 400]
l2 :: [Int]
l2 = take 10 ([ n*n \\ n<-[1..] | n rem 2 == 0 ])
l22 = take 10 ([x*x \\ x <- [1..] | isEven x]) 
l222 = [2*x*2*x \\ x <- [1..10]]

//Start = l222


// 3. List powers of 2 from 1 to 10.
//hint: use x^y (x at power y)
l3 :: [Int]
l3 = [2^x \\ x<-[1..10]]

//Start = l3


// 4. List the divisors of 90.
l4 :: [Int]
l4 = [x \\ x <- [1..90] | 90 rem x == 0] 

//Start = l4


// 5. List “dominoes”: [(0,0),(0,1),(1,1),(0,2),(1,2),(2,2),...,(9,9)]
// Domino (1,0) is not in the list because it is already in it as (0,1).
l5 :: [(Int, Int)]
l5 = [(x, y) \\ y<-[0..9], x<-[0..y]] 

//Start = l5


// 6. Construct the list [(1,'a'),(2,'b'),…(…,'z')]
l6 :: [(Int, Char)]
l6 = [(x,y)\\ x<-[1..] & y<-['a'..'z']]

//Start = l6


// 7. Generate a list of length 10 whose elements are False, True, False, True, … (alternating)
//l7 :: [Bool]
l7 = flatten [x \\ x<-[[False, True]], y<-[1..5]]
l77  = flatten [[False, True] \\ y<-[1..5]]
l777 = [isEven x \\ x <-[1..10]]

//Start = l777


// 8. Is 123457 a prime number?
l8 :: Bool
l8 = (length [x \\ x <- [2..123456] | 123457 rem x == 0]) == 0

//Start = l8


// 9. Generate the list [(0,10),(1,9),…,(10,0)].
l9 :: [(Int, Int)]
l9 = [(x,y) \\ x<-[0..10] , y<-[0..10] | x+y==10]
l99 = [(x,y) \\ x<-[0..10] & y<-[10,9..0]]
l999 = [(x,10-x) \\ x<-[0..10]]
//Start = l999

// 10. Generate a list that contains all (hour, minute) pairs in a day.
l10 :: [(Int, Int)]
l10 = [(x,y) \\ x<- [0..23] , y<-[0..59]]

//Start = l10


// 11. (bonus point) Generate a list that contains all (month, day) pairs in a 365-day 
l11 :: [(Int, Int)]
l11 = [(x,y) \\ x<-[1..12] & z<-[31,28,31,30,31,30,31,31,30,31,30,31], y<-[1..31] | y<=z]

Start = l11




 



module examples8
import StdEnv

// 1. Is x a power of 10?
iprod :: Int -> Bool 
iprod x
| (x == 1) = True
| (x rem 10 == 0) = iprod (x / 10)
= False

//Start = iprod 123 // False


// 2. Write a function to compute 1+2+3+4+5+6+...+n
isum :: Int -> Int
isum n 
| n==0 = 0
| n>0 = n + isum (n-1)

isum1 n = foldr (+) 0 [1..n]

//Start = isum 10
//Start = isum1 10


// 3. Compute the number of sublists in a list
nrlist :: [[Int]] -> Int
nrlist l = length l

//Start = nrlist [[1, 2, 3], [3, 4], [5, 7, 8, 9]]


// 4. Compute the number of all the elements in a list
nrelist :: [[Int]] -> Int
nrelist l = length (flatten l)

//Start = nrelist [[1, 2, 3], [3, 4], [5, 7, 8, 9]]


// 5. Keep the first n and the last n elements of a list
cut :: [Int] Int -> [Int]
cut l x 
| h < x = []
| h == x = []
= (take x l) ++ (drop (h-x) l)
where h = length l

//Start = cut [1, 2, 3, 4, 5, 6, 7, 8, 9] 3

cut1 l x  
| h < x = []
| h == x = []
= (take x l) ++ reverse (take x (reverse l))
where h = length l

//Start = cut1 [1, 2, 3, 4, 5, 6, 7, 8, 9] 3


// 6. Delete the 0 element from a list
delzero :: [Int] -> [Int]
delzero []   =  []
delzero [0:t]   = delzero t 
delzero [h:t] = [h : delzero t]

//Start = delzero [1, 0, 5, 0, 0, 6, 7, 5, 0, 0, 0, 8, 0, 5, 0, 0, 0] 

delzero1 l = filter ((<>)0) l

//Start = delzero1 [1, 0, 5, 0, 0, 6, 7, 5, 0, 0, 0, 8, 0, 5, 0, 0, 0] 


// 7. compute  1*2 + 2*3 + ... + n*n+1 
sums :: Int -> Int
sums n 
| n > 0 = sums (n-1) + n*(n+1)
= 0

//Start = sums 5

sumsp n = sumsp1 n 0

sumsp1 x y 
| x == y = 0
= y*(y+1) + sumsp1 x (y+1) 

//Start = sumsp 5

sums1 0 = 0
sums1 n = (n+1)*n + sums1 (n-1)

//Start = sums1 5


// 8. write a function duplicates which checks if there are neighbouring duplicates in a list
duplic :: [Int] -> Bool
duplic []     = False
duplic [x]     = False
duplic [x, y :xs] = (x == y) || duplic [y : xs]

//Start = duplic [1, 1]
//Start = duplic [2]
//Start = duplic [1, 2, 3, 4, 5, 6, 7, 8, 9]
//Start = duplic [1, 0, 5, 0, 0, 6, 7, 5, 0, 0, 0, 8, 0, 5, 0, 0, 0] 


// 9. write a function that removes neighbour duplicates in a list
duplicrem :: [Int] -> [Int] 
duplicrem []     = []
duplicrem [x]     = [x]
duplicrem [x, y :xs]
| (x == y) = duplicrem xs
= [x : duplicrem [y :xs]]

//Start = duplicrem [1, 0, 5, 0, 0, 6, 7, 5, 0, 0, 0, 8, 0, 5, 0, 0, 0]


// 10. filter the even elements of a list
g :: [Int] -> [Int]
g l = filter isEven l

//Start = g [1, 0, 5, 0, 0, 6, 7, 5, 0, 0, 0, 8, 0, 5, 0, 0, 0] 


// 11. compute the half of the elements of a list using map

half :: Int -> Int
half x = x / 2

halfs :: [Int] -> [Int]
halfs y = map half y 

//Start = halfs [1, 2, 3, 4, 5, 6, 7, 8, 9]


// 12. transform the sub-sub lists into one list of sublists
f :: [[[Int]]] -> [[Int]]
f lists = foldr (++) [] lists

//Start = f [[[1,2,3], [3,4,5]], [[1,2,3], [3,4,5], [7,8,9]]] 
// result : [[1,2,3],[3,4,5],[1,2,3],[3,4,5],[7,8,9]]

f1 lists = flatten lists

//Start = f1 [[[1,2,3], [3,4,5]], [[1,2,3], [3,4,5], [7,8,9]]] 


// 13. delete the last element of the sublists
dellast :: [[Int]] -> [[Int]] 
dellast x = map init x

//Start = dellast [[1,2,3],[3,4,5],[1,2,3],[3,4,5],[7,8,9]]


// 14.  generate the following list [(1,1),(2,2),(3,3),(4,4),(5,5)]
l1 :: [(Int, Int)]
l1 = [(x,x) \\ x<-[1..5]]

//Start = l1


// 15. generate [(1,2,3),(2,4,6),(3,6,9),(4,8,12),(5,10,15)]
l2 :: [(Int, Int, Int)]
l2 = [(y, 2*y, 3*y) \\ y<-[1..5]]

Start = l2



module examples11
import StdEnv 

// 1. Triple a number.
triple :: Int -> Int
triple x = 3*x

//Start = triple 3


// 2. Check if a number is odd.
isoddnr :: Int -> Bool
isoddnr x = (x rem 2 == 1)

//Start = isoddnr 6


// 3. Check if a number is the sum of two other given numbers.
issum :: Int Int Int -> Bool
issum x y z = (x == y + z)

//Start = issum 10 6 3


// 4. Add 100 to a number.
add100 :: Int -> Int
add100 x = x + 100

//Start = add100 5


// 5. Check if a number is multiple of 10.
ismult10 :: Int -> Bool
ismult10 x = (x rem 10 == 0)

//Start = ismult10 20


// 6. Add the numbers from 1..N in a recursive function (N positive).
addn :: Int -> Int
addn n 
| n == 1 = 1
= n + addn (n-1)

//Start = addn 5


// 7. Compute the N choose K value (N and K are positive, K ).
n_choose_k :: Int Int -> Int
n_choose_k n k
| k == 0 = 1
= (n-k+1)/k * n_choose_k n (k-1)

//Start = n_choose_k 5 2


// 8. Compute the cube of a number
cube :: Int -> Int
cube x = x * x * x

Start = cube 4


// 9*. Define addition as recursive function.
//recadd :: Int -> Int

//Start = recadd 5



module examples21

import StdEnv


// 1. Reverse every sublist of a list
revsub :: [[Int]] ->  [[Int]]
revsub [] = []
revsub [x:xs] = [reverse x : revsub xs]

Start = revsub [[1,2,3],[5,6],[],[7,8,9,10]]


// 2. Generate a list with every fifth element form 0 to 500.

//Start = [0,5..500]


// 3. Delete the first and the last element of a list.
del_firstlast :: [Int] -> [Int]
del_firstlast x 
| length x > 1 = init (tl x)
= []

//Start = del_firstlast [1..10]
//Start = del_firstlast []
//Start = del_firstlast [1]


// 4. Compute for a given positive n the sum of 2i(2i+1), for i from 1 to n. E.g. for n=3 the sum is 68.
f :: Int -> Int
f n 
| n < 1 = abort "n should be positive"
= g 1 n

g :: Int Int -> Int
g i n
| i == n+1 = 0 
= 2*i*(2*i+1) + g (i+1) n

//Start = f 3


// 5. Cut a list in two parts at the middle. E.g. cut [1..10] -> [[1,2,3,4,5],[6,7,8,9,10]]
// and for cut [1..11] the result is [[1,2,3,4,5],[6,7,8,9,10,11]].
cut :: [Int] -> [[Int]]
cut x = [ take (y/2) x, drop (y/2) x]
where y = length x

//Start = cut [1..10]
//Start = cut [1..11]
//Start = cut []
//Start = cut [1]


// 6. Generate a list with every 500th element form -10000 to 10000.

//Start = [-10000,-9500..10000]


// 7. Keep the last elements of the sublists of a list in one list (the sublists are not empty).
// [[1,2,3],[5,6],[1],[7,8,9,10]] -> [3,6,1,10]
lasts :: [[Int]] -> [Int]
lasts [] = []
lasts [x:xs] = [last x] ++ lasts xs

//Start = lasts [[1,2,3],[5,6],[1],[7,8,9,10]]


// 8. Extract the middle element of a non-empty list. E.g. for [1..5] is 3, for [1..4] is 3.
middle :: [Int] -> Int
middle x = x!!(y/2)
where y = length x

//Start = middle [1..5] 
//Start = middle [1..4] 
//Start = middle [1]


// 9. Instert 0 in front of every sublist of a list.
// E.g. for [[1,2,3],[5,6],[],[7,8,9,10]] the result is [[0,1,2,3],[0,5,6],[0],[0,7,8,9,10]]
ins0 :: [[Int]] -> [[Int]]
ins0 [] = []
ins0 [x:xs] = [ [0]++x : ins0 xs]

//Start = ins0 [[1,2,3],[5,6],[],[7,8,9,10]]


// 10. Delete the last element of each sublist of a list.
// E.g. for [[1,2,3],[5,6],[],[7,8,9,10]] the result is [[1,2],[5],[],[7,8,9]]
lastdel :: [[Int]] -> [[Int]]
lastdel [] = []
lastdel [x:xs] = [init x : lastdel xs]

//Start = lastdel [[1,2,3],[5,6],[],[7,8,9,10]]


// 11. Compute the sum 1+ 2*2+ 3*3*3+ 4*4*4*4+ 5*5*5*5*5+ ...+n*n*n*...*n 
// where n is a positive number.
power :: Int -> Int
power n = powers n n

powers :: Int Int -> Int
powers x y
| y == 0 = 1 
= x * powers x (y-1)

sumpowers :: Int -> Int
sumpowers n 
| n < 1 = abort "n should be positive"
| n == 1 = 1
= power n + sumpowers (n-1)

//Start = power 3
//Start = sumpowers 3
//Start = sumpowers 5
//Start = sumpowers 0



module examples31

import StdEnv

// Earlier exemples rewritten with map and/or fold.

// 1. Operations with lists: write functions for the followings
// keep the head of every sublist (sublists are not empty)
// e.g. [[1, 2, 3], [3, 4], [5, 7, 8, 9]] -> [1, 3, 5]
heads :: [[Int]] -> [Int]
heads x = map hd x

//Start = heads [[1, 2, 3], [3, 4], [5, 7, 8, 9]]


// 2. Keep the tails of the sublists of a list.
// e.g. [[1, 2, 3], [3, 4], [5, 7, 8, 9]] -> [[2, 3], [4], [7, 8, 9]] 
tails :: [[Int]] -> [[Int]]
tails x = map tl x

//Start = tails [[1, 2, 3], [3, 4], [5, 7, 8, 9]]


// 3. Triple the elements of a list.
triples :: [Int] -> [Int]
triples x = map ((*)3) x

//Start = triples [1..5]


// 4. Check if the numbers of a list are odd.
isoddnrs :: [Int] -> [Bool]
isoddnrs x = map isOdd x

//Start = isoddnrs [1..5]


// 5. Add 100 to the numbers of a list.
add100 :: [Int] -> [Int]
add100 x = map  ((+) 100) x

//Start = add100 [1..8]


// 6. Check if the numbers of a list are multiple of 10.
ismult10 :: [Int] -> [Bool]
ismult10 x = map (\y = y rem 10 == 0) x

//Start = ismult10 [1..20]


// 7. Collect in a list the last digists of the numbers of a list.
lastdigits :: [Int] -> [Int]
lastdigits x = map f x
where f z = z rem 10

//Start = lastdigits [1..35]


// 8. Compute the cube of the numbers of a list.
cube :: Int -> Int
cube x = x * x * x

cubes :: [Int] -> [Int]
cubes x = map cube x

//Start = cubes [1..10]
//Start = cubes []


// 9.  Add the numbers of 1..N (N positive) using foldr.
addn :: Int -> Int
addn n 
| n < 0 = abort "for n negative there is no sum defined"
= foldr (+) 0 [1..n]

//Start = addn 5
//Start = addn 0
//Start = addn -2
//Start = addn 10


// 10. Reverse every sublist of a list.
revsub :: [[Int]] ->  [[Int]]
revsub x = map reverse x 

//Start = revsub [[1,2,3],[5,6],[],[7,8,9,10]]


// 11. Keep the last elements of the sublists of a list in one list 
// (the sublists are not empty).
// [[1,2,3],[5,6],[1],[7,8,9,10]] -> [3,6,1,10]
lasts :: [[Int]] -> [Int]
lasts x = map last x

//Start = lasts [[1,2,3],[5,6],[1],[7,8,9,10]]


// 12. Instert 0 in front of every sublist of a list.
// E.g. for [[1,2,3],[5,6],[],[7,8,9,10]] the result is 
// [[0,1,2,3],[0,5,6],[0],[0,7,8,9,10]]
ins0 :: [[Int]] -> [[Int]]
ins0 x = map ((++) [0]) x

//Start = ins0 [[1,2,3],[5,6],[],[7,8,9,10]]


// 13. Delete the last element of each sublist of a list.
// E.g. for [[1,2,3],[5,6],[],[7,8,9,10]] the result is [[1,2],[5],[],[7,8,9]]
lastdel :: [[Int]] -> [[Int]]
lastdel x = map init x

//Start = lastdel [[1,2,3],[5,6],[],[7,8,9,10]]


// 14. Compute the product of the elements of a list using foldr.
prodf :: [Int] -> Int
prodf x = foldr (*) 1 x

//Start = prodf [1,5,2,4]


// 15. Compute 1*1 + 2*2 + ... + n*n  for n positive using map and foldr.
sumsqr :: Int -> Int
sumsqr n = foldr (+) 0 (map (\x=x*x)  [1..n])

Start = sumsqr 5 // 55



module examples81
import StdEnv

// 1. Compute the sum of the list of tuples [(1,1), (2,2), (3,3)] -> (6,6)

sumtup l = ( sum (fst x), sum (snd x)) 
where x = unzip l 

//Start = sumtup [(1,1), (2,2), (3,3)]


// 2. Generate 5 tuples like [(1,2),(2,3),(3,4),(4,5),(5,6)]
increase = take 5 [(i,i+1) \\ i<-[1..]]

//Start = increase


// 3. Make triple tuples like [(1,2,3),(4,5,6),(7,8,9),(10,11,12),(13,14,15)]
tripl = take 5 [(i,i+1,i+2) \\ i<-[1,4..]]

//Start = tripl


// 4. Given a list of lists, transform it into tuples of sublists such that two 
// continous sublists form pairs 
// (if there are odd number of sublists the last has as pair the empty list)

pairs [] = []
pairs [x] = [(x,[])]
pairs [x,y : rest] = [(x,y) : pairs rest]

//Start = pairs [[1,2,3], [5,6], [7,8,9,10], [11,3], [1..5]]
//Start = pairs [[1,2,3], [5,6], [7,8,9,10], [11,3]]


// 5. Given a list of tuples form a list of triple tuples with the original 
// numbers and their sum

triplesum l = [(fst a, snd a, fst a+ snd a) \\ a <-l]

//Start = triplesum [(1,2),(2,3),(3,4),(4,5),(5,6)]

triplesum1 l = [(x,y,x+y) \\ x <- (fst a) & y <- (snd a)] 
where a = unzip l 

//Start = triplesum1 [(1,2),(2,3),(3,4),(4,5),(5,6)]

triplesum2 l = [(x,y,x+y) \\ x <- (map fst l) & y <- (map snd l)] 

//Start = triplesum2 [(1,2),(2,3),(3,4),(4,5),(5,6)]


// 6. Generate quadruples of a number, its square, its cube, and its biquadratic
// where the number are in the 1..20 interval

quadruple = [(x,x*x,x*x*x,x*x*x*x) \\ x <-[1..20]] 

//Start = quadruple

quadruple1 = [(x,x^2,x^3,x^4) \\ x <-[1..20]] 

//Start = quadruple1


// 7. Form triple tuples of 3 lists selecting one element from each list.
// E.g. for ([1..10],[20..25],[35..47]) the result is 
// [(1,20,35),(2,21,36),(3,22,37),(4,23,38),(5,24,39),(6,25,40)]

tri l = [(x,y,z) \\ x <- fst3 l & y <- snd3 l & z <- thd3 l]

//Start = tri ([1..10],[20..25],[35..47])

tri1 (a,b,c) = [(x,y,z) \\ x <- a & y <- b & z <- c]

Start = tri1 ([1..10],[20..25],[35..47])



module midterm
import StdEnv


// 1. Generate the first 10 positive elements of a list in which a number is multiple of 5 but is not multiple of 10.

glist :: [Int]
glist = take 10 ([5*x \\ x <- [1..] | not (isEven x) ] )

glist1 = take 10 [x \\ x<-[1..]|(x rem 5==0) &&(x rem 10 <>0)]

glist2 = [5*x\\x<-[1,3..19]]

//Start = glist

// 2. Compute the product of the elements of the sublists of a list (you MUST use map)

multiply :: [Int] -> Int
multiply [] = 1
multiply list =  foldr (*) 1 list

plist :: [[Int]] -> [Int]
plist list = map multiply list

plist1 x = map prod x

//Start = plist [[1, 2, 3], [3, 4], [5, 7, 8, 9], []]

// 3. Given a list of tuples make a tuple of 2 lists like:

clist :: [(Int, Int)] -> ([Int], [Int])
clist list = unzip list

h1 [] = []
h1 [(x,y):xs] =   [x] ++ h1 xs
d1 [] = []
d1 [(x,y):xs] =   [y] ++ d1 xs
clist1 x = (h1 x,d1 x)

clist2 l = ([fst x \\ x<-l], [snd x \\ x<-l])

//Start = clist [(1, 2), (3,1), (8, 4), (5, 7), (8, 9)] // ([1,3,8,5,8],[2,1,4,7,9])

// 4. Insert x as second element in every sublist of a list.
// if the sublist was empty then x will be the only element in the new sublist.
// [[1,2], [3,4,5], [6,5,9,7], [], [8]] 100 -> [[1,100,2], [3,100,4,5], [6,100,5,9,7], [100], [8,100]]
insertx :: [Int] Int -> [Int]
insertx [] x = [x]
insertx [n:ns] x = [n,x:ns]

xlist :: [[Int]] Int -> [[Int]]
xlist list x = map (\n = insertx n x) list

ins :: Int [Int] -> [Int]
ins n [] = [n]
ins n x = [hd x] ++ [n] ++ (drop 1 x)

xlist2 x n = map (ins n) x

xlist1 [] n = [n]
xlist1 [x] n = [x,n]
xlist1 [x,y:xs] n = [x,n,y: xs]

xlist3 [] _ = []
xlist3 [x:xs] n = [xlist1 x n : xlist3 xs n]

//Start = xlist [[1,2], [3,4,5], [6,5,9,7], [], [8]] 100

// 5. Generate pairs like in the following:
// [[1,2,3], [4,5], [6,7,8], []] -> [(1,6),(2,9),(3,21),(4,0)]

fpair` :: [[Int]] Int -> [(Int, Int)]
fpair` [] y = []
fpair` [x:xs] y = [(y+1, (foldr (+) 0 x)): fpair` xs (y+1)]

fpair :: [[Int]] -> [(Int, Int)]
fpair list = fpair` list 0

fpair1 x = zip([1..], map sum x)


fpair11 [] = 0
fpair11 [x:xs] = x + fpair1 xs
fpair22 [] = []
fpair22 [x:xs] = [fpair11 x : fpair22 xs]
fpair2 x = [(x,y)\\x<-[1..]&y<-fpair22 x]

fpair3 l = [(x,y) \\ x<-[1..] & y<-(map sum l)]

	
//Start = fpair [[1,2,3],[4,5],[6,7,8],[]]


// 6. Extract the second element of each sublist of a list (if there is no second element, ignore that sublist)
// e.g. [[1,2,3], [3,4,5,6], [], [5,7,8], [1], [8,9]]-> [2,4,7,9]

qlist :: [[Int]] -> [Int]
qlist [] = []
qlist [x:xs] 
| length x >1 = [ last (take 2 x) : qlist xs]
= qlist xs


extr:: [Int] -> [Int]
extr [] = []
extr [x:xs] = [x]++ (drop 1 xs)

qlist1 x = flatten(map extr x)


cd [] = []
cd [x:xs] 
|x==[] = cd xs
|length x ==1 = cd xs
=[x:cd xs] 

qlist11 [x,y:xs] = y 
qlist2 [] = []
qlist2 [x:xs] =map qlist11 (cd [x:xs]) 


qlist3 l = map second ll 
where ll = [x \\ x <-l | length x <> 0  && length x <> 1]

second :: [Int] -> Int
second x = x!!1


//Start = qlist [[1,2,3], [3,4,5,6], [], [5,7,8], [1], [8,9]]


// 7. Check if a list contains 2 equal elements one after the other
// (it can be anywhere in the list)
// for [1,2,3,3,3,2,4,5] is True for [1 .. 5] is False

dlist :: [Int] -> Bool
dlist [] = False
dlist [x] = False
dlist [x,y:xs]
| x==y = True
= dlist [y:xs]


dlist1 x =  foldr (||) False (map check (zip (x, tl x)))

check :: (Int,Int) -> Bool
check (x,y) = (x==y)

//Start = dlist [1,2,3,4,3,2,4,8,5,5]


// 8. Generate the following list
// [[1],[1,2],[1,2,3],[1,2,3,4],[1,2,3,4,5],[1,2,3,4,5,6],[1,2,3,4,5,6,7],[1,2,3,4,5,6,7,8],[1,2,3,4,5,6,7,8,9],[1,2,3,4,5,6,7,8,9,10]]

nlist :: Int -> [[Int]]
nlist n = [x \\ y <- [1..n], x<-[[1..y]]]

nlist1 n = take 10[[x..y] \\ x<-[1..n],y<-[1..n] ]

nlist2 n = [[x\\x<-[1..y]]\\y<-[1..n]]


f :: Int -> [[Int]]
f n 
| n > 0 = [[1..n] : f (n-1)]
= []

nlist3 n = reverse (f n)


//Start = nlist 10



module testmidterm_group2

import StdEnv

// For the mark 2: ex1 and ex2 and ex3 and ex4 MUST be done
// For a mark greater then 2 ex.1,2,3,4 + any from ex. 5(1p), ex6(1p), ex7(1p), ex8(1p)

// 1. Compute the sum of the elements of the sublists of a list (you must use map) 
plist :: [[Int]] -> [Int]
plist x = map sum x

//Start = plist [[1, 2, 3], [3, 4], [5, 7, 8, 9], []]


// 2. Insert x as second element in every sublist of a list.
// if the sublist was empty then x will be the only element in the new sublist. 
// [[1,2], [3,4,5], [6,5,9,7], [], [8]] 10 -> [[1,10,2], [3,10,4,5], [6,10,5,9,7], [10], [8,10]]

xlist :: [[Int]] Int -> [[Int]]
xlist l a = map (aux a) l

aux :: Int [Int] -> [Int]
aux a [] = [a]
aux a [y:ys] = [y, a : ys]
  
//Start = aux [1,2] 10
//Start = aux [] 10

//Start = xlist [[1,2], [3,4,5], [6,5,9,7],[]] 10


// 3. Given a list of triple tuples make a tuple of 3 lists like:
clist :: [(Int, Int, Int)] -> ([Int], [Int], [Int])
clist l = ([fst3 x \\ x<-l], [snd3 x \\ x<-l], [thd3 x \\ x<-l])

//Start = clist [(1,2,1), (3,1,4), (8,5,4), (5,7,0), (8,9,1)]  // ([1,3,8,5,8],[2,1,5,7,9],[1,4,4,0,1])


// 4. Generate the first 10 positive elements of a list in which a number is multiple of 25, but is not multiple of 100.
glist :: [Int]
glist = take 10 [x \\ x<- [1..] | (x rem 25 == 0) && (x rem 100 <> 0)]

//Start = glist


// 5. Generate pairs like in the following: 
// [[1,2,3], [4,5], [6,1,8], []] -> [(1,6),(2,20),(3,48),(4,1)]
fpair :: [[Int]] -> [(Int, Int)]
fpair l = [(x,y) \\ x<-[1..] & y<-(map prod l)]

//Start = fpair [[1,2,3],[4,5],[6,1,8],[]]


// 6. Generate the following list
// [[1],[1,2],[1,2,3],[1,2,3,4],[1,2,3,4,5],[1,2,3,4,5,6],
// [1,2,3,4,5,6,7],[1,2,3,4,5,6,7,8],[1,2,3,4,5,6,7,8,9],[1,2,3,4,5,6,7,8,9,10]]
f :: Int -> [[Int]]
f n 
| n > 0 = [[1..n] : nlist (n-1)]
= []

nlist :: Int -> [[Int]]
nlist x = reverse (f x)

//Start = nlist 10


// 7. Check if a list contains 3 equal elements one after the other 
// (it can be anywhere in the list) 
// for [1,2,3,3,3,2,4,5] is True for [1 .. 5] is False
dlist :: [Int] -> Bool
dlist [] = False
dlist [x] = False
dlist [x,y] = False
dlist [x, y, z : l]
|((x==y) && (y==z))  = True
= dlist [y,z : l]

//Start = dlist [1,2,2,3,4,3,3,2,4,5,5,5] 


// 8. Extract the third element of the sublists (if there is no such element, ignore that sublist)
// [[1,2,3], [3,4,5,6], [], [5,7,8,11], [1], [8,9]]-> [3,5,8]
qlist :: [[Int]] -> [Int]
qlist l = map second ll 
where ll = [x \\ x <-l | length x <> 0  && length x <> 1 && length x <> 2]

second :: [Int] -> Int
second x = x!!2

Start = qlist [[1,2,3], [3,4,5,6], [], [5,7,8,11], [1], [8,9]]


module homework
import StdEnv 

// name: murtada alnakhli
// neptun code: RKB1O2


//One kilo of apple is 200 HUF. If you buy more then 5 kilos, 
//but less then 10, than the price per kilo is 150. However,
//the price for more then 10 kilos is of 100 HUF for a kilo. 
//Compute how much do you have to pay if you buy if you buy some kilos of apple. 

price :: Int -> Int
price x 
|(x == 1) = 200*x 
|(x <  5) = 200*x 
|(x == 5) = 150*x 
|(x < 10) = 150*x  
|(x >= 10)= 100*x

 
Start = price 7

module homework2
import StdEnv



// Define the function MakeFrequenceTable that makes a frequency table for the elements of a list.

// The table consists of a list of lists. Each sub-list consists of an element from the original list, number of occurrences and its frequency (percentage).

// MakeFrequenceTable :: [Int] -> [[Int]]

// Examples:

// MakeFrequenceTable [] = [[]]

// MakeFrequenceTable [1] = [[1,1,100]]

// MakeFrequenceTable [1,2] = [[1,1,50],[2,1,50]]

// MakeFrequenceTable [1..10] = [[1,1,10],[2,1,10],[3,1,10],[4,1,10],[5,1,10],[6,1,10],[7,1,10],[8,1,10],[9,1,10],[10,1,10]]

// MakeFrequenceTable [1,3,2,3,2] = [[1,1,20],[3,2,40],[2,2,40]]

list :: Int Int -> Int
list  x y = x/y*100

fristlist :: Int -> Int
fristlist y  = y*100



MakeFrequencyTable :: [Int] -> [[Int]]
MakeFrequencyTable [] = [] 
MakeFrequencyTable  
= removeDup [[y,n,m] \\   y <-  [x :xs] , m <- [(fristlist (Occurrences y [x : xs])/length [x :xs])] , n <- [(Occurrences y [x : xs])]] 




Occurrences :: y [y] -> Int | == y
Occurrences m [] = 0
Occurrences m [x : xs] = f m [x : xs] 0
where
                f m [] i = i
                f m [x : xs] i
                | m == x = f x xs i+1
                = f m xs i


//Start = MakeFrequencyTable []
//Start = MakeFrequencyTable [1]
//Start = MakeFrequencyTable [1,2]
Start = MakeFrequencyTable [1..10]
//Start = MakeFrequencyTable [1,3,2,3,2]

module homework3
import StdEnv


//NEPTUN CODE: RKB1O2
//NAME: MYRTADA ALNAKHLI



//A meteorological institute records every day the maximum temperature of a day.
// Each week they have a sublist of daily maximums.
// Every week-end they register the records of the week into a list.
// So the input is of type [[Real]].

//Write in a list the maximum temperatures of the weeks of a period.

//Write in a list the average temperatures of the weeks of a period.

//Compute the average max temperature of the inspected period. (i.e. average of list 1.)

//E.g for 3 weeks they have [[1.1,12.0,13.0,16.2,12.5,3.0,11.3],[1.5,1.0,11.0,17.8,11.5,3.9,1.3],
//[5.8,10.0,11.0,1.2,11.5,3.9,1.3]]

//answer for 1. is [16.2, 17.8, 11.5]

//answer for 2. is [9.87, 6.85, 6.38]

//answer for 3. is 15.16

//remarks: Conversion from an integer to a real number is done by the function toReal.


maximumtemperatures :: [Real] -> [Real]
maximumtemperatures [] = []
maximumtemperatures i = take 1 (reverse (sort i))
//Start = maximumtemperatures [1.1,12.0,13.0,16.2,12.5,3.0,11.3]

averagetemperatures :: [Real] -> [Real]
averagetemperatures [] = [toReal (0)]
averagetemperatures x = [sum x /toReal (length x)]
//Start = averagetemperatures   [1.1,12.0,13.0,16.2,12.5,3.0,11.3]

frist :: [[Real]] -> [Real]
frist [] = []
frist [x : xs] = maximumtemperatures x ++ frist xs
//Start = frist [[1.1,12.0,13.0,16.2,12.5,3.0,11.3]]

second :: [[Real]] -> [Real]
second [] = []
second [x : xs] = averagetemperatures x ++ second xs
//Start = second [[1.1,12.0,13.0,16.2,12.5,3.0,11.3]]

third :: [[Real]] -> [Real] 
third [] = []
third [x : xs] = averagetemperatures (frist [x : xs])
//Start = third [[1.1,12.0,13.0,16.2,12.5,3.0,11.3]]


module homework4
import StdEnv


//NAME: MURTADA ALNAKHLI

//NEPTUNCODE: RKB1O2


//A Turing-like encoding machine is encrypts numbers by attaching 1 at the beginning and at the end of the number,
// and by inserting 0 between the digits.. Write an encrypting for a list of integers. 
// E.g. [101, 25, 388, 77, 0, 1234] -> [1100011, 12051, 1308081, 17071, 101, 110203041].
//  To check that the machine is working properly, 
//write also the decrypting of the elements of a list.
// Thus the property: list == decoding (coding list) shoudl hold at the end.


first :: [Int] -> [Int]
first [] = []
first x = ([1]++x++[1])

/*first :: [Int] -> [Int]
first x = map f x
where f z = z rem  10 
*/

second :: [Int] -> [Int]
second [] = []
second x = (reverse(drop 1 (reverse x)))

third :: [Int] -> [Int]
third [] = [] 
third [x : xs] = [x,0 :third xs]

/*addnum :: [Int] -> [Int]
addnum l =  map (\x = x*10+100) l
*/



forth :: [Int] -> Int
forth [] = 0
forth [x:xs]= (x*(10^(length[x:xs]-1)) + forth xs)

fifth :: Int [Int] -> [Int]
fifth 0 x = x
fifth xs x = (fifth (xs/10) [xs rem 10: x])

sixth :: Int -> [Int] 
sixth 0 = [0]
sixth x = (fifth x[])


Start = map (\xs=forth(first(second(third(sixth xs)))))   [101,25,388,77,0,1234]



module homework5
import StdEnv

//NAME: MURTADA ALNAKHLI
//NEPTUN: RKB1O2


//Given a list of sublists compute the followings:

//delete every second element of the sublists

//sort each sublist

//at the end concatenate them in one list

//E.g. [[2,1,4],[5,8,3,1],[],[1..10],[10,3,8]] -> [2,4,3,5,1,3,5,7,9,8,10]



Delete ::[Int]->[Int]
Delete [] = []
Delete x = [hd x : Delete (drop 2 x)]

//Start = Delete [10,3,8]

Sort :: [Int] -> [Int]
Sort [] = []
Sort [i : xs] = Sort [ x \\ x <- xs | x < i] ++ [i] ++ Sort [ x \\ x <- xs | x >= i]

Last :: [[Int]] -> [Int]
Last x = flatten (map Sort (map Delete x))

Start = Last [[2,1,4],[5,8,3,1],[],[1..10],[10,3,8]]




module homework6
import StdEnv

//NEPTUN: RKB1O2
//NAME: MURTADA ALNAKHLI

// Generate the following 3 lists!

// l1 contains the first 10 pairs of odd an even numbers like [(1,2), (3,4), (5,6), ...] done

// l1::[(Int,Int)]

// The number, the double and the triple of a number of the numbers from 1 to 10. done

// l2::[(Int,Int,Int)]

// Generate five times the list from one to five like: [[1,2,3,4,5],[1,2,3,4,5],[1,2,3,4,5],[1,2,3,4,5],[1,2,3,4,5]]

// l3::[[Int]]

//Start = (l1, l2, l3)

l1 :: [(Int, Int)]
l1 = [(x,y) \\ x <- [1,3..19] & y <- [2,4..20] ]
//Start = l1
l2 :: [(Int, Int, Int)]
l2 = [(x,y,z) \\ x <- [1..10] , y <- [x*2] , z <- [x*3]]
//Start = l2
l3 :: [[Int]]
l3 = [[x \\ x <-[1..5]] \\ y <-[1,2,3,4,5]]
//Start = l3
Start = (l1, l2, l3)



module homework7
import StdEnv

//MURTADA ALNAKHLI
//RKB1O2

// 1. Create using an input list of tuples a new list of tuples like:
// [(1,1), (2,6), (3,9)] -> [(1,1,2), (2,6,8), (3,9,12)]
Create :: [(Int, Int)] -> [(Int, Int, Int)]
Create [] = []
Create [(x,y) : i] = [(x,y,x+y) : Create i]

Start = Create [(1,1), (2,6), (3,9)]


// 2. Compute the average of tuple elements using map
averages  :: [(Int, Int)] -> [Real]
averages [] = []
averages [(x,y) : i] = [toReal(x+y)/2.0 : averages i]
//Start = averages [(1,1), (2,4), (3,9)]



// 3. Put the product of the sublist elements in a list, you must use foldr
sublistsp :: [[Int]] -> [Int]
sublistsp [] = []
sublistsp [x : xs] = [foldr (*) 1 x : sublistsp xs]
//Start = sublistsp [[1, 2, 3], [3, 4], [5, 7, 1]] // [6,12,35]



// 4. Generate the following list of lists
// [[1],[2,1],[3,2,1],[4,3,2,1],[5,4,3,2,1]]
genNlist :: Int -> [[Int]]
genNlist  x = [ reverse [1..y] \\  y <- [1..x]]
//Start = genNlist 5

// 5. Delete every third element of a list
// e.g. [1,2,3,4,3,2,4,5] -> [1,2,4,3,4,5]
delthird :: [Int] -> [Int]
delthird [] = []
delthird i = (take 2 i) ++ delthird (drop 3 i)
//Start = delthird [1,2,3,4,3,2,4,5]


module homework8
import StdEnv



// 1. Delete from a list the elements that are squares e.g. [1,3,4,2,1,6,9,1,2,16] -> [3,2,6,2]

Delete :: [Int] -> [Int]
Delete x
| length x == 0 = []
| hd x > 1 && Delete1 (hd x)((hd x)/2) == False = [hd x] ++ Delete (tl x)
= Delete (tl x)

Delete1:: Int Int -> Bool
Delete1 y x
| x * x == y = True
| x == 1 = False
= Delete1 y (x-1)
//Start = Delete [1,3,4,2,1,6,9,1,2,16]


// 2. Remove the duplicates of a list that are in a sequence, i.e. one after the other
// e.g. [1,2,2,2,2,3,4,2,2,1,1,1,3,3,5,4,4,4,4] -> [1,2,3,4,2,1,3,5,4]

Remove :: [Int] -> [Int]
Remove n
| length n == 1 = [hd n]
| hd n == hd (tl n) = Remove (tl n)
= [hd n] ++ Remove (tl n)
//Start = Remove [1,2,2,2,2,3,4,2,2,1,1,1,3,3,5,4,4,4,4]



// 3. Generate elements from 1 to x first in increasing order, then in decreasing order.
// e.g. if x=4 then the list is [1,2,3,4,4,3,2,1]

Generate :: Int -> [Int]
Generate n = [1..n] ++ reverse[1..n]
Start = Generate 4



