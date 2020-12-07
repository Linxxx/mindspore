/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "c_ops/lp_normalization.h"
#include "c_ops/op_utils.h"
#include "utils/check_convert_utils.h"

namespace mindspore {
void LpNormalization::Init(int64_t axis, int64_t p) {
  this->set_axis(axis);
  this->set_p(p);
}

void LpNormalization::set_axis(int64_t axis) { this->AddAttr(kAxis, MakeValue(axis)); }

int64_t LpNormalization::get_axis() const {
  auto value_ptr = this->GetAttr(kAxis);
  return GetValue<int64_t>(value_ptr);
}

void LpNormalization::set_p(int64_t p) { this->AddAttr(kP, MakeValue(p)); }

int64_t LpNormalization::get_p() const {
  auto value_ptr = this->GetAttr(kP);
  return GetValue<int64_t>(value_ptr);
}
REGISTER_PRIMITIVE_C(kNameLpNormalization, LpNormalization);
}  // namespace mindspore
