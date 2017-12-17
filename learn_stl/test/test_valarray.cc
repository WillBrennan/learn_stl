#include "learn_stl/valarray.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(valarray, Construction) {
    using valarray = learn::valarray<double>;

    ASSERT_NO_THROW({ valarray val_a(0.2, 30); });
    ASSERT_NO_THROW({ const auto val_a = valarray(0.3, 10); });
}

TEST(valarray, Addition) {
    using valarray = learn::valarray<double>;
    valarray val_a(0.35, 20);
    valarray val_b(0.232, 20);

    const valarray result = val_a + val_b;

    valarray val_expt(0.35 + 0.232, 20);
    EXPECT_EQ(result, val_expt);
}