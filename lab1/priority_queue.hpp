#pragma once
#include <vector>
#include <stdexcept>
#include <ostream>
#include <algorithm>

class PriorityQueue
{
public:
  using value_type = int;
  using size_type = std::size_t;

  // --- Constructors / Destructor ---
  PriorityQueue() : is_max_heap_(true) {}
  explicit PriorityQueue(bool max_heap) : is_max_heap_(max_heap) {}
  explicit PriorityQueue(const std::vector<value_type> &values, bool max_heap = true)
      : data_(values), is_max_heap_(max_heap)
  {
    build_heap();
  }

  ~PriorityQueue() = default;

  PriorityQueue(const PriorityQueue &other) = default;
  PriorityQueue(PriorityQueue &&other) noexcept = default;

  PriorityQueue &operator=(const PriorityQueue &other) = default;
  PriorityQueue &operator=(PriorityQueue &&other) noexcept = default;

  // --- Basic methods ---
  void push(value_type value)
  {
    data_.push_back(value);
    sift_up(data_.size() - 1);
  }

  void pop()
  {
    if (empty())
      throw std::runtime_error("PriorityQueue is empty");
    std::swap(data_.front(), data_.back());
    data_.pop_back();
    if (!empty())
      sift_down(0);
  }

  const value_type &top() const
  {
    if (empty())
      throw std::runtime_error("PriorityQueue is empty");
    return data_.front();
  }

  bool empty() const noexcept { return data_.empty(); }
  size_type size() const noexcept { return data_.size(); }
  void clear() noexcept { data_.clear(); }
  void reserve(size_type count) { data_.reserve(count); }
  bool is_max_heap() const noexcept { return is_max_heap_; }

  // --- Operators ---
  bool operator==(const PriorityQueue &rhs) const noexcept
  {
    return (data_ == rhs.data_) && (is_max_heap_ == rhs.is_max_heap_);
  }

  bool operator!=(const PriorityQueue &rhs) const noexcept
  {
    return !(*this == rhs);
  }

  friend std::ostream &operator<<(std::ostream &os, const PriorityQueue &pq)
  {
    os << (pq.is_max_heap_ ? "MaxHeap" : "MinHeap") << " [";
    for (size_type i = 0; i < pq.data_.size(); ++i)
    {
      os << pq.data_[i];
      if (i + 1 < pq.data_.size())
        os << ", ";
    }
    os << "]\n";

    size_type level_count = 1;
    size_type printed = 0;

    for (size_type i = 0; i < pq.data_.size(); ++i)
    {
      os << pq.data_[i] << " ";
      ++printed;

      if (printed == level_count)
      {
        os << "\n";
        level_count *= 2;
        printed = 0;
      }
    }
    return os;
  }

  // --- Debug ---
  bool verify_heap(std::ostream &os = std::cout) const
  {
    for (size_type i = 0; i < data_.size(); ++i)
    {
      size_type left = 2 * i + 1;
      size_type right = 2 * i + 2;
      if (left < data_.size() && !(compare(data_[i], data_[left]) || data_[i] == data_[left]))
      {
        os << *this;
        return false;
      }

      if (right < data_.size() && !(compare(data_[i], data_[right]) || data_[i] == data_[right]))
      {
        os << *this;
        return false;
      }
    }
    return true;
  }

private:
  std::vector<value_type> data_;
  bool is_max_heap_;

  bool compare(value_type lhs, value_type rhs) const
  {
    return is_max_heap_ ? lhs > rhs : lhs < rhs;
  }

  void sift_up(size_type index)
  {
    while (index > 0)
    {
      size_type parent = (index - 1) / 2;
      if (compare(data_[index], data_[parent]))
      {
        std::swap(data_[index], data_[parent]);
        index = parent;
      }
      else
      {
        break;
      }
    }
  }

  void sift_down(size_type index)
  {
    size_type count = data_.size();
    for (;;)
    {
      size_type left = 2 * index + 1;
      size_type right = 2 * index + 2;
      size_type best = index;

      if (left < count && compare(data_[left], data_[best]))
        best = left;
      if (right < count && compare(data_[right], data_[best]))
        best = right;

      if (best != index)
      {
        std::swap(data_[index], data_[best]);
        index = best;
      }
      else
        break;
    }
  }

  void build_heap()
  {
    for (size_type i = data_.size() / 2; i-- > 0;)
      sift_down(i);
  }
};
