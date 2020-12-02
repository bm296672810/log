#include <assert.h>
#include <iostream>
#include <sstream>

#ifdef __linux
#include <sys/stat.h>
#include <sys/time.h>
#elif __WIN32

#endif

#include "log_manger.h"

namespace bm {

std::vector<std::string> split(const std::string& str, const std::string& sp)
{
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type pos = 0;
    do
    {
        pos = str.find(sp, start);
        if(pos != std::string::npos)
        {
            result.push_back(str.substr(start, pos - start));
            start = pos + sp.size();
        }
        else
            result.push_back(str.substr(start));
    }
    while(pos != std::string::npos);

    return result;
}
void mkdirs(const std::string& path, mode_t mode)
{
    std::vector<std::string> paths = split(path, "/");

    std::string dir;
    for(auto i : paths)
    {
        dir += i + "/";
        mkdir(dir.c_str(), mode);
    }
}

log_manger::log_manger(const std::string& log_path):
    m_log_path(log_path)
{
    mkdirs(log_path, 0766);

    _open_file();
}
log_manger::~log_manger()
{
    m_fs_info.flush();
    m_fs_info.close();
}

void log_manger::write(const char* curr_file_name, const char* curr_func_name, int line_num, log_leval leval, const std::string& content)
{
    _write(curr_file_name, curr_func_name, line_num);
    _write(leval);
    _write(content);
    _endl();
    _flush();
}
void log_manger::_endl()
{
    lock_guard_mutex lock(m_lock);
    m_fs_info << std::endl;
}
void log_manger::_flush()
{
    lock_guard_mutex lock(m_lock);
    m_fs_info.flush();
}
void log_manger::_write(log_leval leval)
{
    lock_guard_mutex lock(m_lock);
    m_fs_info << get_log_leval_string(leval) << ":";
}
void log_manger::_write(const std::string& content)
{
    lock_guard_mutex lock(m_lock);
    m_fs_info << content;
}
void log_manger::_write(const char* curr_file_name, const char* curr_func_name, int line_num)
{
    lock_guard_mutex lock(m_lock);
    m_fs_info << curr_file_name << "##" << curr_func_name << "##" << line_num;
    m_fs_info << get_current_time();
}

void log_manger::_open_file()
{
    lock_guard_mutex lock(m_lock);
    std::string t = get_current_date();
    std::string file_path = m_log_path + "/info_" + t + ".log";
    if(m_fs_info.is_open())
        return;
    
    m_fs_info.open(file_path, std::ios::app | std::ios::in | std::ios::out);
}

std::string log_manger::get_current_date()
{
    char d[50] = {0};

    struct tm *local = NULL;
    time_t t = time(NULL);
    local = localtime(&t);

    sprintf(d, "%d-%d-%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    return std::string(d);
}
std::string log_manger::get_current_time()
{
    struct tm *local = NULL;
    time_t t = time(NULL);
    local = localtime(&t);

    char d[100] = {0};
    printf(d, "[%d-%d-%d %d:%d:%d]",
           local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
           local->tm_hour, local->tm_min, local->tm_sec);

    return std::string(d);
}

std::string log_manger::get_log_leval_string(log_leval leval)
{
    std::string result;
    switch(leval)
    {
    case INFO:
        result = "[INFO]";
        break;
    case WARNING:
        result = "[WARNING]";
        break;
    case ERROR:
        result = "[ERROR]";
        break;
    default:
        result = "[NO_LEVAL]";
        break;
    }
    return result;
}

}
