#include <gtest/gtest.h>
#include <utility>
#include "Document.h"
#include "Row.h"
#include "filesystem"
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

TEST(DocumentTest, ConstructorSyntaxHighlight)
{
  std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
  std::filesystem::path filePath = dirPath /= "DocumentTestFiles/cpp-sample.cpp";
    
  Document doc(filePath);
  EXPECT_EQ(doc.GetRowsLength(), 5);
  
  Row r1 = doc.GetRows()[0];
  EXPECT_EQ(r1.getHighlights().size(), 1);
  EXPECT_EQ(r1.getHighlights()[0].h_type, highlight_type::String);
  EXPECT_EQ(r1.getHighlights()[0].h_idx, std::make_pair(9, 19));

  Row r2 = doc.GetRows()[1];
  EXPECT_EQ(r2.getHighlights().size(), 0);

  Row r3 = doc.GetRows()[2];
  EXPECT_EQ(r3.getHighlights().size(), 2);
  EXPECT_EQ(r3.getHighlights()[0].h_type, highlight_type::Keyword);
  EXPECT_EQ(r3.getHighlights()[0].h_idx, std::make_pair(0, 3));
  EXPECT_EQ(r3.getHighlights()[1].h_type, highlight_type::Identifier);
  EXPECT_EQ(r3.getHighlights()[1].h_idx, std::make_pair(4, 8));

  Row r4 = doc.GetRows()[3];
  EXPECT_EQ(r4.getHighlights().size(), 5);
  EXPECT_EQ(r4.getHighlights()[0].h_type, highlight_type::Keyword);
  EXPECT_EQ(r4.getHighlights()[0].h_idx, std::make_pair(1, 4));
  EXPECT_EQ(r4.getHighlights()[1].h_type, highlight_type::Identifier);
  EXPECT_EQ(r4.getHighlights()[1].h_idx, std::make_pair(6, 10));
  EXPECT_EQ(r4.getHighlights()[2].h_type, highlight_type::String);
  EXPECT_EQ(r4.getHighlights()[2].h_idx, std::make_pair(14, 27));
  EXPECT_EQ(r4.getHighlights()[3].h_type, highlight_type::Keyword);
  EXPECT_EQ(r4.getHighlights()[3].h_idx, std::make_pair(31, 34));
  EXPECT_EQ(r4.getHighlights()[4].h_type, highlight_type::Identifier);
  EXPECT_EQ(r4.getHighlights()[4].h_idx, std::make_pair(36, 40));

  Row r5 = doc.GetRows()[4];
  EXPECT_EQ(r5.getHighlights().size(), 0);
}

TEST(DocumentTest, InsertSyntaxHighlight)
{
  std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
  std::filesystem::path filePath = dirPath /= "DocumentTestFiles/cpp-sample.cpp";
    
  Document doc(filePath);
  EXPECT_EQ(doc.GetRowsLength(), 5);

  doc.Insert(26, 3, '!');
  EXPECT_EQ(doc.GetRows()[3].getText(), "\tstd::cout << \"Hello World!\" << std::endl;");
  
  Row r1 = doc.GetRows()[0];
  EXPECT_EQ(r1.getHighlights().size(), 1);
  EXPECT_EQ(r1.getHighlights()[0].h_type, highlight_type::String);
  EXPECT_EQ(r1.getHighlights()[0].h_idx, std::make_pair(9, 19));

  Row r2 = doc.GetRows()[1];
  EXPECT_EQ(r2.getHighlights().size(), 0);

  Row r3 = doc.GetRows()[2];
  EXPECT_EQ(r3.getHighlights().size(), 2);
  EXPECT_EQ(r3.getHighlights()[0].h_type, highlight_type::Keyword);
  EXPECT_EQ(r3.getHighlights()[0].h_idx, std::make_pair(0, 3));
  EXPECT_EQ(r3.getHighlights()[1].h_type, highlight_type::Identifier);
  EXPECT_EQ(r3.getHighlights()[1].h_idx, std::make_pair(4, 8));

  Row r4 = doc.GetRows()[3];
  EXPECT_EQ(r4.getHighlights().size(), 5);
  EXPECT_EQ(r4.getHighlights()[0].h_type, highlight_type::Keyword);
  EXPECT_EQ(r4.getHighlights()[0].h_idx, std::make_pair(1, 4));
  EXPECT_EQ(r4.getHighlights()[1].h_type, highlight_type::Identifier);
  EXPECT_EQ(r4.getHighlights()[1].h_idx, std::make_pair(6, 10));
  EXPECT_EQ(r4.getHighlights()[2].h_type, highlight_type::String);
  EXPECT_EQ(r4.getHighlights()[2].h_idx, std::make_pair(14, 28));
  EXPECT_EQ(r4.getHighlights()[3].h_type, highlight_type::Keyword);
  EXPECT_EQ(r4.getHighlights()[3].h_idx, std::make_pair(32, 35));
  EXPECT_EQ(r4.getHighlights()[4].h_type, highlight_type::Identifier);
  EXPECT_EQ(r4.getHighlights()[4].h_idx, std::make_pair(37, 41));

  Row r5 = doc.GetRows()[4];
  EXPECT_EQ(r5.getHighlights().size(), 0);
}

TEST(DocumentTest, DeleteSimpleSyntax)
{
  std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
  std::filesystem::path filePath = dirPath /= "DocumentTestFiles/cpp-sample.cpp";
    
  Document doc(filePath);
  EXPECT_EQ(doc.GetRowsLength(), 5);

  doc.Delete(3, 2);
  EXPECT_EQ(doc.GetRows()[2].getText(), "intmain() {");
  
  Row r1 = doc.GetRows()[0];
  EXPECT_EQ(r1.getHighlights().size(), 1);
  EXPECT_EQ(r1.getHighlights()[0].h_type, highlight_type::String);
  EXPECT_EQ(r1.getHighlights()[0].h_idx, std::make_pair(9, 19));

  Row r2 = doc.GetRows()[1];
  EXPECT_EQ(r2.getHighlights().size(), 0);

  Row r3 = doc.GetRows()[2];
  EXPECT_EQ(r3.getHighlights().size(), 1);
  EXPECT_EQ(r3.getHighlights()[0].h_type, highlight_type::Identifier);
  EXPECT_EQ(r3.getHighlights()[0].h_idx, std::make_pair(0, 7));

  Row r4 = doc.GetRows()[3];
  EXPECT_EQ(r4.getHighlights().size(), 5);
  EXPECT_EQ(r4.getHighlights()[0].h_type, highlight_type::Keyword);
  EXPECT_EQ(r4.getHighlights()[0].h_idx, std::make_pair(1, 4));
  EXPECT_EQ(r4.getHighlights()[1].h_type, highlight_type::Identifier);
  EXPECT_EQ(r4.getHighlights()[1].h_idx, std::make_pair(6, 10));
  EXPECT_EQ(r4.getHighlights()[2].h_type, highlight_type::String);
  EXPECT_EQ(r4.getHighlights()[2].h_idx, std::make_pair(14, 27));
  EXPECT_EQ(r4.getHighlights()[3].h_type, highlight_type::Keyword);
  EXPECT_EQ(r4.getHighlights()[3].h_idx, std::make_pair(31, 34));
  EXPECT_EQ(r4.getHighlights()[4].h_type, highlight_type::Identifier);
  EXPECT_EQ(r4.getHighlights()[4].h_idx, std::make_pair(36, 40));

  Row r5 = doc.GetRows()[4];
  EXPECT_EQ(r5.getHighlights().size(), 0);
}

TEST(DocumentTest, DeleteFirstLine)
{
  std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
  std::filesystem::path filePath = dirPath /= "DocumentTestFiles/cpp-sample.cpp";
    
  Document doc(filePath);
  EXPECT_EQ(doc.GetRowsLength(), 5);
  
  doc.Delete(-1, 3);
  EXPECT_EQ(doc.GetRowsLength(), 4);

  Row r1 = doc.GetRows()[0];
  EXPECT_EQ(r1.getHighlights().size(), 1);
  EXPECT_EQ(r1.getHighlights()[0].h_type, highlight_type::String);
  EXPECT_EQ(r1.getHighlights()[0].h_idx, std::make_pair(9, 19));

  Row r2 = doc.GetRows()[1];
  EXPECT_EQ(r2.getHighlights().size(), 0);

  Row r3 = doc.GetRows()[2];
  EXPECT_EQ(r3.getHighlights().size(), 7);
  EXPECT_EQ(r3.getHighlights()[0].h_type, highlight_type::Keyword);
  EXPECT_EQ(r3.getHighlights()[0].h_idx, std::make_pair(0, 3));
  EXPECT_EQ(r3.getHighlights()[1].h_type, highlight_type::Identifier);
  EXPECT_EQ(r3.getHighlights()[1].h_idx, std::make_pair(4, 8));
  EXPECT_EQ(r3.getHighlights()[2].h_type, highlight_type::Keyword);
  EXPECT_EQ(r3.getHighlights()[2].h_idx, std::make_pair(13, 16));
  EXPECT_EQ(r3.getHighlights()[3].h_type, highlight_type::Identifier);
  EXPECT_EQ(r3.getHighlights()[3].h_idx, std::make_pair(18, 22));
  EXPECT_EQ(r3.getHighlights()[4].h_type, highlight_type::String);
  EXPECT_EQ(r3.getHighlights()[4].h_idx, std::make_pair(26, 39));
  EXPECT_EQ(r3.getHighlights()[5].h_type, highlight_type::Keyword);
  EXPECT_EQ(r3.getHighlights()[5].h_idx, std::make_pair(43, 46));
  EXPECT_EQ(r3.getHighlights()[6].h_type, highlight_type::Identifier);
  EXPECT_EQ(r3.getHighlights()[6].h_idx, std::make_pair(48, 52));

  Row r4 = doc.GetRows()[3];
  EXPECT_EQ(r4.getHighlights().size(), 0);
}

