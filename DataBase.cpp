#include <cstring>
#include <memory.h>
#include <memory>
#include <stdint.h>

#define CMMON_DEV_TYPE      81
#define CMMON_MODEL         0x0001
#define CMMON_SN            0x0123
#define CMMON_VERHARD       0x01
#define CMMON_VERSOFT       0x12
#define COMMON_DAY          23
#define COMMON_MONTH        02
#define COMMON_YEAR         2015
#define DB_FIX_BASE_ADDRESS 0
#define SIZE_HEADER         110

using ptr = uint8_t*;
using rcode = uint16_t;

struct stResult {
    float f;
};

struct stLinaCoef {
    float f[3];
};

struct stSetLinaCoef {
    stLinaCoef Coef[20];
};

enum ID : uint16_t {
    ID_PAR_VF = 0x0000,
    ID_PAR_VF_STAT_1 = 0x0002,
    ID_PAR_VF_STAT_0 = 0x0003,
    ID_PAR_TIME_STAMP = 0x0004,
    ID_PAR_PIPE_TEMP = 0x0006,
    ID_PAR_PIPE_TEMP_STAT = 0x0008,
    ID_PAR_ELECTRONICS_TEMP = 0x0009,
    ID_PAR_ELECTRONICS_TEMP_STAT = 0x000B,
    ID_PAR_SIGNAL_STRENGTH_VAL = 0x000C,
    ID_PAR_VELOSITY_FLOW = 0x000D,
    ID_PAR_VELOSITY_FLOW_STAT = 0x000F,
    ID_PAR_PROCESS_TEMP1 = 0x0010,
    ID_PAR_PROCESS_TEMP1_STAT = 0x0012,
    ID_PAR_PROCESS_TEMP2 = 0x0013,
    ID_PAR_PROCESS_TEMP2_STAT = 0x0015,
    ID_PAR_PROCESS_PRESS = 0x0016,
    ID_PAR_PROCESS_PRESS_STAT = 0x0018,

    ID_PAR_MODEL = 0x00D2,
    ID_PAR_VARIANT = 0x00D4,
    ID_PAR_DEV_TYPE = 0x0400,
    ID_PAR_SN = 0x0401,
    ID_PAR_VERHARD = 0x0403,
    ID_PAR_VERSOFT = 0x0404,
    ID_PAR_DAY = 0x0405,
    ID_PAR_MONTH = 0x0406,
    ID_PAR_YEAR = 0x0407,
    ID_PAR_ADDR = 0x0408,
    ID_PAR_SPEED = 0x0409,
    ID_PAR_PARITY = 0x040A,
    ID_PAR_PROTOCOL_TYPE = 0x040B,

    ID_PAR_TEXT = 0x040E,

    ID_PAR_RESET = 0x0418,
    ID_PAR_INITMODE = 0x0419,

    ID_PAR_RESTORE_CLB = 0x041D,
    ID_PAR_SAVE_CLB = 0x041E,
    ID_PAR_USRLOCK = 0x04FE,
    ID_PAR_CLBLOCK = 0x04FF,

    ID_PAR_SENSOR_TYPE = 0x0500,
    ID_PAR_FLANGE_TYPE = 0x0501,
    ID_PAR_PIPE_INSIDE_DIAM = 0x0502,
    ID_PAR_LINE_SIZE = 0x0504,
    ID_PAR_PROCESS_FLUID = 0x0505,
    ID_PAR_VF_LIMIT_HI = 0x0506,
    ID_PAR_VF_LIMIT_LO = 0x0508,
    ID_PAR_PIPE_TEMP_HI = 0x050A,
    ID_PAR_PIPE_TEMP_LO = 0x050C,
    ID_PAR_ELECTRONICS_TEMP_HI = 0x050E,
    ID_PAR_ELECTRONICS_TEMP_LO = 0x0510,
    ID_PAR_VF_CUTOFF = 0x0512,
    ID_PAR_VF_CUTOFF_HYST = 0x0514,
    ID_PAR_VF_CUTOFF_MAX = 0x0516,
    ID_PAR_VF_CUTOFF_MIN = 0x0518,
    ID_PAR_VF_INTERVAL = 0x051A,
    ID_PAR_PROCESS_TEMP_MODE = 0x051C,
    ID_PAR_PROCESS_TEMP1_HI = 0x051D,
    ID_PAR_PROCESS_TEMP1_LO = 0x051F,
    ID_PAR_PROCESS_TEMP2_HI = 0x0521,
    ID_PAR_PROCESS_TEMP2_LO = 0x0523,
    ID_PAR_PROCESS_PRESS_MODE = 0x0525,
    ID_PAR_PROCESS_PRESS_HI = 0x0526,
    ID_PAR_PROCESS_PRESS_LO = 0x0528,

    ID_PAR_RSL_ABS_UPS = 0x6A00,
    ID_PAR_RSL_ABS_DNS = 0x6A02,
    ID_PAR_RSL_VOL_FLOWRATTE = 0x6A04,
    ID_PAR_CFG_METER_CONSTANT = 0x6A06,
    ID_PAR_CFG_DELTA_TOF_OFFSET = 0x6A08,
    ID_PAR_CFG_ABS_TOF_ADDITIONAL = 0x6A0A,
    ID_PAR_ERRExe = 0x6A0C,
    ID_PAR_ERRAlg = 0x6A0D,
    ID_PAR_ERRGlob = 0x6A0E,
    ID_PAR_CFG_SS = 0x6A0F,
    ID_PAR_CFG_FR = 0x6A11,
    ID_PAR_CFG_M = 0x6A13,
    ID_PAR_CFG_L = 0x6A15,
    ID_PAR_CFG_L1 = 0x6A17,
    ID_PAR_CFG_S = 0x6A19,
    ID_PAR_CFG_AVRG = 0x6A1B,
    ID_PAR_CFG_ANGLE_Y = 0x6A1D,

    ID_PAR_RQ_UPDATE = 0x6B00,
    ID_PAR_CFG_GAP_PLS_ADC_START = 0x6B01,
    ID_PAR_CFG_NUM_PLS = 0x6B02,
    ID_PAR_CFG_GAP_UPS_DNS = 0x6B03,
    ID_PAR_CFG_GAP_UPS_UPS = 0x6B04,
    ID_PAR_CFG_GAIN_CONTROL = 0x6B05,
    ID_PAR_CFG_XT2_FREQ = 0x6B06,
    ID_PAR_CFG_ADC_SAMP_FREQ = 0x6B07,
    ID_PAR_CFG_SIG_SAMP_FREQ = 0x6B08,
    ID_PAR_CFG_ADC_OVERSAMPLING = 0x6B09,
    ID_PAR_CFG_CAPTURE_DURATION = 0x6B0A,
    ID_PAR_CFG_LEA_ENABLE = 0x6B0B,
    ID_PAR_CFG_F1 = 0x6B1C,
    ID_PAR_CFG_F2 = 0x6B1D,
    ID_PAR_CFG_PATTERN_OPT = 0x6B1E,

    ID_PAR_TEMP_ENABLE = 0x6B1F,
    ID_PAR_TEMP_PERIOD = 0x6B20,
    ID_PAR_TRIG_DLY = 0x6B21,

    ID_PAR_CFG_CONT_CAPTURE = 0x6C0C,
    ID_PAR_CFG_FREQ_SWEEP = 0x6C0E,
    ID_PAR_CFG_COMP_OPTION = 0x6C10,
    ID_PAR_CFG_ENV_CROSSING = 0x6C12,
    ID_PAR_CFG_NEG_RNG_DTOF = 0x6C14,
    ID_PAR_CFG_POS_RNG_DTOF = 0x6C16,
    ID_PAR_CFG_NEG_RNG_ABS_TOF = 0x6C18,
    ID_PAR_CFG_POS_RNG_ABS_TOF = 0x6C1A,
    ID_PAR_CFG_STR_PGA_IN_COUNT = 0x6C1C,
    ID_PAR_CFG_PARAM6 = 0x6C1E,

    ID_PAR_RSL_DTOF = 0x6C20,

    ID_PAR_CHMODE = 0x6060,

    ID_PAR_LCD_SWH = 0x5068,
    ID_PAR_Err1 = 0x506D,
    ID_PAR_Err2 = 0x506E,
    ID_PAR_APPLAY_TDC = 0x5071,

    ID_PAR_RCALC_TOTAL = 0x5076,
    ID_PAR_RCALC_PULS = 0x5078,

    ID_PAR_CAPTURE_LEN = 0x7000,
    ID_PAR_CAPTURE_UP_000 = 0x7100,
    ID_PAR_CAPTURE_UP_050 = 0x7132,
    ID_PAR_CAPTURE_UP_100 = 0x7164,
    ID_PAR_CAPTURE_UP_150 = 0x7196,

    ID_PAR_CAPTURE_DN_000 = 0x7200,
    ID_PAR_CAPTURE_DN_050 = 0x7232,
    ID_PAR_CAPTURE_DN_100 = 0x7264,
    ID_PAR_CAPTURE_DN_150 = 0x7296,

    ID_PAR_R2RESULT = 0x7300,
    ID_PAR_R2MPARAMS = 0x7300 + sizeof(stResult) / 2,

    ID_PAR_R1RESULT = 0x7500,
    ID_PAR_R1MPARAMS = 0x7500 + sizeof(stResult) / 2,

    ID_PAR_DELAYTRIG = 0x7700,
    ID_PAR_EE_CFG_CC_LINE0009 = 0x7701,
    ID_PAR_EE_CFG_CC_LINE1019 = 0x7701 + sizeof(stLinaCoef) * 10 / 2,

    ID_PAR_R2TOF = 0x7800,
    ID_BLOADER = 0xFFF0,
};

struct stMainParams {
    float D;
    uint16_t F0;
    uint16_t F1;
    uint16_t Capture;
    uint16_t cntPuls;
    uint16_t deltaUptoDn;
    uint16_t delayMeathrum;
    uint16_t Gain;
};

struct stMergeVal {
    struct
    {
        float FR;
        float SS;
        float M;
        float mark;
    } RM;
    struct {
        stMainParams MP;
        stResult Rslt;
    } Ray[2];
    uint16_t Avr;
};

struct ComandHandler {
    uint32_t abs_tof_additional;
    uint32_t delta_tof_offset;
    uint32_t param1_AbsTOF_Computation_mode;
    uint32_t param2_ulp_bias_delay;
    uint32_t param3_TM_A_start_ppg;
    uint32_t param4_TM_B_adc_on;
    uint32_t param5_TM_C_pga_bias;
    uint32_t param7_uss_xtal_settling_count;
    uint32_t param8_env_cross_threshold;
    uint32_t param9_searchRngTracking;
    uint32_t user_param10;
    uint32_t user_param6;
    uint16_t adc_samp_freq;
    uint16_t appReqUpdate;
    uint16_t capture_duration;
    uint16_t flowrate_unit;
    uint16_t gain_control;
    uint16_t gap_pls_adc_start;
    uint16_t gap_ups_dns;
    uint16_t gap_ups_ups;
    uint16_t num_pls;
    uint16_t over_sampling_rate;
    uint16_t patternOption;
    uint16_t sig_samp_freq;
    uint16_t transFreq1;
    uint16_t transFreq2;
    uint16_t ussxt_freq;
    float meter_constant;
};

ComandHandler gCommandHandler;
int16_t CaptureDn[250];
int16_t CaptureLen;
int16_t CaptureUp[250];
uint16_t fTask_Eho;
uint16_t fTask_SynxroParamR1, fTask_SynxroParamR2;
uint8_t fTask_LoadParamRay2, fTask_SaveParamRay1;
float RCalc;
float RCalcPuls;
int16_t eDelayTrig = 20;
stMergeVal MergeVal;
stSetLinaCoef LinaCoef;
static uint16_t wApplay;
static uint8_t HART_TEXT[] = "USonicFlowmeter";
static uint8_t bDBReset;
uint16_t errCodeAlg = 0;
uint16_t errCodeExe = 0;
uint16_t errCodeGlob = 0;
uint16_t fSkipMesrument;
uint16_t wBootLoader = 0;
uint32_t TimeStamp = 0;
uint8_t LCD_swh;
uint8_t MEASURE_MODE;
uint8_t fSaveParam = 0;
uint8_t fWtIniUart;
uint8_t idxP2P;
uint8_t invertP2P;
volatile uint16_t tdc_state_flag;
volatile uint16_t timer_trigger_freq;

void memcpyFloat(ptr pTo, ptr pFrpm);
rcode DB_PutPar(ID bId, ptr pPar);
rcode DB_GetPar(ID bId, ptr pPar, uint8_t bSize);

void CheckApplay(void) {
    uint16_t wTmp;
    DB_GetPar(ID_PAR_APPLAY_TDC, (ptr)&wTmp, sizeof(uint16_t));
    if (wTmp) {
        wTmp = 0;
        DB_PutPar(ID_PAR_APPLAY_TDC, (ptr)&wTmp);
    }
};

void SynxroParamR1(void) {
    fSkipMesrument = 1;
}

void memcpyFloat(ptr pTo, ptr pFrom) {
    *(pTo + 0) = *(pFrom + 2);
    *(pTo + 1) = *(pFrom + 3);
    *(pTo + 2) = *(pFrom + 0);
    *(pTo + 3) = *(pFrom + 1);
}

void ToFrom50(uint16_t* pTo, uint16_t* pFrom) {
    uint16_t i;
    for (i = 0; i < 50; i++)
        pTo[i] = pFrom[i];
}

void memcpy32(ptr pTo, ptr pFrom) {
    *(pTo + 0) = *(pFrom + 2);
    *(pTo + 1) = *(pFrom + 3);
    *(pTo + 2) = *(pFrom + 0);
    *(pTo + 3) = *(pFrom + 1);
}

void ConvertTxt(ptr pTo, ptr pFrom, uint16_t len) {
    uint16_t i = len / 2;
    char t;
    for (i = 0; i < len / 2; i++) {
        t = *(pTo + 0 + i * 2);
        *(pTo + 0 + i * 2) = *(pFrom + 1 + i * 2);
        *(pTo + 1 + i * 2) = t;
    }
}

// static void ReadData(uint16_t wAddr, ptr p, size_t Size) {
//     size_t i;
//     for(i = 0; Size--; i++) {
//         *(p + i) = EE_DataRead(wAddr++);
//     }
//     return;
// }

// static void WriteData(uint16_t wAddr, ptr p, size_t Size) {
//     size_t i;
//     for(i = 0; Size--; i++)
//         EE_DataWrite(*(p + i), wAddr++);
//     EE_DataSetCRC7();
//     return;
// }

// const t_RMU30_Params cParams;

// void DB_Init(void) {

//    if(!EE_DataCalc()) {
//        WriteData(0, (ptr)&cParams, sizeof(t_RMU30_Params));
//    }
//}

//#define READ_PAR(member)                                            \
//    do {                                                            \
//        uint8_t bSizePar;                                              \
//        bSizePar = sizeof(cParams.member);                          \
//        if(bSizePar > bSize)                                        \
//            return (-1);                                            \
//        ReadData(offsetof(t_RMU30_Params, member), pPar, bSizePar); \
//        return (bSizePar);                                          \
//    } while(0)

//#define WRITE_PAR(member)                                            \
//    do {                                                             \
//        uint8_t bSizePar;                                               \
//        bSizePar = sizeof(cParams.member);                           \
//        WriteData(offsetof(t_RMU30_Params, member), pPar, bSizePar); \
//        return (FALSE);                                              \
//    } while(0)

//#define PAR(Op, IdPar, m) \
//    case IdPar: {         \
//        Op(m);            \
//    }

// static rcode _DB_GetPar(uint8_t bId, ptr pPar, uint8_t bSize) {
//     RW_PAR(READ_PAR);
//     return (-1);
// }

// static bool _DB_PutPar(uint8_t bId, ptr pPar) {
//     RW_PAR(WRITE_PAR);
//     return (TRUE);
// }

void* memcpy(void* _Dst, void const* _Src, size_t _Size);
uint16_t _DB_GetPar(ID bId, ptr pPar, uint8_t bSize);

rcode DB_GetPar(ID bId, ptr pPar, uint8_t bSize) {
    rcode size = 0;
    uint32_t tmp32 = 0;
    uint16_t tmp16 = 0;
    float tmpFloat = 0;

    switch (bId) {
    case ID_PAR_SPEED:
        tmp16 = 6;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PARITY:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROTOCOL_TYPE:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_ADDR:
        tmp16 = 1;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_RSL_VOL_FLOWRATTE:
        memcpyFloat(pPar, (ptr)&MergeVal.RM.FR);
        size = sizeof(float);
        break;
    case ID_PAR_CFG_SS:
        memcpyFloat(pPar, (ptr)&MergeVal.RM.SS);
        size = sizeof(float);
        break;
    case ID_PAR_CFG_FR:
        memcpyFloat(pPar, (ptr)&MergeVal.RM.FR);
        size = sizeof(float);
        break;
    case ID_PAR_CFG_M:
        memcpyFloat(pPar, (ptr)&MergeVal.RM.M);
        size = sizeof(float);
        break;
    case ID_PAR_CFG_AVRG:
        tmp16 = MergeVal.Avr;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_VF:
        memcpyFloat(pPar, (ptr)&MergeVal.RM.M);
        size = sizeof(float);
        break;
    case ID_PAR_VF_STAT_1:
        if (errCodeGlob)
            tmp16 = (1 << 12);
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_VF_STAT_0:
        tmp16 = (errCodeAlg << 8) | (errCodeExe);
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_TIME_STAMP:
        tmp32 = MergeVal.RM.mark;
        memcpy32(pPar, (ptr)&tmp32);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_PIPE_TEMP:
        tmpFloat = 0;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_PIPE_TEMP_STAT:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_ELECTRONICS_TEMP:
        tmpFloat = 0;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_ELECTRONICS_TEMP_STAT:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_SIGNAL_STRENGTH_VAL:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_VELOSITY_FLOW:
        tmpFloat = MergeVal.RM.FR;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VELOSITY_FLOW_STAT:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROCESS_TEMP1:
        tmpFloat = 0;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_TEMP1_STAT:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROCESS_TEMP2:
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_TEMP2_STAT:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROCESS_PRESS:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_PRESS_STAT:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_RESTORE_CLB:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_SAVE_CLB:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_USRLOCK:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CLBLOCK:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_SENSOR_TYPE:
        tmp16 = 0x0100;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_FLANGE_TYPE:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PIPE_INSIDE_DIAM:
        tmpFloat = MergeVal.Ray[0].MP.D;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_LINE_SIZE:
        tmp16 = 25;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROCESS_FLUID:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_VF_LIMIT_HI:
        tmpFloat = 100;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VF_LIMIT_LO:
        tmpFloat = 0;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_PIPE_TEMP_HI:
        tmpFloat = 200;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_PIPE_TEMP_LO:
        tmpFloat = -100;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_ELECTRONICS_TEMP_HI:
        tmpFloat = 100;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_ELECTRONICS_TEMP_LO:
        tmpFloat = -100;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VF_CUTOFF:
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VF_CUTOFF_HYST:
        tmpFloat = 1;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VF_CUTOFF_MAX:
        tmpFloat = 98;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VF_CUTOFF_MIN:
        tmpFloat = 2;
        memcpyFloat(pPar, (ptr)&tmpFloat);
        size = sizeof(float);
        break;
    case ID_PAR_VF_INTERVAL:
        tmp32 = 500;
        memcpy32(pPar, (ptr)&tmp32);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_PROCESS_TEMP_MODE:

        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROCESS_TEMP1_HI:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_TEMP1_LO:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_TEMP2_HI:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_TEMP2_LO:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_PRESS_MODE:
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_PROCESS_PRESS_HI:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_PROCESS_PRESS_LO:
        memcpy(pPar, &tmpFloat, sizeof(float));
        size = sizeof(float);
        break;
    case ID_PAR_CFG_GAP_PLS_ADC_START:
        tmp16 = gCommandHandler.gap_pls_adc_start;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_NUM_PLS:
        tmp16 = gCommandHandler.num_pls;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_GAP_UPS_DNS:
        tmp16 = gCommandHandler.gap_ups_dns;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_GAP_UPS_UPS:
        tmp16 = gCommandHandler.gap_ups_ups;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_GAIN_CONTROL:
        tmp16 = gCommandHandler.gain_control;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_METER_CONSTANT:
        memcpyFloat(pPar, (ptr)&gCommandHandler.meter_constant);
        size = sizeof(float);
        break;
    case ID_PAR_CFG_XT2_FREQ:
        tmp16 = gCommandHandler.ussxt_freq;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_ADC_SAMP_FREQ:
        tmp16 = gCommandHandler.adc_samp_freq;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_SIG_SAMP_FREQ:
        tmp16 = gCommandHandler.sig_samp_freq;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_ADC_OVERSAMPLING:
        tmp16 = gCommandHandler.over_sampling_rate;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_DELTA_TOF_OFFSET:
        memcpy32(pPar, (ptr)&gCommandHandler.delta_tof_offset);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_ABS_TOF_ADDITIONAL:
        memcpy32(pPar, (ptr)&gCommandHandler.abs_tof_additional);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_CAPTURE_DURATION:
        tmp16 = gCommandHandler.capture_duration;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_CONT_CAPTURE:
        memcpy32(pPar, (ptr)&gCommandHandler.param1_AbsTOF_Computation_mode);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_FREQ_SWEEP:
        memcpy32(pPar, (ptr)&gCommandHandler.param2_ulp_bias_delay);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_COMP_OPTION:
        memcpy32(pPar, (ptr)&gCommandHandler.param3_TM_A_start_ppg);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_ENV_CROSSING:
        memcpy32(pPar, (ptr)&gCommandHandler.param4_TM_B_adc_on);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_NEG_RNG_DTOF:
        memcpy32(pPar, (ptr)&gCommandHandler.param7_uss_xtal_settling_count);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_POS_RNG_DTOF:
        memcpy32(pPar, (ptr)&gCommandHandler.param8_env_cross_threshold);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_NEG_RNG_ABS_TOF:
        memcpy32(pPar, (ptr)&gCommandHandler.param9_searchRngTracking);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_POS_RNG_ABS_TOF:
        memcpy32(pPar, (ptr)&gCommandHandler.user_param10);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_F1:
        tmp16 = gCommandHandler.transFreq1;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_F2:
        tmp16 = gCommandHandler.transFreq2;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_PATTERN_OPT:
        tmp16 = gCommandHandler.patternOption;
        memcpy32(pPar, (ptr)&tmp32);
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CFG_STR_PGA_IN_COUNT:
        memcpy32(pPar, (ptr)&gCommandHandler.param5_TM_C_pga_bias);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_PARAM6:
        memcpy32(pPar, (ptr)&gCommandHandler.user_param6);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_CFG_LEA_ENABLE:
        tmp16 = gCommandHandler.flowrate_unit;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_RQ_UPDATE:
        tmp16 = gCommandHandler.appReqUpdate;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_R1MPARAMS:
        memcpy(pPar, (uint8_t*)&MergeVal.Ray[0].MP, sizeof(stMainParams));
        size = sizeof(stMainParams);
        break;
    case ID_PAR_R1RESULT:
        memcpy(pPar, (uint8_t*)&MergeVal.Ray[0].Rslt, sizeof(stResult));
        size = sizeof(stResult);
        break;
    case ID_PAR_R2MPARAMS:
        fTask_LoadParamRay2 = 1;
        memcpy(pPar, (uint8_t*)&MergeVal.Ray[1].MP, sizeof(stMainParams));
        size = sizeof(stMainParams);
        break;
    case ID_PAR_R2RESULT:
        memcpy(pPar, (uint8_t*)&MergeVal.Ray[1].Rslt, sizeof(stResult));
        size = sizeof(stResult);
        break;
    case ID_PAR_DELAYTRIG:
        memcpy(pPar, &eDelayTrig, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CAPTURE_LEN:
        tmp16 = CaptureLen;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_CAPTURE_UP_000:
        memcpy(pPar, (uint8_t*)&CaptureUp[0], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_UP_050:
        memcpy(pPar, (uint8_t*)&CaptureUp[50], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_UP_100:
        memcpy(pPar, (uint8_t*)&CaptureUp[100], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_UP_150:
        memcpy(pPar, (uint8_t*)&CaptureUp[150], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_DN_000:
        memcpy(pPar, (uint8_t*)&CaptureDn[0], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_DN_050:
        memcpy(pPar, (uint8_t*)&CaptureDn[50], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_DN_100:
        memcpy(pPar, (uint8_t*)&CaptureDn[100], 100);
        size = 100;
        break;
    case ID_PAR_CAPTURE_DN_150:
        memcpy(pPar, (uint8_t*)&CaptureDn[150], 100);
        size = 100;
        break;
    case ID_PAR_RCALC_TOTAL:
        memcpyFloat(pPar, (ptr)&RCalc);
        size = sizeof(float);
        break;
    case ID_PAR_RCALC_PULS:
        memcpyFloat(pPar, (ptr)&RCalcPuls);
        size = sizeof(float);
        break;
    case ID_PAR_LCD_SWH:
        tmp16 = LCD_swh;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_MODEL:
        tmp32 = CMMON_MODEL;
        memcpy32(pPar, (ptr)&tmp32);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_VERHARD:
        tmp16 = CMMON_VERHARD;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_VARIANT:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_DAY:
        tmp16 = COMMON_DAY;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_MONTH:
        tmp16 = COMMON_MONTH;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_YEAR:
        tmp16 = COMMON_YEAR;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_TEXT:
        ConvertTxt(pPar, (ptr)HART_TEXT, 16);
        size = 16;
        break;
    case ID_PAR_INITMODE:
        tmp16 = 0;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_VERSOFT:
        tmp16 = CMMON_VERSOFT;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_DEV_TYPE:
        tmp16 = CMMON_DEV_TYPE;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_SN:
        tmp32 = CMMON_SN;
        memcpy32(pPar, (ptr)&tmp32);
        size = sizeof(uint32_t);
        break;
    case ID_PAR_RESET:
        memcpy(pPar, &bDBReset, sizeof(uint8_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_APPLAY_TDC:
        memcpy(pPar, &wApplay, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_ERRAlg:
        tmp16 = errCodeAlg;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_ERRExe:
        tmp16 = errCodeExe;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_ERRGlob:
        tmp16 = errCodeGlob;
        memcpy(pPar, &tmp16, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    case ID_PAR_EE_CFG_CC_LINE0009:
        memcpy(pPar, (uint8_t*)&LinaCoef.Coef[0], sizeof(stLinaCoef) * 10);
        size = sizeof(stLinaCoef) * 10;
        break;
    case ID_PAR_EE_CFG_CC_LINE1019:
        memcpy(pPar, (uint8_t*)&LinaCoef.Coef[10], sizeof(stLinaCoef) * 10);
        size = sizeof(stLinaCoef) * 10;
        break;
    case ID_BLOADER:
        memcpy(pPar, &wBootLoader, sizeof(uint16_t));
        size = sizeof(uint16_t);
        break;
    default:
        // size = _DB_GetPar(bId, pPar, bSize)
        ;
    }
    return (size);
}

rcode DB_PutPar(ID bId, ptr pPar) {
    bool size;
    size = 0;
    if ((ID_PAR_SPEED == bId) || (ID_PAR_ADDR == bId) || (ID_PAR_PARITY == bId)) {
        fWtIniUart = 1;
    };

    switch (bId) {
    case ID_BLOADER:
        memcpy(&wBootLoader, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_SPEED:
        break;
    case ID_PAR_PARITY:
        break;
    case ID_PAR_PROTOCOL_TYPE:
        break;
    case ID_PAR_ADDR:
        break;
    case ID_PAR_CFG_AVRG:
        memcpy((ptr)&MergeVal.Avr, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_GAP_PLS_ADC_START:
        memcpy((ptr)&gCommandHandler.gap_pls_adc_start, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_NUM_PLS:
        memcpy((ptr)&gCommandHandler.num_pls, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_GAP_UPS_DNS:
        memcpy((ptr)&gCommandHandler.gap_ups_dns, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_GAP_UPS_UPS:
        memcpy((ptr)&gCommandHandler.gap_ups_ups, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_GAIN_CONTROL:
        memcpy((ptr)&gCommandHandler.gain_control, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_METER_CONSTANT:
        memcpy((ptr)&gCommandHandler.meter_constant, pPar, sizeof(float));
        break;
    case ID_PAR_CFG_XT2_FREQ:
        memcpy((ptr)&gCommandHandler.ussxt_freq, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_ADC_SAMP_FREQ:
        memcpy((ptr)&gCommandHandler.adc_samp_freq, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_SIG_SAMP_FREQ:
        memcpy((ptr)&gCommandHandler.sig_samp_freq, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_ADC_OVERSAMPLING:
        memcpy((ptr)&gCommandHandler.over_sampling_rate, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_DELTA_TOF_OFFSET:
        memcpy((ptr)&gCommandHandler.delta_tof_offset, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_ABS_TOF_ADDITIONAL:
        memcpy((ptr)&gCommandHandler.abs_tof_additional, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_CAPTURE_DURATION:
        memcpy((ptr)&gCommandHandler.capture_duration, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_LEA_ENABLE:
        memcpy((ptr)&gCommandHandler.flowrate_unit, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_RQ_UPDATE:
        gCommandHandler.appReqUpdate = 1;
        fTask_SynxroParamR1 = 1;
        break;
    case ID_PAR_CFG_CONT_CAPTURE:
        memcpy((ptr)&gCommandHandler.param1_AbsTOF_Computation_mode, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_FREQ_SWEEP:
        memcpy((ptr)&gCommandHandler.param2_ulp_bias_delay, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_COMP_OPTION:
        memcpy((ptr)&gCommandHandler.param3_TM_A_start_ppg, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_ENV_CROSSING:
        memcpy((ptr)&gCommandHandler.param4_TM_B_adc_on, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_NEG_RNG_DTOF:
        memcpy((ptr)&gCommandHandler.param7_uss_xtal_settling_count, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_POS_RNG_DTOF:
        memcpy((ptr)&gCommandHandler.param8_env_cross_threshold, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_NEG_RNG_ABS_TOF:
        memcpy((ptr)&gCommandHandler.param9_searchRngTracking, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_POS_RNG_ABS_TOF:
        memcpy((ptr)&gCommandHandler.user_param10, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_F1:
        memcpy((ptr)&gCommandHandler.transFreq1, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_F2:
        memcpy((ptr)&gCommandHandler.transFreq2, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_PATTERN_OPT:
        memcpy((ptr)&gCommandHandler.patternOption, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CFG_STR_PGA_IN_COUNT:
        memcpy((ptr)&gCommandHandler.param5_TM_C_pga_bias, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_CFG_PARAM6:
        memcpy((ptr)&gCommandHandler.user_param6, pPar, sizeof(uint32_t));
        break;
    case ID_PAR_RCALC_TOTAL:
        memcpy((ptr)&RCalc, pPar, sizeof(float));
        break;
    case ID_PAR_RCALC_PULS:
        memcpy((ptr)&RCalcPuls, pPar, sizeof(float));
        break;
    case ID_PAR_LCD_SWH:
        memcpy((uint8_t*)&MergeVal.Ray[0].MP, pPar, sizeof(stMainParams));
        LCD_swh = *pPar;
        break;
    case ID_PAR_RESET:
        memcpy(&bDBReset, pPar, sizeof(uint8_t));
        break;
    case ID_PAR_APPLAY_TDC:
        memcpy(&wApplay, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_CAPTURE_LEN:
        memcpy(&fTask_Eho, pPar, sizeof(uint16_t));
        break;
    case ID_PAR_R1MPARAMS:
        memcpy((uint8_t*)&MergeVal.Ray[0].MP, pPar, sizeof(stMainParams));
        gCommandHandler.transFreq1 = MergeVal.Ray[0].MP.F0;
        gCommandHandler.transFreq2 = MergeVal.Ray[0].MP.F1;
        gCommandHandler.gap_pls_adc_start = MergeVal.Ray[0].MP.Capture;
        gCommandHandler.num_pls = MergeVal.Ray[0].MP.cntPuls;
        gCommandHandler.gap_ups_dns = MergeVal.Ray[0].MP.deltaUptoDn;
        gCommandHandler.gap_ups_ups = MergeVal.Ray[0].MP.delayMeathrum;
        gCommandHandler.gain_control = MergeVal.Ray[0].MP.Gain;
        fTask_SaveParamRay1 = 1;
        fTask_SynxroParamR1 = 1;
        break;
    case ID_PAR_R2MPARAMS:
        memcpy((uint8_t*)&MergeVal.Ray[1].MP, pPar, sizeof(stMainParams));
        fTask_SynxroParamR2 = 1;
        break;
    case ID_PAR_DELAYTRIG:
        memcpy((uint8_t*)&eDelayTrig, pPar, sizeof(eDelayTrig));
        break;
    case ID_PAR_EE_CFG_CC_LINE0009:
        memcpy((uint8_t*)&LinaCoef.Coef[0], pPar, (sizeof(stLinaCoef) * 10));
        break;
    case ID_PAR_EE_CFG_CC_LINE1019:
        memcpy((uint8_t*)&LinaCoef.Coef[10], pPar, sizeof(stLinaCoef) * 10);
        break;
    default:
        // size = _DB_PutPar(bId, pPar)
        ;
    }
    return (size);
}
