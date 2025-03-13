#include "../s21_decimal.h"

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 1;
  if (result) {
    *result = value;
    s21_set_sign(result, 1 - s21_get_sign(*result));
    error = 0;
  }

  return error;
}

// Округляет Decimal до ближайшего целого числа
int s21_round(s21_decimal value, s21_decimal *result) {
  int error = S21_OK;
  int sign = s21_get_sign(value);

  s21_set_sign(&value, S21_POSITIVE);
  error = s21_truncate(value, result);

  if (error == S21_OK) {
    // если изначальное число и результат отличаются (т.е. сразу число было не
    // целое)
    if (s21_is_greater(value, *result)) {
      s21_decimal diff;
      s21_decimal one = {{1, 0, 0, 0}};  // 1
      s21_decimal half = {{5, 0, 0, 0}};
      s21_set_scale(&half, 1);  // 0.5

      // Вычисляем дробную часть
      error = s21_sub(value, *result, &diff);

      // если разница изначального числа и результата больше либо равна 0.5,
      // добавляем единицу
      if (error == 0 && s21_is_greater_or_equal(diff, half)) {
        error = s21_add(*result, one, result);
      }
    }
    s21_set_sign(result, sign);
  }

  // -0 приводим к 0
  if (s21_is_zero(*result)) {
    s21_zero_decimal(result);
  }

  return error;
}

// int s21_round(s21_decimal value, s21_decimal *result) {
//   int error = S21_OK;
//   int sign = s21_get_sign(value);
//   s21_set_sign(&value, S21_POSITIVE);
//   error = s21_truncate(value, result);

//   if (error == S21_OK && s21_is_greater(value, *result)) {
//     s21_decimal diff = {{0, 0, 0, 0}};
//     s21_decimal one = {{1, 0, 0, 0}};
//     s21_decimal half = {{5, 0, 0, 0}};
//     s21_set_scale(&half, S21_NEGATIVE);

//     // Вычисляем дробную часть
//     error = s21_sub(value, *result, &diff);

//     if (error == S21_OK) {
//       // Если исходное число отрицательное и diff == 0.5, не прибавляем 1
//       if (sign == S21_NEGATIVE && s21_is_equal(diff, half)) {
//         // Для -0.5 оставляем result = 0
//       } else if (s21_is_greater_or_equal(diff, half)) {
//         // Для всех остальных случаев >= 0.5 прибавляем 1
//         error = s21_add(*result, one, result);
//       }
//     }
//   }

//   if (result->bits[0] == 0) {
//     s21_set_sign(result, S21_POSITIVE);
//   } else {
//     s21_set_sign(result, sign);
//   }

//   return error;
// }

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = S21_OK;

  // -0 приводим к 0
  if (s21_is_zero(value)) {
    s21_zero_decimal(&value);
  }

  int sign = s21_get_sign(value);
  error = s21_truncate(value, result);

  // если изначальное число и результат отличаются (т.е. сразу число было не
  // целое) и число отрицательное, вычитаем единицу
  if (error == S21_OK && sign == S21_NEGATIVE &&
      !s21_is_equal(value, *result)) {
    s21_decimal one = {{1, 0, 0, 0}};  // 1
    error = s21_sub(*result, one, result);
  }

  return error;
}

// int s21_floor(s21_decimal value, s21_decimal *result) {
//   int error = S21_OK;
//   int sign = s21_get_sign(value);

//   s21_zero_decimal(result);
//   error = s21_truncate(value, result);

//   if (error == S21_OK) {
//     // если изначальное число и результат отличаются (т.е. сразу число было
//     не
//     // целое) и число отрицательное, вычитаем единицу
//     if (sign == S21_NEGATIVE && !s21_is_equal(value, *result)) {
//       s21_decimal one = {{1, 0, 0, 0}};  // 1
//       error = s21_sub(*result, one, result);
//     }
//     // Устанавливаем знак результата
//     if (error == S21_OK && s21_is_zero(*result)) {
//       s21_set_sign(result, S21_POSITIVE);
//     }
//   }

//   return error;
// }

// Отбрасывает дробную часть
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  s21_zero_decimal(result);
  int sign = s21_get_sign(value);
  int scale = s21_get_scale(value);

  if (scale != 0 && scale <= 28) {
    s21_decimal temp = value;
    s21_set_scale(&temp, 0);
    s21_set_sign(&temp, S21_POSITIVE);
    for (int i = 0; i < scale; i++) {
      divide_by_ten(&temp);
    }
    *result = temp;
  } else if (scale == 0) {
    *result = value;
  } else {
    error = 1;
  }
  if (!error) {
    s21_set_scale(result, 0);
    if (s21_is_zero(*result)) {
      s21_set_sign(result, S21_POSITIVE);
    } else {
      s21_set_sign(result, sign);
    }
  }

  return error;
}
