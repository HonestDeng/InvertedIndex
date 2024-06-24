#include <gtest/gtest.h>
#include "../include/FileSystem.h"

using namespace InvertedIndex;

class FileSystemTest : public ::testing::Test {
protected:
  void SetUp() override {
    fs = new FileSystem();
  }

  void TearDown() override {
    delete fs;
  }

  FileSystem* fs;
};

TEST_F(FileSystemTest, CreateRootDirectory) {
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 0);
}

TEST_F(FileSystemTest, MkDirAndLS) {
  fs->MkDir("dir1", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->name_, "dir1");
  EXPECT_TRUE(nodes[0]->isDir_);
}

TEST_F(FileSystemTest, MkDirDuplicate) {
  fs->MkDir("dir1", "user1");
  fs->MkDir("dir1", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);  // Duplicate should not be created
}

TEST_F(FileSystemTest, TouchAndLS) {
  fs->Touch("file1", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->name_, "file1");
  EXPECT_FALSE(nodes[0]->isDir_);
}

TEST_F(FileSystemTest, TouchDuplicate) {
  fs->Touch("file1", "user1");
  fs->Touch("file1", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);  // Duplicate should not be created
}

TEST_F(FileSystemTest, CD) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  EXPECT_EQ(fs->LS().size(), 0);
  fs->CD("..");
  EXPECT_EQ(fs->LS().size(), 1);
}

TEST_F(FileSystemTest, CDInvalidDir) {
  fs->CD("nonexistent");
  EXPECT_EQ(fs->LS().size(), 0);  // Should remain in root
}

TEST_F(FileSystemTest, ChOwnByName) {
  fs->Touch("file1", "user1");
  fs->ChOwn("file1", "user2");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->owner_, "user2");
}

TEST_F(FileSystemTest, ChOwnByNode) {
  fs->Touch("file1", "user1");
  const auto& nodes = fs->LS();
  fs->ChOwn(nodes[0], "user2");
  EXPECT_EQ(nodes[0]->owner_, "user2");
}

TEST_F(FileSystemTest, RmFile) {
  fs->Touch("file1", "user1");
  fs->Rm("file1");
  EXPECT_EQ(fs->LS().size(), 0);
}

TEST_F(FileSystemTest, RmNonexistentFile) {
  fs->Rm("nonexistent");
  EXPECT_EQ(fs->LS().size(), 0);  // Should remain unaffected
}

TEST_F(FileSystemTest, RmDir) {
  fs->MkDir("dir1", "user1");
  fs->RmDir("dir1");
  EXPECT_EQ(fs->LS().size(), 0);
}

TEST_F(FileSystemTest, RmNonexistentDir) {
  fs->RmDir("nonexistent");
  EXPECT_EQ(fs->LS().size(), 0);  // Should remain unaffected
}

TEST_F(FileSystemTest, NestedDirectories) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->MkDir("dir2", "user1");
  fs->CD("dir2");
  fs->Touch("file1", "user1");
  EXPECT_EQ(fs->LS().size(), 1);
  fs->CD("..");
  EXPECT_EQ(fs->LS().size(), 1);
  fs->CD("..");
  EXPECT_EQ(fs->LS().size(), 1);
}

TEST_F(FileSystemTest, RecursiveRmDir) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->MkDir("dir2", "user1");
  fs->CD("dir2");
  fs->Touch("file1", "user1");
  fs->CD("..");
  fs->CD("..");
  fs->RmDir("dir1");
  EXPECT_EQ(fs->LS().size(), 0);  // Entire directory tree should be removed
}

TEST_F(FileSystemTest, CDUpFromRoot) {
  fs->CD("..");
  EXPECT_EQ(fs->LS().size(), 0);  // Should remain in root
}

TEST_F(FileSystemTest, DeeplyNestedDirectories) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->MkDir("dir2", "user1");
  fs->CD("dir2");
  fs->MkDir("dir3", "user1");
  fs->CD("dir3");
  fs->Touch("file1", "user1");
  fs->CD("..");
  fs->CD("..");
  fs->CD("..");
  fs->RmDir("dir1");
  EXPECT_EQ(fs->LS().size(), 0);  // Ensure entire nested structure is removed
}

TEST_F(FileSystemTest, ChOwnDeeplyNestedDirectories) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->MkDir("dir2", "user1");
  fs->CD("dir2");
  fs->MkDir("dir3", "user1");
  fs->CD("dir3");
  fs->Touch("file1", "user1");
  fs->ChOwn("file1", "user2");
  fs->CD("..");
  fs->ChOwn("dir3", "user2");
  fs->CD("..");
  fs->ChOwn("dir2", "user2");
  fs->CD("..");
  fs->ChOwn("dir1", "user2");
  const auto& nodes = fs->LS();
  EXPECT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->owner_, "user2");
}

TEST_F(FileSystemTest, SpecialCharacterFileNames) {
  fs->Touch("file@name!", "user1");
  fs->Touch("file#name$", "user1");
  fs->Touch("file%name^", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 3);
  EXPECT_EQ(nodes[0]->name_, "file@name!");
  EXPECT_EQ(nodes[1]->name_, "file#name$");
  EXPECT_EQ(nodes[2]->name_, "file%name^");
}

TEST_F(FileSystemTest, SpecialCharacterDirNames) {
  fs->MkDir("dir@name!", "user1");
  fs->MkDir("dir#name$", "user1");
  fs->MkDir("dir%name^", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 3);
  EXPECT_EQ(nodes[0]->name_, "dir@name!");
  EXPECT_EQ(nodes[1]->name_, "dir#name$");
  EXPECT_EQ(nodes[2]->name_, "dir%name^");
}

TEST_F(FileSystemTest, LargeNumberOfFiles) {
  const int num_files = 1000;
  for (int i = 0; i < num_files; ++i) {
    fs->Touch("file" + std::to_string(i), "user1");
  }
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), num_files);
  for (int i = 0; i < num_files; ++i) {
    EXPECT_EQ(nodes[i]->name_, "file" + std::to_string(i));
  }
}

TEST_F(FileSystemTest, LargeNumberOfDirectories) {
  const int num_dirs = 1000;
  for (int i = 0; i < num_dirs; ++i) {
    fs->MkDir("dir" + std::to_string(i), "user1");
  }
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), num_dirs);
  for (int i = 0; i < num_dirs; ++i) {
    EXPECT_EQ(nodes[i]->name_, "dir" + std::to_string(i));
  }
}

TEST_F(FileSystemTest, ChangeOwnerNonexistentFile) {
  fs->ChOwn("nonexistent", "user2");
  EXPECT_EQ(fs->LS().size(), 0);  // Should remain unaffected
}

TEST_F(FileSystemTest, RemoveFileFromNestedDir) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->Touch("file1", "user1");
  fs->Rm("file1");
  EXPECT_EQ(fs->LS().size(), 0);
  fs->CD("..");
  fs->RmDir("dir1");
  EXPECT_EQ(fs->LS().size(), 0);
}

TEST_F(FileSystemTest, ChangeOwnerOfDirectoryAndContents) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->Touch("file1", "user1");
  fs->MkDir("dir2", "user1");
  fs->ChOwn("file1", "user2");
  fs->ChOwn("dir2", "user2");
  fs->CD("..");
  fs->ChOwn("dir1", "user3");

  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->owner_, "user3");

  fs->CD("dir1");
  const auto& nested_nodes = fs->LS();
  ASSERT_EQ(nested_nodes.size(), 2);
  EXPECT_EQ(nested_nodes[0]->owner_, "user2");  // file1 should still have user2
  EXPECT_EQ(nested_nodes[1]->owner_, "user2");  // dir2 should still have user2
}

TEST_F(FileSystemTest, DeepRecursionCreateDelete) {
  fs->MkDir("dir1", "user1");
  fs->CD("dir1");
  fs->MkDir("dir2", "user1");
  fs->CD("dir2");
  fs->MkDir("dir3", "user1");
  fs->CD("dir3");
  fs->MkDir("dir4", "user1");
  fs->CD("dir4");
  fs->Touch("file1", "user1");

  // Go back to root and remove dir1
  fs->CD("..");
  fs->CD("..");
  fs->CD("..");
  fs->CD("..");
  fs->RmDir("dir1");

  EXPECT_EQ(fs->LS().size(), 0);  // Ensure entire nested structure is removed
}

TEST_F(FileSystemTest, ComplexPathNavigation) {
  fs->MkDir("dir1", "user1");
  fs->MkDir("dir2", "user1");
  fs->CD("dir1");
  fs->MkDir("dir3", "user1");
  fs->CD("dir3");
  fs->MkDir("dir4", "user1");

  // Navigate back to root and into dir2
  fs->CD("..");
  fs->CD("..");
  fs->CD("..");
  fs->CD("dir2");
  EXPECT_EQ(fs->LS().size(), 0);  // dir2 should be empty

  // Navigate back to dir1/dir3/dir4
  fs->CD("..");
  fs->CD("dir1");
  fs->CD("dir3");
  fs->CD("dir4");
  EXPECT_EQ(fs->LS().size(), 0);  // dir4 should be empty
}

TEST_F(FileSystemTest, MaxLengthFileName) {
  std::string long_name(255, 'a');
  fs->Touch(long_name, "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->name_, long_name);
}

TEST_F(FileSystemTest, MaxLengthDirName) {
  std::string long_name(255, 'a');
  fs->MkDir(long_name, "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->name_, long_name);
}

TEST_F(FileSystemTest, EmptyFileName) {
  fs->Touch("", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->name_, "");  // Empty file name should be allowed
}

TEST_F(FileSystemTest, EmptyDirName) {
  fs->MkDir("", "user1");
  const auto& nodes = fs->LS();
  ASSERT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes[0]->name_, "");  // Empty directory name should be allowed
}

TEST_F(FileSystemTest, RapidCreateDeleteFiles) {
  const int num_operations = 1000;
  for (int i = 0; i < num_operations; ++i) {
    fs->Touch("file" + std::to_string(i), "user1");
  }
  for (int i = 0; i < num_operations; ++i) {
    fs->Rm("file" + std::to_string(i));
  }
  EXPECT_EQ(fs->LS().size(), 0);  // Ensure all files are removed
}

TEST_F(FileSystemTest, RapidCreateDeleteDirs) {
  const int num_operations = 1000;
  for (int i = 0; i < num_operations; ++i) {
    fs->MkDir("dir" + std::to_string(i), "user1");
  }
  for (int i = 0; i < num_operations; ++i) {
    fs->RmDir("dir" + std::to_string(i));
  }
  EXPECT_EQ(fs->LS().size(), 0);  // Ensure all directories are removed
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
