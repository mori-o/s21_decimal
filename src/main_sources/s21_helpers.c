#include "../s21_decimal.h"

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
  for (int i = 7; i >= 0; i--) {
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

void s21_set_sign_256(s21_int256 *num, int sign) {
  if (sign == S21_POSITIVE) {
    num->bits[7] &= ~(1 << 31);
  } else if (sign == S21_NEGATIVE) {
    num->bits[7] |= (1 << 31);
  }
}

/*ФУНКЦИИ ПОЛУЧЕНИЯ/УСТАНОВКИ СТЕПЕНЕЙ/ЗНАКОВ В ТИПАХ ДАННЫХ INT256/DECIMAL*/
void s21_set_scale(s21_decimal *num, int scale) {
  num->bits[3] &= ~(0xFF << 16);
  num->bits[3] |= (scale << 16);
}

void s21_set_scale_256(s21_int256 *num, int scale) {
  num->bits[7] &= ~(0xFF << 16);
  num->bits[7] |= (scale << 16);
}

int s21_get_sign(s21_decimal num) { return (num.bits[3] >> 31) & 1; }
int s21_get_sign_256(s21_int256 num) { return (num.bits[7] >> 31) & 1; }

int s21_get_scale(s21_decimal num) { return (num.bits[3] >> 16) & 0xFF; }
int s21_get_scale_256(s21_int256 num) { return (num.bits[7] >> 16) & 0xFF; }

int s21_is_zero(s21_decimal num) {
  return num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0;
}

int s21_is_zero_256(s21_int256 num) {
  int res = 1;
  for (int i = 0; i < 7; i++) {
    res *= (num.bits[i] == 0);
  }
  return res;
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

// void multiply_by_ten(s21_decimal *dec) {
//   unsigned long long result[4] = {0};
//   for (int i = 0; i < 3; i++) {
//     unsigned long long temp = (unsigned long long)dec->bits[i] * 10;
//     result[i] += temp;
//     if (result[i] > 0xFFFFFFFF) {
//       result[i + 1] += result[i] >> 32;
//       result[i] &= 0xFFFFFFFF;
//     }
//   }
//   for (int i = 0; i < 3; i++) {
//     dec->bits[i] = (int)result[i];
//   }
// }

// умножение мантиссы на 10
// в случае переполнения возвращает ошибку
int multiply_by_ten(s21_decimal *value) {
  s21_int256 val256;
  s21_to_int256(*value, &val256);
  multiply_by_ten_256(&val256);
  return s21_from_int256(val256, value);
}

void multiply_by_ten_256(s21_int256 *dec) {
  unsigned long long result[7] = {0};
  for (int i = 0; i < 7; i++) {
    unsigned long long temp = (unsigned long long)dec->bits[i] * 10;
    result[i] += temp;
    if (result[i] > 0xFFFFFFFF) {
      result[i + 1] += result[i] >> 32;
      result[i] &= 0xFFFFFFFF;
    }
  }
  for (int i = 0; i < 7; i++) {
    dec->bits[i] = (int)result[i];
  }
}

// void divide_by_ten(s21_decimal *value) {
//   uint32_t remainder = 0;
//   for (int i = 2; i >= 0; i--) {
//     // Объединяем остаток и текущий разряд в 64-битное число.
//     unsigned long long cur =
//         ((unsigned long long)remainder << 32) | value->bits[i];
//     value->bits[i] = (uint32_t)(cur / 10);
//     remainder = (uint32_t)(cur % 10);
//   }
// }

// деление мантиссы на 10
// в случае переполнения возвращает ошибку
int divide_by_ten(s21_decimal *value) {
  s21_int256 val256;
  s21_to_int256(*value, &val256);
  divide_by_ten_256(&val256);
  return s21_from_int256(val256, value);
}

// деление мантиссы на 10
void divide_by_ten_256(s21_int256 *value) {
  uint32_t remainder = 0;
  for (int i = 6; i >= 0; i--) {
    // Объединяем остаток и текущий разряд в 64-битное число.
    unsigned long long cur =
        ((unsigned long long)remainder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(cur / 10);
    remainder = (uint32_t)(cur % 10);
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

void s21_normalize_scale_256(s21_int256 *value1, s21_int256 *value2) {
  int scale1 = s21_get_scale_256(*value1);
  int scale2 = s21_get_scale_256(*value2);
  int diff = scale1 - scale2;

  if (diff > 0) {
    for (int i = 0; i < diff; i++) {
      multiply_by_ten_256(value2);
    }
    s21_set_scale_256(value2, scale1);
  } else if (diff < 0) {
    for (int i = 0; i < -diff; i++) {
      multiply_by_ten_256(value1);
    }
    s21_set_scale_256(value1, scale2);
  }
}

/*ЗАНУЛЕНИЕ ИНТ256/DECIMAL*/
void s21_zero_decimal(s21_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
}

void s21_zero_int256(s21_int256 *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[4] =
      dst->bits[5] = dst->bits[6] = dst->bits[7] = 0;
}

void s21_shift_decimal_left(s21_decimal *dst, int num) {
  int buffer[3] = {0};
  // прохождение по каждому сдвигу
  for (int k = 0; k < num; k++) {
    // извлечение крайнего выпадающего бита
    for (int i = 0; i < 3; i++) {
      buffer[i] = s21_get_bit(*dst, (i + 1) * 32 - 1);
    }
    // сдвиг каждого из блоков на 1 бит влево
    for (int i = 0; i < 3; i++) {
      dst->bits[i] <<= 1;
    }
    // установка крайнего бита
    for (int i = 0; i < 2; i++) {
      s21_set_bit(dst, (i + 1) * 32, buffer[i]);
    }
  }
}

void s21_shift_int256_left(s21_int256 *dst, int num) {
  int buffer[7] = {0};
  // прохождение по каждому сдвигу
  for (int k = 0; k < num; k++) {
    // извлечение крайнего выпадающего бита
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_256(*dst, (i + 1) * 32 - 1);
    }
    // сдвиг каждого из блоков на 1 бит влево
    for (int i = 0; i < 7; i++) {
      dst->bits[i] <<= 1;
    }
    // установка крайнего бита
    for (int i = 0; i < 6; i++) {
      s21_set_bit_256(dst, (i + 1) * 32, buffer[i]);
    }
  }
}

void s21_shift_decimal_right(s21_decimal *dst, int num) {
  for (int k = 0; k < num; k++) {
    unsigned int carry0 = dst->bits[1] & 1;
    unsigned int carry1 = dst->bits[2] & 1;
    dst->bits[0] = (dst->bits[0] >> 1) | (carry0 << 31);
    dst->bits[1] = (dst->bits[1] >> 1) | (carry1 << 31);
    dst->bits[2] >>= 1;
  }
}

void s21_shift_int256_right(s21_int256 *dst, int num) {
  unsigned int carry[6] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 6; i++) {
      carry[i] = dst->bits[i + 1] & 1;
      // unsigned int carry1 = dst->bits[2] & 1;
      dst->bits[i] = (dst->bits[i] >> 1) | (carry[i] << 31);
      // dst->bits[1] = (dst->bits[1] >> 1) | (carry1 << 31);
    }
    dst->bits[6] >>= 1;
  }
}

int s21_check_int256(s21_int256 value) {
  return value.bits[0] + value.bits[2] + value.bits[3] + value.bits[4] +
         value.bits[5] + value.bits[6] + value.bits[7];
}

void s21_to_int256(s21_decimal value1, s21_int256 *value2) {
  s21_zero_int256(value2);
  value2->bits[0] = value1.bits[0];
  value2->bits[1] = value1.bits[1];
  value2->bits[2] = value1.bits[2];
  value2->bits[7] = value1.bits[3];
}

int s21_from_int256(s21_int256 value, s21_decimal *result) {
  s21_zero_decimal(result);

  // предварительная проверка аргументов функции
  if (s21_is_zero_256(value)) {
    return S21_OK;
  }

  int error = S21_OK;
  s21_decimal max = MAX_DECIMAL;
  s21_int256 max256;
  s21_to_int256(max, &max256);

  int scale = s21_get_scale_256(value);
  int sign = s21_get_sign_256(value);
  s21_set_sign_256(&value, S21_POSITIVE);

  s21_int256 last_num = {{0}}; // последняя отсеченная цифра
  int is_round = 0; // нужно ли округление

  // пока мантисса больше максимального числа или пока scale > 28, делим на 10
  while ((!s21_is_zero_256(value) || is_round) && (s21_value_handle_256(value, max256) == S21_BIGGER || (s21_value_handle_256(value, max256) == S21_EQ && is_round) || scale > MAX_EXPONENT)) {
    is_round = 0;
    s21_int256 prev_tmp = value;
    divide_by_ten_256(&value);
    scale--;

    s21_int256 next_tmp = value;
    multiply_by_ten_256(&next_tmp);

    s21_sub_int256_man(prev_tmp, next_tmp, &last_num);

    // Если нужно будет добавлять единицу, чтобы округлить по правилам банковского округления
    // (0.5 округляется до ближайшего четного)
    if (last_num.bits[0] > 5 || (last_num.bits[0] == 5 && s21_get_bit_256(value, 0) == 1)) {
      is_round = 1;
    }
  }

  if (is_round) {
    s21_int256 one = {{0}};
    one.bits[0] = 1;
    s21_add_int256_man(value, one, &value);
  }

  // Если после всех делений на 10 число стало равно 0, значит число было слишком маленьким,
  // а если scale < 0, значит было слишком большим
  // if (scale < 0) {
  //   error = S21_OVERFLOW;
  // }
  // if (s21_is_zero_256(value) || (scale < 0 && sign == S21_NEGATIVE)) {
  //   error = S21_UNDERFLOW;
  // }

  // Если после всех делений на 10 scale < 0, значит число было слишком большим
  if (scale < 0) {
    error = (sign == S21_POSITIVE) ? S21_OVERFLOW : S21_UNDERFLOW;
  }

  if (error == S21_OK) {
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];
    s21_set_scale(result, scale);
    s21_set_sign(result, sign);
  }

  if (s21_is_zero(*result)) {
    s21_zero_decimal(result);
  }

  return error;
}

void s21_set_value(s21_decimal *num, unsigned long long value) {
  num->bits[0] = value & 0xFFFFFFFF;
  num->bits[1] = (value >> 32) & 0xFFFFFFFF;
  num->bits[2] = (value >> 63) & 0xFFFFFFFF;
}

int s21_get_max_int(s21_decimal *value, int i) {
  int error = 0;
  if (i < 0 || i > 3) {
    error = S21_OVERFLOW;
  }
  value->bits[i] = INT32_MAX;
  s21_set_bit(value, (31 * (i + 1)) + i, 1);
  return error;
}

int s21_get_max_decimal(s21_decimal *value) {
  int error = 0;
  for (int i = 0; i < 3; i++) error = s21_get_max_int(value, i);
  return error;
}

int s21_get_min_decimal(s21_decimal *value) {
  int error = 0;
  for (int i = 0; i < 3; i++) error = s21_get_max_int(value, i);
  s21_set_sign(value, S21_NEGATIVE);
  return error;
}
