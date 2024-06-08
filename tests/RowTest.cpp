#include <gtest/gtest.h>
#include "Row.h"

TEST(RowTest, InsertAndDeleteTests) {
  Row row;
  EXPECT_EQ(row.getText(), "");
  EXPECT_EQ(row.getLen(), 0);

  row.insertText(0, 'h');
  row.insertText(1, 'e');
  row.insertText(2, 'l');
  row.insertText(3, 'l');
  row.insertText(4, 'o');
  
  EXPECT_EQ(row.getText(), "hello");
  EXPECT_EQ(row.getLen(), 5);

  row.insertText(1, 'a');
  EXPECT_EQ(row.getText(), "haello");
  EXPECT_EQ(row.getLen(), 6);

  row.deleteChar(0);
  EXPECT_EQ(row.getText(), "aello");
  row.deleteChar(1);
  EXPECT_EQ(row.getText(), "allo");
  row.deleteChar(2);
  EXPECT_EQ(row.getText(), "alo");
  row.deleteChar(5);
  EXPECT_EQ(row.getText(), "alo");
}

TEST(RowTest, AppendTests) {
  Row row1("Hello");
  Row row2("World!");

  row1.appendRow(row2);
  EXPECT_EQ(row1.getText(), "HelloWorld!");
  EXPECT_EQ(row1.getLen(), 11);

  row1.removeString(5);
  EXPECT_EQ(row1.getText(), "Hello");
  EXPECT_EQ(row1.getLen(), 5);
}

TEST(RowTest, EdgeCases) {
  Row row;
  row.deleteChar(0);
  EXPECT_EQ(row.getText(), "");

  Row row1("Hello");
  row1.removeString(6);
  EXPECT_EQ(row1.getText(), "Hello");
  EXPECT_EQ(row1.getLen(), 5);

  row1.removeString(5);
  EXPECT_EQ(row1.getText(), "Hello");
  EXPECT_EQ(row1.getLen(), 5);
}
