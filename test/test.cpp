#include "gtest/gtest.h"
#include "../project/faceDet/faceDet.h"

using namespace facedetection;
using namespace std;

TEST(faceDet, findFaces)
{
    cv::CascadeClassifier cascade;
    ASSERT_TRUE(cascade.load("haarcascade_frontalface_alt2.xml"));
    std::vector<cv::Rect> resFases = faceDet("./testsamples/one_face.jpg", cascade);
    ASSERT_EQ(resFases.size(), 1);
    resFases.clear();
    resFases = faceDet("./testsamples/two_fases.jpg", cascade);
    ASSERT_EQ(resFases.size(), 2);
    resFases.clear();
    resFases = faceDet("./testsamples/no_face.jpg", cascade);
    ASSERT_EQ(resFases.size(), 0);
}