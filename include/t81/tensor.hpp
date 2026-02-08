#ifndef T81_TENSOR_HPP
#define T81_TENSOR_HPP

#include <stdexcept>
#include <utility>
#include <vector>

namespace t81 {

template <typename T>
class T729TensorBase {
public:
  T729TensorBase() = default;

  explicit T729TensorBase(std::vector<int> shape)
      : shape_(std::move(shape)), data_(size_from_shape_(shape_)) {}

  T729TensorBase(std::vector<int> shape, std::vector<T> data)
      : shape_(std::move(shape)), data_(std::move(data)) {
    if (data_.size() != size_from_shape_(shape_)) {
      throw std::invalid_argument("T729Tensor: data size mismatch");
    }
  }

  const std::vector<int>& shape() const { return shape_; }
  const std::vector<T>& data() const { return data_; }
  std::vector<T>& data() { return data_; }

private:
  static std::size_t size_from_shape_(const std::vector<int>& shape) {
    if (shape.empty()) return 0;
    std::size_t n = 1;
    for (int d : shape) {
      if (d <= 0) throw std::invalid_argument("T729Tensor: non-positive dimension");
      n *= static_cast<std::size_t>(d);
    }
    return n;
  }

  std::vector<int> shape_;
  std::vector<T> data_;
};

using T729Tensor = T729TensorBase<float>;

} // namespace t81

#endif
