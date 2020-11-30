#include <filesystem>

#include <gtest/gtest.h>

#include "driver.hh"


class ParseTests : public testing::TestWithParam<std::string> {
protected:
    //void SetUp() override {}

    // void TearDown() override {}

    Driver Driver_;
};


TEST_P(ParseTests, SuccessParse) {
    ASSERT_EQ(Driver_.parse(GetParam()), 0);
    ASSERT_EQ(Driver_.Evaluate(), 0);
}


std::vector<std::string> FilesToTest(const std::vector<std::string>& testDirs) {
    std::vector<std::string> res;
    for (const auto& testDir : testDirs) {
        for (auto& file : std::filesystem::directory_iterator(testDir)) {
            if (file.is_regular_file()) { res.push_back(file.path()); }
        }
    }
    return res;
}


INSTANTIATE_TEST_SUITE_P(SuccessTest, ParseTests,
                         ::testing::ValuesIn(FilesToTest({"parse_tests", "if_statements_tests",
                                                          "print_and_exp_tests"})));