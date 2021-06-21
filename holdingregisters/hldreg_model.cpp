#include "hldreg_model.h"
#include "EnumHelper.h"

namespace HoldingRegisters {

Model::Model(QObject* parent)
    : QAbstractTableModel(parent)
    , m_data {
        Data { Data::REG_01, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_02, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_03, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_04, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_05, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_06, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_07, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_08, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_09, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_10, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_11, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_12, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_13, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_14, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_15, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_16, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_17, uint16_t {}, Data::ReadWrite },
        Data { Data::REG_18, float {}, Data::ReadWrite },
        Data { Data::REG_19, float {}, Data::ReadWrite },
        //        Data{ Data::REG_20, uint16_t{}, Data::ReadWrite },
        //          Data{Data::ID_BLOADER, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_ADDR, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_APPLAY_TDC, uint16_t{}, Data::ReadWrite},
        //          //        RegData { RegData::ID_PAR_CAPTURE_DN_000, 100, RegData::Read },
        //          //        RegData { RegData::ID_PAR_CAPTURE_DN_050, 100, RegData::Read },
        //          //        RegData { RegData::ID_PAR_CAPTURE_DN_100, 100, RegData::Read },
        //          //        RegData { RegData::ID_PAR_CAPTURE_DN_150, 100, RegData::Read },
        //          Data{Data::ID_PAR_CAPTURE_LEN, uint16_t{}, Data::ReadWrite},
        //          //        RegData { RegData::ID_PAR_CAPTURE_UP_000, 100, RegData::Read },
        //          //        RegData { RegData::ID_PAR_CAPTURE_UP_050, 100, RegData::Read },
        //          //        RegData { RegData::ID_PAR_CAPTURE_UP_100, 100, RegData::Read },
        //          //        RegData { RegData::ID_PAR_CAPTURE_UP_150, 100, RegData::Read },
        //          Data{Data::ID_PAR_CFG_ABS_TOF_ADDITIONAL, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_ADC_OVERSAMPLING, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_ADC_SAMP_FREQ, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_AVRG, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_CAPTURE_DURATION, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_COMP_OPTION, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_CONT_CAPTURE, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_DELTA_TOF_OFFSET, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_ENV_CROSSING, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_F1, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_F2, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_FR, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_CFG_FREQ_SWEEP, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_GAIN_CONTROL, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_GAP_PLS_ADC_START, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_GAP_UPS_DNS, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_GAP_UPS_UPS, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_LEA_ENABLE, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_M, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_CFG_METER_CONSTANT, float{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_NEG_RNG_ABS_TOF, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_NEG_RNG_DTOF, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_NUM_PLS, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_PARAM6, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_PATTERN_OPT, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_POS_RNG_ABS_TOF, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_POS_RNG_DTOF, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_SIG_SAMP_FREQ, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_SS, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_CFG_STR_PGA_IN_COUNT, uint32_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CFG_XT2_FREQ, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_CLBLOCK, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_DAY, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_DELAYTRIG, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_DEV_TYPE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_EE_CFG_CC_LINE0009, SetLinaCoef{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_ELECTRONICS_TEMP, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_ELECTRONICS_TEMP_HI, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_ELECTRONICS_TEMP_LO, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_ELECTRONICS_TEMP_STAT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_FLANGE_TYPE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_INITMODE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_LCD_SWH, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_LINE_SIZE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_MODEL, uint32_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_MONTH, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PARITY, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PIPE_INSIDE_DIAM, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PIPE_TEMP, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PIPE_TEMP_HI, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PIPE_TEMP_LO, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PIPE_TEMP_STAT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_FLUID, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_PRESS, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_PRESS_HI, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_PRESS_LO, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_PRESS_MODE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_PRESS_STAT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP1, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP1_HI, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP1_LO, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP1_STAT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP2, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP2_HI, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP2_LO, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP2_STAT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROCESS_TEMP_MODE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_PROTOCOL_TYPE, uint16_t{}, Data::ReadOnly},
        //          //        RegData { RegData::ID_PAR_R1RESULT, stResult {}, RegData::Read },
        //          //        RegData { RegData::ID_PAR_R2RESULT, stResult {}, RegData::Read },
        //          Data{Data::ID_PAR_RCALC_PULS, float{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_RCALC_TOTAL, float{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_RESET, uint16_t{}, Data::ReadWrite},
        //          Data{Data::ID_PAR_RESTORE_CLB, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_RQ_UPDATE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_RSL_VOL_FLOWRATTE, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_SAVE_CLB, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_SENSOR_TYPE, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_SIGNAL_STRENGTH_VAL, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_SN, uint32_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_SPEED, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_TEXT, Text{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_TIME_STAMP, uint32_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_USRLOCK, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VARIANT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VELOSITY_FLOW, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VELOSITY_FLOW_STAT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VERHARD, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VERSOFT, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_CUTOFF, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_CUTOFF_HYST, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_CUTOFF_MAX, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_CUTOFF_MIN, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_INTERVAL, uint32_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_LIMIT_HI, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_LIMIT_LO, float{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_STAT_0, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_VF_STAT_1, uint16_t{}, Data::ReadOnly},
        //          Data{Data::ID_PAR_YEAR, uint16_t{}, Data::ReadOnly},
    } //
{
    auto typeName = []<size_t... Is>(std::index_sequence<Is...>)->QStringList
    {
        QStringList list;
        ((list << QString(typeid(std::get<Is>(Data::variant {})).name())), ...);
        return list << QString("All");
    };
    m_type = typeName(std::make_index_sequence<std::variant_size_v<HoldingRegisters::Data::variant>> {});
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    auto& data = m_data[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Name:
            return EnumHelper::toString(data.id);
        case Id:
            return "0x" + QString("%1").arg(data.id, 4, 16, QChar('0')).toUpper();
        case Type:
            return std::visit([](auto&& arg) { return typeid(std::decay_t<decltype(arg)>).name(); }, data.data);
        case Acces:
            return EnumHelper::toString(data.acces);
        case Value:
            return data.toVariant();
        case Error:
            return data.error;
        }
    } else if (role == Qt::UserRole) {
        switch (index.column()) {
        case Name:
            return EnumHelper::toString(data.id);
        case Id:
            return data.id;
        case Type:
            return data.data.index();
        case Acces:
            return data.acces;
        case Value:
            return QVariant::fromValue(&data);
        case Error:
            return data.error;
        }
    } else if (role == Qt::EditRole && index.column() == Value)
        return std::visit([](auto&& arg) { return QVariant::fromValue(arg); }, data.data);
    else if (role == Qt::TextAlignmentRole && index.column())
        return Qt::AlignCenter;

    return {};
}

bool Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto& data = m_data[index.row()];
    if (role == Qt::EditRole && index.column() == Value) {
        data.fromVariant(value);
        return true;
    } else if (role == Qt::EditRole && index.column() == Error) {
        data.error = value.toString();
        return true;
    }
    return {};
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return EnumHelper::toString(Columns(section));
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags Model::flags(const QModelIndex& index) const
{
    auto& data = m_data[index.row()];
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == Value && data.acces == Data::ReadWrite)
        flags |= Qt::ItemIsEditable;
    return flags;
}

const Data::variant& Model::data(uint row) const { return m_data[row].data; }

Data::variant& Model::data(uint row) { return m_data[row].data; }

Data::ID Model::id(uint row) const { return m_data[row].id; }

const QStringList& Model::type() const { return m_type; }

const QStringList& Model::acces() const { return m_acces; }

} // namespace Reg
