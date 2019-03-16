module A where

import Prelude hiding( (||) ,(&&))
import Data.Char
True || _ =True
False || x=x

swap::(a,b)->(b,a)
swap(a,b)=(b,a)

distance :: Floating t => (t, t) -> (t, t) -> t
distance (a,b) (c,d)=sqrt((a-b)**2+(c-d)**2)

toUpperFirst :: String -> String
toUpperFirst (a:xs)=[toUpper a] ++ xs