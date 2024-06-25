//
// Created by 邓实诚 on 2024/6/24.
//

#ifndef INVERTEDINDEX_MATCHER_H
#define INVERTEDINDEX_MATCHER_H

#include <string>

namespace InvertedIndex {
class FSNode;

class Matcher {
protected:
  std::string condition_;
public:
  explicit Matcher(std::string condition) : condition_(std::move(condition)) {}

  virtual bool match(FSNode*) const = 0;

  virtual ~Matcher() = default;
};

class SuffixMatcher : public Matcher {
public:
  explicit SuffixMatcher(std::string condition) : Matcher(std::move(condition)) {}

  bool match(FSNode*) const override;
};

class SizeMatcher : public Matcher {
public:
  explicit SizeMatcher(std::string condition) : Matcher(std::move(condition)) {}

  bool match(FSNode*) const override;
};

class OwnerMatcher : public Matcher {
public:
  explicit OwnerMatcher(std::string condition) : Matcher(std::move(condition)) {}

  bool match(FSNode*) const override;
};

class TimeMatcher : public Matcher {
public:
  explicit TimeMatcher(std::string condition) : Matcher(std::move(condition)) {}

  bool match(FSNode*) const override;
};

} // namespace InvertedIndex

#endif //INVERTEDINDEX_MATCHER_H
