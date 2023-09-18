/**
* \file            bit_manipulation.h
* \brief           Header file with useful macro's for bit manipulation
*/
/*
*  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* This file is part of the Universal Hal Framework.
*
* Author:          Victor Hogeweij <hogeweyv@gmail.com>
*/

#ifndef ATMELSAMD21_BIT_MANIPULATION_H
#define ATMELSAMD21_BIT_MANIPULATION_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BITMASK_COMPARE(NUM, MASK)       (NUM & MASK)
#define BIT_IS_SET(VAL, BIT_NUM)         ((VAL >> BIT_NUM) & 1)
#define SHIFT_ONE_LEFT_BY_N(N)           (1 << N)

#define GET_LOWER_4_BITS_OF_BYTE(x)      BITMASK_COMPARE(x, 0xF)
#define GET_UPPER_4_BITS_OF_BYTE(x)      BITMASK_COMPARE((x >> 4), 0xF)

#define PIN_IS_EVEN_NUMBER(x)            (x % 2) == 0

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //ATMELSAMD21_BIT_MANIPULATION_H
