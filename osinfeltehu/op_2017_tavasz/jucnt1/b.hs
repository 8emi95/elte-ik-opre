module A where

mountain :: Integer -> [Integer]
mountain n = [1..n] ++ [n-1,n-2..1]
