#include "scriptIO.h"
#include "ECSManager.h"
#include "common.hpp"

#include "components/MaterialComponent.h"

#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>


void TourBillon::scriptIO::init()
{
	ECSManager::Instance()->AddListener(Events::LOAD_YAML, METHOD_LISTENER(scriptIO::ReadYAMLconfig));

}

void TourBillon::scriptIO::ReadYAMLconfig(const CEvent& event)
{
	//cereal test
	cereal::JSONOutputArchive archive(os);
	archive(CEREAL_NVP(p1));

	std::ifstream is("person.json");
	cereal::JSONInputArchive archive(is);
	archive(p2);

	//ECS Mode
	ReflectPath* pfile_path = static_cast<ReflectPath*>(event.event_data);
	//YAML::Node config = YAML::LoadFile(*pfile_path);
	std::ifstream ifs(*pfile_path);
	if (!ifs.is_open()) {
		LOG_ERROR("file open failed")
		return;
	}

	Json::Value root;
	Json::CharReaderBuilder reader;
	std::string errs;

	if (Json::parseFromStream(reader, ifs, &root, &errs))
	{
		if (root.isMember("rhi")) {
			ReadRHIConfig(root["rhi"]);
		}
		if (root.isMember("scene")) {
			ReadSceneConfig(root["scene"]);
		}
	}
}

void TourBillon::scriptIO::ReadRHIConfig(const Json::Value& node)
{
	//非ECS，读写方式固定
	//renderpass
	if (node.isMember("RenderPass"))
	{
		const Json::Value& n_renderpass = node["RenderPass"];
		const Json::Value& n_attachments = n_renderpass["attachments"];
		for(int i = 0; i < n_attachments.size(); ++i) {
			//std::cout << "Number " << i << ": " << n_attachments[i].asInt() << std::endl;
			n_attachments[i]["type"];
		}
	}
	
}

void TourBillon::scriptIO::ReadSceneConfig(const Json::Value& node)
{
	//从ECS中获取对象类型
	//all ECS config

	if (node.isMember("entities"))
		return;
	const Json::Value& n_entities = node["entities"];


	for (int i = 0; i < n_entities.size(); ++i) {
		Entity entity = ECSManager::Instance()->CreateEntity();

		const Json::Value& n_components = n_entities[i]["components"];
		for (int j = 0; j < n_components.size(); ++j) {


		}
	}
}
