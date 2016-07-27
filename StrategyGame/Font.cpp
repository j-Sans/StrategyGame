//
//  Font.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/27/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!In-Practice/Text-Rendering by Joey De Vries
//  This has been altered from the original code, and is not endoresed nor endorsed by LearnOpenGL
//

#include "Font.hpp"

Font::Font(const char* fontPath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "Error initializing FreeType font library" << std::endl;
    
    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
        std::cout << "Error loading font" << std::endl;
    
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    
}