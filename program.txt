const max = (a: int, b: int) -> {
   var result: int = 0;
   if (a > b)
   {
     result = a;
   }
   else
   {
     result = b;
   };
   return result;
};

const writeMax = (max: int -> int -> int, a: int, b: int) -> println(max(a, b));

writeMax(max, 10, 20);