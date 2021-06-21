#pragma once

#include <QObject>
#include <QVariant>
#include <variant>

class QStyleOptionViewItem;
namespace HoldingRegisters {

//auto getType(auto&& arg) {
//    struct {
//        using type = std::decay_t<decltype(arg)>;
//    } s;
//    return s;
//}

struct Text {
    char data[16] {};
};

struct LinaCoef {
    static constexpr size_t size = 3;
    float f[size];
    auto begin() const { return f; }
    auto end() const { return f + size; }
};

struct SetLinaCoef {
    static constexpr size_t size = 10;
    LinaCoef coef[size];
    auto begin() const { return coef; }
    auto end() const { return coef + size; }
};

struct Data {
    using variant = std::variant<uint16_t, uint32_t, float, Text, SetLinaCoef>;
    enum Acces {
        ReadOnly,
        ReadWrite
    };
    Q_ENUM(Acces)
    enum { Offset = 1 };
    enum ID : uint16_t {
        REG_01 = 0x0000,
        REG_02 = REG_01 + Offset,
        REG_03 = REG_02 + Offset,
        REG_04 = REG_03 + Offset,
        REG_05 = REG_04 + Offset,
        REG_06 = REG_05 + Offset,
        REG_07 = REG_06 + Offset,
        REG_08 = REG_07 + Offset,
        REG_09 = REG_08 + Offset,
        REG_10 = REG_09 + Offset,
        REG_11 = REG_10 + Offset,
        REG_12 = REG_11 + Offset,
        REG_13 = REG_12 + Offset,
        REG_14 = REG_13 + Offset,
        REG_15 = REG_14 + Offset,
        REG_16 = REG_15 + Offset,
        REG_17 = REG_16 + Offset,
        REG_18 = REG_17 + Offset,
        REG_19 = REG_18 + Offset * 2,
        //        REG_20 = REG_19 + Offset,
        /*        ID_BLOADER = 0xFFF0,
        ID_PAR_ADDR = 0x0408,
        ID_PAR_APPLAY_TDC = 0x5071,
        ID_PAR_CAPTURE_DN_000 = 0x7200,
        ID_PAR_CAPTURE_DN_050 = 0x7232,
        ID_PAR_CAPTURE_DN_100 = 0x7264,
        ID_PAR_CAPTURE_DN_150 = 0x7296,
        ID_PAR_CAPTURE_LEN = 0x7000,
        ID_PAR_CAPTURE_UP_000 = 0x7100,
        ID_PAR_CAPTURE_UP_050 = 0x7132,
        ID_PAR_CAPTURE_UP_100 = 0x7164,
        ID_PAR_CAPTURE_UP_150 = 0x7196,
        ID_PAR_CFG_ABS_TOF_ADDITIONAL = 0x6A0A,
        ID_PAR_CFG_ADC_OVERSAMPLING = 0x6B09,
        ID_PAR_CFG_ADC_SAMP_FREQ = 0x6B07,
        ID_PAR_CFG_AVRG = 0x6A1B,
        ID_PAR_CFG_CAPTURE_DURATION = 0x6B0A,
        ID_PAR_CFG_COMP_OPTION = 0x6C10,
        ID_PAR_CFG_CONT_CAPTURE = 0x6C0C,
        ID_PAR_CFG_DELTA_TOF_OFFSET = 0x6A08,
        ID_PAR_CFG_ENV_CROSSING = 0x6C12,
        ID_PAR_CFG_F1 = 0x6B1C,
        ID_PAR_CFG_F2 = 0x6B1D,
        ID_PAR_CFG_FR = 0x6A11,
        ID_PAR_CFG_FREQ_SWEEP = 0x6C0E,
        ID_PAR_CFG_GAIN_CONTROL = 0x6B05,
        ID_PAR_CFG_GAP_PLS_ADC_START = 0x6B01,
        ID_PAR_CFG_GAP_UPS_DNS = 0x6B03,
        ID_PAR_CFG_GAP_UPS_UPS = 0x6B04,
        ID_PAR_CFG_LEA_ENABLE = 0x6B0B,
        ID_PAR_CFG_M = 0x6A13,
        ID_PAR_CFG_METER_CONSTANT = 0x6A06,
        ID_PAR_CFG_NEG_RNG_ABS_TOF = 0x6C18,
        ID_PAR_CFG_NEG_RNG_DTOF = 0x6C14,
        ID_PAR_CFG_NUM_PLS = 0x6B02,
        ID_PAR_CFG_PARAM6 = 0x6C1E,
        ID_PAR_CFG_PATTERN_OPT = 0x6B1E,
        ID_PAR_CFG_POS_RNG_ABS_TOF = 0x6C1A,
        ID_PAR_CFG_POS_RNG_DTOF = 0x6C16,
        ID_PAR_CFG_SIG_SAMP_FREQ = 0x6B08,
        ID_PAR_CFG_SS = 0x6A0F,
        ID_PAR_CFG_STR_PGA_IN_COUNT = 0x6C1C,
        ID_PAR_CFG_XT2_FREQ = 0x6B06,
        ID_PAR_CLBLOCK = 0x04FF,
        ID_PAR_DAY = 0x0405,
        ID_PAR_DELAYTRIG = 0x7700,
        ID_PAR_DEV_TYPE = 0x0400,
        ID_PAR_EE_CFG_CC_LINE0009 = 0x7701,
        ID_PAR_ELECTRONICS_TEMP = 0x0009,
        ID_PAR_ELECTRONICS_TEMP_HI = 0x050E,
        ID_PAR_ELECTRONICS_TEMP_LO = 0x0510,
        ID_PAR_ELECTRONICS_TEMP_STAT = 0x000B,
        ID_PAR_FLANGE_TYPE = 0x0501,
        ID_PAR_INITMODE = 0x0419,
        ID_PAR_LCD_SWH = 0x5068,
        ID_PAR_LINE_SIZE = 0x0504,
        ID_PAR_MODEL = 0x00D2,
        ID_PAR_MONTH = 0x0406,
        ID_PAR_PARITY = 0x040A,
        ID_PAR_PIPE_INSIDE_DIAM = 0x0502,
        ID_PAR_PIPE_TEMP = 0x0006,
        ID_PAR_PIPE_TEMP_HI = 0x050A,
        ID_PAR_PIPE_TEMP_LO = 0x050C,
        ID_PAR_PIPE_TEMP_STAT = 0x0008,
        ID_PAR_PROCESS_FLUID = 0x0505,
        ID_PAR_PROCESS_PRESS = 0x0016,
        ID_PAR_PROCESS_PRESS_HI = 0x0526,
        ID_PAR_PROCESS_PRESS_LO = 0x0528,
        ID_PAR_PROCESS_PRESS_MODE = 0x0525,
        ID_PAR_PROCESS_PRESS_STAT = 0x0018,
        ID_PAR_PROCESS_TEMP1 = 0x0010,
        ID_PAR_PROCESS_TEMP1_HI = 0x051D,
        ID_PAR_PROCESS_TEMP1_LO = 0x051F,
        ID_PAR_PROCESS_TEMP1_STAT = 0x0012,
        ID_PAR_PROCESS_TEMP2 = 0x0013,
        ID_PAR_PROCESS_TEMP2_HI = 0x0521,
        ID_PAR_PROCESS_TEMP2_LO = 0x0523,
        ID_PAR_PROCESS_TEMP2_STAT = 0x0015,
        ID_PAR_PROCESS_TEMP_MODE = 0x051C,
        ID_PAR_PROTOCOL_TYPE = 0x040B,
        ID_PAR_R1RESULT = 0x7500,
        ID_PAR_R2RESULT = 0x7300,
        ID_PAR_RCALC_PULS = 0x5078,
        ID_PAR_RCALC_TOTAL = 0x5076,
        ID_PAR_RESET = 0x0418,
        ID_PAR_RESTORE_CLB = 0x041D,
        ID_PAR_RQ_UPDATE = 0x6B00,
        ID_PAR_RSL_VOL_FLOWRATTE = 0x6A04,
        ID_PAR_SAVE_CLB = 0x041E,
        ID_PAR_SENSOR_TYPE = 0x0500,
        ID_PAR_SIGNAL_STRENGTH_VAL = 0x000C,
        ID_PAR_SN = 0x0401,
        ID_PAR_SPEED = 0x0409,
        ID_PAR_TEXT = 0x040E,
        ID_PAR_TIME_STAMP = 0x0004,
        ID_PAR_USRLOCK = 0x04FE,
        ID_PAR_VARIANT = 0x00D4,
        ID_PAR_VELOSITY_FLOW = 0x000D,
        ID_PAR_VELOSITY_FLOW_STAT = 0x000F,
        ID_PAR_VERHARD = 0x0403,
        ID_PAR_VERSOFT = 0x0404,
        ID_PAR_VF = 0x0000,
        ID_PAR_VF_CUTOFF = 0x0512,
        ID_PAR_VF_CUTOFF_HYST = 0x0514,
        ID_PAR_VF_CUTOFF_MAX = 0x0516,
        ID_PAR_VF_CUTOFF_MIN = 0x0518,
        ID_PAR_VF_INTERVAL = 0x051A,
        ID_PAR_VF_LIMIT_HI = 0x0506,
        ID_PAR_VF_LIMIT_LO = 0x0508,
        ID_PAR_VF_STAT_0 = 0x0003,
        ID_PAR_VF_STAT_1 = 0x0002,
        ID_PAR_YEAR = 0x0407,*/
    };
    Q_ENUM(ID)

    ID id;
    variant data;
    Acces acces;
    QString error {};

    QVariant toVariant() const;

    void fromVariant(const QVariant& value);

    void setModelData(QWidget* editor);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& style) const;

    void setEditorData(QWidget* editor) const;

    Q_GADGET
};

} // namespace Reg

Q_DECLARE_METATYPE(HoldingRegisters::Text)
Q_DECLARE_METATYPE(HoldingRegisters::Data)
Q_DECLARE_METATYPE(HoldingRegisters::SetLinaCoef)
