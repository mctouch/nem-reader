#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <cstdio>
#include <ctime>

namespace fs = std::filesystem;

// Logging utilities
#define LOG_DEBUG(msg) if (log_level == "DEBUG") std::cout << msg << std::endl
#define LOG_WARNING(msg) if (log_level == "WARNING" || log_level == "DEBUG") std::cout << msg << std::endl

const std::string LOG_FORMAT = "%Y-%m-%d %H:%M:%S";

// Function prototypes
std::vector<std::pair<std::string, std::vector<std::string>>> nmis_in_file(const fs::path& nemfile);
std::vector<std::string> output_as_csv(const fs::path& nemfile, const fs::path& output_dir, int set_interval);
std::string output_as_daily_csv(const fs::path& nemfile, const fs::path& output_dir);
void output_as_sqlite(const fs::path& nemfile, const fs::path& output_dir, const std::string& output_file, std::optional<int> set_interval);
void extend_sqlite(const fs::path& db_path);

// Globals
std::string log_level = "WARNING";

void list_nmis(const fs::path& nemfile, bool verbose) {
    log_level = verbose ? "DEBUG" : "WARNING";
    auto nmis = nmis_in_file(nemfile);
    std::cout << "The following NMI[suffix] exist in this file:" << std::endl;
    for (const auto& [nmi, suffixes] : nmis) {
        std::cout << nmi << "[";
        for (size_t i = 0; i < suffixes.size(); ++i) {
            std::cout << suffixes[i];
            if (i < suffixes.size() - 1) std::cout << ",";
        }
        std::cout << "]" << std::endl;
    }
}

void output_csv(const fs::path& nemfile, bool verbose, int set_interval, const fs::path& outdir) {
    log_level = verbose ? "DEBUG" : "WARNING";
    auto files = output_as_csv(nemfile, outdir, set_interval);
    for (const auto& fname : files) {
        std::cout << "Created " << fname << std::endl;
    }
}

void output_csv_daily(const fs::path& nemfile, bool verbose, const fs::path& outdir) {
    log_level = verbose ? "DEBUG" : "WARNING";
    auto fname = output_as_daily_csv(nemfile, outdir);
    std::cout << "Created " << fname << std::endl;
}

void output_sqlite(const fs::path& nemfile, const fs::path& outdir, const std::string& output_file, std::optional<int> set_interval, bool verbose) {
    log_level = verbose ? "DEBUG" : "WARNING";
    std::vector<fs::path> files;

    if (fs::is_directory(nemfile)) {
        std::cout << "Getting files in directory " << nemfile << std::endl;
        for (const auto& entry : fs::directory_iterator(nemfile)) {
            if (entry.path().extension() == ".csv" || entry.path().extension() == ".zip") {
                files.push_back(entry.path());
            }
        }
    } else {
        files.push_back(nemfile);
    }

    for (const auto& fp : files) {
        std::cout << "Processing " << fp << std::endl;
        try {
            output_as_sqlite(fp, outdir, output_file, set_interval);
        } catch (const std::exception& e) {
            std::cerr << "Not a valid nem file: " << fp << " (" << e.what() << ")" << std::endl;
        }
    }

    auto db_path = outdir / output_file;
    extend_sqlite(db_path);
    std::cout << "Finished exporting to DB." << std::endl;
}

int main(int argc, char** argv) {
    // Argument parsing and application logic placeholder.
    // Replace with a proper argument parsing library like CLI11 or Boost.Program_options.
    std::cout << "NEM Reader Application (C++)" << std::endl;
    return 0;
}