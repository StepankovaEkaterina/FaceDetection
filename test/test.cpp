#include "gtest/gtest.h"
#include "faceDet/faceDet.h"

using namespace facedetection;
using namespace std;

TEST(faceDet, findFaces)
{
    std::vector<cv::Rect> resFases = faceDet("c:\\testsamples\\oneface\\one_face.jpg");
    ASSERT_EQ(resFases.size(), 1);
    resFases.clear();
    resFases = faceDet("c:\\testsamples\\twofaces\\twofaces.jpg");
    ASSERT_EQ(resFases.size(), 2);
    resFases.clear();
    resFases = faceDet("c:\\testsamples\\noface\\no_face.jpg");
    ASSERT_EQ(resFases.size(), 0);
}