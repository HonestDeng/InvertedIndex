//
// Created by 邓实诚 on 2024/6/24.
//

#include "../include/Matcher.h"
#include "../include/FileSystem.h"

namespace InvertedIndex {

bool SuffixMatcher::match(FSNode* node) const {
  return node->suffix_ == condition_;
}

bool SizeMatcher::match(FSNode* node) const {
  return node->file_size_ == condition_;
}

bool OwnerMatcher::match(FSNode* node) const {
  return node->owner_ == condition_;
}

bool TimeMatcher::match(FSNode* node) const {
  return node->time_ == condition_;
}

} // namespace InvertedIndex