#include "s21_decimal.h"

int s21_add(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int error = 0,
      sign_res = 0;  // sing_res нужен для случаев когда два знака -, чтобы
                     // обращать резульат S21_BIGGER в S21_LESS и наоборот
  s21_zero_decimal(
      result);  // Зануление типа decimal и int256 необходимо потому что
                // заполнение может быть случайным при инициализации

  int sign1 = s21_get_sign(value1);
  int sign2 = s21_get_sign(value2);
  // Обработка двух отрицательных чисел, считаются как два положительных и
  // результат инвертируется (кроме случаев S21_EQUAL)
  if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
    sign_res = S21_NEGATIVE;
    s21_set_sign(&value1, S21_POSITIVE);
    s21_set_sign(&value2, S21_POSITIVE);
  }

  // Если знаки чисел разные это обозначает что следует выполнить вычитание т.к.
  // -v1 + v2 = v2 - v1
  if (sign1 != sign2) {
    int sign = sign1;
    s21_set_sign(&value1, S21_POSITIVE);
    s21_set_sign(&value2, S21_POSITIVE);

    S21_POSITIVE == sign ? s21_sub(value2, value1, result)
                         : s21_sub(value1, value2, result);
    if (sign)
      s21_set_sign(result, S21_NEGATIVE);  // отслеживание знака минус чтобы он
                                           // не потерялся при расчетах
  } else {
    s21_int256 v1 = {0}, v2 = {0}, r = {0};
    int scale = 0;

    // Уравнивание масштабов
    s21_scale_handle(value1, value2);

    // Перенос данных из s21_decimal в s21_int256
    s21_to_int256(value1, &v1);
    s21_to_int256(value2, &v2);

    // Сложение двух s21_big_decimal
    s21_add_int256(v1, v2, &r);
    // printf("\n");
    // print_decimal(v1); можешь раскометировать чтобы посмотреть данные которые
    // занесены в int256
    // printf("\n");
    // print_int256(r);
    // printf("\n");
    // Проверка на переполнение
    s21_from_int256(r, result);
    if (s21_check_overflow(r)) {
      error = 1;  // Число слишком велико
    } else {
      scale = s21_get_scale(
          value1);  // Масштабы уже уравнены поэтому scale у них одинаковый
      s21_set_scale(result, scale);
    }
  }

  // Обработка случая двух отрицательных чисел
  if (sign_res == S21_NEGATIVE) {
    s21_set_sign(result, S21_NEGATIVE);
  }

  // Обработка ошибок
  if (error == 1 && s21_get_sign(*result)) error = 2;  // Число слишком мало
  // if (error) s21_zero_decimal(result); зануление результата если ошибка

  return error;
}
// фунция бинарного сложения в рамках переменной типа int256
void s21_add_int256(s21_int256 value_1, s21_int256 value_2,
                    s21_int256 *result) {
  int res = 0, ovf = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_256(value_1, i) + s21_get_bit_256(value_2, i) + ovf;
    ovf = res / 2;
    s21_set_bit_256(result, i, res % 2);
  }

  // print_int256(*result); можешь раскоментировать чтобы посмотреть результат
  // сложения
}
int s21_check_overflow(s21_int256 value) {
  for (int i = 3; i < 8; i++) {
    if (value.bits[i] != 0) {
      return 1;  // ошибка
    }
  }
  return 0;  // нету ошибки, можно реализовать с помощью переменной еррор и
             // присваивания ей значений
}
// перенос значений из 256 в decimal
void s21_from_int256(s21_int256 value, s21_decimal *result) {
  result->bits[0] = value.bits[0];
  result->bits[1] = value.bits[1];
  result->bits[2] = value.bits[2];
  result->bits[3] = 0;
}
int s21_sub(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int error = 0,
      sign_res = 0;  // sing_res нужен для случаев когда два знака -, чтобы
                     // обращать резульат S21_BIGGER в S21_LESS и наоборот
  s21_zero_decimal(
      result);  // Зануление типа decimal и int256 необходимо потому что
                // заполнение может быть случайным при инициализации
  int sign1 = s21_get_sign(value1);
  int sign2 = s21_get_sign(value2);

  // Если оба числа отрицательные
  if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
    s21_set_sign(&value1, S21_POSITIVE);
    s21_set_sign(&value2, S21_POSITIVE);

    error = s21_add(value1, value2,
                    result);  // результат вычитания будет результатом сложения
                              // двух отрицательных чисел P.S. 21.02 11:30 -
                              // ощущение что это неправильная логика т.к. -v1 -
                              // (-v2) = v2 - v1, мб надо переделать ifы

    s21_set_sign(result, S21_NEGATIVE);
  } else if (sign1 != sign2) {
    // Если знаки чисел разные v1 - (-v2) = v1 + v2 то есть нужно отправить в
    // функцию s21_add
    int sign = sign1;
    s21_set_sign(&value1, S21_POSITIVE);
    s21_set_sign(&value2, S21_POSITIVE);
    error = sign ? s21_add(value2, value1, result)
                 : s21_add(value1, value2, result);
    if (sign) s21_set_sign(result, S21_NEGATIVE);
  } else {
    // Если знаки чисел одинаковые
    s21_int256 v1 = {0}, v2 = {0}, r = {0};
    s21_normalize_scale(&value1, &value2);
    s21_to_int256(value1, &v1);
    s21_to_int256(value2, &v2);
    if (s21_is_greater(value1, value2) == 0) {
      sign_res = S21_NEGATIVE;
      s21_int256 tmp = v1;
      v1 = v2;
      v2 = tmp;
    }
    s21_sub_int256(v1, v2, &r);
    s21_from_int256(r, result);
    s21_set_scale(result, s21_get_scale(value1));
  }

  if (sign_res == S21_NEGATIVE) {
    s21_set_sign(result, S21_NEGATIVE);
  }
  if (is_zero(*result)) {
    s21_set_sign(result, S21_POSITIVE);
  }
  // Обработка ошибок
  if (error == 1 && s21_get_sign(*result)) error = 2;  // Число слишком мало
  // if (error) s21_zero_decimal(result); зануление

  return error;
}
void s21_sub_int256(s21_int256 value_1, s21_int256 value_2,
                    s21_int256 *result) {
  int tmp = 0, res = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_256(value_1, i) - s21_get_bit_256(value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_bit_256(result, i, res % 2);
  }
  // printf("\n");
  // print_int256(*result);
  // printf("\n");
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_flag = 0;

  s21_zero_decimal(result);

  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  if ((sign1 == S21_POSITIVE && sign2 == S21_POSITIVE) ||
      (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE)) {
    sign_flag = S21_POSITIVE;
  } else if (sign1 != sign2) {
    sign_flag = S21_NEGATIVE;
  }

  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);

  s21_decimal multiplier = value_2;

  // Если i-й бит равен 1, прибавляем текущее multiplier к result
  // После каждой итерации сдвигаем multiplier на 1 бит влево,
  // то есть умножаем его на 2 (получаем value_2 * 2^(i+1))
  for (int i = 0; i < 96 && error == S21_OK; i++) {
    if (s21_get_bit(value_1, i)) {
      // возведение 2 в степень i
      s21_shift_decimal_left(&multiplier, i);
      s21_decimal temp;
      error = s21_add(*result, multiplier, &temp);
      *result = temp;
    }
  }

  // находим позиции старших битов
  int i = 95, j = 95;
  while (i >= 0 && !s21_get_bit(value_1, i)) {
    i--;
  }
  while (j >= 0 && !s21_get_bit(value_2, j)) {
    j--;
  }

  int shift = (i >= j) ? i - j : 0;

  // если скейлы 0, то ничего не произойдет, а если нет - поставится запятая в
  // нужном месте
  s21_set_scale(result, scale1 + scale2 - shift);
  s21_set_sign(result, sign_flag);

  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_flag = 0;

  if (is_zero(value_2)) {
    error = S21_DIV_BY_ZERO;
  }

  s21_zero_decimal(result);

  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  if ((sign1 == S21_POSITIVE && sign2 == S21_POSITIVE) ||
      (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE)) {
    sign_flag = S21_POSITIVE;
  } else if (sign1 != sign2) {
    sign_flag = S21_NEGATIVE;
  }

  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);
  s21_normalize_scale(&value_1, &value_2);

  s21_decimal dividend = value_1;
  s21_decimal divisor = value_2;

  // целая часть
  s21_decimal quotient;
  s21_zero_decimal(&quotient);

  // находим позиции старших битов
  int i = 95, j = 95;
  while (i >= 0 && !s21_get_bit(dividend, i)) {
    i--;
  }
  while (j >= 0 && !s21_get_bit(divisor, j)) {
    j--;
  }

  // расстояние, на которое нужно сдвинуть делитель
  // чтобы его старший бит совпал с старшим битом делимого
  // (если же единичка второго числа левее первого числа то по итогу у нас
  // полуится число вида - 0.xxxx)
  int shift = (i >= j) ? i - j : 0;

  // сборка целой части путем сдвига делителя влево
  // если первое число больше, то отнимаем от делителя сдвинутый делитель
  for (int k = shift; k >= 0 && !error; k--) {
    s21_decimal shifted = divisor;
    if (k > 0) {
      s21_shift_decimal_left(&shifted, k);
    }
    if (s21_is_greater_or_equal(dividend, shifted)) {
      error = s21_sub(dividend, shifted, &dividend);
      s21_set_bit(&quotient, k, 1);
    }
  }

  // целая часть заполнена, переход к дробной
  s21_decimal remainder = dividend;
  s21_decimal final_result;
  s21_zero_decimal(&final_result);
  final_result = quotient;

  s21_set_sign(&result, sign_flag);

  return error;
}

void s21_shift_decimal_left(s21_decimal *dst, int num) {
  int buffer[3] = {0};
  // прохождение по каждому сдвигу
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 3; i++) {
      // извлечение крайнего выпадающего бита
      buffer[i] = s21_get_bit(*dst, (i + 1) * 32);
    }
    // сдвиг каждого из блоков на 1 бит влево
    for (int i = 0; i < 3; i++) {
      dst->bits[i] <<= 1;
      // установка крайнего бита
      s21_set_bit(dst, (i + 1) * 32 - 1, buffer[i]);
    }
  }
}

void s21_shift_decimal_right(s21_decimal *dst, int num) {
  for (int k = 0; k < num; k++) {
    // Сохраняем младший бит следующего блока
    unsigned int carry0 = dst->bits[1] & 1;  // для bits[0]
    unsigned int carry1 = dst->bits[2] & 1;  // для bits[1]

    dst->bits[0] = (dst->bits[0] >> 1) | (carry0 << 31);
    dst->bits[1] = (dst->bits[1] >> 1) | (carry1 << 31);
    dst->bits[2] >>= 1;  // самый старший блок сдвигается без переноса
  }
}