#include "matchengine.h"
#include "gtest/gtest.h"

TEST(MatchEngine, char_to_uint16) {
	EXPECT_EQ(char_into_uint16_t('a', 'b'), 0x6162);
	EXPECT_EQ(char_into_uint16_t('1', '0'), 0x3130);
	EXPECT_EQ(char_into_uint16_t('A', 'B'), 0x4142);
}
