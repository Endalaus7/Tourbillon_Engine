#pragma once

#include "IOTool.h"

#include <json/json.h>

namespace TourBillon
{
	class scriptIO :public IOTool
	{
	public:
		virtual void init()override;

		void ReadYAMLconfig(const CEvent&);
	protected:
		void ReadRHIConfig(const Json::Value& node);//����rhi��Դ
		void ReadSceneConfig(const Json::Value& node);//��������Դ
	};
}