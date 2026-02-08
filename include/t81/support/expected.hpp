#pragma once

#include <expected>

namespace t81 {

template <typename T, typename E>
using expected = std::expected<T, E>;

}
