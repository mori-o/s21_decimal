#include "../s21_decimal.h"

/*Функция s21_comapre возвращает 3 значения: S21_EQ S21_BIGGER S21_LESS которые
 * уже передаются в функции сравнения с помощью тернарных операций */
// int s21_compare(s21_decimal value1, s21_decimal value2) {
//   // s21_normalize_scale(&value1, &value2);
//   int code = s21_sign_handle(value1, value2);
//   if (code == S21_EQ) {
//     // code = s21_truncate_comparison(value1, value2);
//     code = s21_value_handle(value1, value2);
//     if (code == S21_EQ) {
//       code = s21_scale_handle(value1, value2);
//     } else {
//       s21_normalize_scale(&value1, &value2);
//       code = s21_value_handle(value1, value2);
//     }
//   }
//   if (s21_get_sign(value1) == S21_NEGATIVE &&
//       s21_get_sign(value2) == S21_NEGATIVE) {
//     code = s21_reverse_sign(code);
//   }

//   return code;
// }

int s21_compare(s21_decimal value1, s21_decimal value2) {
  s21_int256 val1_256, val2_256;
  s21_to_int256(value1, &val1_256);
  s21_to_int256(value2, &val2_256);

  return s21_compare_256(val1_256, val2_256);
}

int s21_compare_256(s21_int256 value1, s21_int256 value2) {
  int code = s21_sign_handle_256(value1, value2);

  if (code == S21_EQ) {
    code = s21_value_handle_256(value1, value2);
    if (code == S21_EQ) {
      code = s21_scale_handle_256(value1, value2);
    } else {
      s21_normalize_scale_256(&value1, &value2);
      code = s21_value_handle_256(value1, value2);
    }
  }
  if (s21_get_sign_256(value1) == S21_NEGATIVE &&
      s21_get_sign_256(value2) == S21_NEGATIVE) {
    code = s21_reverse_sign(code);
  }

  return code;
}

int s21_reverse_sign(int code) {
  if (code != S21_EQ) {
    if (code == S21_BIGGER) {
      code = S21_LESS;
    } else if (code == S21_LESS) {
      code = S21_BIGGER;
    }
  }
  return code;
}

int s21_scale_handle_256(s21_int256 value1, s21_int256 value2) {
  int error = S21_EQ;
  int scale1 = s21_get_scale_256(value1);
  int scale2 = s21_get_scale_256(value2);
  if (scale1 == scale2) {
    error = S21_EQ;
  } else if (scale1 > scale2) {
    error = S21_LESS;
  } else {
    error = S21_BIGGER;
  }
  return error;
}

// int s21_sign_handle(s21_decimal value1, s21_decimal value2) {
//   int code = S21_EQ;
//   // -0 == 0
//   if (!s21_is_zero(value1) || !s21_is_zero(value2)) {
//     int sign1 = s21_get_sign(value1);
//     int sign2 = s21_get_sign(value2);
//     if (sign1 == S21_POSITIVE && sign2 == S21_NEGATIVE) {
//       code = S21_BIGGER;
//     } else if (sign1 == S21_NEGATIVE && sign2 == S21_POSITIVE) {
//       code = S21_LESS;
//     }
//   }
//   return code;
// }

int s21_sign_handle_256(s21_int256 value1, s21_int256 value2) {
  int code = S21_EQ;
  // -0 == 0
  if (!s21_is_zero_256(value1) || !s21_is_zero_256(value2)) {
    int sign1 = s21_get_sign_256(value1);
    int sign2 = s21_get_sign_256(value2);
    if (sign1 == S21_POSITIVE && sign2 == S21_NEGATIVE) {
      code = S21_BIGGER;
    } else if (sign1 == S21_NEGATIVE && sign2 == S21_POSITIVE) {
      code = S21_LESS;
    }
  }
  return code;
}

// int s21_value_handle(s21_decimal value1, s21_decimal value2) {
//   int code = S21_EQ;

//   if (value1.bits[2] > value2.bits[2] || (value1.bits[2] == value2.bits[2] && value1.bits[1] > value2.bits[1]) ||
//     (value1.bits[2] == value2.bits[2] && value1.bits[1] == value2.bits[1] && value1.bits[0] > value2.bits[0])) {
//     code = S21_BIGGER;
//   } else if (value1.bits[2] < value2.bits[2] ||
//              (value1.bits[2] == value2.bits[2] && value1.bits[1] < value2.bits[1]) ||
//              (value1.bits[2] == value2.bits[2] && value1.bits[1] == value2.bits[1] && value1.bits[0] < value2.bits[0])) {
//     code = S21_LESS;
//   }
//   return code;
// }

int s21_value_handle(s21_decimal value1, s21_decimal value2) {
  s21_int256 val1, val2;
  s21_to_int256(value1, &val1);
  s21_to_int256(value2, &val2);

  return s21_value_handle_256(val1, val2);
}

int s21_value_handle_256(s21_int256 value1, s21_int256 value2) {
  int code = S21_EQ;

  // s21_zero_int256(&value1);
  // value1.bits[0] = 0b11111011010011001011111000000000;
  // s21_zero_int256(&value2);
  // value2.bits[1] = 1;

  for (int i = 6; i >= 0 && code == S21_EQ; i--) {
    if (value1.bits[i] > value2.bits[i]) {
      code = S21_BIGGER;
    } else if (value1.bits[i] < value2.bits[i]) {
      code = S21_LESS;
    }
  }

  return code;
}

int s21_truncate_comparison(s21_decimal value1, s21_decimal value2) {
  int error;
  s21_decimal quotient1 = value1, quotient2 = value2;
  error = s21_truncate(value1, &quotient1);
  error = s21_truncate(value2, &quotient2);
  if (!error) {
    if (quotient1.bits[0] == quotient2.bits[0] &&
        quotient1.bits[1] == quotient2.bits[1] &&
        quotient1.bits[2] == quotient2.bits[2]) {
      error = S21_EQ;
    } else if (quotient1.bits[2] > quotient2.bits[2] ||
               quotient1.bits[1] > quotient2.bits[1] ||
               quotient1.bits[0] > quotient2.bits[0]) {
      error = S21_BIGGER;
    } else {
      error = S21_LESS;
    }
  }
  return error;
}

int s21_is_less(s21_decimal value1, s21_decimal value2) {
  return (s21_compare(value1, value2) == S21_LESS) ? 1 : 0;
}

int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2) {
  return (s21_compare(value1, value2) == S21_LESS) ||
                 (s21_compare(value1, value2) == S21_EQ)
             ? 1
             : 0;
}

int s21_is_greater(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_BIGGER) ? 1 : 0;
}

int s21_is_greater_256(s21_int256 d1, s21_int256 d2) {
  return (s21_compare_256(d1, d2) == S21_BIGGER) ? 1 : 0;
}

int s21_is_greater_or_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_BIGGER) || (s21_compare(d1, d2) == S21_EQ)
             ? 1
             : 0;
}

int s21_is_greater_or_equal_256(s21_int256 d1, s21_int256 d2) {
  return (s21_compare_256(d1, d2) == S21_BIGGER) || (s21_compare_256(d1, d2) == S21_EQ)
             ? 1
             : 0;
}

int s21_is_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_EQ) ? 1 : 0;
}

int s21_is_not_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_EQ) ? 0 : 1;
}
