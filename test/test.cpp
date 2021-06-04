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
        size_t size = 0;
        cv::Rect* rectsArr = faceDet(test.first.c_str());
        if(rectsArr)
            size = _msize(rectsArr) / sizeof(*rectsArr);
        ASSERT_EQ(size, test.second);
    }

    freeLibrary();
}