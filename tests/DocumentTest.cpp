#include <gtest/gtest.h>
#include "Document.h"

/*
 * Credit to Sando Dargo's Blog on writing Unit tests on non-code files.
 * source:
 * https://www.sandordargo.com/blog/2024/02/21/cpp-tests-with-resources#copy-the-resources-to-the-build-file
 * 
 * Aside from that Google Gemini also helped in writing trivial tasks.
 */

TEST(DocumentTest, ConstructorTest)
{
    std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
    std::filesystem::path filePath = dirPath /= "DocumentTestFiles/sample.txt";

    Document doc(filePath);
    EXPECT_EQ(doc.GetRowsLength(), 5);
    EXPECT_EQ(doc.GetRows()[0].getText(), "I am finally writing unit tests.");
}

TEST(DocumentTest, ConstructorEmpty)
{
    // Test default constructor
    Document doc;
    EXPECT_EQ(doc.GetRowsLength(), 0);
}

TEST(DocumentTest, ConstructorPath)
{
    // Test constructor with path (assuming Document opens an empty file)
    std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
    std::filesystem::path filePath = dirPath /= "DocumentTestFiles/empty.txt";

    Document doc(filePath);
    EXPECT_EQ(doc.GetRowsLength(), 0);
}

TEST(DocumentTest, OpenInvalidPath)
{
    // Test Open method with an invalid path
    std::string invalid_path = "non-existent-file.txt";
    Document doc(invalid_path);
    EXPECT_EQ(doc.GetRowsLength(), 0); // Expect non-zero return on error

    // Now Test from valid to invalid.
    std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
    std::filesystem::path filePath = dirPath /= "DocumentTestFiles/sample.txt";

    Document doc2(filePath);
    EXPECT_EQ(doc2.GetRowsLength(), 5);
    EXPECT_EQ(doc2.GetRows()[0].getText(), "I am finally writing unit tests.");

    EXPECT_EQ(doc2.Open(invalid_path), -1);
    EXPECT_EQ(doc2.GetRowsLength(), 0);
}

// TEST(DocumentTest, Insert)
// {
//     // Test Insert method
//     Document doc;
//     doc.Insert(0, 0, 'a');
//     std::vector<Row> rows = doc.GetRows();
//     EXPECT_EQ(rows.size(), 1);
//     EXPECT_EQ(rows[0].getText(), 'a');
// }