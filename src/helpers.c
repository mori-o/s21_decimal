
#include "s21_decimal.h"
/*ФУНКЦИИ ВЫВОДА ИНТ256/ДЕЦИМАЛА/BITS 1 ИНТА*/
void print_binary(unsigned int num) {
  for (int i = 31; i >= 0; i--) {
    printf("%d", (num >> i) & 1);
  }
  printf("\n");
}

void print_decimal(s21_decimal num) {
  for (int i = 3; i >= 0; i--) {
    print_binary(num.bits[i]);
  }
}
void print_int256(s21_int256 num) {
  for (int i = 8; i >= 0; i--) {
    print_binary(num.bits[i]);
  }
}

void s21_set_sign(s21_decimal *num, int sign) {
  if (sign == S21_POSITIVE) {
    num->bits[3] &= ~(1 << 31);
  } else if (sign == S21_NEGATIVE) {
    num->bits[3] |= (1 << 31);
  }
}
/*ФУНКЦИИ ПОЛУЧЕНИЯ/УСТАНОВКИ СТЕПЕНЕЙ/ЗНАКОВ В ТИПАХ ДАННЫХ INT256/DECIMAL*/
void s21_set_scale(s21_decimal *num, int scale) {
  num->bits[3] &= ~(0xFF << 16);
  num->bits[3] |= (scale << 16);
}

int s21_get_sign(s21_decimal num) { return (num.bits[3] >> 31) & 1; }

int s21_get_scale(s21_decimal num) { return (num.bits[3] >> 16) & 0xFF; }
int is_zero(s21_decimal num) {
  return num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0;
}
int s21_get_bit(s21_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}
void s21_set_bit(s21_decimal *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}
int s21_get_bit_256(s21_int256 dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}
void s21_set_bit_256(s21_int256 *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}

// double decimal_to_double(s21_decimal decimal) {
//   double result = 0.0;
//   double integer_part = (double)decimal.bits[0] +
//                         (double)decimal.bits[1] * pow(2.0, 32.0) +
//                         (double)decimal.bits[2] * pow(2.0, 64.0);

//   result = integer_part;

//   int scale = s21_get_scale(decimal);
//   result /= pow(10.0, (double)scale);

//   int sign = s21_get_sign(decimal);
//   if (sign) {
//     result = -result;
//   }

//   return result;
// }
void multiply_by_ten(s21_decimal *dec) {
  unsigned long long result[4] = {0};
  for (int i = 0; i < 3; i++) {
    unsigned long long temp = (unsigned long long)dec->bits[i] * 10;
    result[i] += temp;
    if (result[i] > 0xFFFFFFFF) {
      result[i + 1] += result[i] >> 32;
      result[i] &= 0xFFFFFFFF;
    }
  }
  for (int i = 0; i < 3; i++) {
    dec->bits[i] = (int)result[i];
  }
}
s21_decimal decimal_copy(s21_decimal source) {
  s21_decimal copy;
  for (int i = 0; i < 4; i++) {
    copy.bits[i] = source.bits[i];
  }
  return copy;
}
void s21_normalize_scale(s21_decimal *value1, s21_decimal *value2) {
  int scale1 = s21_get_scale(*value1);
  int scale2 = s21_get_scale(*value2);
  int diff = scale1 - scale2;

  if (diff > 0) {
    for (int i = 0; i < diff; i++) {
      multiply_by_ten(value2);
    }
    s21_set_scale(value2, scale1);
  } else if (diff < 0) {
    for (int i = 0; i < -diff; i++) {
      multiply_by_ten(value1);
    }
    s21_set_scale(value1, scale2);
  }
}
/*ЗАНУЛЕНИЕ ИНТ256/DECIMAL*/
void s21_zero_decimal(s21_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
}
void s21_zero_int256(s21_int256 *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[4] =
      dst->bits[5] = dst->bits[6] = dst->bits[7] = dst->bits[8] = 0;
}

int s21_find_highest_bit_int256(s21_int256 num) {
  for (int i = 255; i >= 0; i--) {
    if (s21_get_bit_256(num, i)) {
      return i;
    }
  }
  return -1;
}
/*СМЕЩЕНИЕ ВПРАВО/ВЛЕВО ВНУТРИ ПЕРЕМЕННОЙ ИНТ256*/
int s21_shift_int256_left(s21_int256 *dst, int num) {
  int error = 0;
  int buffer[8] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_256(*dst, (i + 1) * 32 - 1);
    }
    for (int i = 7; i > 0 && !error; i--) {
      if (s21_get_bit_256(*dst, 255)) error = 1;
      dst->bits[i] <<= 1;
      s21_set_bit_256(dst, i * 32, buffer[i - 1]);
    }
    dst->bits[0] <<= 1;
  }
  return error;
}
void s21_shift_int256_right(s21_int256 *dst, int num) {
  int buffer[7] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_256(*dst, (i + 1) * 32);
    }
    for (int i = 0; i < 7; i++) {
      dst->bits[i] >>= 1;
      s21_set_bit_256(dst, (i + 1) * 32 - 1, buffer[i]);
    }
    dst->bits[7] >>= 1;
  }
}
int s21_check_int256(s21_int256 value) {
  return value.bits[0] + value.bits[2] + value.bits[3] + value.bits[4] +
         value.bits[5] + value.bits[6] + value.bits[7];
}
void s21_to_int256(s21_decimal value1, s21_int256 *value2) {
  value2->bits[0] = value1.bits[0];
  value2->bits[1] = value1.bits[1];
  value2->bits[2] = value1.bits[2];
}

int s21_is_greater_int256(s21_int256 value1, s21_int256 value2) {
  int ret = S21_EQ;
  if (value1.bits[8] > value2.bits[8] || value1.bits[7] > value2.bits[7] ||
      value1.bits[6] > value2.bits[6] || value1.bits[5] > value2.bits[5] ||
      value1.bits[4] > value2.bits[4] || value1.bits[3] > value2.bits[3] ||
      value1.bits[2] > value2.bits[2] || value1.bits[1] > value2.bits[1] ||
      value1.bits[0] > value2.bits[0]) {
    ret = S21_BIGGER;
  } else if (value1.bits[8] < value2.bits[8] ||
             value1.bits[7] < value2.bits[7] ||
             value1.bits[6] < value2.bits[6] ||
             value1.bits[5] < value2.bits[5] ||
             value1.bits[4] < value2.bits[4] ||
             value1.bits[3] < value2.bits[3] ||
             value1.bits[2] < value2.bits[2] ||
             value1.bits[1] < value2.bits[1] ||
             value1.bits[0] < value2.bits[0]) {
    ret = S21_LESS;
  }
  return ret;
}
void s21_set_value(s21_decimal *num, long long value) {
  num->bits[0] = value & 0xFFFFFFFF;
  num->bits[1] = (value >> 32) & 0xFFFFFFFF;
  num->bits[2] = (value >> 63) & 0xFFFFFFFF;
}