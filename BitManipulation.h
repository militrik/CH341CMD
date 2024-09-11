#ifndef BIT_MANIPULATION_H
#define BIT_MANIPULATION_H

#include <cstdint>

// Установка біта на позиції n
#define SET_BIT(x, n) ((x) |= (1U << (n)))

// Скидання біта на позиції n
#define CLEAR_BIT(x, n) ((x) &= ~(1U << (n)))

// Інверсія біта на позиції n
#define TOGGLE_BIT(x, n) ((x) ^= (1U << (n)))

// Перевірка біта на позиції n (повертає true, якщо біт встановлений)
#define CHECK_BIT(x, n) (!!((x) & (1U << (n))))

// Маска для біта на позиції n
#define BIT_MASK(n) (1U << (n))

#define SET_PATTERN(x, mask, pattern) ((x) = ((x) & ~(mask)) | ((pattern) & (mask)))


#endif // BIT_MANIPULATION_H
