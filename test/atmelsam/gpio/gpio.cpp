#include "fff.h"
#include "gmock/gmock.h"
#include "hal_gpio.h"
#include "sam.h"

static Port MockPort = {};

static Eic MockEic = {};

static Gclk MockGclk = {};

Gclk* GCLK = &MockGclk;

Port* PORT = &MockPort;

Eic* EIC = &MockEic;

Sercom MockSercom[6] = {};

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(NVIC_EnableIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_DisableIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_ClearPendingIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_SetPriority, IRQn_Type, uint8_t);

TEST(HAL_GPIO, gpio_set_pin_mode_output) {
    uint32_t   expected_output = 0;
    gpio_pin_t input = 0;
    for (uint8_t port_num = 0; port_num < PORT_GROUPS; port_num++) {
        for (uint8_t pin_num = 0; pin_num < 32; pin_num++) {
            PORT->Group[port_num].PINCFG[pin_num].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN;
            expected_output = 1 << pin_num;
            input = (pin_num | ((port_num + 1) << 8));
            gpio_set_pin_mode(input, GPIO_MODE_OUTPUT);
            EXPECT_EQ(PORT->Group[port_num].DIRSET.reg, expected_output);
            EXPECT_EQ(PORT->Group[port_num].PINCFG[pin_num].reg, 0);
            PORT->Group[port_num].DIR.reg = expected_output;
            EXPECT_EQ(gpio_get_pin_mode(input), GPIO_MODE_OUTPUT);
            PORT->Group[port_num].DIRSET.reg = 0;
            PORT->Group[port_num].DIR.reg = 0;
        }
    }
}

TEST(HAL_GPIO, gpio_set_pin_mode_input) {
    uint32_t   expected_output = 0;
    gpio_pin_t input = 0;
    for (uint8_t port_num = 0; port_num < PORT_GROUPS; port_num++) {
        for (uint8_t pin_num = 0; pin_num < 32; pin_num++) {
            PORT->Group[port_num].PINCFG[pin_num].reg = PORT_PINCFG_PMUXEN;
            expected_output = 1 << pin_num;
            input = (pin_num | ((port_num + 1) << 8));
            gpio_set_pin_mode(input, GPIO_MODE_INPUT);
            EXPECT_EQ(PORT->Group[port_num].DIRCLR.reg, expected_output);
            EXPECT_EQ(PORT->Group[port_num].PINCFG[pin_num].reg, PORT_PINCFG_INEN);
            EXPECT_EQ(gpio_get_pin_mode(input), GPIO_MODE_INPUT);
            PORT->Group[port_num].DIRCLR.reg = 0;
        }
    }
}

TEST(HAL_GPIO, gpio_set_pin_mode_mux_groups) {
    uint32_t   output = 0;
    gpio_pin_t input = 0;
    for (uint8_t port_num = 0; port_num < PORT_GROUPS; port_num++) {
        for (uint8_t pin_func = GPIO_MODE_A; pin_func < GPIO_MODE_N; pin_func++) {
            for (uint8_t pin_num = 0; pin_num < 32; pin_num++) {
                input = (pin_num | ((port_num + 1) << 8));
                PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXO = 15;
                PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXE = 15;
                PORT->Group[port_num].PINCFG[pin_num].reg = 0;
                gpio_set_pin_mode(input, static_cast<gpio_mode_t>(pin_func));
                if (pin_num % 2) {
                    output = PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXO;
                    EXPECT_EQ(PORT->Group[port_num].PINCFG[pin_num].reg, PORT_PINCFG_PMUXEN);
                    EXPECT_EQ(output, pin_func);
                    EXPECT_EQ(gpio_get_pin_mode(input), pin_func);
                } else {
                    output = PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXE;
                    EXPECT_EQ(output, pin_func);
                    EXPECT_EQ(gpio_get_pin_mode(input), pin_func);
                }
            }
        }
    }
}

TEST(HAL_GPIO, gpio_set_pin_options_pull_down) {
    uint32_t   output = 0;
    gpio_pin_t input = 0;
    gpio_opt_t function_option;

    for (uint8_t port_num = 0; port_num < 1; port_num++) {
            for (uint8_t pin_num = 0; pin_num < 32; pin_num++) {
                PORT->Group[port_num].PINCFG[pin_num].reg = 0;
                PORT->Group[port_num].DIR.reg = 0;
                PORT->Group[port_num].OUTSET.reg = 0;
                PORT->Group[port_num].OUTCLR.reg = 0;
                PORT->Group[port_num].CTRL.reg = 0;
                input = (pin_num | ((port_num + 1) << 8));
                function_option = GPIO_OPT_PULL_DOWN;
                printf("\n\n TESTING WITH PARAMETERS: %x , %d \n\n", input, function_option);
                gpio_set_pin_options(input, function_option);
                EXPECT_EQ(gpio_get_pin_options(input), function_option);
                PORT->Group[port_num].PINCFG[pin_num].reg = 0;
                PORT->Group[port_num].DIR.reg = 0;
                PORT->Group[port_num].OUTSET.reg = 0;
                PORT->Group[port_num].OUTCLR.reg = 0;
                PORT->Group[port_num].CTRL.reg = 0;
            }
    }
}

TEST(HAL_GPIO, gpio_set_pin_options_pull_up) {
    uint32_t   output = 0;
    gpio_pin_t input = 0;
    gpio_opt_t function_option;

    for (uint8_t port_num = 0; port_num < 1; port_num++) {
            for (uint8_t pin_num = 0; pin_num < 32; pin_num++) {
                PORT->Group[port_num].PINCFG[pin_num].reg = 0;
                PORT->Group[port_num].DIR.reg = 0;
                PORT->Group[port_num].OUTSET.reg = 0;
                PORT->Group[port_num].OUTCLR.reg = 0;
                PORT->Group[port_num].CTRL.reg = 0;
                input = (pin_num | ((port_num + 1) << 8));
                function_option = GPIO_OPT_PULL_UP;
                gpio_set_pin_options(input, function_option);
                EXPECT_EQ(PORT->Group[port_num].PINCFG[pin_num].reg, PORT_PINCFG_PULLEN);
                EXPECT_EQ(PORT->Group[port_num].OUTSET.reg, (1 << pin_num));
                PORT->Group[port_num].OUT.reg = 1 << pin_num;
                EXPECT_EQ(gpio_get_pin_options(input), function_option);
                PORT->Group[port_num].PINCFG[pin_num].reg = 0;
                PORT->Group[port_num].DIR.reg = 0;
                PORT->Group[port_num].OUTSET.reg = 0;
                PORT->Group[port_num].OUTCLR.reg = 0;
                PORT->Group[port_num].OUT.reg = 0;
                PORT->Group[port_num].CTRL.reg = 0;
            }
    }
}

TEST(NVIC_TEST, call) {
    NVIC_EnableIRQ(EIC_IRQn);
    ASSERT_EQ(NVIC_EnableIRQ_fake.call_count, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS()) {}
    return 0;
}