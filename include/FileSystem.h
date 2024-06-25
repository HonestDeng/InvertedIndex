//
// Created by 邓实诚 on 2024/6/24.
//

#ifndef INVERTEDINDEX_FILESYSTEM_H
#define INVERTEDINDEX_FILESYSTEM_H

#include <string>
#include <vector>
#include <map>

namespace InvertedIndex {

class Matcher;

using FileID = size_t;

struct FSNode {
  std::string name_;
  std::string suffix_;
  std::string owner_;
  std::string time_;
  std::string file_size_;

  FileID id_;  // id_是文件系统中的唯一标识
  bool isDir_;
  std::vector<FSNode *> children_;
  std::vector<FSNode *> path_;

  std::string EmitPath() {
    std::string path;
    for (auto node: path_) {
      path += node->name_ + "/";
    }
    return path + name_;
  }

  ~FSNode() {
    for (auto child: children_) {
      delete child;
    }
  }
};

class FileSystem {
private:
  FSNode *root_;
  FSNode *current_;
  FileID next_id_;

  std::map<FileID, FSNode *> id2node_;

  void LoadJson(const std::string &fs_path);

  void RMHelper(FSNode *node);

  void FindHelper(FSNode *node, Matcher *matcher, std::vector<std::string> &result);

public:
  FileSystem();

  FileSystem(const std::string &fs_path);

  ~FileSystem();

  void CD(const std::string &dir_name);

  void ChOwn(FSNode *, const std::string &owner);

  void ChOwn(const std::string &, const std::string &owner);

  void ChTime(FSNode *, const std::string &time);

  void ChTime(const std::string &, const std::string &time);

  void Touch(const std::string &file_name, const std::string &owner);

  void MkDir(const std::string &dir_name, const std::string &owner);

  void Rm(const std::string &file_name);

  void RmDir(const std::string &dir_name);

  const std::vector<FSNode *> LS() const;

  std::vector<std::string> Find(Matcher *);
};

} // namespace InvertedIndex


#endif //INVERTEDINDEX_FILESYSTEM_H
