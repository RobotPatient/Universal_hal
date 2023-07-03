#include <hal_i2c.h>
#include <stddef.h>

static inline void sercomi2cm_wait_for_sync(const void *const hw, const uint32_t reg)
{
	while (((Sercom *)hw)->I2CM.SYNCBUSY.reg & reg) {
	};
}

static inline void sercomi2cs_wait_for_sync(const void *const hw, const uint32_t reg)
{
	while (((Sercom *)hw)->I2CS.SYNCBUSY.reg & reg) {
	};
}

static inline void disableSercom(const void *const hw) {
    ((Sercom*)hw)->I2CM.CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;
    const uint32_t waitflags = (SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    sercomi2cm_wait_for_sync(hw, waitflags);
}

uint16_t sercomi2cm_read_STATUS_BUSSTATE_bf(const void *const hw) {
	sercomi2cm_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SYSOP);
	return (((Sercom *)hw)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE_Msk) >> SERCOM_I2CM_STATUS_BUSSTATE_Pos;
}

static inline void hri_sercomi2cm_clear_STATUS_reg(const void *const hw, uint16_t mask) {
	 ((Sercom*)hw)->I2CM.STATUS.reg = mask;
	 sercomi2cm_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SYSOP);
}

void i2c_enable(const void *const hw){
	int timeout         = 65535;
	int timeout_attempt = 4;
	((Sercom*)hw)->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
	sercomi2cm_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
	while (sercomi2cm_read_STATUS_BUSSTATE_bf(hw) != 0x1) {
		timeout--;

		if (timeout <= 0) {
			if (--timeout_attempt)
				timeout = 65535;
			else
				return;
			hri_sercomi2cm_clear_STATUS_reg(hw, SERCOM_I2CM_STATUS_BUSSTATE(0x1));
		}
	}
}

void i2c_init(I2CInst* I2C_instance, unsigned long baudate) {
    const bool InvalidSercomInstNum = (I2C_instance->Sercom_inst_num < 0 || I2C_instance->Sercom_inst_num > 5);
    const bool InvalidSercomInst = (I2C_instance->SercomInst == NULL);
    const bool InvalidClockGen = (I2C_instance->ClockGen < 0 || I2C_instance->ClockGen > 6);
if (InvalidSercomInst || InvalidSercomInstNum || InvalidClockGen){ 
    return;
}
// Set the clock system
#ifdef __SAMD51__

#else
PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + I2C_instance->Sercom_inst_num);
GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_GEN(I2C_instance->ClockGen) | GCLK_GENCTRL_ID(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val+I2C_instance->Sercom_inst_num) | GCLK_CLKCTRL_ID_SERCOMX_SLOW;
#endif

const bool SlaveConfiguration = (I2C_instance->OpMode == I2COpModeSlave && I2C_instance->I2CAddr != 0);
if(SlaveConfiguration) {
    i2c_set_slave_mode(I2C_instance, I2C_instance->I2CAddr);
} else {
    Sercom* SercomInst = I2C_instance->SercomInst;
    const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
    if(SercomEnabled) disableSercom(SercomInst);
    SercomInst->I2CM.CTRLA.reg =  (SERCOM_I2CM_CTRLA_SWRST |SERCOM_I2CM_CTRLA_MODE(5));
    const uint32_t waitflags = (SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    sercomi2cm_wait_for_sync(SercomInst, waitflags);
    SercomInst->I2CM.CTRLA.reg = ( 0 << SERCOM_I2CM_CTRLA_LOWTOUTEN_Pos      /* SCL Low Time-Out: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_INACTOUT_Pos /* Inactive Time-Out: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_SCLSM_Pos    /* SCL Clock Stretch Mode: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_SPEED_Pos    /* Transfer Speed: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_SEXTTOEN_Pos /* Slave SCL Low Extend Time-Out: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_MEXTTOEN_Pos /* Master SCL Low Extend Time-Out: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_SDAHOLD_Pos  /* SDA Hold Time: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_PINOUT_Pos   /* Pin Usage: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
	        | 5 << SERCOM_I2CM_CTRLA_MODE_Pos);
    sercomi2cm_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_MASK);
    const uint32_t SercomBaudLow = (((48000000 - (baudate * 10) - (215 * (baudate / 100) * (48000000 / 10000) / 1000)) * 10 + 5) / (baudate * 10));
    const uint32_t SercomBaud = ((SercomBaudLow & 0x1) ? (SercomBaudLow / 2) + ((SercomBaudLow / 2 + 1) << 8) : (SercomBaudLow / 2));
    SercomInst->I2CM.BAUD.reg = SercomBaud;
	//i2c_enable(SercomInst);
}

}

void i2c_deinit(I2CInst* I2C_instance) {

}

void i2c_set_baudrate(I2CInst* I2C_instance, unsigned long baudate) {

}

void i2c_set_slave_mode(I2CInst* I2C_instance, unsigned short addr) {
Sercom* SercomInst = I2C_instance->SercomInst;
const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
if(SercomEnabled) disableSercom(SercomInst);
SercomInst->I2CS.CTRLA.reg = (SERCOM_I2CS_CTRLA_SWRST | SERCOM_I2CS_CTRLA_MODE(4));
sercomi2cs_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_SWRST);
SercomInst->I2CS.CTRLA.reg = (0 << SERCOM_I2CS_CTRLA_LOWTOUTEN_Pos      /* SCL Low Time-Out: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_SCLSM_Pos    /* SCL Clock Stretch Mode: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_SPEED_Pos    /* Transfer Speed: 0 */
	        | 0 << SERCOM_I2CS_CTRLA_SEXTTOEN_Pos /* Slave SCL Low Extend Time-Out: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_SDAHOLD_Pos  /* SDA Hold Time: 0 */
	        | 0 << SERCOM_I2CS_CTRLA_PINOUT_Pos   /* Pin Usage: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
	        | 4 << SERCOM_I2CS_CTRLA_MODE_Pos);
sercomi2cs_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_MASK);
SercomInst->I2CS.ADDR.reg = (0 << SERCOM_I2CS_ADDR_ADDRMASK_Pos       /* Address Mask: 0 */
	                      | 0 << SERCOM_I2CS_ADDR_TENBITEN_Pos /* Ten Bit Addressing Enable: disabled */
	                      | 0 << SERCOM_I2CS_ADDR_GENCEN_Pos   /* General Call Address Enable: disabled */
                          | addr << SERCOM_I2CS_ADDR_ADDR_Pos);

}

void i2c_write_blocking(I2CInst* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit) {
	//i2c_enable(I2C_instance->SercomInst);
	I2C_instance->SercomInst->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_SMEN;
	sercomi2cm_wait_for_sync((I2C_instance->SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
	bool sclsm = I2C_instance->SercomInst->I2CM.CTRLA.bit.SCLSM;
	if(size == 1 && sclsm){
		I2C_instance->SercomInst->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
		sercomi2cm_wait_for_sync(I2C_instance->SercomInst, SERCOM_I2CM_SYNCBUSY_SYSOP);
	} else {
		I2C_instance->SercomInst->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
		sercomi2cm_wait_for_sync(I2C_instance->SercomInst, SERCOM_I2CM_SYNCBUSY_SYSOP);
	}
	I2C_instance->SercomInst->I2CM.ADDR.reg = addr;
	sercomi2cm_wait_for_sync((I2C_instance->SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
}

void i2c_write_non_blocking(I2CInst* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit) {

}

char i2c_read_blocking(I2CInst* I2C_instance, const unsigned short addr, unsigned char* read_buff) {
 return -1;
}

char i2c_read_non_blocking(I2CInst* I2C_instance, const unsigned short addr, unsigned char* read_buff) {
 return -1;
}

void i2c_attach_read_interrupt(I2CInst* I2C_instance, void* cb_method) {

}

void i2c_attach_write_interrupt(I2CInst* I2C_instance, void* cb_method) {

}