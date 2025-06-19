var mixedArguments = (a: int, b: int, c: string, d: int[]) -> {
    println(a - b);
    println(c);
    println(d);
};

const m1 = mixedArguments(2);
const m2 = m1(3, "Ohaya");
m2([3, 2, 1]);