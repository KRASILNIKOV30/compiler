const foldLeft = (f: int -> int -> int, init: int, arr: int[]) -> {
    if (arrayLength(arr) == 0) {
        return init;
    };

    var accum = f(init, arr[0]);

    iter (item of arr < drop(1)) {
        accum = f(item, accum);
    };

    return accum;
};

const sumArr = foldLeft((acc: int, curr: int) -> acc + curr, 0);
const multArr = foldLeft((acc: int, curr: int) -> acc * curr, 1);

const arr = [1, 2, 3, 4];
println(sumArr(arr));
println(multArr(arr));
