#include <matchengine.h>
#include <gmock/gmock.h>
#include "gtest/gtest.h"
#include "mock/mockkeygen.h"

namespace {

	class MatchEngineFix : public testing::Test {

	protected:
		std::vector<std::pair<float, float>> points_source;
		ScreenInfo empty_screen;
		std::vector<Key> fake_keys;

		testing::NiceMock<MockKeygen> mock_gen;
		MatchEngine<MockKeygen&> eng;

		MatchEngineFix() :
			points_source{ { 1.0f, 2.0f } ,{ 4.0f, 4.0f } },
			empty_screen{},
			fake_keys{ Key{ 'f','f' }, Key{ 'j','j' } },
			eng(mock_gen)
		{
			ON_CALL(mock_gen, generate(testing::_, testing::_))
				.WillByDefault(testing::Return(fake_keys));
		}
	};


	TEST(MatchEngine, char_to_uint16) {
		EXPECT_EQ(char_into_uint16_t('a', 'b'), 0x6162);
		EXPECT_EQ(char_into_uint16_t('1', '0'), 0x3130);
		EXPECT_EQ(char_into_uint16_t('A', 'B'), 0x4142);
	}

	TEST_F(MatchEngineFix, generate_keys) {
		EXPECT_CALL(mock_gen, generate(testing::_, testing::_)).Times(1);
		auto generated_keys1 = eng.get_target_keys(points_source, empty_screen);
		EXPECT_EQ(fake_keys, generated_keys1);
	}

	TEST_F(MatchEngineFix, match_using_chars) {

		eng.get_target_keys(points_source, empty_screen);

		EXPECT_FALSE(eng.match_target_keys('f'));
		auto result2 = eng.match_target_keys('f');
		EXPECT_TRUE(result2);

		EXPECT_EQ(points_source.front(), result2.value());
	}


	TEST_F(MatchEngineFix, reset_by_matching_chars) {

		EXPECT_CALL(mock_gen, generate(testing::_, testing::_)).Times(2);

		eng.get_target_keys(points_source, empty_screen);

		auto result1 = eng.match_target_keys('f');
		EXPECT_FALSE(result1);

		auto result2 = eng.match_target_keys('f');
		EXPECT_TRUE(result2);
		EXPECT_EQ(points_source.front(), result2.value());
		
		//buffer is reset here
		auto generated_keys2 = eng.get_target_keys(points_source, empty_screen);
		EXPECT_EQ(fake_keys, generated_keys2);

		auto result3 = eng.match_target_keys('j');
		EXPECT_FALSE(result3);

		auto result4 = eng.match_target_keys('j');
		EXPECT_TRUE(result4);
		EXPECT_EQ(points_source.back(), result4.value());
	}

	TEST_F(MatchEngineFix, reset_buffer) {

		eng.get_target_keys(points_source, empty_screen);

		EXPECT_FALSE(eng.match_target_keys('f'));

		eng.clear();

		EXPECT_FALSE(eng.match_target_keys('f'));
		auto result2 = eng.match_target_keys('f');
		EXPECT_TRUE(result2);

		EXPECT_EQ(points_source.front(), result2.value());
	}

	TEST_F(MatchEngineFix, reset_after_random_chars) {

		eng.get_target_keys(points_source, empty_screen);

		EXPECT_FALSE(eng.match_target_keys('x'));
		EXPECT_FALSE(eng.match_target_keys('y'));

		EXPECT_FALSE(eng.match_target_keys('f'));
		auto result2 = eng.match_target_keys('f');
		EXPECT_TRUE(result2);

		EXPECT_EQ(points_source.front(), result2.value());
	}

	TEST_F(MatchEngineFix, match_without_generation) {

		EXPECT_FALSE(eng.match_target_keys('f'));
		EXPECT_FALSE(eng.match_target_keys('f'));

	}

	TEST_F(MatchEngineFix, match_directly_after_match) {

		eng.get_target_keys(points_source, empty_screen);

		EXPECT_FALSE(eng.match_target_keys('f'));
		auto result1 = eng.match_target_keys('f');
		EXPECT_TRUE(result1);

		EXPECT_EQ(points_source.front(), result1.value());

		EXPECT_FALSE(eng.match_target_keys('g'));
		EXPECT_FALSE(eng.match_target_keys('g'));

	}

}