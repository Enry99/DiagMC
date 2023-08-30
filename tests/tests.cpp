#include <gtest/gtest.h>
#include <diagmc/functions.h>


TEST(TestSuiteName, TestName)
{
	EXPECT_EQ(somma(5,5), 10);
}


TEST(TestSuiteName, TestName_wrong)
{
	EXPECT_EQ(somma(5,5), 11) << "Problem!";
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}