#include <hal_i2c.h>
#include <stddef.h>
#include <sercom_dep.h>

/*
 * @brief This formula is used to calculate the baud rate steps.
 * The formula is a rewritten form of the formula found on page 483 (section 28.10.3) of the SAMD21 Family datasheet.
 * The original formula is: fscl = 1/(10 + 2*BAUD_STEPS + fgclk + Trise)
 * If no external pullup resistors are added the Trise will be the equivalent of fgclk/10
 * fgclk will be the fast gclk frequency (probably same as the CPU frequency)
 * fscl will be the output clock frequency (100/400/1000 KHz)
 * Baud_steps are the amount of steps needed to generate the right output frequency
 */
#define calculate_baudrate(clock_freq, baud_freq) (clock_freq/(2*baud_freq)) - (10*clock_freq/(2*clock_freq)) - 5

/*
 * @brief Macros used in ISR for acknowledging and finishing the transaction
 * SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE will send an acknowledge bit to the slave device and request the next byte to read
 * SERCOM_I2C_MASTER_NACK_AND_STOP will send a NACK to the slave device and stops the transaction by sending a stop bit.
 */
#define SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE SERCOM_I2CM_CTRLB_CMD(2) | SERCOM_I2CM_CTRLB_SMEN
#define SERCOM_I2C_MASTER_NACK_AND_STOP  SERCOM_I2CM_CTRLB_CMD(3) | SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_SMEN

volatile bustransaction_t SercomBusTrans[6] = {{SERCOMACT_NONE, 0, NULL, NULL, 0, 0,},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0,},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0,},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0,},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0,},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0,}};



/*
 * @brief Helper function which waits for the sercom peripheral to get in sync and finish requested operations.
 *        By continually reading its I2CM syncbusy register.
 * @param hw Pointer to the sercom peripheral to be manipulated or read
 * @param bits_to_read The bits to read within the syncbusy register
 * @note Possible bits that can be read are: SERCOM_I2CM_SYNCBUSY_SWRST
 *                                           SERCOM_I2CM_SYNCBUSY_ENABLE
 *                                           SERCOM_I2CM_SYNCBUSY_SYSOP
 * @note This function can only be used for use with I2C host/master configuration
 */
static inline void i2c_master_wait_for_sync(const void *const hw, const uint32_t bits_to_read)
{
    while (((Sercom *)hw)->I2CM.SYNCBUSY.reg & bits_to_read) {
    };
}

/*
 * @brief This function gets the current bus-state of the I2C bus.
 *        By reading the SERCOM I2CM Status register.
 * @param  hw Pointer to the sercom peripheral to be manipulated or read
 * @return busstate of I2C bus: 0x00: UNKNOWN
 *                              0x01: IDLE
 *                              0x02: OWNER
 *                              0x03: BUSY
 *                              Other: N/A
 * @note This function can only be used for use with I2C host/master configuration
 */
uint8_t get_i2c_master_busstate(const void *const hw) {
    i2c_master_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SYSOP);
    return (((Sercom *)hw)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE_Msk) >> SERCOM_I2CM_STATUS_BUSSTATE_Pos;
}


void wait_for_idle_busstate(Sercom *SercomInst){
    int timeout         = 65535;
    int timeout_attempt = 4;
    while (get_i2c_master_busstate(SercomInst) != 0x1) {
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

void i2c_master_data_send_irq(const void *const hw, volatile bustransaction_t *Transaction ) {
    Sercom* sercom_instance = ((Sercom*)hw);
    const bool write_buffer_exists = (Transaction->write_buffer != NULL);
    const bool has_bytes_left_to_write = (Transaction->buf_cnt < Transaction->buf_size);
    if(write_buffer_exists && has_bytes_left_to_write) {
        sercom_instance->I2CM.DATA.reg = Transaction->write_buffer[Transaction->buf_cnt++];
    } else {
        const bool send_stop_bit = (Transaction->transaction_type != SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP);
        if(send_stop_bit){
            sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        } else {
            sercom_instance->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;
        }
        Transaction->transaction_type = SERCOMACT_NONE;
        Transaction->buf_cnt = 0;
    }
}

void i2c_master_data_recv_irq(const void *const hw, volatile bustransaction_t *Transaction){
    Sercom* sercom_instance = ((Sercom*)hw);
    if(Transaction->read_buffer != NULL && Transaction->buf_cnt < Transaction->buf_size) {
        Transaction->read_buffer[Transaction->buf_cnt++] = sercom_instance->I2CM.DATA.reg;
        const bool last_byte_read = !(Transaction->buf_cnt < Transaction->buf_size);
        sercom_instance->I2CM.CTRLB.reg = last_byte_read ? SERCOM_I2C_MASTER_NACK_AND_STOP : SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE;
    } else {
        sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        Transaction->transaction_type = SERCOMACT_NONE;
        Transaction->buf_cnt = 0;
    }
}

void i2c_slave_handler(const void *const hw, volatile bustransaction_t* Data) {
    Sercom* sercom_instance = ((Sercom*)hw);
    const bool addressMatchInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH;
    const bool stopInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC;
    const bool dataReadyInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_DRDY;
    const bool isReadTransaction = sercom_instance->I2CS.STATUS.bit.DIR;
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
    if(SERCOM5->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM5, &SercomBusTrans[5]);
    } else if (SERCOM5->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM5, &SercomBusTrans[5]);
    } else if(SERCOM5->I2CS.INTFLAG.bit.AMATCH || SERCOM5->I2CS.INTFLAG.bit.DRDY || SERCOM5->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM5, &SercomBusTrans[5]);
    }
}

void SERCOM4_Handler(void)
{
	if(SERCOM4->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM4, &SercomBusTrans[4]);
	} else if (SERCOM4->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM4, &SercomBusTrans[4]);
    } else if(SERCOM4->I2CS.INTFLAG.bit.AMATCH || SERCOM4->I2CS.INTFLAG.bit.DRDY || SERCOM4->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM4, &SercomBusTrans[4]);
    }
}
void SERCOM3_Handler(void)
{
    if(SERCOM3->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM3, &SercomBusTrans[3]);
    } else if (SERCOM3->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM3, &SercomBusTrans[3]);
    } else if(SERCOM3->I2CS.INTFLAG.bit.AMATCH || SERCOM3->I2CS.INTFLAG.bit.DRDY || SERCOM3->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM3, &SercomBusTrans[3]);
    }
}

void SERCOM2_Handler(void)
{
    if(SERCOM2->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM2, &SercomBusTrans[2]);
    } else if (SERCOM2->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM2, &SercomBusTrans[2]);
    } else if(SERCOM2->I2CS.INTFLAG.bit.AMATCH || SERCOM2->I2CS.INTFLAG.bit.DRDY || SERCOM2->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM2, &SercomBusTrans[2]);
    }
}

void SERCOM1_Handler(void)
{
    if(SERCOM1->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM1, &SercomBusTrans[1]);
    } else if (SERCOM1->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM1, &SercomBusTrans[1]);
    } else if(SERCOM1->I2CS.INTFLAG.bit.AMATCH || SERCOM1->I2CS.INTFLAG.bit.DRDY || SERCOM1->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM1, &SercomBusTrans[1]);
    }
}

void SERCOM0_Handler(void)
{
    if(SERCOM0->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM0, &SercomBusTrans[0]);
    } else if (SERCOM0->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM0, &SercomBusTrans[0]);
    } else if(SERCOM0->I2CS.INTFLAG.bit.AMATCH || SERCOM0->I2CS.INTFLAG.bit.DRDY || SERCOM0->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM0, &SercomBusTrans[0]);
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


void i2c_init(const i2c_periph_inst_t* i2c_instance, const unsigned long baudrate) {
    const bool InvalidSercomInstNum = (i2c_instance->sercom_inst_num < 0 || i2c_instance->sercom_inst_num > 5);
    const bool InvalidSercomInst = (i2c_instance->sercom_inst == NULL);
    const bool InvalidClockGen = (i2c_instance->clk_gen_slow < 0 || i2c_instance->clk_gen_slow > 6 || i2c_instance->clk_gen_fast < 0 || i2c_instance->clk_gen_fast > 6);
if (InvalidSercomInst || InvalidSercomInstNum || InvalidClockGen){ 
    return;
}
// Set the clock system
#ifdef __SAMD51__

#else
PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + i2c_instance->sercom_inst_num);
GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(i2c_instance->clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
while (GCLK->STATUS.bit.SYNCBUSY)
		;
GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(i2c_instance->clk_gen_fast) | ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c_instance->sercom_inst_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(i2c_instance->clk_gen_fast);
while (GCLK->STATUS.bit.SYNCBUSY)
		;
#endif


const bool SlaveConfiguration = (i2c_instance->operating_mode == I2COpModeSlave && i2c_instance->i2c_slave_addr != 0);
if(SlaveConfiguration) {
    i2c_set_slave_mode(i2c_instance, i2c_instance->i2c_slave_addr);
	SercomBusTrans[i2c_instance->sercom_inst_num].transaction_type = SERCOM_INT_I2CS;
} else {
    Sercom* SercomInst = i2c_instance->sercom_inst;
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
    SercomInst->I2CM.BAUD.reg = calculate_baudrate(i2c_instance->fast_clk_gen_frequency, baudrate);
	int timeout         = 65535;
	int timeout_attempt = 4;
	SercomInst->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
    SercomInst->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_SMEN;
    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
	while (get_i2c_master_busstate(SercomInst) != 0x1) {
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
	SercomBusTrans[i2c_instance->sercom_inst_num].transaction_type = SERCOM_INT_I2CM;
}	
	const enum IRQn irq_type = (SERCOM0_IRQn + i2c_instance->sercom_inst_num);
	NVIC_EnableIRQ(irq_type);
	NVIC_SetPriority(irq_type, 2);
}

void i2c_deinit(const i2c_periph_inst_t* I2C_instance) {
	disableSercom(I2C_instance->sercom_inst);
}

void i2c_set_baudrate(const i2c_periph_inst_t* I2C_instance, const unsigned long baudrate) {
    Sercom* SercomInst = I2C_instance->sercom_inst;
    wait_for_idle_busstate(SercomInst);
    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_MASK);
    disableSercom(SercomInst);
    i2c_init(I2C_instance, baudrate);
}

void i2c_set_slave_mode(const i2c_periph_inst_t* I2C_instance, const unsigned short addr) {
Sercom* SercomInst = I2C_instance->sercom_inst;
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

void i2c_write_non_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit) {
    Sercom* SercomInst = I2C_instance->sercom_inst;
    wait_for_idle_busstate(SercomInst);
    const sercom_num_t Sercom_inst_num = I2C_instance->sercom_inst_num;
    volatile bustransaction_t* TransactionData = &SercomBusTrans[Sercom_inst_num];
    while((SercomInst->I2CM.STATUS.bit.BUSSTATE != 0x1) && SercomBusTrans[Sercom_inst_num].transaction_type != SERCOMACT_NONE && SercomInst->I2CM.INTFLAG.reg == 0);
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
	TransactionData->write_buffer = write_buff;
	TransactionData->buf_size = size;
	TransactionData->transaction_type = stop_bit ? SERCOMACT_I2C_DATA_TRANSMIT_STOP : SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP;
    TransactionData->buf_cnt = 0;
    SercomInst->I2CM.ADDR.reg = (addr << 1);
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
}

void i2c_write_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned char addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit) {
    Sercom* SercomInst = I2C_instance->sercom_inst;
	i2c_write_non_blocking(I2C_instance, addr, write_buff, size, stop_bit);
    wait_for_idle_busstate(SercomInst);
}



void i2c_read_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned short addr, unsigned char* read_buff, const unsigned char amount_of_bytes) {
    i2c_read_non_blocking(I2C_instance, addr, read_buff, amount_of_bytes);
    Sercom* SercomInst = I2C_instance->sercom_inst;
    wait_for_idle_busstate(SercomInst);
}

void i2c_read_non_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned short addr, unsigned char* read_buff, const unsigned char amount_of_bytes) {
    Sercom* SercomInst = I2C_instance->sercom_inst;
    wait_for_idle_busstate(SercomInst);
    const sercom_num_t Sercom_inst_num = I2C_instance->sercom_inst_num;
    volatile bustransaction_t* TransactionData = &SercomBusTrans[Sercom_inst_num];
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    TransactionData->read_buffer = read_buff;
    TransactionData->buf_size = amount_of_bytes;
    TransactionData->transaction_type = SERCOMACT_I2C_DATA_RECEIVE_STOP;
    TransactionData->buf_cnt = 0;
    SercomInst->I2CM.ADDR.reg = (addr << 1) | 1;
    i2c_master_wait_for_sync((SercomInst), SERCOM_I2CM_SYNCBUSY_SYSOP);
}
