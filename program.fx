const arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

const isEven = (a: int) -> (a mod 2 == 0);
const square = (a: int) -> a * a;

iter (item of arr
    < drop(1)
    < take(8)
    < reverse
    < filter(isEven)
    < transform(square)
) {
    println(item);
};

const cube = (a: int) -> a * a * a;
const arrFn = [
    (a: int) -> a * a,
    (a: int) -> a * a * a
];

var pipeline = 2;

iter (fn of arrFn) {
    pipeline = fn(pipeline);
};

println(pipeline);