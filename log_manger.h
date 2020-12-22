#ifndef __LOG_MANGER_H__
#define __LOG_MANGER_H__

#include <vector>
#include <deque>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <fstream>

namespace bm{
enum log_leval
{
    INFO,
    WARNING,
    ERROR,
};

std::vector<std::string> split(const std::string& str, const std::string& sp);
void mkdirs(const std::string& path, mode_t mode);

typedef std::lock_guard<std::mutex> lock_guard_mutex;

class log_manger;

class log_manger
{
public:
    log_manger(const std::string& log_path = "");
    ~log_manger();

    void write(const char* curr_file_name, const char* curr_func_name, int line_num, log_leval leval, const std::string& content);

    std::fstream& stream()
    {
        return m_fs_info;
    }

    void _write(const char* curr_file_name, const char* curr_func_name, int line_num);
    void _write(log_leval leval);
    void _write(const std::string& content);
    void _endl();
    void _flush();
    void _open_file();
private:

    std::string get_current_date();
    std::string get_current_time();

    std::string get_log_leval_string(log_leval leval);

private:
    std::string m_log_path;
    std::fstream m_fs_info;

    std::mutex m_lock;
};


extern log_manger s_log_manger;
}

using namespace bm;
inline void bm_write_log(const char* curr_file_name, const char* curr_func_name, int line_num, log_leval leval)
{
    s_log_manger._open_file();
    
    s_log_manger._write(curr_file_name, curr_func_name, line_num);
    s_log_manger._write(leval);
}
#define BMLOG(info) bm_write_log(__FILE__, __FUNCTION__, __LINE__, info);\
    s_log_manger.stream()

#endif
