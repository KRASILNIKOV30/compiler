var fib = (a: int) -> a;
fib = (n: int) -> {
    if (n <= 1) {
        return n;
    };
    return fib(n - 1) + fib(n - 2);
};

println(fib(15));
