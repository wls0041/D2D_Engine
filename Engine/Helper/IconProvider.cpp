#include "stdafx.h"
#include "IconProvider.h"

void IconProvider::Initialize(Context * context)
{
	this->context = context;

	Load("../../_Assets/Icon/folder.png", IconType::Thumbnail_File);
	Load("../../_Assets/Icon/file.png", IconType::Thumbnail_File);
	Load("../../_Assets/Icon/playButton.png", IconType::Button_Play);
	Load("../../_Assets/Icon/stopButton.png", IconType::Button_Stop);
	Load("../../_Assets/Icon/pauseButton.png", IconType::Button_Pause);
	Load("../../_Assets/Icon/log_info.png", IconType::Log_Info);
	Load("../../_Assets/Icon/log_warning.png", IconType::Log_Warning);
	Load("../../_Assets/Icon/log_error.png", IconType::Log_Error);
}

Thumbnail * IconProvider::Load(const std::string & path, const IconType & type)
{
	if (type != IconType::Custom) {
		for (auto &thumbnail : thumbnails) {
			if (thumbnail.type == type) return &thumbnail;
		}
	}
	else {
		for (auto &thumbnail : thumbnails) { //custum은 미리 지정해놓은 type이 아닌 임의의 파일을 사용하므로 path로 찾음
			if (thumbnail.path == path) return &thumbnail;
		}
	}

	if (FileSystem::IsDirectory(path)) return GetThumbnailFromType(IconType::Thumbnail_Foler);
	if (FileSystem::IsSupportedImageFile(path)) {
		auto texture = new Texture(context);
		texture->LoadFromFile(path);

		return &thumbnails.emplace_back(type, texture, path);//알아서 내부에서 생성되기 때문에 이렇게 사용가능
	}

	return GetThumbnailFromType(IconType::Thumbnail_Foler); //지금은 모든 파일의 이미지를 동일하게 함. 구분하고 싶으면 if문 추가
}

ID3D11ShaderResourceView * IconProvider::GetShaderResourceFromType(const IconType & type)
{
	return Load("", type)->texture->GetShaderResourceView();
}

ID3D11ShaderResourceView * IconProvider::GetShaderResourceFromPath(const std::string & path)
{
	return Load(path)->texture->GetShaderResourceView();
}

const bool IconProvider::ImageButton(const Thumbnail & thumbnail, const ImVec2 & size)
{
	bool bPressed = ImGui::ImageButton(thumbnail.texture->GetShaderResourceView(), size);
	return bPressed;
}

const bool IconProvider::ImageButton(const IconType & type, const float & size)
{
	bool bPressed = ImGui::ImageButton(GetShaderResourceFromType(type), ImVec2(size, size));
	return bPressed;
}

const bool IconProvider::ImageButton(const std::string & path, const float & size)
{
	bool bPressed = ImGui::ImageButton(GetShaderResourceFromPath(path), ImVec2(size, size));
	return bPressed;
}

Thumbnail * IconProvider::GetThumbnailFromType(const IconType & type)
{
	for (auto &thumbnail : thumbnails) {
		if (thumbnail.type == type) return &thumbnail;
	}
	return nullptr;
}
