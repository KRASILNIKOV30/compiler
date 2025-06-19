const createMultiplier = (factor: float) -> {
    return (n: float) -> n * factor;
};

const doubler = createMultiplier(2.0);

println(doubler(3.2));