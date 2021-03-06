// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lite/backends/arm/math/quantize.h"
#include <algorithm>
#include <cmath>

namespace paddle {
namespace lite {
namespace arm {
namespace math {

void QuantizeActvation(const float* input,
                       int8_t* output,
                       float* scale,
                       int size,
                       int bit_length) {
  auto abs_compare_func = [](float a, float b) {
    return (std::abs(a) < std::abs(b));
  };

  float abs_max_value =
      std::abs(*std::max_element(input, input + size, abs_compare_func));
  float scale_value = abs_max_value / ((1 << (bit_length - 1)) - 1);

  auto quant_func = [scale_value](float x) {
    return static_cast<int8_t>(std::round(x / scale_value));
  };

  std::transform(input, input + size, output, quant_func);
  *scale = scale_value;
}

}  // namespace math
}  // namespace arm
}  // namespace lite
}  // namespace paddle
