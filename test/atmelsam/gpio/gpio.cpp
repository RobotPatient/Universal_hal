#include "fff.h"
#include "gmock/gmock.h"
#include "sam.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(NVIC_EnableIRQ, IRQn_Type);

TEST(NVIC_TEST, call) {
    NVIC_EnableIRQ(EIC_IRQn);
    ASSERT_EQ(NVIC_EnableIRQ_fake.call_count, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS()) {}
    return 0;
}