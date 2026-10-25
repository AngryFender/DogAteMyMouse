#include <matchengine.h>
#include <gmock/gmock.h>
#include "gtest/gtest.h"
#include "mock/mockkeygen.h"

TEST(MatchEngine, char_to_uint16) {
	EXPECT_EQ(char_into_uint16_t('a', 'b'), 0x6162);
	EXPECT_EQ(char_into_uint16_t('1', '0'), 0x3130);
	EXPECT_EQ(char_into_uint16_t('A', 'B'), 0x4142);
}

TEST(MatchEngine, generate_keys_and_match) {

	std::vector<std::pair<float, float>> points_source = { { 1.0f, 2.0f } ,{4.0f, 4.0f} };
	ScreenInfo empty_screen = {};
	std::vector<Key> fake_keys = { Key{ 'f','f' }, Key{ 'j','j' } };

	MockKeygen mockGen;

	EXPECT_CALL(mockGen, generate(testing::_, testing::_))
        .Times(2)
        .WillRepeatedly(testing::Return(fake_keys));

	MatchEngine<MockKeygen&> eng(mockGen);

	auto generated_keys1 = eng.get_target_keys(points_source, empty_screen);
	EXPECT_EQ(fake_keys, generated_keys1);

	auto result1 = eng.match_target_keys('f');
	EXPECT_FALSE(result1);

	auto result2 = eng.match_target_keys('f');
	EXPECT_TRUE(result2);
	EXPECT_EQ(points_source.front(), result2.value());

	auto generated_keys2 = eng.get_target_keys(points_source, empty_screen);
	EXPECT_EQ(fake_keys, generated_keys2);

	auto result3 = eng.match_target_keys('j');
	EXPECT_FALSE(result3);

	auto result4 = eng.match_target_keys('j');
	EXPECT_TRUE(result4);
	EXPECT_EQ(points_source.back(), result4.value());
}
