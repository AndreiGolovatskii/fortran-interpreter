#include <filesystem>

#include <gtest/gtest.h>

#include "driver.hh"


class TInterpreterTest : public testing::TestWithParam<std::string> {
protected:
    // void SetUp() override {}

    // void TearDown() override {}

    void TestInterpreter(const std::string& testDir) {
        TestInterpreter(testDir + "/src.f95", testDir + "/in.txt", testDir + "/out.txt");
    }

    void TestInterpreter(const std::string& fileSrc, const std::string& fileIn, const std::string& fileExpectedOut) {

        std::ifstream in(fileIn);
        std::ifstream expectedOut(fileExpectedOut);

        std::ostringstream out;
        std::ostringstream err;

        TDriver driver{in, out, err};
        ASSERT_EQ(driver.parse(fileSrc), 0);
        ASSERT_EQ(driver.Evaluate(), 0);

        std::istringstream realOut(out.str());
        while (realOut && expectedOut) {
            std::string outString;
            std::string expectedString;

            realOut >> outString;
            expectedOut >> expectedString;
            ASSERT_EQ(outString, expectedString);
        }
        ASSERT_FALSE(realOut);
        ASSERT_FALSE(expectedOut);
    }

    void TestParse(const std::string& src) {
        TDriver driver;
        ASSERT_EQ(driver.parse(src), 0);
    }
};


TEST_P(TInterpreterTest, ParseTest) { TestParse(GetParam()); }


std::vector<std::string> FilesToTest(const std::vector<std::string>& testDirs) {
    std::vector<std::string> res;
    for (const auto& testDir : testDirs) {
        for (auto& file : std::filesystem::directory_iterator(testDir)) {
            if (file.is_regular_file()) { res.push_back(file.path()); }
        }
    }
    return res;
}

INSTANTIATE_TEST_SUITE_P(ParseTest, TInterpreterTest, ::testing::ValuesIn(FilesToTest({"parse_tests"})));


TEST_F(TInterpreterTest, SimplePrintStatements) { TestInterpreter("prints"); }

TEST_F(TInterpreterTest, SimpleDoStatements) { TestInterpreter("do"); }


//TEST_F(TInterpreterTest, DoStatements) { TestInterpreter("simple_statements/do_statements.f95"); }