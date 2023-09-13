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
 */

#ifndef ATMELSAMD21_DEFAULT_IRQ_HANDLERS_H
#define ATMELSAMD21_DEFAULT_IRQ_HANDLERS_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void SERCOM0_Handler(void) __attribute__((used));
void SERCOM1_Handler(void) __attribute__((used));
void SERCOM2_Handler(void) __attribute__((used));
void SERCOM3_Handler(void) __attribute__((used));
void SERCOM4_Handler(void) __attribute__((used));
void SERCOM5_Handler(void) __attribute__((used));

extern volatile bustransaction_t SercomBusTrans[6];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //ATMELSAMD21_DEFAULT_IRQ_HANDLERS_H
