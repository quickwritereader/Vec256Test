#pragma once

#include <ATen/cpu/vec256/intrinsics.h>
#include <ATen/cpu/vec256/vec256_base.h>
#include <ATen/cpu/vec256/vsx/vsx_helpers.h>
namespace at {
namespace vec256 {
// See Note [Acceptable use of anonymous namespace in header]
namespace {

template <>
class Vec256<int32_t> {
 private:
  union {
    struct {
      __vi _vec0;
      __vi _vec1;
    };
    struct {
      __vib _vecb0;
      __vib _vecb1;
    };

  } __attribute__((__may_alias__));

 public:
  using value_type = int32_t;
  using vec_internal_type = __vi;
  using vec_internal_mask_type = __vib;
  static constexpr int size() {
    return 8;
  }
  Vec256() {}
  __inline_attrs Vec256(__vi v) : _vec0{v}, _vec1{v} {}
  __inline_attrs Vec256(__vib vmask) : _vecb0{vmask}, _vecb1{vmask} {}
  __inline_attrs Vec256(__vi v1, __vi v2) : _vec0{v1}, _vec1{v2} {}
  __inline_attrs Vec256(__vib v1, __vib v2) : _vecb0{v1}, _vecb1{v2} {}
  __inline_attrs Vec256(int32_t scalar)
      : _vec0{vec_splats(scalar)}, _vec1{vec_splats(scalar)} {}
  __inline_attrs Vec256(
      int32_t scalar1,
      int32_t scalar2,
      int32_t scalar3,
      int32_t scalar4,
      int32_t scalar5,
      int32_t scalar6,
      int32_t scalar7,
      int32_t scalar8)
      : _vec0{__vi{scalar1, scalar2, scalar3, scalar4}},
        _vec1{__vi{scalar5, scalar6, scalar7, scalar8}} {}
  inline __inline_attrs const vec_internal_type& vec0() const {
    return _vec0;
  }
  inline __inline_attrs const vec_internal_type& vec1() const {
    return _vec1;
  }

  template <uint64_t mask>
  static std::enable_if_t<mask == 0, Vec256<int32_t>> __inline_attrs
  blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    return a;
  }

  template <uint64_t mask>
  static std::enable_if_t<(mask & 255) == 255, Vec256<int32_t>> __inline_attrs
  blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    return b;
  }

  template <uint64_t mask>
  static std::enable_if_t<mask == 15, Vec256<int32_t>> __inline_attrs
  blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    return {b._vec0, a._vec1};
  }

  template <uint64_t mask>
  static std::enable_if_t<(mask > 0 && mask < 15), Vec256<int32_t>>
      __inline_attrs blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    constexpr uint32_t g0 = (mask & 1) * 0xffffffff;
    constexpr uint32_t g1 = ((mask & 2) >> 1) * 0xffffffff;
    constexpr uint32_t g2 = ((mask & 4) >> 2) * 0xffffffff;
    constexpr uint32_t g3 = ((mask & 8) >> 3) * 0xffffffff;
    const __vib mask_1st = (__vib){g0, g1, g2, g3};

    return {(__vi)vec_sel(a._vec0, b._vec0, (__vib)mask_1st), a._vec1};
  }

  template <uint64_t mask>
  static std::enable_if_t<
      (mask > 15 && (mask & 255) != 255 && ((mask & 15) == 15)),
      Vec256<int32_t>>
      __inline_attrs blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    constexpr uint32_t mask2 = (mask & 255) >> 4;
    constexpr uint32_t g0_2 = (mask2 & 1) * 0xffffffff;
    constexpr uint32_t g1_2 = ((mask2 & 2) >> 1) * 0xffffffff;
    constexpr uint32_t g2_2 = ((mask2 & 4) >> 2) * 0xffffffff;
    constexpr uint32_t g3_2 = ((mask2 & 8) >> 3) * 0xffffffff;

    const __vib mask_2nd = (__vib){g0_2, g1_2, g2_2, g3_2};
    // generated masks
    return {b._vec0, (__vi)vec_sel(a._vec1, b._vec1, (__vib)mask_2nd)};
  }

  template <uint64_t mask>
  static std::enable_if_t<
      (mask > 15 && ((mask & 255) != 255) && ((mask & 15) == 0)),
      Vec256<int32_t>>
      __inline_attrs blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    constexpr uint32_t mask2 = (mask & 255) >> 4;
    constexpr uint32_t g0_2 = (mask2 & 1) * 0xffffffff;
    constexpr uint32_t g1_2 = ((mask2 & 2) >> 1) * 0xffffffff;
    constexpr uint32_t g2_2 = ((mask2 & 4) >> 2) * 0xffffffff;
    constexpr uint32_t g3_2 = ((mask2 & 8) >> 3) * 0xffffffff;

    const __vib mask_2nd = (__vib){g0_2, g1_2, g2_2, g3_2};
    // generated masks
    return {a, (__vi)vec_sel(a._vec1, b._vec1, (__vib)mask_2nd)};
  }

  template <uint64_t mask>
  static std::enable_if_t<
      (mask > 15 && ((mask & 255) != 255) && ((mask & 15) != 0) &&
       ((mask & 15) != 15)),
      Vec256<int32_t>>
      __inline_attrs blend(const Vec256<int32_t>& a, const Vec256<int32_t>& b) {
    constexpr uint32_t g0 = (mask & 1) * 0xffffffff;
    constexpr uint32_t g1 = ((mask & 2) >> 1) * 0xffffffff;
    constexpr uint32_t g2 = ((mask & 4) >> 2) * 0xffffffff;
    constexpr uint32_t g3 = ((mask & 8) >> 3) * 0xffffffff;
    constexpr uint32_t mask2 = (mask & 255) >> 4;
    constexpr uint32_t g0_2 = (mask2 & 1) * 0xffffffff;
    constexpr uint32_t g1_2 = ((mask2 & 2) >> 1) * 0xffffffff;
    constexpr uint32_t g2_2 = ((mask2 & 4) >> 2) * 0xffffffff;
    constexpr uint32_t g3_2 = ((mask2 & 8) >> 3) * 0xffffffff;

    const __vib mask_1st = (__vib){g0, g1, g2, g3};
    const __vib mask_2nd = (__vib){g0_2, g1_2, g2_2, g3_2};
    // generated masks
    return {
        (__vi)vec_sel(a._vec0, b._vec0, (__vib)mask_1st),
        (__vi)vec_sel(a._vec1, b._vec1, (__vib)mask_2nd)};
  }

  static Vec256<int32_t> __inline_attrs blendv(
      const Vec256<int32_t>& a,
      const Vec256<int32_t>& b,
      const Vec256<int32_t>& mask) {
    // the mask used here returned by comparision of vec256
    // assuming this we can use the same mask directly with vec_sel
    // warning intel style mask will not work properly
    return {
        vec_sel(a._vec0, b._vec0, mask._vecb0),
        vec_sel(a._vec1, b._vec1, mask._vecb1)};
  }

  static Vec256<int32_t> arange(int32_t base = 0.f, int32_t step = 1.f) {
    return Vec256<int32_t>(
        base,
        base + step,
        base + 2 * step,
        base + 3 * step,
        base + 4 * step,
        base + 5 * step,
        base + 6 * step,
        base + 7 * step);
  }
  static Vec256<int32_t> set(
      const Vec256<int32_t>& a,
      const Vec256<int32_t>& b,
      size_t count = size()) {
    switch (count) {
      case 0:
        return a;
      case 1:
        return blend<1>(a, b);
      case 2:
        return blend<3>(a, b);
      case 3:
        return blend<7>(a, b);
      case 4:
        return blend<15>(a, b);
      case 5:
        return blend<31>(a, b);
      case 6:
        return blend<63>(a, b);
      case 7:
        return blend<127>(a, b);
    }

    return b;
  }
  static Vec256<value_type> __inline_attrs
  loadu(const void* ptr, int count = size()) {
    if (count == size()) {
      return {
          vec_vsx_ld(offset0, reinterpret_cast<const value_type*>(ptr)),
          vec_vsx_ld(offset16, reinterpret_cast<const value_type*>(ptr))};
    }

    __at_align32__ value_type tmp_values[size()];
    std::memcpy(tmp_values, ptr, std::min(count, size()) * sizeof(value_type));

    return {vec_vsx_ld(offset0, tmp_values), vec_vsx_ld(offset16, tmp_values)};
  }
  void __inline_attrs store(void* ptr, int count = size()) const {
    if (count == size()) {
      vec_vsx_st(_vec0, offset0, reinterpret_cast<value_type*>(ptr));
      vec_vsx_st(_vec1, offset16, reinterpret_cast<value_type*>(ptr));
    } else if (count > 0) {
      __at_align32__ value_type tmp_values[size()];
      vec_vsx_st(_vec0, offset0, tmp_values);
      vec_vsx_st(_vec1, offset16, tmp_values);
      std::memcpy(
          ptr, tmp_values, std::min(count, size()) * sizeof(value_type));
    }
  }
  const int32_t& operator[](int idx) const = delete;
  int32_t& operator[](int idx) = delete;

  Vec256<int32_t> angle() const {
    return Vec256<int32_t>{0};
  }
  Vec256<int32_t> real() const {
    return *this;
  }
  Vec256<int32_t> imag() const {
    return Vec256<int32_t>{0};
  }
  Vec256<int32_t> conj() const {
    return *this;
  }

  Vec256<int32_t> __inline_attrs abs() const {
    return {vec_abs(_vec0), vec_abs(_vec1)};
  }

  Vec256<int32_t> __inline_attrs neg() const {
    return {vec_neg(_vec0), vec_neg(_vec1)};
  }

  DEFINE_MEMBER_OP(operator==, int32_t, vec_cmpeq)
  DEFINE_MEMBER_OP(operator!=, int32_t, vec_cmpne)
  DEFINE_MEMBER_OP(operator<, int32_t, vec_cmplt)
  DEFINE_MEMBER_OP(operator<=, int32_t, vec_cmple)
  DEFINE_MEMBER_OP(operator>, int32_t, vec_cmpgt)
  DEFINE_MEMBER_OP(operator>=, int32_t, vec_cmpge)
  DEFINE_MEMBER_OP_AND_ONE(eq, int32_t, vec_cmpeq)
  DEFINE_MEMBER_OP_AND_ONE(ne, int32_t, vec_cmpne)
  DEFINE_MEMBER_OP_AND_ONE(lt, int32_t, vec_cmplt)
  DEFINE_MEMBER_OP_AND_ONE(le, int32_t, vec_cmple)
  DEFINE_MEMBER_OP_AND_ONE(gt, int32_t, vec_cmpgt)
  DEFINE_MEMBER_OP_AND_ONE(ge, int32_t, vec_cmpge)
  DEFINE_MEMBER_OP(operator+, int32_t, vec_add)
  DEFINE_MEMBER_OP(operator-, int32_t, vec_sub)
  DEFINE_MEMBER_OP(operator*, int32_t, vec_mul)
  DEFINE_MEMBER_EMULATE_BINARY_OP(operator/, int32_t, /)
  DEFINE_MEMBER_OP(maximum, int32_t, vec_max)
  DEFINE_MEMBER_OP(minimum, int32_t, vec_min)
  DEFINE_MEMBER_OP(operator&, int32_t, vec_and)
  DEFINE_MEMBER_OP(operator|, int32_t, vec_or)
  DEFINE_MEMBER_OP(operator^, int32_t, vec_xor)
};

template <>
Vec256<int32_t> inline maximum(
    const Vec256<int32_t>& a,
    const Vec256<int32_t>& b) {
  return a.maximum(b);
}

template <>
Vec256<int32_t> inline minimum(
    const Vec256<int32_t>& a,
    const Vec256<int32_t>& b) {
  return a.minimum(b);
}

} // namespace
} // namespace vec256
} // namespace at
