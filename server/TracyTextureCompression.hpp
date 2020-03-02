#ifndef __TRACY__TEXTURECOMPRESSION_HPP__
#define __TRACY__TEXTURECOMPRESSION_HPP__

#include <stdint.h>
#include <stdlib.h>

#include "TracySlab.hpp"

struct ZSTD_CCtx_s;
struct ZSTD_DCtx_s;

namespace tracy
{

struct FrameImage;

class TextureCompression
{
public:
    TextureCompression();
    ~TextureCompression();

    void Pack( struct ZSTD_CCtx_s* ctx, char*& buf, size_t& bufsz, const char* image, uint32_t inBytes, uint32_t& csz ) const;

    template<size_t Size>
    const char* Pack( const char* image, uint32_t inBytes, uint32_t& csz, Slab<Size>& slab )
    {
        const auto outsz = PackImpl( image, inBytes );
        auto ptr = (char*)slab.AllocBig( outsz );
        memcpy( ptr, m_buf, outsz );
        csz = outsz;
        return ptr;
    }

    const char* Unpack( const FrameImage& image );

private:
    uint32_t PackImpl( const char* image, uint32_t inBytes );

    char* m_buf;
    size_t m_bufSize;
    struct ZSTD_CCtx_s* m_cctx;
    struct ZSTD_DCtx_s* m_dctx;
};

}

#endif