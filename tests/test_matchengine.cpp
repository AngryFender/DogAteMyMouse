#include <matchengine.h>
#include <gmock/gmock.h>
#include "gtest/gtest.h"
#include "mock/mockkeygen.h"

TEST(MatchEngine, char_to_uint16) {
	EXPECT_EQ(char_into_uint16_t('a', 'b'), 0x6162);
	EXPECT_EQ(char_into_uint16_t('1', '0'), 0x3130);
	EXPECT_EQ(char_into_uint16_t('A', 'B'), 0x4142);
}

TEST(MatchEngine, generate_random) {

	std::vector<std::pair<float, float>> fakepoints = { { 1.0f, 2.0f } };
	ScreenInfo fakeScreen = {};
	std::vector<Key> fakeKeys = {};

	MockKeygen mockGen;


	EXPECT_CALL(mockGen, generate(testing::_, testing::_))
        .Times(1)
        .WillOnce(testing::Return(fakeKeys));


	MatchEngine<MockKeygen&> eng(mockGen);
	eng.get_target_keys(fakepoints, fakeScreen);
}
