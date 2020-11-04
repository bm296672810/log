#ifndef __LOG_MANGER_H__
#define __LOG_MANGER_H__

#include <vector>
#include <deque>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <fstream>
using namespace std;
namespace bm{
enum log_leval
{
    INFO,
    WARNING,
    ERROR,
};

vector<string> split(const string& str, const string& sp);
void mkdirs(const string& path, mode_t mode);

class log_manger;

class log_manger
{
public:
    log_manger(const string& log_path = "");
    ~log_manger();

    void write(const char* curr_file_name, const char* curr_func_name, int line_num, log_leval leval, const string& content);

    log_manger& operator<<(string content);
    template<class T>
    log_manger& operator<<(T content)
    {
        lock_guard<mutex> lock(m_lock);
        m_fs_info << content;
        return *this;
    }

    void _write(const char* curr_file_name, const char* curr_func_name, int line_num);
    void _write(log_leval leval);
    void _write(const string& content);
    void _endl();
    void _flush();
    void _open_file();
private:

    string get_current_date();
    string get_current_time();

    string get_log_leval_string(log_leval leval);

private:
    string m_log_path;
    fstream m_fs_info;
//    fstream m_fs_error;
    mutex m_lock;
};



static log_manger s_log_manger("log/today");
}

using namespace bm;
inline void bm_write_log(const char* curr_file_name, const char* curr_func_name, int line_num, log_leval leval)
{
    s_log_manger._open_file();
    
    s_log_manger._write(curr_file_name, curr_func_name, line_num);
    s_log_manger._write(leval);
}
#define LOG(info) bm_write_log(__FILE__, __FUNCTION__, __LINE__, info);\
    s_log_manger

#endif
