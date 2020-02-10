// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <fstream>
#include <kiwano/core/Logger.h>
#include <kiwano/core/Exception.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/utils/ResourceCache.h>

namespace kiwano
{
namespace __resource_cache_01
{
bool LoadJsonData(ResourceCache* loader, Json const& json_data);
bool LoadXmlData(ResourceCache* loader, const pugi::xml_node& elem);
}  // namespace __resource_cache_01

namespace
{
Map<String, Function<bool(ResourceCache*, Json const&)>> load_json_funcs = {
    { "latest", __resource_cache_01::LoadJsonData },
    { "0.1", __resource_cache_01::LoadJsonData },
};

Map<String, Function<bool(ResourceCache*, const pugi::xml_node&)>> load_xml_funcs = {
    { "latest", __resource_cache_01::LoadXmlData },
    { "0.1", __resource_cache_01::LoadXmlData },
};
}  // namespace

ResourceCache::ResourceCache() {}

ResourceCache::~ResourceCache()
{
    Clear();
}

bool ResourceCache::LoadFromJsonFile(String const& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_ERROR("%s failed: File not found.", __FUNCTION__);
        return false;
    }

    Json json_data;

    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);
        ifs.open(full_path.c_str());
        ifs >> json_data;
        ifs.close();
    }
    catch (std::wifstream::failure& e)
    {
        KGE_ERROR("%s failed: Cannot open file. (%s)", __FUNCTION__, e.what());
        return false;
    }
    catch (Json::exception& e)
    {
        KGE_ERROR("%s failed: Cannot parse to JSON. (%s)", __FUNCTION__, e.what());
        return false;
    }
    return LoadFromJson(json_data);
}

bool ResourceCache::LoadFromJson(Json const& json_data)
{
    try
    {
        String version = json_data["version"];

        auto load = load_json_funcs.find(version);
        if (load != load_json_funcs.end())
        {
            return load->second(this, json_data);
        }
        else if (version.empty())
        {
            return load_json_funcs["latest"](this, json_data);
        }
        else
        {
            KGE_ERROR("%s failed: unknown resource data version", __FUNCTION__);
        }
    }
    catch (Json::exception& e)
    {
        KGE_ERROR("%s failed: JSON data is invalid. (%s)", __FUNCTION__, e.what());
        return false;
    }
    return false;
}

bool ResourceCache::LoadFromXmlFile(String const& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_ERROR("%s failed: File not found.", __FUNCTION__);
        return false;
    }

    String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);

    pugi::xml_document     doc;
    pugi::xml_parse_result result = doc.load_file(full_path.c_str(), pugi::parse_default, pugi::encoding_auto);

    if (result)
    {
        return LoadFromXml(doc);
    }
    else
    {
        KGE_ERROR("%s failed: XML [%s] parsed with errors: %s", __FUNCTION__, full_path.c_str(), result.description());
        return false;
    }
}

bool ResourceCache::LoadFromXml(const pugi::xml_document& doc)
{
    if (pugi::xml_node root = doc.child("resources"))
    {
        String version;
        if (auto version_node = root.child("version"))
            version = version_node.child_value();

        auto load = load_xml_funcs.find(version);
        if (load != load_xml_funcs.end())
        {
            return load->second(this, root);
        }
        else if (version.empty())
        {
            return load_xml_funcs["latest"](this, root);
        }
        else
        {
            KGE_ERROR("%s failed: unknown resource data version", __FUNCTION__);
        }
    }
    return false;
}

size_t ResourceCache::AddFrameSequence(String const& id, Vector<String> const& files)
{
    if (files.empty())
        return 0;

    Vector<FramePtr> frames;
    frames.reserve(files.size());

    for (const auto& file : files)
    {
        FramePtr ptr = new (std::nothrow) Frame;
        if (ptr)
        {
            if (ptr->Load(file))
            {
                frames.push_back(ptr);
            }
        }
    }
    return AddFrameSequence(id, frames);
}

size_t ResourceCache::AddFrameSequence(String const& id, Vector<Resource> const& resources)
{
    if (resources.empty())
        return 0;

    Vector<FramePtr> frames;
    frames.reserve(resources.size());

    for (const auto& res : resources)
    {
        FramePtr ptr = new (std::nothrow) Frame;
        if (ptr)
        {
            if (ptr->Load(res))
            {
                frames.push_back(ptr);
            }
        }
    }
    return AddFrameSequence(id, frames);
}

size_t ResourceCache::AddFrameSequence(String const& id, Vector<FramePtr> const& frames)
{
    if (frames.empty())
        return 0;

    FrameSequencePtr fs = FrameSequence::Create(frames);
    if (fs)
    {
        AddObject(id, fs);
        return fs->GetFramesCount();
    }
    return 0;
}

size_t ResourceCache::AddFrameSequence(String const& id, FramePtr frame, int cols, int rows, float padding_x,
                                       float padding_y)
{
    if (cols <= 0 || rows <= 0)
        return 0;

    if (!frame)
        return 0;

    float raw_width  = frame->GetWidth();
    float raw_height = frame->GetHeight();
    float width      = (raw_width - (cols - 1) * padding_x) / cols;
    float height     = (raw_height - (rows - 1) * padding_y) / rows;

    Vector<FramePtr> frames;
    frames.reserve(rows * cols);

    float dty = 0;
    for (int i = 0; i < rows; i++)
    {
        float dtx = 0;
        for (int j = 0; j < cols; j++)
        {
            FramePtr ptr = new (std::nothrow) Frame;
            if (ptr)
            {
                ptr->SetTexture(frame->GetTexture());
                ptr->SetCropRect(Rect{ dtx, dty, dtx + width, dty + height });
                frames.push_back(ptr);
            }
            dtx += (width + padding_x);
        }
        dty += (height + padding_y);
    }

    FrameSequencePtr fs = new (std::nothrow) FrameSequence;
    if (fs)
    {
        fs->AddFrames(frames);
        AddObject(id, fs);
        return fs->GetFramesCount();
    }
    return 0;
}

bool ResourceCache::AddObject(String const& id, ObjectBasePtr obj)
{
    if (obj)
    {
        object_cache_.insert(std::make_pair(id, obj));
        return true;
    }
    return false;
}

void ResourceCache::Remove(String const& id)
{
    object_cache_.erase(id);
}

void ResourceCache::Clear()
{
    object_cache_.clear();
}

ObjectBasePtr ResourceCache::Get(String const& id) const
{
    auto iter = object_cache_.find(id);
    if (iter == object_cache_.end())
        return nullptr;
    return (*iter).second;
}

}  // namespace kiwano

namespace kiwano
{
namespace __resource_cache_01
{
struct GlobalData
{
    String path;
};

bool LoadTexturesFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const String& type,
                          const String& file)
{
    if (!gdata)
        return false;

    if (type == "gif")
    {
        // GIF image
        GifImagePtr gif = new (std::nothrow) GifImage;
        if (gif && gif->Load(gdata->path + file))
        {
            return loader->AddObject(id, gif);
        }
    }

    if (!file.empty())
    {
        // Simple image
        FramePtr frame = new (std::nothrow) Frame;
        if (frame && frame->Load(gdata->path + file))
        {
            return loader->AddObject(id, frame);
        }
    }
    return false;
}

bool LoadTexturesFromData(ResourceCache* loader, GlobalData* gdata, const String& id,
                          const Vector<String>& files)
{
    if (!gdata)
        return false;

    if (files.empty())
        return true;

    // Frames
    Vector<FramePtr> frames;
    frames.reserve(files.size());
    for (const auto& file : files)
    {
        FramePtr frame = new Frame;
        if (frame->Load(gdata->path + file))
        {
            frames.push_back(frame);
        }
    }
    FrameSequencePtr frame_seq = FrameSequence::Create(frames);
    if (frame_seq)
    {
        return !!loader->AddObject(id, frame_seq);
    }
    return false;
}

bool LoadTexturesFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const String& file, int rows,
                          int cols, float padding_x, float padding_y)
{
    if (!gdata)
        return false;

    if (!file.empty())
    {
        if (rows || cols)
        {
            // Frame slices
            FramePtr frame = new (std::nothrow) Frame;
            if (frame && frame->Load(gdata->path + file))
            {
                return !!loader->AddFrameSequence(id, frame, std::max(cols, 1), std::max(rows, 1), padding_x,
                                                  padding_y);
            }
        }
        else
        {
            // Simple image
            FramePtr frame = new (std::nothrow) Frame;
            if (frame && frame->Load(gdata->path + file))
            {
                return loader->AddObject(id, frame);
            }
        }
    }
    return false;
}

bool LoadFontsFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const String& file)
{
    if (!gdata)
        return false;

    FontPtr font = new (std::nothrow) Font;
    if (font && font->Load(gdata->path + file))
    {
        return loader->AddObject(id, font);
    }
    return false;
}

bool LoadJsonData(ResourceCache* loader, Json const& json_data)
{
    GlobalData global_data;
    if (json_data.count("path"))
    {
        global_data.path = json_data["path"];
    }

    if (json_data.count("images"))
    {
        for (const auto& image : json_data["images"])
        {
            String id, type, file;
            int    rows = 0, cols = 0;

            if (image.count("id"))
                id = image["id"].get<String>();
            if (image.count("type"))
                type = image["type"].get<String>();
            if (image.count("file"))
                file = image["file"].get<String>();
            if (image.count("rows"))
                rows = image["rows"].get<int>();
            if (image.count("cols"))
                cols = image["cols"].get<int>();

            if (rows || cols)
            {
                float padding_x = 0, padding_y = 0;
                if (image.count("padding-x"))
                    padding_x = image["padding-x"].get<float>();
                if (image.count("padding-y"))
                    padding_y = image["padding-y"].get<float>();

                if (!LoadTexturesFromData(loader, &global_data, id, file, rows, cols, padding_x, padding_y))
                    return false;
            }

            if (image.count("files"))
            {
                Vector<String> files;
                files.reserve(image["files"].size());
                for (const auto& file : image["files"])
                {
                    files.push_back(file.get<String>());
                }
                if (!LoadTexturesFromData(loader, &global_data, id, files))
                    return false;
            }
            else
            {
                if (!LoadTexturesFromData(loader, &global_data, id, type, file))
                    return false;
            }
        }
    }

    if (json_data.count("fonts"))
    {
        for (const auto& font : json_data["fonts"])
        {
            String id, file;

            if (font.count("id"))
                id = font["id"].get<String>();
            if (font.count("file"))
                file = font["file"].get<String>();

            if (!LoadFontsFromData(loader, &global_data, id, file))
                return false;
        }
    }
    return true;
}

bool LoadXmlData(ResourceCache* loader, const pugi::xml_node& elem)
{
    GlobalData global_data;
    if (auto path = elem.child("path"))
    {
        global_data.path = path.child_value();
    }

    if (auto images = elem.child("images"))
    {
        for (auto image : images.children())
        {
            String id, type, file;
            int    rows = 0, cols = 0;

            if (auto attr = image.attribute("id"))
                id.assign(attr.value());
            if (auto attr = image.attribute("type"))
                type = attr.value();
            if (auto attr = image.attribute("file"))
                file = attr.value();
            if (auto attr = image.attribute("rows"))
                rows = attr.as_int(0);
            if (auto attr = image.attribute("cols"))
                cols = attr.as_int(0);

            if (rows || cols)
            {
                float padding_x = 0, padding_y = 0;
                if (auto attr = image.attribute("padding-x"))
                    padding_x = attr.as_float(0.0f);
                if (auto attr = image.attribute("padding-y"))
                    padding_y = attr.as_float(0.0f);

                if (!LoadTexturesFromData(loader, &global_data, id, file, rows, cols, padding_x, padding_y))
                    return false;
            }

            if (file.empty() && !image.empty())
            {
                Vector<String> files_arr;
                for (auto file : image.children())
                {
                    if (auto path = file.attribute("path"))
                    {
                        files_arr.push_back(path.value());
                    }
                }
                if (!LoadTexturesFromData(loader, &global_data, id, files_arr))
                    return false;
            }
            else
            {
                if (!LoadTexturesFromData(loader, &global_data, id, type, file))
                    return false;
            }
        }
    }

    if (auto fonts = elem.child("fonts"))
    {
        for (auto font : fonts.children())
        {
            String id, file;
            if (auto attr = font.attribute("id"))
                id.assign(attr.value());
            if (auto attr = font.attribute("file"))
                file = attr.value();

            if (!LoadFontsFromData(loader, &global_data, id, file))
                return false;
        }
    }
    return true;
}
}  // namespace __resource_cache_01
}  // namespace kiwano
