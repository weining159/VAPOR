#pragma once

#include <vapor/VolumeAlgorithm.h>

namespace VAPoR {
    
    class VolumeRegular : public VolumeAlgorithm {
    public:
        virtual int LoadData(const Grid *grid);
        virtual ShaderProgram *GetShader(ShaderManager *sm);
    };
    
}
