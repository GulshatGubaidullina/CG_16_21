#ifndef TEXTURE_H
#define	TEXTURE_H
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <Magick++.h>
#include <Magick++/Blob.h>
#include <Magick++/Exception.h>

//#include <ImageMagick-6/Magick++.h>
//#include <ImageMagick-6/Magick++/Blob.h>
//#include <ImageMagick-6/Magick++/Exception.h>

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
};

#endif	/* TEXTURE_H */
