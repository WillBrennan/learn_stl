# Algorithm

#### `for_each`
Why does `for_each` return the UnaryPredicate? This is done so the predicate can change state as the its applied, this is removed in the parallel-execution version as elements can be applied out of order.