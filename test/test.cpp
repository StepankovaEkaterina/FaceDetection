#include "gtest/gtest.h"
#include "faceDet/faceDet.h"

TEST(faceDet, findFaces)
{
    initLibrary();
    
    std::vector< std::pair<std::string, int>> testSamples = 
    {
        { "testsamples\\oneface\\one_face.jpg", 1 },
        { "testsamples\\twofaces\\twofaces.jpg", 2 },
        { "testsamples\\noface\\noface.jpg", 0 }
    };

    for(auto& test: testSamples)
    {  
        TRectsFace res{};      
        int hr = faceDet(test.first.c_str(), res);
        ASSERT_EQ(hr, 0);
        delete [] res.rects;
        ASSERT_EQ(res.count, test.second);
    }

    freeLibrary();
}