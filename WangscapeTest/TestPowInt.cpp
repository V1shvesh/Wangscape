#include <gtest/gtest.h>

#include <type_traits>

#include <utils.h>

template<typename I>
class TestPowInt : public ::testing::Test
{
protected:
    I zero;
    I one;
    I two;
    I three;
    I five;
    I max;
    I digits;
    TestPowInt()
    {
        zero = 0;
        one = 1;
        two = 2;
        three = 3;
        five = 5;
        max = std::numeric_limits<I>::max();
        digits = static_cast<I>(std::numeric_limits<I>::digits);

    };
    ~TestPowInt() {};
};

typedef ::testing::Types<
    char, short, int, long, long long,
    unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>
    IntTypes;
TYPED_TEST_CASE(TestPowInt, IntTypes);

TYPED_TEST(TestPowInt, TestPowInt00)
{
    EXPECT_EQ(this->one, pow(this->zero, this->zero));
}

TYPED_TEST(TestPowInt, TestPowIntX0)
{
    EXPECT_EQ(this->one, pow(this->one, this->zero));
    EXPECT_EQ(this->one, pow(this->two, this->zero));
    EXPECT_EQ(this->one, pow(this->three, this->zero));
    EXPECT_EQ(this->one, pow(this->five, this->zero));
    EXPECT_EQ(this->one, pow(this->digits, this->zero));
    EXPECT_EQ(this->one, pow(this->max, this->zero));
}

TYPED_TEST(TestPowInt, TestPowInt0X)
{
    EXPECT_EQ(this->zero, pow(this->zero, this->one));
    EXPECT_EQ(this->zero, pow(this->zero, this->two));
    EXPECT_EQ(this->zero, pow(this->zero, this->three));
    EXPECT_EQ(this->zero, pow(this->zero, this->five));
    EXPECT_EQ(this->zero, pow(this->zero, this->digits));
    EXPECT_EQ(this->zero, pow(this->zero, this->max));
}

TYPED_TEST(TestPowInt, TestPowIntX1)
{
    EXPECT_EQ(this->one, pow(this->one, this->one));
    EXPECT_EQ(this->two, pow(this->two, this->one));
    EXPECT_EQ(this->three, pow(this->three, this->one));
    EXPECT_EQ(this->five, pow(this->five, this->one));
    EXPECT_EQ(this->digits, pow(this->digits, this->one));
    EXPECT_EQ(this->max, pow(max, this->one));
}

TYPED_TEST(TestPowInt, TestPowInt1X)
{
    EXPECT_EQ(this->one, pow(this->one, this->one));
    EXPECT_EQ(this->one, pow(this->one, this->two));
    EXPECT_EQ(this->one, pow(this->one, this->three));
    EXPECT_EQ(this->one, pow(this->one, this->five));
    EXPECT_EQ(this->one, pow(this->one, this->digits));
    EXPECT_EQ(this->one, pow(this->one, this->max));
}

TYPED_TEST(TestPowInt, TestPowIntExamples)
{
    EXPECT_EQ(4, pow(this->two, this->two));
    EXPECT_EQ(9, pow(this->three, this->two));
    EXPECT_EQ(25, pow(this->five, this->two));
    EXPECT_EQ(this->one << (this->digits - 1), pow(this->two, this->digits - 1));

    EXPECT_EQ(8, pow(this->two, this->three));
    EXPECT_EQ(27, pow(this->three, this->three));
    EXPECT_EQ(125, pow(this->five, this->three));

    EXPECT_EQ(32, pow(this->two, this->five));
}

TYPED_TEST(TestPowInt, TestPowIntTooLarge)
{
    EXPECT_ANY_THROW(pow(this->two, this->digits));
    EXPECT_ANY_THROW(pow(this->two, this->max));
    EXPECT_ANY_THROW(pow(this->three, this->digits));
    EXPECT_ANY_THROW(pow(this->three, this->max));
    EXPECT_ANY_THROW(pow(this->digits, this->digits));
    EXPECT_ANY_THROW(pow(this->digits, this->max));
    EXPECT_ANY_THROW(pow(this->max, this->digits));
    EXPECT_ANY_THROW(pow(this->max, this->max));
}
