//
// Created by 邓实诚 on 2024/6/24.
//

#include "../include/Matcher.h"

namespace InvertedIndex {

bool Matcher::match(const std::string &text) const {
  return text.find(condition_) != std::string::npos;
}

bool SuffixMatcher::match(const std::string &text) const {
  return Matcher::match(text);
}

bool SizeMatcher::match(const std::string &text) const {
  return Matcher::match(text);
}

bool OwnerMatcher::match(const std::string &text) const {
  return Matcher::match(text);
}

bool TimeMatcher::match(const std::string &text) const {
  return Matcher::match(text);
}

} // namespace InvertedIndex