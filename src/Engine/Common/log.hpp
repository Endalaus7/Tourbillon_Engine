#pragma once
//a base log,or use LogSystem
#include "common.hpp"
#include <iostream>
#include <string>
#include <filesystem>
namespace TourBillon
{
	
	class Log :public Singleton<Log>
	{
	public:
		enum LogType
		{
			TYPE_LOG_DEBUG,
			TYPE_LOG_WARNING,
			TYPE_LOG_ERROR,

		};

		void LogMessage(const std::string& message, LogType type, std::string filepath, int lineNum)
		{
			std::filesystem::path file_path = filepath;
			// 提取文件名
			std::string file_name = file_path.filename().string();

			switch (type)
			{
			case TYPE_LOG_DEBUG:
				std::cout << "#LOG DEBUG[" << file_name << "," << std::to_string(lineNum) << "]:" << message << std::endl;
				break;
			case TYPE_LOG_WARNING:
				std::cout << "#LOG WARNING[" << file_name << "," << std::to_string(lineNum) << "]:" << message << std::endl;
				break;
			case TYPE_LOG_ERROR:
				std::cout << "#LOG ERROR[" << file_name << "," << std::to_string(lineNum) << "]:" << message << std::endl;
				throw std::runtime_error(message);
				break;
			default:
				break;
			}
		}
	};

}

#define LOG_MESSAGE(LOG_LEVEL,...) TourBillon::Log::Instance()->LogMessage(__VA_ARGS__,LOG_LEVEL,__FILE__,__LINE__);




#ifndef _DEBUG
//#ifndef _RELEASE
#define LOG_DEBUG(...) {}
#define LOG_WARNING(...) {}
#define LOG_ERROR(...) {}
#else
#define LOG_DEBUG(...) LOG_MESSAGE(TourBillon::Log::TYPE_LOG_DEBUG,__VA_ARGS__);
#define LOG_WARNING(...) LOG_MESSAGE(TourBillon::Log::TYPE_LOG_DEBUG,__VA_ARGS__);
#define LOG_ERROR(...) LOG_MESSAGE(TourBillon::Log::TYPE_LOG_ERROR,__VA_ARGS__);
#endif