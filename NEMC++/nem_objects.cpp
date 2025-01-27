#include <string>
#include <vector>
#include <map>
#include <optional>
#include <ctime>

struct HeaderRecord {
    std::string version_header;
    std::optional<std::tm> creation_date;
    std::string from_participant;
    std::string to_participant;
    std::string file_name;
    bool assumed;
};

struct NmiDetails {
    std::string nmi;
    std::string nmi_configuration;
    std::string register_id;
    std::string nmi_suffix;
    std::string mdm_datastream_identifier;
    std::string meter_serial_number;
    std::string uom;
    int interval_length;
    std::optional<std::tm> next_scheduled_read_date;
};

struct Reading {
    std::tm t_start;
    std::tm t_end;
    double read_value;
    std::string uom;
    std::string meter_serial_number;
    std::optional<std::string> quality_method;
    std::optional<std::string> event_code;
    std::optional<std::string> event_desc;
    std::optional<double> val_start;
    std::optional<double> val_end;
};

struct BasicMeterData {
    std::string nmi;
    std::string nmi_configuration;
    std::string register_id;
    std::string nmi_suffix;
    std::string mdm_data_stream_identifier;
    std::string meter_serial_number;
    std::string direction_indicator;
    std::string previous_register_read;
    std::optional<std::tm> previous_register_read_datetime;
    std::string previous_quality_method;
    int previous_reason_code;
    std::string previous_reason_description;
    std::string current_register_read;
    std::optional<std::tm> current_register_read_datetime;
    std::string current_quality_method;
    int current_reason_code;
    std::string current_reason_description;
    double quantity;
    std::string uom;
    std::optional<std::tm> next_scheduled_read_date;
    std::optional<std::tm> update_datetime;
    std::optional<std::tm> msats_load_datetime;
};

struct IntervalRecord {
    std::optional<std::tm> interval_date;
    std::vector<Reading> interval_values;
    std::string quality_method;
    std::string meter_serial_number;
    std::string reason_code;
    std::string reason_description;
    std::optional<std::tm> update_datetime;
    std::optional<std::tm> msats_load_datetime;
};

struct EventRecord {
    int start_interval;
    int end_interval;
    std::string quality_method;
    std::string reason_code;
    std::string reason_description;
};

struct B2BDetails12 {
    std::string trans_code;
    std::string ret_service_order;
    std::optional<std::tm> read_datetime;
    std::string index_read;
};

struct B2BDetails13 {
    std::string previous_trans_code;
    std::string previous_ret_service_order;
    std::string current_trans_code;
    std::string current_ret_service_order;
};

class NEMReadings {
public:
    std::map<std::string, std::map<std::string, std::vector<Reading>>> readings;
    std::map<std::string, std::map<std::string, std::vector<int>>> transactions;

    NEMReadings(
        const std::map<std::string, std::map<std::string, std::vector<Reading>>>& readings,
        const std::map<std::string, std::map<std::string, std::vector<int>>>& transactions)
        : readings(readings), transactions(transactions) {}
};

class NEMData {
public:
    HeaderRecord header;
    std::map<std::string, std::map<std::string, std::vector<Reading>>> readings;
    std::map<std::string, std::map<std::string, std::vector<int>>> transactions;

    NEMData(
        const HeaderRecord& header,
        const std::map<std::string, std::map<std::string, std::vector<Reading>>>& readings,
        const std::map<std::string, std::map<std::string, std::vector<int>>>& transactions)
        : header(header), readings(readings), transactions(transactions) {}

    std::vector<std::string> get_nmis() const {
        std::vector<std::string> nmi_keys;
        for (const auto& [key, _] : transactions) {
            nmi_keys.push_back(key);
        }
        return nmi_keys;
    }
};
