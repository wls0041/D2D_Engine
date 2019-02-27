#pragma once
//custom -> 미리 가지고 있지 않음. 나머지 -> 미리 가지고 있음
enum class IconType : uint { Custom, Button_Play, Button_Stop, Button_Pause };

struct Thumbnail { 
	Thumbnail() : type(IconType::Custom), texture(nullptr), path("") {}
	Thumbnail(const IconType &type, Texture *texture, const std::string &path) : type(type), texture(texture), path(path) {}

	IconType type;
	Texture *texture;
	std::string path;
};

class IconProvider final
{
public:
	~IconProvider() = default;

	IconProvider(const IconProvider&) = delete;
   IconProvider &operator=(const IconProvider&) = delete;

	static IconProvider &Get() {
		static IconProvider instance;
		return instance;
	}

	void Initialize(class conetxt *context);

	Thumbnail *Load(const std::string &path, const IconType &type = IconType::Custom);

	ID3D11ShaderResourceView *GetShaderResourceFromType(const IconType &type);
	ID3D11ShaderResourceView *GetShaderResourceFromPath(const std::string &path);

	const bool ImageButton(const IconType &type, const float &size);
	const bool ImageButton(const std::string &path, const float &size);

private:
	IconProvider() = default;
	Thumbnail *GetThumbnailFromType(const IconType &type);

private:
	class Context *context;
	std::vector<Thumbnail> thumbnails;
};