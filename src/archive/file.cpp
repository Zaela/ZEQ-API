
#include "file.hpp"

bool File::exists(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "rb");
    
    if (fp)
    {
        fclose(fp);
        return true;
    }
    
    return false;
}

byte* File::openBinary(const std::string& path, uint32_t& len)
{
    FILE* fp = fopen(path.c_str(), "rb");
    
    if (!fp)
    {
        len = 0;
        return nullptr;
    }
    
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);

    if (len == 0)
    {
        fclose(fp);
        return nullptr;
    }

    rewind(fp);
    byte* data = new byte[len];
    fread(data, 1, len, fp);
    fclose(fp);
    
    return data;
}
