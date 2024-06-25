//
// Created by 邓实诚 on 2024/6/24.
//

#include "../include/FileSystem.h"
#include "../include/Matcher.h"

namespace InvertedIndex {

FileSystem::FileSystem() {
  next_id_ = 0;
  root_ = new FSNode;
  root_->name_ = "root";
  root_->owner_ = "root";
  root_->id_ = next_id_++;
  root_->isDir_ = true;
  current_ = root_;
}

FileSystem::~FileSystem() {
  delete root_;
}

void FileSystem::CD(const std::string &dir_name) {
  if (dir_name == "..") {
    if (current_ != root_) {
      current_ = current_->path_.back();
    }
  } else {
    for (auto child: current_->children_) {
      if (child->name_ == dir_name && child->isDir_) {
        current_ = child;
        break;
      }
    }
  }
}

void FileSystem::ChOwn(FSNode *node, const std::string &owner) {
  node->owner_ = owner;
}

void FileSystem::ChOwn(const std::string &file_name, const std::string &owner) {
  for (auto child: current_->children_) {
    if (child->name_ == file_name) {
      child->owner_ = owner;
      break;
    }
  }
}

void FileSystem::ChTime(FSNode *node, const std::string &time) {
  node->time_ = time;
}

void FileSystem::ChTime(const std::string &file_name, const std::string &time) {
  for (auto child: current_->children_) {
    if (child->name_ == file_name) {
      child->time_ = time;
      break;
    }
  }
}

void FileSystem::Touch(const std::string &file_name, const std::string &owner) {
  if (file_name == ".." || file_name == ".") {
    return;
  }
  // Check if the file already exists
  for (auto child: current_->children_) {
    if (child->name_ == file_name && !child->isDir_) {
      return;
    }
  }
  auto *file = new FSNode;
  file->name_ = file_name;
  file->owner_ = owner;
  file->id_ = next_id_++;
  file->isDir_ = false;
  file->suffix_ = file_name.substr(file_name.find_last_of('.') + 1);
  file->path_ = current_->path_;
  file->path_.push_back(current_);
  id2node_[file->id_] = file;
  current_->children_.push_back(file);
}

void FileSystem::MkDir(const std::string &dir_name, const std::string &owner) {
  if (dir_name == ".." || dir_name == ".") {
    return;
  }
  // Check if the directory already exists
  for (auto child: current_->children_) {
    if (child->name_ == dir_name && child->isDir_) {
      return;
    }
  }
  auto *dir = new FSNode;
  dir->name_ = dir_name;
  dir->owner_ = owner;
  dir->id_ = next_id_++;
  dir->isDir_ = true;
  dir->path_ = current_->path_;
  dir->path_.push_back(current_);
  id2node_[dir->id_] = dir;
  current_->children_.push_back(dir);
}

void FileSystem::RMHelper(InvertedIndex::FSNode *node) {
  for (auto child: node->children_) {
    RMHelper(child);
  }
  node->children_.clear();
  id2node_.erase(node->id_);
  delete node;
}

void FileSystem::Rm(const std::string &file_name) {
  for (auto it = current_->children_.begin(); it != current_->children_.end(); ++it) {
    if ((*it)->name_ == file_name && !(*it)->isDir_) {
      id2node_.erase((*it)->id_);
      delete *it;
      current_->children_.erase(it);
      return;
    }
  }
  int a = 0;
}

void FileSystem::RmDir(const std::string &dir_name) {
  for (auto it = current_->children_.begin(); it != current_->children_.end(); ++it) {
    if ((*it)->name_ == dir_name && (*it)->isDir_) {
      RMHelper(*it);
      current_->children_.erase(it);
      break;
    }
  }
}

const std::vector<FSNode *> FileSystem::LS() const {
  return current_->children_;
}

void FileSystem::FindHelper(InvertedIndex::FSNode *node, InvertedIndex::Matcher *matcher,
                            std::vector<std::string> &result) {
  if (matcher->match(node)) {
    result.push_back(node->EmitPath());
  }
  for (auto child: node->children_) {
    FindHelper(child, matcher, result);
  }
}

std::vector<std::string> FileSystem::Find(Matcher * matcher) {
  std::vector<std::string> result;
  FindHelper(root_, matcher, result);
  return result;
}

};