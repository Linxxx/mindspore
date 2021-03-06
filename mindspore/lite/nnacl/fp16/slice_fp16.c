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
#include "nnacl/fp16/slice_fp16.h"
#include <string.h>
#include "nnacl/op_base.h"
#include "nnacl/errorcode.h"

void DoSliceFp16(const float16_t *input, float16_t *output, SliceParameter *param, int thread_id) {
  int32_t out_dim1 = param->size_[1];
  int32_t out_dim2 = param->size_[2];
  int32_t out_dim3 = param->size_[3];
  size_t out_stride2 = out_dim3;
  size_t out_stride1 = out_stride2 * out_dim2;
  size_t out_stride0 = out_stride1 * out_dim1;
  size_t count_per_thread = UP_DIV(out_dim1, param->op_parameter_.thread_num_);
  size_t thread_stride = thread_id * count_per_thread;
  size_t copy_size = param->size_[3] * sizeof(float16_t);
  size_t in_stride2 = param->shape_[3];
  size_t in_stride1 = param->shape_[2] * in_stride2;
  size_t in_stride0 = param->shape_[1] * in_stride1;
  for (int i = 0; i < param->size_[0]; ++i) {
    size_t out_offset0 = i * out_stride0;
    size_t in_offset0 = (i + param->begin_[0]) * in_stride0 + param->begin_[3];
    for (size_t j = 0; j < count_per_thread; ++j) {
      size_t k = j + thread_stride;
      if (k >= out_dim1) {
        break;
      }
      size_t out_offset1 = k * out_stride1 + out_offset0;
      size_t in_offset1 = (k + param->begin_[1]) * in_stride1 + in_offset0;
      for (int l = 0; l < out_dim2; ++l) {
        size_t out_offset = out_offset1 + l * out_stride2;
        size_t in_offset = in_offset1 + (l + param->begin_[2]) * in_stride2;
        memcpy(output + out_offset, input + in_offset, copy_size);
      }
    }
  }
}

void DoSliceFp16NoParallel(const float16_t *input, float16_t *output, SliceParameter *param) {
  size_t copy_size = param->size_[3] * sizeof(float16_t);
  size_t in_stride2 = param->shape_[3];
  size_t in_stride1 = param->shape_[2] * in_stride2;
  size_t in_stride0 = param->shape_[1] * in_stride1;
  size_t out_offset = 0;
  for (int32_t dim0 = param->begin_[0]; dim0 < param->end_[0]; ++dim0) {
    size_t in_offset0 = dim0 * in_stride0 + param->begin_[3];
    for (size_t dim1 = param->begin_[1]; dim1 < param->end_[1]; ++dim1) {
      size_t in_offset1 = dim1 * in_stride1 + in_offset0;
      for (int32_t dim2 = param->begin_[2]; dim2 < param->end_[2]; ++dim2) {
        size_t in_offset = in_offset1 + dim2 * in_stride2;
        memcpy(output + out_offset, input + in_offset, copy_size);
        out_offset += param->size_[3];
      }
    }
  }
}
