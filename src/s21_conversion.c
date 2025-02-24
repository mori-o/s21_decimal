#include "s21_decimal.h"
// не мое но лежит тут просто
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_zero_decimal(dst);
  int error = 0;
  if (src < 0) {
    s21_set_sign(dst, S21_NEGATIVE);
    src = (-1) * src;
  }
  if (src > INT_MAX)
    error = 1;
  else
    dst->bits[0] = src;
  return error;
}

// int s21_from_float_to_decimal(float src, s21_decimal *dst) {
//   s21_zero_decimal(dst);
//   int return_value = 0;
//   if (isinf(src) || isnan(src)) {
//     return_value = 1;
//   } else {
//     if (src != 0) {
//       int sign = *(int *)&src >> 31;
//       int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
//       double temp = (double)fabs(src);
//       int off = 0;
//       for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++)
//       {
//       }
//       temp = round(temp);
//       if (off <= 28 && (exp > -94 && exp < 96)) {
//         floatbits mant = {0};
//         temp = (float)temp;
//         for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
//         }
//         mant.fl = temp;
//         exp = ((*(int *)&mant.fl & ~0x80000000) >> 23) - 127;
//         dst->bits[exp / 32] |= 1 << exp % 32;
//         for (int i = exp - 1, j = 22; j >= 0; i--, j--)
//           if ((mant.ui & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
//         dst->bits[3] = (sign << 31) | (off << 16);
//       }
//     }
//   }
//   return return_value;
// }

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (dst == NULL) return 1;  // Ошибка: dst не может быть NULL

  // Получение знака
  int sign = s21_get_sign(src);

  // Получение мантиссы
  long long int_part =
      (unsigned long long)src.bits[0] | ((unsigned long long)src.bits[1] << 32);

  // Если bits[2] не равен 0, то число слишком велико
  if (src.bits[2] != 0) {
    return 1;  // Ошибка: число слишком велико
  }

  // Получение степени масштабирования
  int exponent = s21_get_scale(src);

  // Деление на 10^exponent
  for (int i = 0; i < exponent; i++) {
    int_part /= 10;
  }

  // Проверка на переполнение
  if (int_part > INT_MAX || int_part < INT_MIN) return 1;

  // Установка результата
  *dst = (int)int_part;
  if (sign) *dst = -*dst;

  return 0;  // Успешное выполнение
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) return 1;  // Ошибка: dst не может быть NULL

  // Получение знака
  int sign = s21_get_sign(src);

  // Получение мантиссы
  long long int_part =
      (unsigned long long)src.bits[0] | ((unsigned long long)src.bits[1] << 32);

  // Если bits[2] не равен 0, то число слишком велико
  if (src.bits[2] != 0) {
    return 1;  // Ошибка: число слишком велико
  }

  // Получение степени масштабирования
  int exponent = s21_get_scale(src);

  // Умножение на 10^exponent
  double result = (double)int_part;
  for (int i = 0; i < exponent; i++) {
    result /= 10;
  }

  // Установка знака
  if (sign) result = -result;

  // Проверка на переполнение
  if (result > FLT_MAX || result < -FLT_MAX) return 1;

  // Установка результата
  *dst = (float)result;

  return 0;  // Успешное выполнение
}