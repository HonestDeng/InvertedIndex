//
// Created by 邓实诚 on 2024/6/24.
//

#include "../include/FileSystem.h"

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
    for (auto child : current_->children_) {
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
  for (auto child : current_->children_) {
    if (child->name_ == file_name && !child->isDir_) {
      return;
    }
  }
  auto *file = new FSNode;
  file->name_ = file_name;
  file->owner_ = owner;
  file->id_ = next_id_++;
  file->isDir_ = false;
  file->path_ = current_->path_;
  file->path_.push_back(current_);
  current_->children_.push_back(file);
}

void FileSystem::MkDir(const std::string &dir_name, const std::string &owner) {
  if (dir_name == ".." || dir_name == ".") {
    return;
  }
  // Check if the directory already exists
  for (auto child : current_->children_) {
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
  current_->children_.push_back(dir);
}

void FileSystem::Rm(const std::string &file_name) {
  for (auto it = current_->children_.begin(); it != current_->children_.end(); ++it) {
    if ((*it)->name_ == file_name && !(*it)->isDir_) {
      delete *it;
      current_->children_.erase(it);
      break;
    }
  }
}

void FileSystem::RmDir(const std::string &dir_name) {
  for (auto it = current_->children_.begin(); it != current_->children_.end(); ++it) {
    if ((*it)->name_ == dir_name && (*it)->isDir_) {
      delete *it;
      current_->children_.erase(it);
      break;
    }
  }
}

const std::vector<FSNode*> FileSystem::LS() const {
  return current_->children_;
}
};