#include <hal_i2c.h>
#include <stddef.h>
#include <sercom_dep.h>


#define calculate_baudrate(clock_freq, baud_freq) (clock_freq/(2*baud_freq)) - (10*clock_freq/(2*clock_freq)) - 5
typedef struct {
    const uint8_t* write_buf;
    uint8_t* read_buf;
    uint8_t buf_size;
    uint8_t buf_cnt;
    SercomActions_t CurrAction;
    SercomInterfaceType_t CurrInterface;
}SercomData_t;

volatile SercomData_t SERCOMData[6] = {{NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                                       {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                                       {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                                       {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                                       {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                                       {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT}};

static inline void i2c_master_wait_for_sync(const void *const hw, const uint32_t reg)
{
    while (((Sercom *)hw)->I2CM.SYNCBUSY.reg & reg) {
    };
}

uint16_t sercomi2cm_read_STATUS_BUSSTATE_bf(const void *const hw) {
    i2c_master_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SYSOP);
    return (((Sercom *)hw)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE_Msk) >> SERCOM_I2CM_STATUS_BUSSTATE_Pos;
}

void wait_for_idle_busstate(Sercom *SercomInst){
    int timeout         = 65535;
    int timeout_attempt = 4;
    while (sercomi2cm_read_STATUS_BUSSTATE_bf(SercomInst) != 0x1) {
        timeout--;

        if (timeout <= 0) {
            if (--timeout_attempt)
                timeout = 65535;
            else
                return;
        }
    }
    /*
     * Wait an additional 20 cycles
     * There seems to be internal buffering inside the sercom peripheral, which seems to be stacking transactions.
     * To overcome this an extra delay has been added. Otherwise, read or write transactions will be collected and stacked
     * after each other.
     * TODO: Change this delay to flag/checking other means of overcoming this problem
    */
    timeout = 20;
    do{
        timeout--;
    } while(timeout >= 1);
}

void i2c_master_handler(const void *const hw, volatile SercomData_t* Data) {
    Sercom* SercomInst = ((Sercom*)hw);
    if(Data->buf_cnt < Data->buf_size && Data->write_buf != NULL) {
        if (SercomInst->I2CM.INTFLAG.bit.MB) {
            SercomInst->I2CM.DATA.reg = Data->write_buf[Data->buf_cnt++];

        } else {
            Data->read_buf[Data->buf_cnt++] = SercomInst->I2CM.DATA.reg;
            SercomInst->I2CM.CTRLB.reg = (Data->buf_cnt < Data->buf_size) ? SERCOM_I2CM_CTRLB_CMD(2) | SERCOM_I2CM_CTRLB_SMEN : SERCOM_I2CM_CTRLB_CMD(3) | SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_SMEN;
        }
    }
    else {
        const bool StopBit = (Data->CurrAction != SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP);
        if(StopBit){
            SercomInst->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3) | SERCOM_I2CM_CTRLB_ACKACT;
        } else {
            SercomInst->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;
        }
        Data->CurrAction = SERCOMACT_NONE;
        Data->buf_cnt = 0;
    }
}


void i2c_slave_handler(const void *const hw, volatile SercomData_t* Data) {
    Sercom* SercomInst = ((Sercom*)hw);
    const bool addressMatchInt = SercomInst->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH;
    const bool stopInt = SercomInst->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC;
    const bool dataReadyInt = SercomInst->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_DRDY;
    const bool isReadTransaction = SercomInst->I2CS.STATUS.bit.DIR;
    if(addressMatchInt) {
        i2c_slave_adressmatch_irq(hw);
    }
    if(stopInt) {
        i2c_slave_stop_irq(hw);
    }
    if(dataReadyInt && isReadTransaction) {
        i2c_slave_data_send_irq(hw);
    }
    if(dataReadyInt && !isReadTransaction) {
        i2c_slave_data_recv_irq(hw);
    }
}

void i2c_master_send_data_irq(const void *const hw) {
    ((Sercom*)hw)->I2CM.INTFLAG.reg = 0x01;
    ((Sercom*)hw)->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
    ((Sercom*)hw)->I2CM.CTRLB.reg |= (SERCOM_I2CM_CTRLB_CMD(0x3));
}
void i2c_master_recv_data_irq(const void *const hw) {
    ((Sercom*)hw)->I2CM.INTFLAG.reg = 0x01;
    ((Sercom*)hw)->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
    ((Sercom*)hw)->I2CM.CTRLB.reg |= (SERCOM_I2CM_CTRLB_CMD(0x3));
}

void i2c_slave_data_recv_irq(const void *const hw) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
}

void i2c_slave_data_send_irq(const void *const hw) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
}

void i2c_slave_stop_irq(const void *const hw) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
}
void i2c_slave_adressmatch_irq(const void *const hw) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
}

void SERCOM5_Handler(void)
{
	if(SERCOMData[5].CurrInterface == SERCOM_INT_I2CM) {
        i2c_master_handler(SERCOM5, &SERCOMData[5]);
	}
}

void SERCOM4_Handler(void)
{
	if(SERCOMData[4].CurrInterface == SERCOM_INT_I2CM) {
        i2c_master_handler(SERCOM4, &SERCOMData[4]);
	} else if(SERCOMData[4].CurrInterface == SERCOM_INT_I2CS) {
        i2c_slave_handler(SERCOM4, &SERCOMData[4]);
    }
}
void SERCOM3_Handler(void)
{
	if(SERCOMData[3].CurrInterface == SERCOM_INT_I2CM) {
        i2c_master_handler(SERCOM3, &SERCOMData[3]);
	}
}

void SERCOM2_Handler(void)
{
	if(SERCOMData[2].CurrInterface == SERCOM_INT_I2CM) {
        i2c_master_handler(SERCOM2, &SERCOMData[2]);
	}
}

void SERCOM1_Handler(void)
{
	if(SERCOMData[1].CurrInterface == SERCOM_INT_I2CM) {
        i2c_master_handler(SERCOM1, &SERCOMData[1]);
	}
}




static inline void sercomi2cs_wait_for_sync(const void *const hw, const uint32_t reg)
{
	while (((Sercom *)hw)->I2CS.SYNCBUSY.reg & reg) {
	};
}

static inline void disableSercom(const void *const hw) {
    ((Sercom*)hw)->I2CM.CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;
    const uint32_t waitflags = (SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    i2c_master_wait_for_sync(hw, waitflags);
}



static inline void hri_sercomi2cm_clear_STATUS_reg(const void *const hw, uint16_t mask) {
	 ((Sercom*)hw)->I2CM.STATUS.reg = mask;
    i2c_master_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SYSOP);
}


void i2c_init(const I2CInst* I2C_instance, const unsigned long baudrate) {
    const bool InvalidSercomInstNum = (I2C_instance->Sercom_inst_num < 0 || I2C_instance->Sercom_inst_num > 5);
    const bool InvalidSercomInst = (I2C_instance->SercomInst == NULL);
    const bool InvalidClockGen = (I2C_instance->ClockGenSlow < 0 || I2C_instance->ClockGenSlow > 6 || I2C_instance->ClockGenFast < 0 || I2C_instance->ClockGenFast > 6);
if (InvalidSercomInst || InvalidSercomInstNum || InvalidClockGen){ 
    return;
}
// Set the clock system
#ifdef __SAMD51__

#else
PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + I2C_instance->Sercom_inst_num);
GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(I2C_instance->ClockGenSlow)  | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
while (GCLK->STATUS.bit.SYNCBUSY)
		;
GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(I2C_instance->ClockGenFast) | ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val+I2C_instance->Sercom_inst_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(I2C_instance->ClockGenFast);
while (GCLK->STATUS.bit.SYNCBUSY)
		;
#endif


const bool SlaveConfiguration = (I2C_instance->OpMode == I2COpModeSlave && I2C_instance->I2CAddr != 0);
if(SlaveConfiguration) {
    i2c_set_slave_mode(I2C_instance, I2C_instance->I2CAddr);
	SERCOMData[I2C_instance->Sercom_inst_num].CurrInterface = SERCOM_INT_I2CS;
} else {
    Sercom* SercomInst = I2C_instance->SercomInst;
    const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
    if(SercomEnabled) disableSercom(SercomInst);
    SercomInst->I2CM.CTRLA.reg =  (SERCOM_I2CM_CTRLA_SWRST |SERCOM_I2CM_CTRLA_MODE(5));
    const uint32_t waitflags = (SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    i2c_master_wait_for_sync(SercomInst, waitflags);
    SercomInst->I2CM.CTRLA.reg = ( 0 << SERCOM_I2CM_CTRLA_LOWTOUTEN_Pos      /* SCL Low Time-Out: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_INACTOUT_Pos /* Inactive Time-Out: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_SCLSM_Pos    /* SCL Clock Stretch Mode: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_SPEED_Pos    /* Transfer Speed: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_SEXTTOEN_Pos /* Slave SCL Low Extend Time-Out: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_MEXTTOEN_Pos /* Master SCL Low Extend Time-Out: 0 */
	        | 0b10 << SERCOM_I2CM_CTRLA_SDAHOLD_Pos  /* SDA Hold Time: 0 */
	        | 0 << SERCOM_I2CM_CTRLA_PINOUT_Pos   /* Pin Usage: disabled */
	        | 0 << SERCOM_I2CM_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
	        | 5 << SERCOM_I2CM_CTRLA_MODE_Pos);

    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_MASK);
    SercomInst->I2CM.BAUD.reg = calculate_baudrate(I2C_instance->ClockFrequency, baudrate);
	int timeout         = 65535;
	int timeout_attempt = 4;
	SercomInst->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
    SercomInst->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_SMEN;
    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
	while (sercomi2cm_read_STATUS_BUSSTATE_bf(SercomInst) != 0x1) {
		timeout--;

		if (timeout <= 0) {
			if (--timeout_attempt)
				timeout = 65535;
			else
				return;
			hri_sercomi2cm_clear_STATUS_reg(SercomInst, SERCOM_I2CM_STATUS_BUSSTATE(0x1));
		}
	}
	SercomInst->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB;
	SERCOMData[I2C_instance->Sercom_inst_num].CurrInterface = SERCOM_INT_I2CM;
}	
	const enum IRQn irq_type = (SERCOM0_IRQn + I2C_instance->Sercom_inst_num);
	NVIC_EnableIRQ(irq_type);
	NVIC_SetPriority(irq_type, 2);
}

void i2c_deinit(const I2CInst* I2C_instance) {
	disableSercom(I2C_instance->SercomInst);
}

void i2c_set_baudrate(const I2CInst* I2C_instance, const unsigned long baudrate) {
    Sercom* SercomInst = I2C_instance->SercomInst;
    wait_for_idle_busstate(SercomInst);
    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_MASK);
    disableSercom(SercomInst);
    i2c_init(I2C_instance, baudrate);
}

void i2c_set_slave_mode(const I2CInst* I2C_instance, const unsigned short addr) {
Sercom* SercomInst = I2C_instance->SercomInst;
const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
if(SercomEnabled) disableSercom(SercomInst);
SercomInst->I2CS.CTRLA.reg = (SERCOM_I2CS_CTRLA_SWRST | SERCOM_I2CS_CTRLA_MODE(4));
sercomi2cs_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_SWRST);
SercomInst->I2CS.CTRLA.reg = (1 << SERCOM_I2CS_CTRLA_LOWTOUTEN_Pos      /* SCL Low Time-Out: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_SCLSM_Pos    /* SCL Clock Stretch Mode: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_SPEED_Pos    /* Transfer Speed: 0 */
	        | 1 << SERCOM_I2CS_CTRLA_SEXTTOEN_Pos /* Slave SCL Low Extend Time-Out: disabled */
	        | 0b10 << SERCOM_I2CS_CTRLA_SDAHOLD_Pos  /* SDA Hold Time: 0 */
	        | 0 << SERCOM_I2CS_CTRLA_PINOUT_Pos   /* Pin Usage: disabled */
	        | 0 << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
	        | 4 << SERCOM_I2CS_CTRLA_MODE_Pos);
SercomInst->I2CS.CTRLB.reg |= SERCOM_I2CS_CTRLB_SMEN;
sercomi2cs_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_MASK);
SercomInst->I2CS.ADDR.reg = (0 << SERCOM_I2CS_ADDR_ADDRMASK_Pos       /* Address Mask: 0 */
	                      | 0 << SERCOM_I2CS_ADDR_TENBITEN_Pos /* Ten Bit Addressing Enable: disabled */
	                      | 0 << SERCOM_I2CS_ADDR_GENCEN_Pos   /* General Call Address Enable: disabled */
                          | (addr) << SERCOM_I2CS_ADDR_ADDR_Pos);
SercomInst->I2CS.CTRLA.reg |= SERCOM_I2CS_CTRLA_ENABLE;
SercomInst->I2CS.INTENSET.reg = SERCOM_I2CS_INTENSET_AMATCH | SERCOM_I2CS_INTENSET_PREC | SERCOM_I2CS_INTENSET_DRDY;
}

void i2c_write_non_blocking(const I2CInst* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit) {
    Sercom* SercomInst = I2C_instance->SercomInst;
    wait_for_idle_busstate(SercomInst);
    const SercomNum Sercom_inst_num = I2C_instance->Sercom_inst_num;
    volatile SercomData_t  * TransactionData = &SERCOMData[Sercom_inst_num];
    while((SercomInst->I2CM.STATUS.bit.BUSSTATE != 0x1) && SERCOMData[Sercom_inst_num].CurrAction != SERCOMACT_NONE && SercomInst->I2CM.INTFLAG.reg == 0);
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
	TransactionData->write_buf = write_buff;
	TransactionData->buf_size = size;
	TransactionData->CurrAction = stop_bit ? SERCOMACT_I2C_DATA_TRANSMIT_STOP : SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP;
    TransactionData->buf_cnt = 0;
    SercomInst->I2CM.ADDR.reg = (addr << 1);
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
}

void i2c_write_blocking(const I2CInst* I2C_instance, const unsigned char addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit) {
    Sercom* SercomInst = I2C_instance->SercomInst;
	i2c_write_non_blocking(I2C_instance, addr, write_buff, size, stop_bit);
    wait_for_idle_busstate(SercomInst);
}



void i2c_read_blocking(const I2CInst* I2C_instance, const unsigned short addr, unsigned char* read_buff, const unsigned char amount_of_bytes) {
    i2c_read_non_blocking(I2C_instance, addr, read_buff, amount_of_bytes);
    Sercom* SercomInst = I2C_instance->SercomInst;
    wait_for_idle_busstate(SercomInst);
}

void i2c_read_non_blocking(const I2CInst* I2C_instance, const unsigned short addr, unsigned char* read_buff, const unsigned char amount_of_bytes) {
    Sercom* SercomInst = I2C_instance->SercomInst;
    wait_for_idle_busstate(SercomInst);
    const SercomNum Sercom_inst_num = I2C_instance->Sercom_inst_num;
    volatile SercomData_t  * TransactionData = &SERCOMData[Sercom_inst_num];
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    TransactionData->read_buf = read_buff;
    TransactionData->buf_size = amount_of_bytes;
    TransactionData->CurrAction = SERCOMACT_I2C_DATA_RECEIVE_STOP;
    TransactionData->buf_cnt = 0;
    SercomInst->I2CM.ADDR.reg = (addr << 1) | 1;
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
}
