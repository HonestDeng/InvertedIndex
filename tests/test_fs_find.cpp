#include <gtest/gtest.h>
#include "../include/FileSystem.h"
#include "../include/Matcher.h"

using namespace InvertedIndex;

// 创建测试文件和目录的辅助函数
void CreateTestFile(FileSystem& fs, const std::string& file_name, const std::string& owner) {
  fs.Touch(file_name, owner);
}

void CreateTestDirectory(FileSystem& fs, const std::string& dir_name, const std::string& owner) {
  fs.MkDir(dir_name, owner);
}

class FileSystemTest : public ::testing::Test {
protected:
  FileSystem* fs;

  void SetUp() override {
    fs = new FileSystem();
    // 创建测试环境
    CreateTestDirectory(*fs, "test_dir", "owner1");
    fs->CD("test_dir");
    CreateTestFile(*fs, "file1.txt", "owner1");
    CreateTestFile(*fs, "file2.txt", "owner1");
    CreateTestFile(*fs, "file3.log", "owner1");
    CreateTestFile(*fs, "file with spaces.txt", "owner1");
    CreateTestFile(*fs, "file_with_special_#@!.txt", "owner1");
    CreateTestDirectory(*fs, "sub_dir", "owner1");
    fs->CD("sub_dir");
    CreateTestFile(*fs, "file4.txt", "owner1");
    CreateTestFile(*fs, "file5.TXT", "owner1");
    fs->CD("..");
    CreateTestDirectory(*fs, "another_sub_dir", "owner1");
    fs->CD("another_sub_dir");
    CreateTestFile(*fs, "file6.txt", "owner1");
    fs->CD("..");
    fs->CD("..");
  }

  void TearDown() override {
    delete fs;
  }
};

TEST_F(FileSystemTest, EmptyDirectory) {
  FileSystem emptyFs;
  CreateTestDirectory(emptyFs, "empty_dir", "owner1");
  emptyFs.CD("empty_dir");
  SuffixMatcher matcher("*.txt");
  auto results = emptyFs.Find(&matcher);
  EXPECT_EQ(results.size(), 0);
}

TEST_F(FileSystemTest, NoMatch) {
  SuffixMatcher matcher("*.md");
  auto results = fs->Find(&matcher);
  EXPECT_EQ(results.size(), 0);
}

TEST_F(FileSystemTest, FileAndDirectorySameName) {
  CreateTestDirectory(*fs, "dir", "owner1");
  CreateTestFile(*fs, "dir.txt", "owner1");
  SuffixMatcher matcher("txt");
  auto results = fs->Find(&matcher);
  EXPECT_EQ(results.size(), 7);  // including dir.txt
}

TEST_F(FileSystemTest, DifferentExtensions) {
  SuffixMatcher matcher("log");
  auto results = fs->Find(&matcher);
  EXPECT_EQ(results.size(), 1);
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/file3.log"), results.end());
}

TEST_F(FileSystemTest, RecursiveDirectories) {
  SuffixMatcher matcher("txt");
  auto results = fs->Find(&matcher);
  EXPECT_EQ(results.size(), 6);
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/file1.txt"), results.end());
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/file2.txt"), results.end());
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/file with spaces.txt"), results.end());
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/file_with_special_#@!.txt"), results.end());
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/sub_dir/file4.txt"), results.end());
  EXPECT_NE(std::find(results.begin(), results.end(), "root/test_dir/another_sub_dir/file6.txt"), results.end());
}


TEST_F(FileSystemTest, CaseSensitivity) {
  SuffixMatcher matcher("TXT");
  auto results = fs->Find(&matcher);
  EXPECT_EQ(results.size(), 1);
}

TEST_F(FileSystemTest, PerformanceTest) {
  fs->CD("test_dir");
  CreateTestDirectory(*fs, "performance", "owner1");
  fs->CD("performance");
  for (int i = 0; i < 1000; ++i) {
    CreateTestFile(*fs, "file" + std::to_string(i) + ".txt", "owner1");
  }
  fs->CD("..");
  SuffixMatcher matcher("txt");
  auto results = fs->Find(&matcher);
  EXPECT_GE(results.size(), 1005);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


