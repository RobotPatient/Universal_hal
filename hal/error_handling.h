/**
* \file            gpio_error_handling.h
* \brief           Error handling for GPIO include file
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

#ifndef GPIO_ERROR_HANDLING
#define GPIO_ERROR_HANDLING
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    UHAL_STATUS_INVALID_PARAMETERS = -3,
    UHAL_STATUS_PERIPHERAL_CLOCK_ERROR = -2,
    UHAL_STATUS_ERROR = -1,
    UHAL_STATUS_OK = 0,
    UHAL_STATUS_PERIPHERAL_IN_USE_WARNING = 1,
} uhal_status_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
