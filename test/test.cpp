#include "gtest/gtest.h"
#include "faceDet/faceDet.h"

TEST(faceDet, findFaces)
{
    initLibrary();
    std::vector<cv::Rect> resFases = faceDet("testsamples\\oneface\\one_face.jpg");
    ASSERT_EQ(resFases.size(), 1);
    resFases.clear();
    resFases = faceDet("testsamples\\twofaces\\twofaces.jpg");
    ASSERT_EQ(resFases.size(), 2);
    resFases.clear();
    resFases = faceDet("testsamples\\noface\\noface.jpg");
    ASSERT_EQ(resFases.size(), 0);
    freeLibrary();
}