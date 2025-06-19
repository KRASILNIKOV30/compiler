const size = 9;

const printRow = (rowNumber: int) -> {
    iter (i of [1 .. size] < transform((colNumber: int) -> rowNumber * colNumber)) {
        print(toString(i) + ' ');
    };
    println();
};

println('Таблица Пифагора:');

iter (i of [1 .. size] < transform(printRow)) {};
