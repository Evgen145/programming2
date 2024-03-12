#include "test_runner.h"
#include <sstream>
#include <string>

using namespace std;

    string GetFileName(const std::string& path) {
    size_t lastSlashPos = path.find_last_of("/\\");
    if (lastSlashPos != std::string::npos) {
        return path.substr(lastSlashPos + 1);
    }
    return path;
}

    class Logger {
    public:
        explicit Logger(ostream& output_stream) : os(output_stream), line(0) {}

        void SetLogLine(bool value) { log_line = value; }
        void SetLogFile(bool value) { log_file = value; }

        void SetFile(const string& file_name) { file = file_name; }
        void SetLine(int line_number) { line = line_number; }

        void Log(const string& message) {
            if (log_file && log_line) {
                os << GetFileName(file) << ":" << line << " " << message << endl;
            }
            else if (log_file) {
                os << GetFileName(file) << " " << message << endl;
            }
            else if (log_line) {
                os << "Line " << line << " " << message << endl;
            }
            else {
                os << message << endl;
            }
        }

    private:
        ostream& os;
        bool log_line = false;
        bool log_file = false;
        string file;
        int line; 
    };

#define LOG(logger, message) \
    logger.SetFile(__FILE__); \
    logger.SetLine(__LINE__); \
    logger.Log(message);

void TestLog() {
#line 1 "logger.cpp"

    ostringstream logs;
    Logger l(logs);
    LOG(l, "hello");

    l.SetLogFile(true);
    LOG(l, "hello");

    l.SetLogLine(true);
    LOG(l, "hello");

    l.SetLogFile(false);
    LOG(l, "hello");

    string expected = "hello\n";
    expected += "logger.cpp hello\n";
    expected += "logger.cpp:10 hello\n";
    expected += "Line 13 hello\n";
    ASSERT_EQUAL(logs.str(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLog);
}